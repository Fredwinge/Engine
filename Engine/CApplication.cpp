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
		DoFrame();
	}
}

void CApplication::DoFrame()
{

}