#pragma once
#include <Windows.h>
#include <sstream>
#include <vector>
#include "dxerr\dxerr.h"
//Checks if a HRESULT function has failed, if so, throw an exception
//#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__,__FILE__,hr)

//Graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_ASSERT_NOINFO(hr) __GFX_ASSERT(__LINE__, __FILE__, (hr)) //TODO: REMOVE???
#define GFX_ASSERT_NOINFO(hrcall) if(FAILED(hr = (hrcall))) __GFX_ASSERT(__LINE__,__FILE__,hr)

//Different macros depending of if we're in release or debug mode, since some info isn't available in release mode
#ifndef NDEBUG
#define GFX_ASSERT(hr) __GFX_ASSERT(__LINE__,__FILE__,(hr),m_InfoManager.GetMessages())
#define GFX_ASSERT_INFO(hrcall) m_InfoManager.Set(); if(FAILED(hr = (hrcall))) GFX_ASSERT(hr)

#define GFX_DEVICE_REMOVED_ASSERT(hr) __GFX_ASSERT_DEVICE_REMOVED(__LINE__,__FILE__,(hr), m_InfoManager.GetMessages())
#define GFX_ASSERT_INFO_ONLY(call) m_InfoManager.Set(); (call); {auto v = m_InfoManager.GetMessages(); if(v.empty() == false) { __GFX_ASSERT_INFO_ONLY(__LINE__,__FILE__,v);}}
#else
#define GFX_ASSERT(hr) GraphicsError(__LINE__,__FILE__,(hr))
#define GFX_ASSERT_INFO(hrcall) GFX_ASSERT_NOINFO(hrcall)

#define GFX_DEVICE_REMOVED_ASSERT(hr) GraphicsRemovedReason(__LINE__,__FILE__,(hr))
#define GFX_ASSERT_INFO_ONLY(call) (call)
#endif // !NDEBUG

//Macro for importing infomanager into local scope
//this.GetInfoManager(Graphics& gfx) must exist
#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN(gfx) HRESULT hr; CDXGIInfoManager& m_InfoManager = GetInfoManager((gfx))
#endif



inline void __GFX_ASSERT(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept
{

	std::string errorInfo;

	//Join all info messages with newlines into single string
	for (const auto& msg : infoMsgs)
	{
		errorInfo += msg;
		errorInfo.push_back('\n');
	}
	//remove final newline if it exists
	if (errorInfo.empty() == false)
	{
		errorInfo.pop_back();
	}

	char buf[512];
	DXGetErrorDescriptionA(hr, buf, sizeof(buf));

	std::ostringstream oss;
	oss << "GFX Error" << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << hr
		<< std::dec << " (" << (unsigned long)hr << ")" << std::endl
		<< "[Error String] " << DXGetErrorStringA(hr) << std::endl
		<< "[Description] " << buf << std::endl;

	//Output additional information if there is any
	if (errorInfo.empty() == false)
	{
		oss << "\n[Error Info]\n" << errorInfo << std::endl << std::endl;
	}
	oss << "[File] " << file << std::endl << "[Line] " << line;

	MessageBoxA(nullptr, oss.str().c_str(), "GFX Error", MB_OK | MB_ICONEXCLAMATION);
	exit(1);
}

inline void __GFX_ASSERT_INFO_ONLY(int line, const char* file, std::vector<std::string> infoMsgs = {})
{

	std::string errorInfo;

	//Join all info messages with newline into a single string
	for (const auto& m : infoMsgs)
	{
		errorInfo += m;
		errorInfo.push_back('\n');
	}
	//Remove final newline if it exists
	if (errorInfo.empty() == false)
	{
		errorInfo.pop_back();
	}

	std::ostringstream oss;
	oss << "Graphics Info Error" << std::endl
		<< "\n[Error Info]\n" << errorInfo << std::endl << std::endl;
	oss << "[File] " << file << std::endl << "[Line] " << line;

	MessageBoxA(nullptr, oss.str().c_str(), "GFX Info Error", MB_OK | MB_ICONEXCLAMATION);
	exit(1);
}

inline void __GFX_ASSERT_DEVICE_REMOVED(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {})
{
	std::string errorInfo;

	//Join all info messages with newlines into single string
	for (const auto& msg : infoMsgs)
	{
		errorInfo += msg;
		errorInfo.push_back('\n');
	}
	//remove final newline if it exists
	if (errorInfo.empty() == false)
	{
		errorInfo.pop_back();
	}

	char buf[512];
	DXGetErrorDescriptionA(hr, buf, sizeof(buf));

	std::ostringstream oss;
	oss << "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)" << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << hr
		<< std::dec << " (" << (unsigned long)hr << ")" << std::endl
		<< "[Error String] " << DXGetErrorStringA(hr) << std::endl
		<< "[Description] " << buf << std::endl;

	//Output additional information if there is any
	if (errorInfo.empty() == false)
	{
		oss << "\n[Error Info]\n" << errorInfo << std::endl << std::endl;
	}
	oss << "[File] " << file << std::endl << "[Line] " << line;

	MessageBoxA(nullptr, oss.str().c_str(), "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)", MB_OK | MB_ICONEXCLAMATION);
	exit(1);
}