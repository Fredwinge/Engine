#include "CApplication.h"
#include <sstream>
#include "Drawable\CBox.h"

CApplication::CApplication()
	:
	m_Wnd(800, 600, "Window Thingy")
{

	for (size_t i = 0; i < nDrawables; ++i)
	{
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float>adist{ 0.0f, 3.1415f * 2.0f };
		std::uniform_real_distribution<float>ddist{ 0.0f, 3.1415f * 2.0f };
		std::uniform_real_distribution<float> odist{ 0.0f, 3.1415 * 0.3f };
		std::uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f, 3.0f };
		//std::uniform_int_distribution<int> latdist{ 5,20 };
		//std::uniform_int_distribution<int> longdist{ 10,40 };
		//std::uniform_int_distribution<int> typedist{ 0,3 };

		m_pDrawables.push_back(std::make_unique<CBox>(m_Wnd.Gfx(), rng, adist, ddist, odist, rdist, bdist));
	}

	m_Wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 0.75f, 0.5f, 100.0f));
	m_Wnd.Gfx().SetCamera(m_Camera.GetMatrix());

	m_pTorvudModel = new CModel(m_Wnd.Gfx(), "../Data/Models/Torvud.obj");
}

CApplication::~CApplication()
{
	delete m_pTorvudModel;
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

	auto deltaTime = m_Timer.Mark() * m_fSpeedFactor;
	for (auto& d : m_pDrawables)
	{
		d->Update(m_Wnd.m_Keyboard.KeyisPressed(VK_SPACE) ? 0.0f : deltaTime);
		d->Draw(m_Wnd.Gfx());
	}

	m_pTorvudModel->Update(m_Wnd.m_Keyboard.KeyisPressed(VK_SPACE) ? 0.0f : deltaTime);
	m_pTorvudModel->Draw(m_Wnd.Gfx());

	m_Wnd.Gfx().EndFrame();
}