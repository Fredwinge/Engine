#include "CApplication.h"

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

		if (CWindow::ProcessMessages() == CWindow::APPLICATION_QUIT)
		{
			return -1;
		}

		DoFrame();
	}
}

void CApplication::DoFrame()
{
	m_Wnd.Gfx().BeginFrame();
	m_Wnd.Gfx().EndFrame();
}