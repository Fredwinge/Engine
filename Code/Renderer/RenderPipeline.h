#pragma once
#include "Renderable\IRenderable.h"
#include "Utils\Timer.h"
#include "Bindable/RenderTarget.h"

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

	//Post process tests
	CSampler* m_pSampler;
	CRenderTarget* m_pRenderTarget;
	CMaterial* m_pMaterial;
	CRenderMesh* m_pMesh;

	CRenderTarget* m_pZTarget;
	CRenderTarget* m_pPosTarget;

	//Directional tests
	CMaterial* m_pDirectionalMaterial;

};