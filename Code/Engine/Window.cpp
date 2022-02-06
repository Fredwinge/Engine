#include "Window.h"
#include <sstream>
#include "WindowsAssertMacros.h"
//#include "resource.h"

//CWindow::WindowClass
CWindow::WindowClass CWindow::WindowClass::s_wndClass;

CWindow::WindowClass::WindowClass() noexcept
	:
	m_hInst(GetModuleHandle(nullptr))
{
	/*
	//TODO: FIX ICON STUFF
	//Load icon image
	HICON icon = static_cast<HICON>(LoadImage(
		GetInstance(),								//Handle to instance, so it knows which .EXE to load the data from
		MAKEINTRESOURCE(IDI_ICON1),					//Identifier for the resource it's loading
		IMAGE_ICON,									//Type of image you're loading
		32,											//X-dimension
		32,											//Y-dimension
		0											//Optional flags
	));

	HICON iconSmall = static_cast<HICON>(LoadImage(
		GetInstance(),								//Handle to instance, so it knows which .EXE to load the data from
		MAKEINTRESOURCE(IDI_ICON1),					//Identifier for the resource it's loading
		IMAGE_ICON,									//Type of image you're loading
		16,											//X-dimension
		16,											//Y-dimension
		0											//Optional flags
	));
	*/

	//Most windows versions have a regular version and an 'EX' version, 'EX' stands for extended
	//'EX' are newer than regular version, but not necessarily better

	WNDCLASSEXA wc = {};
	wc.cbSize = sizeof(wc);				//Size of the structure, needed
	wc.style = CS_OWNDC;				//OWNDC creates a device context for every new window
	wc.lpfnWndProc = HandleMsgSetup;	//A pointer to the function which handles ALL messages to the window, by extension, the behaviour of it
	wc.cbClsExtra = 0;					//Allows you to allocate extra bytes that we can use to store custom data
	wc.cbWndExtra = 0;					//Allows us to allocate extra bytes of data for every new instance of this class
	wc.hInstance = GetInstance();		//A handle to the instance containing the window procedure
	//wc.hIcon = icon;					//Handle to class icon
	wc.hCursor = NULL;					//Handle to class cursor
	wc.hbrBackground = NULL;			//Creates a brush which draws to the back of the window, NULL since we'll be using DirectX for all drawing purposes
	wc.lpszMenuName = NULL;				//Menu name, pointless for us
	wc.lpszClassName = GetName();		//Specifies the window class name, needed to pass class name when creating windows
	//wc.hIconSm = iconSmall;				//Handle to smaller icon that we can use for the application

	RegisterClassExA(&wc);


	//Register rawinput for mouse
	RAWINPUTDEVICE Rid;

	Rid.usUsagePage = 0x01;         //HID_USAGE_PAGE_GENERIC
	Rid.usUsage = 0x02;             //HID_USAGE_GENERIC_MOUSE
	Rid.dwFlags = 0;				//No flags necessary
	Rid.hwndTarget = 0;				//No hwnd necessary

	if (RegisterRawInputDevices(&Rid, 1, sizeof(Rid)) == FALSE)
	{
		//Registration failed
		WND_LAST_ERROR();
	}
}

CWindow::WindowClass::~WindowClass()
{
	UnregisterClassA(m_cWndClassName, GetInstance());
}

//CWindow
CWindow::CWindow(int width, int height, const char* name)
	:
	m_Size(width, height)
{

	//Calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		WND_LAST_ERROR();
	}

	OutputDebugString("\nAdjusted window rect");

	//Create window and get hWnd
	m_hWnd = CreateWindowExA(
		0,
		WindowClass::GetName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,								//Window x Position
		CW_USEDEFAULT,								//Window y Position
		wr.right - wr.left,							//Width of window
		wr.bottom - wr.top,							//Height of window
		nullptr,
		nullptr,
		WindowClass::GetInstance(),
		this
	);

	OutputDebugString("\nCreated hWnd");

	//Make sure window is created correctly
	if (m_hWnd == 0)
	{
		WND_LAST_ERROR();
	}

	//Newly created windows start of as hidden so we have to make this one show
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	OutputDebugString("\nWindow shown");

	//Create graphics object
	m_pRenderer = new CRenderer(m_hWnd, m_Size);
	OutputDebugString("\nCreated cgraphics");

}

CWindow::~CWindow()
{
	if(m_pRenderer == nullptr)
		delete m_pRenderer;

	DestroyWindow(m_hWnd);
}

//Set application title
void CWindow::SetTitle(const std::string& title)
{
	if (SetWindowTextA(m_hWnd, title.c_str()) == 0)
	{
		WND_LAST_ERROR();
	}
}

CWindow::Message CWindow::ProcessMessages()
{
	MSG msg;

	while (PeekMessage(
		&msg,							//A pointer to an MSG structure that recieves message information from the thread's message queue 
		nullptr,						//A handle to the window whose messages are to be retrieved
		0,								//Allows us to specify a range of messages we want to pull of the queue
		0,								//If both max and min are 0, all messages are pulled
		PM_REMOVE						//Specifies how messages are handled
	))
	{

		//check for quit because peekmessage does not signal this via return
		if (msg.message == WM_QUIT)
		{
			return CWindow::Message::APPLICATION_QUIT;
		}


		//TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	//return standard message when not quitting app
	return CWindow::Message::UNSPECIFIED;
}


CRenderer* CWindow::GetRenderer()
{

	//m_pGfx should never be null but we error check for debugging purposes
#ifndef NDEBUG
	if (m_pRenderer == nullptr)
	{
		std::ostringstream oss;
		oss << "Windows Error [Gfx ptr was null]" << std::endl 
			<< "[File] " << __FILE__ << std::endl << "[Line] " << __LINE__;

		MessageBoxA(nullptr, oss.str().c_str(), "Windows Error", MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}
#endif // !NDEBUG

	return m_pRenderer;
}

//Message handler for setup
LRESULT WINAPI CWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	//use create parameter passed in from CreateWindow() to store window class pointer
	if (msg == WM_NCCREATE)		//NCCREATE
	{
		//Extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		CWindow* const pWnd = static_cast<CWindow*>(pCreate->lpCreateParams);						//Get pointer to our created window from lpCreateParams
		//Set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));				//Create a link between the window itself and the class we use to control it
		//Set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CWindow::HandleMsgThunk));
		//forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	//if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//Default message handler
LRESULT WINAPI CWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	//retrieve ptr to window class
	CWindow* const pWnd = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT CWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) //noexcept
{

	switch (msg)
	{
		//We don't want the DefProc to handle this message because
		//we want our destructor to destroy the window, so return 0
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		//Clear keystate when window loses focus to prevent input from getting stuck
	case WM_KILLFOCUS:
		m_Keyboard.ClearState();
		break;

		///////////////////// KEYBOARD MESSAGES /////////////////////
	case WM_KEYDOWN:
		//Syskey commands need to be handled to track system keys such as ALT or F1-F10
	case WM_SYSKEYDOWN:

		//lParam checks if key was pressed already right before this message
		//bit 30 ( 0x40000000 ) will be 1 if it was, we only want to run OnKeyPressed
		//if autorepeat is enabled or if the key wasn't pressed right before
		if (!(lParam & 0x40000000) || m_Keyboard.AutoRepeatIsEnabled() == true)	//filter autorepeat
		{
			m_Keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:

		m_Keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:

		m_Keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
		/////////////////// END KEYBOARD MESSAGES ///////////////////
		////////////////////// MOUSE  MESSAGES //////////////////////
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		//Check if mouse is inside client region
		if (pt.x >= 0 && pt.x < m_Size.x && pt.y >= 0 && pt.y < m_Size.y)
		{
			m_Mouse.SetPos(Vector2(pt.x, pt.y));

			//If mouse previously weren't inside client region, set IsInWindow to true
			if (m_Mouse.IsInWindow() == false)
			{
				//SetCapture captures the mouse within a window, disallowing it from leaving
				SetCapture(hWnd);
				m_Mouse.OnMouseEnter();
			}
		}
		//If not inside client region
		else
		{
			//If mouse is outside of region but it's being clicked, allow it to move
			//this is to allow the user the liberty of dragging the window
			if (m_Mouse.LeftIsPressed() == true || m_Mouse.RightIsPressed() == true)	//wParam & (MK_LBUTTON | MK_RBUTTON) also does the trick
			{
				m_Mouse.SetPos(Vector2(pt.x, pt.y));
			}
			// If mouse is outside the client region and no clicking is occuring then you have left the window
			else
			{
				//Release the capture so that the user isn't eternally bound to the window
				ReleaseCapture();
				m_Mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		m_Mouse.OnLeftPressed();

		break;
	}
	case WM_RBUTTONDOWN:
	{
		m_Mouse.OnRightPressed();

		break;
	}

	case WM_LBUTTONUP:
	{
		m_Mouse.OnLeftReleased();

		break;
	}
	case WM_RBUTTONUP:
	{
		m_Mouse.OnRightReleased();

		break;
	}

	case WM_MOUSEWHEEL:
	{
		//Get wheel delta
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);

		m_Mouse.OnWheelDelta(delta);

		break;
	}
	//////////////////// END MOUSE  MESSAGES ////////////////////
	///////////////////// RAWINPUT MESSAGES /////////////////////
	case WM_INPUT:
	{
		UINT dwSize;

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL) 
		{
			break;
		} 

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString ("\nGetRawInputData does not return correct size !\n"); 

		RAWINPUT* raw = (RAWINPUT*)lpb;
		switch (raw->header.dwType)
		{
		case RIM_TYPEMOUSE:
		{
			RAWMOUSE mouse = raw->data.mouse;

			if (mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
			{

				bool isVirtualDesktop = (mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP;

				int width  = GetSystemMetrics(isVirtualDesktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
				int height = GetSystemMetrics(isVirtualDesktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);

				int absoluteX = int((mouse.lLastX / 65535.0f) * width);
				int absoluteY = int((mouse.lLastY / 65535.0f) * height);

				m_Mouse.SetPos(Vector2(absoluteX, absoluteY));
			}
			else if (mouse.lLastX != 0 || mouse.lLastY != 0)
			{
				int relativeX = mouse.lLastX;
				int relativeY = mouse.lLastY;

				m_Mouse.MoveRaw(Vector2(relativeX, relativeY));
			}

			break;
		}
		}
	}
	/////////////////// END RAWINPUT MESSAGES ///////////////////
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//Lock mouse to window
void CWindow::ToggleCursorLock(bool lock)
{
	if (lock == true)
	{
		RECT wr;
		if (GetWindowRect(m_hWnd, &wr) == true)
		{
			ClipCursor(&wr);
		}
		else
		{
			OutputDebugString("\nCouldn't lock cursor to window, do better error call here\n");
		}
	}
	else
		ClipCursor(nullptr);
}

//Set cursor position
void CWindow::SetCursorPosition(Vector2 pos)
{
	RECT wr;
	if (GetWindowRect(m_hWnd, &wr) == true)
	{

		int x = wr.right - wr.left;
		int y = wr.bottom - wr.top;

		x *= pos.x;
		y *= pos.y;

		x += wr.left;
		y += wr.top;

		SetCursorPos(x, y);
		m_Mouse.SetPos(Vector2(x, y));
	}
}

void CWindow::HideCursor(bool hide)
{
	ShowCursor(!hide);
}

Vector2 CWindow::GetWindowCenter()
{
	RECT wr;
	if (GetWindowRect(m_hWnd, &wr) == true)
	{

		int x = wr.right - wr.left;
		int y = wr.bottom - wr.top;

		x *= 0.5f;
		y *= 0.5f;

		x += wr.left;
		y += wr.top;

		return Vector2(x, y);
	}

	return Vector2::Zero;
}