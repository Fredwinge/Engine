#pragma once
#include "CWindow.h"
#include "CTimer.h"

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
};