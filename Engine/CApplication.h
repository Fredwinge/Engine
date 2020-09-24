#pragma once
#include "CWindow.h"
#include "CTimer.h"
#include "CCamera.h"
#include "Drawable\CDrawable.h"

#include "Drawable\CModel.h"

class CApplication
{
public:

	CApplication();
	~CApplication();

	//Main message loop
	int Go();

private:

	void DoFrame();

	CWindow m_Wnd;

	CTimer m_Timer;

	CCamera m_Camera;

	//Debugging variables
	std::vector<std::unique_ptr<CDrawable>> m_pDrawables;
	static constexpr size_t nDrawables = 180;

	float m_fSpeedFactor = 1.0f;

	CModel* m_pTorvudModel;
};