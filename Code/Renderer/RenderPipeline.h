#pragma once
#include "Renderable\IRenderable.h"
#include "Utils\Timer.h"

class CRenderPipeline
{
public:

	enum ERenderPass
	{
		OPAQUE_PASS = 0,

		NUM_PASSES
	};

	CRenderPipeline(CRenderer* pRenderer);

	void AddToQueue(IRenderable* pRenderable, ERenderPass ePass);
	void ClearQueue();
	
	void RenderScene();

private:

	std::vector<IRenderable*> m_RenderQueue[NUM_PASSES];

	CRenderer* m_pSceneRenderer;

	CTimer m_Timer;

};