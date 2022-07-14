#pragma once
#include "Window.h"
#include "Utils/Timer.h"
#include "Utils/Camera.h"
#include "Renderable\IRenderable.h"

#include "Model.h"
#include "RenderCallbacks\IRenderCallback.h"

class CApplication
{
public:

	CApplication();
	~CApplication();

	//Main message loop
	int Run();

private:

	void Update();
	void Render(CRenderer* pRenderer);

	CWindow m_Wnd;

	CTimer m_Timer;

	CCamera m_Camera;

	//Debugging variables
	std::vector<std::unique_ptr<IRenderable>> m_pRenderables;

	float m_fSpeedFactor = 1.0f;

	CModel* m_pTorvudModel;

	IRenderCallback* pRenderCallback;

	bool bCameraFreeFlight = false;
};