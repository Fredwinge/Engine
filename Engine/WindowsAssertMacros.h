#pragma once
#include <Windows.h>
#include <sstream>

//Error helper macros
#define WND_ERROR(hr) __WND_ERROR(__LINE__, __FILE__, hr)
#define WND_LAST_ERROR() __WND_ERROR(__LINE__, __FILE__, GetLastError())

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