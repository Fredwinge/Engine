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

	
	m_PostProcessDepthStencil = new CDepthStencil(m_pSceneRenderer, false, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_ALWAYS);

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
	
	//Disable depth testing before PP passes, so we can actually draw them all.
	m_PostProcessDepthStencil->Bind(m_pSceneRenderer);
	//Clear rendertarget slots so we can use them as shader resources instead
	 m_pSceneRenderer->ClearRenderTargets();
	

	{
		//Deferred lighting
		DeferredLightsBuffer LightBuffer;
		LightBuffer.m_CamPos = m_pSceneRenderer->GetCamera()->GetView().Pos;
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