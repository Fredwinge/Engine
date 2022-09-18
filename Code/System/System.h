#pragma once
#include "Windows/Window.h"

class CSystem
{
public:

	enum class SysMessage
	{
		UNSPECIFIED,		//Standard message which shouldn't trigger a response
		SYSTEM_QUIT
	};

	CSystem();
	~CSystem();

	SysMessage Update();

	CWindow* GetWindow() { return m_pWindow; }

private:
	CWindow* m_pWindow;
};