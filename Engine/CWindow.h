#pragma once
#include <Windows.h>
#include "IException.h"
#include "CKeyboard.h"
#include "CMouse.h"
#include "CGraphics.h"

class CWindow
{

public:

	//Enum for window Return messages
	enum class Message
	{
		APPLICATION_STANDARD,	//Standard message which shouldn't trigger a response
		APPLICATION_QUIT
	};

	//Exception classes
	class Exception : public IException
	{
		using IException::IException;

	public:

		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};

	class HrException : public Exception
	{

	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;

		virtual const char* GetType() const noexcept override	{ return "Windows Exception"; }

		HRESULT GetErrorCode() const noexcept					{ return m_hr; }
		std::string GetErrorString() const noexcept				{ return TranslateErrorCode(m_hr); }

	private:

		HRESULT m_hr;
	};

	class NoGfxException : public Exception
	{

	public:

		using Exception::Exception;

		const char* GetType() const noexcept override			{ return "Windows Exception [No Graphics]"; }
	};

private:

	//Singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:


		static const char* GetName() noexcept	{ return m_cWndClassName; }
		static HINSTANCE GetInstance() noexcept { return s_wndClass.m_hInst; }

	private:

		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* m_cWndClassName = "Direct3D Window";

		static WindowClass s_wndClass;
		HINSTANCE m_hInst;

	};

public:

	CWindow(int width, int height, const char* name);
	~CWindow();
	CWindow(const CWindow&) = delete;
	CWindow& operator=(const CWindow&) = delete;

	void SetTitle(const std::string& title);
	static Message ProcessMessages();

	CGraphics& Gfx();

private:

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) /*noexcept*/;

public:

	CKeyboard m_Keyboard;
	CMouse m_Mouse;

private:

	int m_Width;
	int m_Height;
	HWND m_hWnd;

	CGraphics* m_pGfx;
};

//Error exception helper macros
//Needed to get line and file which the exception was thrown from
#define CHWND_ERROR(hr) __WND_ERROR(__LINE__, __FILE__, hr)
#define CHWND_LAST_ERROR() __WND_ERROR(__LINE__, __FILE__, GetLastError())
#define CHWND_NOGFX_ERROR() __WND_NO_GFX_ERROR(__LINE__, __FILE__)

//TODO: FIND A BETTER PLACE TO PUT THESE ERROR FUNCTIONS!!!!!!

inline void __WND_ERROR(int line, const char* file, HRESULT hr)
{
	//Get error string
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

	std::string errorString;
	if (nMsgLength == 0)
	{
		errorString = "Unidentified error code";
	}
	else
	{
		errorString = pMsgBuffer;
		LocalFree(pMsgBuffer);
	}

	//Set error text
	std::ostringstream oss;
	oss << "Windows Error" << std::endl	
		<< "[Error Code] " << hr << std::endl
		<< "[Description] " << errorString << std::endl
		<< "[File] " << file << std::endl << "[Line] " << line;	

	MessageBoxA(nullptr, oss.str().c_str(), "Windows Error", MB_OK | MB_ICONEXCLAMATION);
	exit(1);
}

inline void __WND_NO_GFX_ERROR(int line, const char* file)
{
	std::ostringstream oss;
	oss << "Windows Error [Gfx ptr was null]" << std::endl 
		<< "[File] " << file << std::endl << "[Line] " << line;

	MessageBoxA(nullptr, oss.str().c_str(), "Windows Error", MB_OK | MB_ICONEXCLAMATION);
	exit(1);
}