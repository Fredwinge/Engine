#include "CApplication.h"
#include <sstream>
#include "Renderable\CBox.h"

CApplication::CApplication()
	:
	m_Wnd(800, 600, "Window Thingy")
{

	for (size_t i = 0; i < numRenderables; ++i)
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

		m_pRenderables.push_back(std::make_unique<CBox>(m_Wnd.Gfx(), rng, adist, ddist, odist, rdist, bdist));
	}

	m_Wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 0.75f, 0.5f, 100.0f));
	m_Wnd.Gfx().SetCamera(m_Camera.GetMatrix());

	m_pTorvudModel = new CModel(m_Wnd.Gfx(), "../Data/Models/Torvud.obj");

	pRenderCallback = new IRenderCallback(m_Wnd.Gfx());

	for (auto& d : m_pRenderables)
	{
		d->SetRenderCallback(pRenderCallback);
	}

	m_pTorvudModel->SetRenderCallback(pRenderCallback);
}

CApplication::~CApplication()
{
	delete m_pTorvudModel;

	delete pRenderCallback;
}

int CApplication::Go()
{
	while (true)
	{

		if (CWindow::ProcessMessages() == CWindow::Message::APPLICATION_QUIT)
		{
			return -1;
		}

		Update();
		Render(m_Wnd.Gfx());
	}
}

void CApplication::Update()
{

	auto deltaTime = m_Timer.Mark() * m_fSpeedFactor;
	for (auto& d : m_pRenderables)
	{
		d->Update(m_Wnd.m_Keyboard.KeyisPressed(VK_SPACE) ? 0.0f : deltaTime);
	}

	m_pTorvudModel->Update(m_Wnd.m_Keyboard.KeyisPressed(VK_SPACE) ? 0.0f : deltaTime);
}

void CApplication::Render(CGraphics& rGfx)
{
	rGfx.BeginFrame(sin(m_Timer.TimeElapsed()));

	for (auto& d : m_pRenderables)
	{
		d->Render(rGfx);
	}

	m_pTorvudModel->Render(rGfx);

	rGfx.EndFrame();
}