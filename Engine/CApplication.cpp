#include "CApplication.h"

CApplication::CApplication()
	:
	m_Wnd(800, 600, "Window Thingy")
{

}

CApplication::~CApplication()
{

}

int CApplication::Go()
{
	while (true)
	{

		if (CWindow::ProcessMessages() == CWindow::APPLICATION_QUIT)
		{
			return -1;
		}

		DoFrame();
	}
}

void CApplication::DoFrame()
{

}