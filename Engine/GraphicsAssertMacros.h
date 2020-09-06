#pragma once
#include <Windows.h>
#include <sstream>
#include <vector>
#include "dxerr\dxerr.h"
//Checks if a HRESULT function has failed, if so, throw an exception
//#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__,__FILE__,hr)

//Graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_ASSERT_NOINFO(hr) GraphicsError(__LINE__, __FILE__, (hr)) //TODO: REMOVE???
#define GFX_ASSERT_NOINFO(hrcall) if(FAILED(hr = (hrcall))) GraphicsError(__LINE__,__FILE__,hr)

//Different macros depending of if we're in release or debug mode, since some info isn't available in release mode
#ifndef NDEBUG
#define GFX_ASSERT(hr) GraphicsError(__LINE__,__FILE__,(hr),m_InfoManager.GetMessages())
#define GFX_ASSERT_INFO(hrcall) m_InfoManager.Set(); if(FAILED(hr = (hrcall))) GFX_ASSERT(hr)

#define GFX_DEVICE_REMOVED_EXCEPT(hr) CGraphics::DeviceRemovedException(__LINE__,__FILE__,(hr), m_InfoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) m_InfoManager.Set(); (call); {auto v = m_InfoManager.GetMessages(); if(v.empty() == false) { CGraphics::InfoException(__LINE__,__FILE__,v);}}
#else
#define GFX_ASSERT(hr) CGraphics::HrException(__LINE__,__FILE__,(hr))
#define GFX_ASSERT_INFO(hrcall) GFX_ASSERT_NOINFO(hrcall)

#define GFX_DEVICE_REMOVED_EXCEPT(hr) CGraphics::DeviceRemovedException(__LINE__,__FILE__,(hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif // !NDEBUG

//Macro for importing infomanager into local scope
//this.GetInfoManager(Graphics& gfx) must exist
#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN(gfx) HRESULT hr; CDXGIInfoManager& m_InfoManager = GetInfoManager((gfx))
#endif



inline void GraphicsError(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept
{

	std::string m_sInfo;

	//Join all info messages with newlines into single string
	for (const auto& msg : infoMsgs)
	{
		m_sInfo += msg;
		m_sInfo.push_back('\n');
	}
	//remove final newline if it exists
	if (m_sInfo.empty() == false)
	{
		m_sInfo.pop_back();
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
	if (m_sInfo.empty() == false)
	{
		oss << "\n[Error Info]\n" << m_sInfo << std::endl << std::endl;
	}
	oss << "[File] " << file << std::endl << "[Line] " << line;

	MessageBoxA(nullptr, oss.str().c_str(), "GFX Error", MB_OK | MB_ICONEXCLAMATION);
	exit(1);
}