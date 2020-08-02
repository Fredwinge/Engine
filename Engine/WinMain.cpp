#include <Windows.h>
#include "CApplication.h"

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	try
	{
		return CApplication{}.Go();
	}
	catch (...)	//Catch unknown exception
	{
		MessageBoxA(nullptr, "No Details Available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	
	return 0;
}