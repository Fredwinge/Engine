#pragma once
#include "System.h"
#include "Utils/Timer.h"
#include "Utils/Camera.h"
#include "Renderable\IRenderable.h"
#include "RenderPipeline.h"

#include "Model.h"

class CApplication
{
public:

	CApplication();
	~CApplication();

	//Main message loop
	int Run();

private:

	void Update();
	void Render();

	CSystem* m_pSystem;

	CTimer m_Timer;

	CCamera m_Camera;

	//Debugging variables
	std::vector<std::unique_ptr<IRenderable>> m_pRenderables;

	float m_fSpeedFactor = 1.0f;

	CModel* m_pTorvudModel;

	bool bCameraFreeFlight = false;

	//TODO: Move somewhere else
	CRenderPipeline* m_pRenderPipeline;
	CRenderer* m_pRenderer;
};