#include "RenderPipeline.h"
#include "../Engine/Primitives/Plane.h"
#include "Utils\Camera.h"
#include <assert.h>

CRenderPipeline::CRenderPipeline(CRenderer* pRenderer)
	:
	m_pSceneRenderer(pRenderer),
	m_pPostProcessMesh(nullptr)
{
	m_pRenderTarget = new CRenderTarget(pRenderer, Vec2(800, 600));
	m_pCopyTarget = new CRenderTarget(pRenderer, Vec2(800, 600));
	m_pZTarget = new CRenderTarget(pRenderer, Vec2(800, 600));
	//We need a bigger format to store positions
	m_pPosTarget = new CRenderTarget(pRenderer, Vec2(800, 600), DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_pSampler = new CSampler(pRenderer);
	m_pPostProcessMaterial = new CMaterial(pRenderer, "PostProcessVertex.cso", "PostProcessPixel.cso");
	
	CPlane<1, 1>::Create(pRenderer, &m_pPostProcessMesh);

	
	m_pNoDepthTestDepthStencil = new CDepthStencil(m_pSceneRenderer, false, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_ALWAYS);

	D3D11_RENDER_TARGET_BLEND_DESC transparencyBlendDesc;
	transparencyBlendDesc.BlendEnable = true;
	transparencyBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparencyBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparencyBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	transparencyBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	transparencyBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	transparencyBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparencyBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pTransparentBlendState = new CBlendState(m_pSceneRenderer, true, transparencyBlendDesc);

	//TODO: Move elsewhere
	m_pDeferredLightingMaterial = new CMaterial(pRenderer, "PostProcessVertex.cso", "DeferredLighting.cso");
	DeferredLightsBuffer LightBuffer;
	memcpy(LightBuffer.m_pDirectonalLights, m_vDirectionalLights, sizeof(m_vDirectionalLights));
	memcpy(LightBuffer.m_pPointLights, m_vPointLights, sizeof(m_vPointLights));
	m_pLightingCBuffer = new CConstantBuffer(m_pSceneRenderer, CConstantBuffer::ETYPE_PIXEL, &LightBuffer, sizeof(DeferredLightsBuffer));
}

void CRenderPipeline::AddToQueue(IRenderable* pRenderable, ERenderPass ePass)
{
	m_RenderQueue[ePass].push_back(pRenderable);
}

void CRenderPipeline::ClearQueue()
{
	for (int i = 0; i < NUM_PASSES; ++i)
	{
		m_RenderQueue[i].clear();
	}
}

void CRenderPipeline::RenderScene()
{
	//TODO: Do somewhere else
	SortRenderQueue();

	m_pSceneRenderer->BeginFrame(sin(m_Timer.TimeElapsed()));

	//DRAW FRAME
	m_pRenderTarget->ClearRenderTarget(m_pSceneRenderer, Vec4(0.0f, 0.0f, 0.2f, 1.0f));
	m_pRenderTarget->Bind(m_pSceneRenderer, 0u);

	m_pZTarget->ClearRenderTarget(m_pSceneRenderer, Vec4(0.0f));
	m_pZTarget->Bind(m_pSceneRenderer, 1u);

	m_pPosTarget->ClearRenderTarget(m_pSceneRenderer);
	m_pPosTarget->Bind(m_pSceneRenderer, 2u);

	m_pCopyTarget->ClearRenderTarget(m_pSceneRenderer, Vec4(0.0f, 0.0f, 0.2f, 1.0f));

	m_pSceneRenderer->SetDefaultDepthState();


	for (size_t i = 0; i < m_RenderQueue[OPAQUE_PASS].size(); ++i)
	{
		m_RenderQueue[OPAQUE_PASS][i]->Render(m_pSceneRenderer);
	}
	
	//Disable depth testing
	m_pNoDepthTestDepthStencil->Bind(m_pSceneRenderer);
	//Clear rendertarget slots so we can use them as shader resources instead
	 m_pSceneRenderer->ClearRenderTargets();
	

	{
		//Deferred lighting
		DeferredLightsBuffer LightBuffer;
		LightBuffer.m_CamPos = Vec4(m_pSceneRenderer->GetCamera()->GetPosition(), 1.0f);
		memcpy(LightBuffer.m_pDirectonalLights, m_vDirectionalLights, sizeof(m_vDirectionalLights));
		memcpy(LightBuffer.m_pPointLights, m_vPointLights, sizeof(m_vPointLights));

		m_pLightingCBuffer->Update(m_pSceneRenderer, &LightBuffer);
		m_pLightingCBuffer->Bind(m_pSceneRenderer);

		m_pRenderTarget->Bind(m_pSceneRenderer, 0u);
		m_pSceneRenderer->GetDeviceContext()->CopyResource(m_pCopyTarget->GetShaderResource(), m_pRenderTarget->GetShaderResource());

		m_pRenderTarget->Bind(m_pSceneRenderer, 0u);
		m_pSceneRenderer->BindShaderResource(0u, m_pCopyTarget->GetShaderResourceView());
		m_pSceneRenderer->BindShaderResource(1u, m_pZTarget->GetShaderResourceView());
		m_pSceneRenderer->BindShaderResource(2u, m_pPosTarget->GetShaderResourceView());
		m_pDeferredLightingMaterial->BindMaterial(m_pSceneRenderer);
		m_pPostProcessMesh->BindBuffers(m_pSceneRenderer);
		m_pSampler->Bind(m_pSceneRenderer);

		m_pSceneRenderer->DrawIndexed(m_pPostProcessMesh->GetIdxCount());
	}

	m_pSceneRenderer->SetDefaultDepthState();
	m_pTransparentBlendState->Bind(m_pSceneRenderer);
	for (size_t i = 0; i < m_RenderQueue[TRANSPARENT_PASS].size(); ++i)
	{
		m_RenderQueue[TRANSPARENT_PASS][i]->Render(m_pSceneRenderer);
	}
	m_pSceneRenderer->SetDefaultBlendState();
	m_pNoDepthTestDepthStencil->Bind(m_pSceneRenderer);

	//Post process
	m_pSceneRenderer->SetDefaultRenderTarget();
	m_pSceneRenderer->BindShaderResource(0u, m_pRenderTarget->GetShaderResourceView());
	m_pPostProcessMaterial->BindMaterial(m_pSceneRenderer);
	m_pPostProcessMesh->BindBuffers(m_pSceneRenderer);
	m_pSampler->Bind(m_pSceneRenderer);
	
	m_pSceneRenderer->DrawIndexed(m_pPostProcessMesh->GetIdxCount());

	//END FRAME
	m_pSceneRenderer->EndFrame();
}

void CRenderPipeline::AddDirectionalLight(SDirectionalLight DirLight)
{
	assert(m_NumDirectionalLights < MAX_DIRECTIONAL_LIGHTS);
	m_vDirectionalLights[m_NumDirectionalLights] = DirLight;
	++m_NumDirectionalLights;
}

void CRenderPipeline::AddPointLight(SPointLight pointLight)
{
	assert(m_NumPointLights < MAX_POINT_LIGHTS);
	m_vPointLights[m_NumPointLights] = pointLight;
	++m_NumPointLights;
}

//TODO: Do a proper sort instead of hacking it like this
#include <map>
void CRenderPipeline::SortRenderQueue()
{
	//TODO: change to distance to camera z-plane, instead of just the position?
	Vec3 camPos = m_pSceneRenderer->GetCamera()->GetPosition();

	std::map<float, IRenderable*> uglySortMap;

	for (uint32 i = 0; i < m_RenderQueue[TRANSPARENT_PASS].size(); ++i)
	{
		IRenderable* pRenderable = m_RenderQueue[TRANSPARENT_PASS][i];
		Vec3 objPos = pRenderable->GetWorldMatrix().Pos.GetXYZ();
		float dist = objPos.GetDistanceTo(camPos);

		uglySortMap[dist] = pRenderable;
	}
	m_RenderQueue[TRANSPARENT_PASS].clear();
	for (std::map<float, IRenderable*>::reverse_iterator it = uglySortMap.rbegin(); it != uglySortMap.rend(); ++it)
	{
		m_RenderQueue[TRANSPARENT_PASS].push_back(it->second);
	}
}