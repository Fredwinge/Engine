#include <Windows.h>
#include "Application.h"

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	CApplication{}.Go();
	
	return 0;
}