#pragma once
#include "CWindow.h"

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
};