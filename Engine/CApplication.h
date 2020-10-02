#pragma once
#include "CWindow.h"
#include "CTimer.h"
#include "CCamera.h"
#include "Renderable\IRenderable.h"

#include "Renderable\CModel.h"
#include "RenderCallbacks\IRenderCallback.h"

class CApplication
{
public:

	CApplication();
	~CApplication();

	//Main message loop
	int Go();

private:

	void Update();
	void Render(CGraphics& rGfx);

	CWindow m_Wnd;

	CTimer m_Timer;

	CCamera m_Camera;

	//Debugging variables
	std::vector<std::unique_ptr<IRenderable>> m_pRenderables;
	static constexpr size_t numRenderables = 180;

	float m_fSpeedFactor = 1.0f;

	CModel* m_pTorvudModel;

	IRenderCallback* pRenderCallback;
};