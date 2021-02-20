#pragma once
#include <Windows.h>
#include "CKeyboard.h"
#include "CMouse.h"
#include "CGraphics.h"
#include "Maths\Vectors.h"

class CWindow
{

public:

	//Enum for window Return messages
	enum class Message
	{
		UNSPECIFIED,		//Standard message which shouldn't trigger a response
		APPLICATION_QUIT
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

	Vector2 GetSize() const noexcept { return m_Size; }

	//TODO: Put somewhere else / do differently?
	void ToggleCursorLock(bool lock);
	//TODO: Definetly put somewhere else
	void SetCursorPosition(Vector2 pos);
	void HideCursor(bool hide);
	Vector2 GetWindowCenter();

private:

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) /*noexcept*/;

public:

	CKeyboard m_Keyboard;
	CMouse m_Mouse;

private:

	HWND m_hWnd;

	Vector2 m_Size;

	CGraphics* m_pGfx;
};