#include "System.h"

CSystem::CSystem()
{
	m_pWindow = new CWindow(800, 600, "Window Thingy");
}

CSystem::~CSystem()
{

}

CSystem::SysMessage CSystem::Update()
{
	if (m_pWindow->ProcessMessages() == CWindow::Message::APPLICATION_QUIT)
	{
		return SysMessage::SYSTEM_QUIT;
	}

	return SysMessage::UNSPECIFIED;
}