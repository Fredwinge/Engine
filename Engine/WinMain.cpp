#include <Windows.h>
#include "CApplication.h"

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	try
	{
		return CApplication{}.Go();
	}
	catch (const IException& e)			//Catch IException
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)		//Catch standard exception
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)							//Catch unknown exception
	{
		MessageBoxA(nullptr, "No Details Available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	
	return 0;
}