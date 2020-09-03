#include <Windows.h>
#include "CApplication.h"

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

#ifdef _DEBUG

	//Try catch is bad performance wise, only use in debug until a potentially better
	//solution is found, we won't need exception info in release anyways since we won't be debugging
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
#else

	CApplication{}.Go();

#endif

	
	return 0;
}