#pragma once

//Checks if a HRESULT function has failed, if so, throw an exception
//#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__,__FILE__,hr)

//Graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) CGraphics::HrException(__LINE__, __FILE__, (hr)) //TODO: REMOVE???
#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw CGraphics::HrException(__LINE__,__FILE__,hr)

//Different macros depending of if we're in release or debug mode, since some info isn't available in release mode
#ifndef NDEBUG
#define GFX_EXCEPT(hr) CGraphics::HrException(__LINE__,__FILE__,(hr),m_InfoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) m_InfoManager.Set(); if(FAILED(hr = (hrcall))) CGraphics::HrException(__LINE__,__FILE__,(hr),m_InfoManager.GetMessages())
#define GFX_DEVICE_REMOVED_EXCEPT(hr) CGraphics::DeviceRemovedException(__LINE__,__FILE__,(hr), m_InfoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) m_InfoManager.Set(); (call); {auto v = m_InfoManager.GetMessages(); if(v.empty() == false) { CGraphics::InfoException(__LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) CGraphics::HrException(__LINE__,__FILE__,(hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
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