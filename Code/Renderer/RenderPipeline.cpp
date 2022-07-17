#include "RenderPipeline.h"

CRenderPipeline::CRenderPipeline(CRenderer* pRenderer)
	:
	m_pSceneRenderer(pRenderer)
{

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


	for (size_t i = 0; i < m_RenderQueue[OPAQUE_PASS].size(); ++i)
	{
		m_RenderQueue[OPAQUE_PASS][i]->Render(m_pSceneRenderer);
	}


	m_pSceneRenderer->EndFrame();
}