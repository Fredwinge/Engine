#include "CApplication.h"
#include <sstream>

CApplication::CApplication()
	:
	m_Wnd(800, 600, "Window Thingy")
{

	m_Wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 0.75f, 0.5f, 100.0f));
}

CApplication::~CApplication()
{

}

int CApplication::Go()
{
	while (true)
	{

		if (CWindow::ProcessMessages() == CWindow::Message::APPLICATION_QUIT)
		{
			return -1;
		}

		DoFrame();
	}
}

void CApplication::DoFrame()
{
	m_Wnd.Gfx().BeginFrame(sin(m_Timer.TimeElapsed()));
	m_Wnd.Gfx().EndFrame();
}