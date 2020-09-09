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
#define CHWND_EXCEPT(hr) CWindow::HrException(__LINE__, __FILE__, hr)
#define CHWND_LAST_EXCEPT() CWindow::HrException(__LINE__, __FILE__, GetLastError())
#define CHWND_NOGFX_EXCEPT() CWindow::NoGfxException(__LINE__, __FILE__)