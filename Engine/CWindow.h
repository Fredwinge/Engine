#pragma once
#include <Windows.h>

class CWindow
{

private:

	//Enum for window Return messages
	enum Message
	{
		APPLICATION_STANDARD,
		APPLICATION_QUIT
	};

	//Singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:

		//Everything here is noexcept because it runs at startup, 
		//so it happens outside our try catch, any exceptions thrown wouldn't be caught

		static const char* GetName() noexcept { return wndClassName; }
		static HINSTANCE GetInstance() noexcept { return s_wndClass.m_hInst; }

	private:

		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Direct3D Window";

		static WindowClass s_wndClass;
		HINSTANCE m_hInst;

	};

public:

	CWindow(int width, int height, const char* name);
	~CWindow();
	CWindow(const CWindow&) = delete;
	CWindow& operator=(const CWindow&) = delete;

	void SetTitle(char* title);
	Message ProcessMessages();

	//CGraphics Gfx();

	//CKeyboard m_Keyboard
	//CMouse m_Mouse

private:

	int m_Width;
	int m_Height;
	HWND m_hWnd;

	//std::unique_ptr<CGraphics> m_pGfx;
};