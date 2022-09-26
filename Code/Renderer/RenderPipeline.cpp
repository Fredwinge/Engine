#include "RenderPipeline.h"
#include "../Engine/Primitives/Plane.h"

CRenderPipeline::CRenderPipeline(CRenderer* pRenderer)
	:
	m_pSceneRenderer(pRenderer)
{
	m_pRenderTarget = new CRenderTarget(pRenderer, Vec2(800, 600));
	m_pZTarget = new CRenderTarget(pRenderer, Vec2(800, 600));
	m_pSampler = new CSampler(pRenderer);
	m_pMaterial = new CMaterial(pRenderer, "PostProcessVertex.cso", "PostProcessPixel.cso");
	m_pMesh = nullptr;
	CPlane<1, 1>::Create(pRenderer, &m_pMesh);
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
	m_pRenderTarget->ClearRenderTarget(m_pSceneRenderer, Vec4(sin(m_Timer.TimeElapsed()), 0.7f, 1.0f, 1.0f));
	m_pRenderTarget->Bind(m_pSceneRenderer, 0u);

	m_pZTarget->ClearRenderTarget(m_pSceneRenderer);
	m_pZTarget->Bind(m_pSceneRenderer, 1u);


	for (size_t i = 0; i < m_RenderQueue[OPAQUE_PASS].size(); ++i)
	{
		m_RenderQueue[OPAQUE_PASS][i]->Render(m_pSceneRenderer);
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