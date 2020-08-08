#include "CWindow.h"
#include <sstream>
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

	WNDCLASSEXA wc = { 0 };
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
}

CWindow::WindowClass::~WindowClass()
{
	UnregisterClassA(m_cWndClassName, GetInstance());
}

//CWindow
CWindow::CWindow(int width, int height, const char* name)
	:
	m_Width(width),
	m_Height(height)
{

	//Calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}

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

	//Make sure window is created correctly
	if (m_hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	//Newly created windows start of as hidden so we have to make this one show
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);

	//Create graphics object
	m_pGfx = std::make_unique<CGraphics>(m_hWnd);

}

CWindow::~CWindow()
{
	DestroyWindow(m_hWnd);
}

//Set application title
void CWindow::SetTitle(const std::string& title)
{
	if (SetWindowTextA(m_hWnd, title.c_str()) == 0)
	{
		throw CHWND_LAST_EXCEPT();
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
	return CWindow::Message::APPLICATION_STANDARD;
}


CGraphics& CWindow::Gfx()
{
	if (m_pGfx == nullptr)
	{
		throw CHWND_NOGFX_EXCEPT();
	}
	return *m_pGfx;
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

LRESULT CWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
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
		if (pt.x >= 0 && pt.x < m_Width && pt.y >= 0 && pt.y < m_Height)
		{
			m_Mouse.OnMouseMove(pt.x, pt.y);

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
				m_Mouse.OnMouseMove(pt.x, pt.y);
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
		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnLeftPressed(pt.x, pt.y);

		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnRightPressed(pt.x, pt.y);

		break;
	}

	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnLeftReleased(pt.x, pt.y);

		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnRightReleased(pt.x, pt.y);

		break;
	}

	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		//Get wheel delta
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);

		m_Mouse.OnWheelDelta(pt.x, pt.y, delta);

		break;
	}
	//////////////////// END MOUSE  MESSAGES ////////////////////
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//Window exception stuff

CWindow::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	m_hr(hr)
{

}

const char* CWindow::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl								//Outputs type of function
		<< "[Error Code] " << GetErrorCode() << std::endl		//Outputs error code
		<< "[Description] " << GetErrorString() << std::endl	//Outputs error string
		<< GetOriginString();									//Outputs origin string

	m_sWhatBuffer = oss.str();

	return m_sWhatBuffer.c_str();
}

std::string CWindow::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuffer = nullptr;

	//Get description string for our error code
	DWORD nMsgLength = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuffer),
		0,
		nullptr
	);

	if (nMsgLength == 0)
	{
		return "Unidentified error code";
	}

	std::string errorString = pMsgBuffer;
	LocalFree(pMsgBuffer);

	return errorString;
}