#include "Maths\CommonMath.h"
#include "CApplication.h"
#include <sstream>
#include "Renderable\CBox.h"
#include "Maths\Matrix.h"

CApplication::CApplication()
	:
	m_Wnd(800, 600, "Window Thingy")
{

	static constexpr size_t numRenderables = 180;
	for (size_t i = 0; i < numRenderables; ++i)
	{
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f, 3.1415f * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f, 3.1415f * 2.0f };
		std::uniform_real_distribution<float> odist{ 0.0f, 3.1415 * 0.3f };
		std::uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f, 3.0f };
		//std::uniform_int_distribution<int> latdist{ 5,20 };
		//std::uniform_int_distribution<int> longdist{ 10,40 };
		//std::uniform_int_distribution<int> typedist{ 0,3 };

		m_pRenderables.push_back(std::make_unique<CBox>(m_Wnd.GetRenderer(), rng, adist, ddist, odist, rdist, bdist));
	}
	OutputDebugString("\nCreated boxes");

	m_Camera.SetProjection(Matrix::CreateProjectionFov(75.0f, 800.0f / 600.0f, 0.5f, 100.0f));
	OutputDebugString("\nSet camera projection");

	m_Wnd.GetRenderer()->SetCamera(&m_Camera);
	OutputDebugString("\nSet active camera");

	//TODO: path macro
	m_pTorvudModel = new CModel(m_Wnd.GetRenderer(), "../Assets/Models/Torvud.obj");
	OutputDebugString("\ncreated torvud model");

	pRenderCallback = new IRenderCallback(m_Wnd.GetRenderer());
	OutputDebugString("\ncreated rendercallback");

	for (auto& d : m_pRenderables)
	{
		d->SetRenderCallback(pRenderCallback);
	}

	OutputDebugString("\nSet boxes rendercallback");
	m_pTorvudModel->SetRenderCallback(pRenderCallback);
	OutputDebugString("\nSet torvud rendercallback");

	//Temp
	m_Camera.MoveCamera(&m_Wnd.m_Keyboard, Vector2(0.0f, 0.0f), 0.0f);
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
		Render(m_Wnd.GetRenderer());
	}
}

void CApplication::Update()
{

	Vector4 bump = { 0, 0, 1, 0 };
	Matrix bk = { 0, -1, 0, 0,
	0, 1, 0, 0,
	0, 0, 0, 0,
	0, 0, 0 ,0 };

	bump *= bk;

	bump.Normalize();

	auto deltaTime = m_Timer.Mark() * m_fSpeedFactor;
	for (auto& d : m_pRenderables)
	{
		d->Update(m_Wnd.m_Keyboard.KeyIsPressed(VK_SPACE) ? 0.0f : deltaTime);
	}

	m_pTorvudModel->Update(m_Wnd.m_Keyboard.KeyIsPressed(VK_SPACE) ? 0.0f : deltaTime);

//TEMP
	if (m_Wnd.m_Keyboard.KeyIsPressed(VK_ESCAPE))
	{
		m_Wnd.ToggleCursorLock(true);
		m_Wnd.HideCursor(true);

		bCameraFreeFlight = true;
	}
	else if (m_Wnd.m_Keyboard.KeyIsPressed(VK_CONTROL))
	{
		m_Wnd.ToggleCursorLock(false);
		m_Wnd.HideCursor(false);

		bCameraFreeFlight = false;
	}

	if (bCameraFreeFlight == true)
	{
		Vector2 deltaMove = m_Wnd.m_Mouse.GetRawDelta() * 8.0f;
		m_Camera.MoveCamera(&m_Wnd.m_Keyboard, deltaMove, deltaTime);
		
		m_Wnd.SetCursorPosition({ 0.5f, 0.5f });

		m_Wnd.m_Mouse.ResetRawDelta();
	}
}

void CApplication::Render(CRenderer* pRenderer)
{
	pRenderer->BeginFrame(sin(m_Timer.TimeElapsed()));

	for (auto& d : m_pRenderables)
	{
		d->Render(pRenderer);
	}

	m_pTorvudModel->Render(pRenderer);

	pRenderer->EndFrame();
}