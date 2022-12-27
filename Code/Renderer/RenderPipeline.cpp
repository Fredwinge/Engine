#include "RenderPipeline.h"
#include "../Engine/Primitives/Plane.h"
#include "Utils\Camera.h"
#include <assert.h>

CRenderPipeline::CRenderPipeline(CRenderer* pRenderer)
	:
	m_pSceneRenderer(pRenderer)
{
	m_pRenderTarget = new CRenderTarget(pRenderer, Vec2(800, 600));
	m_pCopyTarget = new CRenderTarget(pRenderer, Vec2(800, 600));
	m_pZTarget = new CRenderTarget(pRenderer, Vec2(800, 600));
	//We need a bigger format to store positions
	m_pPosTarget = new CRenderTarget(pRenderer, Vec2(800, 600), DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_pSampler = new CSampler(pRenderer);
	m_pMaterial = new CMaterial(pRenderer, "PostProcessVertex.cso", "PostProcessPixel.cso");
	
	m_pMesh = nullptr;
	CPlane<1, 1>::Create(pRenderer, &m_pMesh);

	

	m_PostProcessDepthStencil = new CDepthStencil(m_pSceneRenderer, false, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_ALWAYS);

	//TODO: Move elsewhere
	m_pDirectionalMaterial = new CMaterial(pRenderer, "PostProcessVertex.cso", "BasicDirectional.cso");
	DirectionalLightsBuffer DirLightBuffer;
	memcpy(DirLightBuffer.m_pDirectonalLights, m_vDirectionalLights, sizeof(m_vDirectionalLights));
	const uint32 DirLightBufferSize = sizeof(DirectionalLightsBuffer);
	m_pDirLightCBuffer = new CConstantBuffer(m_pSceneRenderer, CConstantBuffer::ETYPE_PIXEL, &DirLightBuffer, DirLightBufferSize);

	m_pPointLightMaterial = new CMaterial(pRenderer, "PostProcessVertex.cso", "BasicPointlight.cso");
	PointLightBuffer pointLightBuffer;
	memcpy(pointLightBuffer.m_pPoints, m_vPointLights, sizeof(m_vPointLights));
	const uint32 pointLightBufferSize = sizeof(PointLightBuffer);
	m_pPointLightCBuffer = new CConstantBuffer(m_pSceneRenderer, CConstantBuffer::ETYPE_PIXEL, &pointLightBuffer, pointLightBufferSize);
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
	
	//TODO: Combine point/directional lights
	if(true)
	{
		//DIRECTIONAL LIGHTS
		DirectionalLightsBuffer DirLightBuffer;
		DirLightBuffer.m_CamPos = m_pSceneRenderer->GetCamera()->GetView().Pos;
		memcpy(DirLightBuffer.m_pDirectonalLights, m_vDirectionalLights, sizeof(m_vDirectionalLights));

		m_pDirLightCBuffer->Update(m_pSceneRenderer, &DirLightBuffer);
		m_pDirLightCBuffer->Bind(m_pSceneRenderer);

		m_pRenderTarget->Bind(m_pSceneRenderer, 0u);
		m_pSceneRenderer->GetDeviceContext()->CopyResource(m_pCopyTarget->GetShaderResource(), m_pRenderTarget->GetShaderResource());

		m_pRenderTarget->Bind(m_pSceneRenderer, 0u);
		m_pSceneRenderer->BindShaderResource(0u, m_pCopyTarget->GetShaderResourceView());
		m_pSceneRenderer->BindShaderResource(1u, m_pZTarget->GetShaderResourceView());
		m_pSceneRenderer->BindShaderResource(2u, m_pPosTarget->GetShaderResourceView());
		m_pDirectionalMaterial->BindMaterial(m_pSceneRenderer);
		m_pMesh->BindBuffers(m_pSceneRenderer);
		m_pSampler->Bind(m_pSceneRenderer);

		m_pSceneRenderer->DrawIndexed(m_pMesh->GetIdxCount());
	}
	else
	{
		//POINT LIGHTS
		PointLightBuffer pointLightBuffer;
		memcpy(pointLightBuffer.m_pPoints, m_vPointLights, sizeof(m_vPointLights));
		
		m_pPointLightCBuffer->Update(m_pSceneRenderer, &pointLightBuffer);
		m_pPointLightCBuffer->Bind(m_pSceneRenderer);

		m_pRenderTarget->Bind(m_pSceneRenderer, 0u);
		m_pSceneRenderer->GetDeviceContext()->CopyResource(m_pCopyTarget->GetShaderResource(), m_pRenderTarget->GetShaderResource());

		m_pSceneRenderer->BindShaderResource(0u, m_pCopyTarget->GetShaderResourceView());
		m_pSceneRenderer->BindShaderResource(1u, m_pZTarget->GetShaderResourceView());
		m_pSceneRenderer->BindShaderResource(2u, m_pPosTarget->GetShaderResourceView());
		m_pPointLightMaterial->BindMaterial(m_pSceneRenderer);
		m_pMesh->BindBuffers(m_pSceneRenderer);
		m_pSampler->Bind(m_pSceneRenderer);

		m_pSceneRenderer->DrawIndexed(m_pMesh->GetIdxCount());
	}

	//Post process
	m_pSceneRenderer->SetDefaultRenderTarget();
	m_pSceneRenderer->BindShaderResource(0u, m_pRenderTarget->GetShaderResourceView());
	m_pMaterial->BindMaterial(m_pSceneRenderer);
	m_pMesh->BindBuffers(m_pSceneRenderer);
	m_pSampler->Bind(m_pSceneRenderer);
	
	m_pSceneRenderer->DrawIndexed(m_pMesh->GetIdxCount());

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