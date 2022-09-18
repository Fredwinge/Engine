#include "Math\CommonMath.h"
#include "Application.h"
#include <sstream>
#include "Box.h"
#include "Math\Matrix.h"

#include "Primitives\Plane.h"

CApplication::CApplication()
{

	m_pSystem = new CSystem();
	CWindow* pWindow = m_pSystem->GetWindow();
	m_pRenderer = new CRenderer(pWindow->GetHandle(), pWindow->GetSize());
	m_pRenderPipeline = new CRenderPipeline(m_pRenderer);

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

		m_pRenderables.push_back(std::make_unique<CBox>(m_pRenderer, rng, adist, ddist, odist, rdist, bdist));
	}
	OutputDebugString("\nCreated boxes");

	CRenderMesh* pPlaneMesh;
	CPlane<1, 1>::Create(m_pRenderer, &pPlaneMesh);
	m_pRenderables.push_back(std::make_unique<CModel>(m_pRenderer, pPlaneMesh));
	Matrix planeMatrix = m_pRenderables.back().get()->GetWorldMatrix();
	planeMatrix.Pos.SetXYZ(Vec3(0.0f, -10.0f, -10.0f));
	m_pRenderables.back().get()->SetWorldMatrix(planeMatrix);

	m_Camera.SetProjection(Matrix::CreateProjectionFov(75.0f, 800.0f / 600.0f, 0.5f, 100.0f));
	OutputDebugString("\nSet camera projection");

	m_pRenderer->SetCamera(&m_Camera);
	OutputDebugString("\nSet active camera");

	//TODO: path macro
	m_pTorvudModel = new CModel(m_pRenderer, "../../Assets/Models/Torvud.obj");
	OutputDebugString("\ncreated torvud model");

	//Temp
	m_Camera.MoveCamera(&pWindow->m_Keyboard, Vector2(0.0f, 0.0f), 0.0f);
}

CApplication::~CApplication()
{
	delete m_pTorvudModel;

	delete m_pRenderPipeline;
}

int CApplication::Run()
{
	while (true)
	{

		//TODO: This file shouldn't belong in 'Engine' but in some sort of Launcher/Game project instead.
		if (m_pSystem->Update() == CSystem::SysMessage::SYSTEM_QUIT)
		{
			return -1;
		}
		
		Update();
		Render();
	}
}

static const uint32 s_iFPSCap = 144;
static const float s_fFrameDif = 1.0f / s_iFPSCap;

void CApplication::Update()
{
	//TODO: This should be in a better spot
	while (m_Timer.Peek() < s_fFrameDif)
		Sleep(1);

	CWindow* pWindow = m_pSystem->GetWindow();

	auto deltaTime = m_Timer.Mark() * m_fSpeedFactor;
	for (auto& d : m_pRenderables)
	{
		d->Update(pWindow->m_Keyboard.KeyIsPressed(VK_SPACE) ? 0.0f : deltaTime);
	}

	Matrix torvudMatrix = m_pTorvudModel->GetWorldMatrix();
	torvudMatrix.RotatePreMultiply(Vec3(0.0f, pWindow->m_Keyboard.KeyIsPressed(VK_SPACE) ? 0.0f : deltaTime, 0.0f));
	m_pTorvudModel->SetWorldMatrix(torvudMatrix);

//TEMP
	if (pWindow->m_Keyboard.KeyIsPressed(VK_ESCAPE))
	{
		pWindow->ToggleCursorLock(true);
		pWindow->HideCursor(true);

		bCameraFreeFlight = true;
	}
	else if (pWindow->m_Keyboard.KeyIsPressed(VK_CONTROL))
	{
		pWindow->ToggleCursorLock(false);
		pWindow->HideCursor(false);

		bCameraFreeFlight = false;
	}

	if (bCameraFreeFlight == true)
	{
		Vector2 deltaMove = pWindow->m_Mouse.GetRawDelta() * 8.0f;
		m_Camera.MoveCamera(&pWindow->m_Keyboard, deltaMove, deltaTime);
		
		pWindow->SetCursorPosition({ 0.5f, 0.5f });

		pWindow->m_Mouse.ResetRawDelta();
	}
}

void CApplication::Render()
{
	//pRenderer->BeginFrame(sin(m_Timer.TimeElapsed()));

	for (auto& d : m_pRenderables)
	{
		//d->Render(pRenderer);
		m_pRenderPipeline->AddToQueue(d.get(), CRenderPipeline::OPAQUE_PASS);
	}

	//m_pTorvudModel->Render(pRenderer);
	m_pRenderPipeline->AddToQueue(m_pTorvudModel, CRenderPipeline::OPAQUE_PASS);

	m_pRenderPipeline->RenderScene();

	m_pRenderPipeline->ClearQueue();

	//pRenderer->EndFrame();
}