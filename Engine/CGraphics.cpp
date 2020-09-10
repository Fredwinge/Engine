#include "CGraphics.h"
#include "dxerr/dxerr.h"
#include <DirectXMath.h>
#include <sstream>
#include "GraphicsAssertMacros.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//namespace wrl = Microsoft::WRL;
namespace dx = DirectX;


CGraphics::CGraphics(HWND hWnd)
	:
	m_mProjectionMatrix(),
	m_mCameraMatrix()
{

	//Create swapchain description
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	swapDesc.BufferDesc.Width = 0;													//Resolution width, if set to 0, the width is obtained from the output window
	swapDesc.BufferDesc.Height = 0;													//Resolution height, if set to 0, the height is obtained from the output window
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;						//A structure describing the display format / the channels used
	swapDesc.BufferDesc.RefreshRate.Numerator = 0;									//Refresh rate, numerator = top of rational number (???) Setting both to 0 means it's not limited
	swapDesc.BufferDesc.RefreshRate.Denominator = 0;								//Refresh rate, denominator = bottom of rational number (???) Setting both to 0 means it's not limited
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					//Specifies scaling mode, since we don't set width or height, it's left as unspecified
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//Describes scanline drawing mode, | Rasterized scanning pattern, we don't need to specify
	swapDesc.SampleDesc.Count = 1;													//Multisampling count per pixel
	swapDesc.SampleDesc.Quality = 0;												//The image quality level, higher number leads to lower performance
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;							//Describes surface usage and CPU access options for the back buffer | Ours is a rendertarget
	swapDesc.BufferCount = 1;														//Amount of buffers in the swap chain | When set to 1 we get 1 Frontbuffer and 1 backbuffer
	swapDesc.OutputWindow = hWnd;													//Output window
	swapDesc.Windowed = TRUE;														//Describes wether it's windowed or not
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;									//Describes options for handling the contents of the presentation buffer after presenting a surface
	swapDesc.Flags = 0;																//Describes options for swapchain behaviour


	//Only set the debug flag if we're actually in debug mode
	UINT uSwapCreateFlags = 0u;
#ifndef NDEBUG
	uSwapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !NDEBUG


	//Our error checking macro relies on there being a local HRESULT variable (called hr) in the local scope
	HRESULT hr;

	//Creates a device, deviceContext and swapchain
	GFX_ASSERT_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,									//Specifies the display adapter we want, NULL is default
		D3D_DRIVER_TYPE_HARDWARE,					//Driver type, there are other types than HARDWARE but there is little reason to use them
		nullptr,									//Software driver, unless using a SOFTWARE driver type, keep this at null
		uSwapCreateFlags,							//With DEBUG we get debug messages but it's slower than SINGLETHREADED
		nullptr,									//Feature level, indicates the order in which to test version support
		0u,											//The amount of feature levels
		D3D11_SDK_VERSION,							//ALWAYS	  SPECIFY		D3D11_SDK_VERSION // when using DirectX11
		&swapDesc,									//The swapchain description we want to use for swapchain creation
		&m_pSwapChain,								//The Swapchain we want to create
		&m_pDevice,									//The device we want to create
		nullptr,									//Returns the first supported Feature level
		&m_pDeviceContext));						//The device context we want to create


	ID3D11Resource* pBackBuffer = nullptr;



	//Get the backbuffer
	GFX_ASSERT_INFO(m_pSwapChain->GetBuffer(
		0,								//Index of the buffer we want to get, 0 is the backbuffer
		__uuidof(ID3D11Resource),		//uuid of the interface which we want to recieve onto our subobject(???)
		(LPVOID*)&pBackBuffer));		//** to the object we want to store the backbuffers address in

	//Create render target view
	GFX_ASSERT_INFO(m_pDevice->CreateRenderTargetView(
		pBackBuffer,					//The resource which holds the render target we want a view on
		nullptr,						//Description for Render target view configuration. nullptr gives us a default one we will use for now
		&m_pRenderTargetView));			//The *ID3D11RenderTargetView which stores the rendertarget

	//Release pBackBuffer since it's no longer needed
	pBackBuffer->Release();

	//Create Depth Stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

	depthStencilDesc.DepthEnable = true;							//Enable depth testing
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//Decides if the depth stencil can be written to, there are only two options: ZERO and ALL
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;				//Determines what kind of comparison is going to be done to decide which pixels are drawn or not

	GFX_ASSERT_INFO(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState));

	//Bind depth stencil state to the pipeline
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1u);

	D3D11_TEXTURE2D_DESC depthStencilTxtDesc = {};
	
	depthStencilTxtDesc.SampleDesc.Count = 1;
	depthStencilTxtDesc.SampleDesc.Quality = 0;
	depthStencilTxtDesc.Format = DXGI_FORMAT_D32_FLOAT; //For depth stencil we would want DXGI_FORMAT_D24_UNORM_S8_UINT, since it's split between depth and stencil
	depthStencilTxtDesc.Width = 800;
	depthStencilTxtDesc.Height = 600;
	depthStencilTxtDesc.ArraySize = 1;					//The amount of textures, we only need 1
	depthStencilTxtDesc.MipLevels = 1;
	depthStencilTxtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTxtDesc.Usage = D3D11_USAGE_DEFAULT;

	GFX_ASSERT_INFO(m_pDevice->CreateTexture2D(&depthStencilTxtDesc, nullptr, &m_pDepthStencilTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;

	GFX_ASSERT_INFO(m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture, &depthStencilViewDesc, &m_pDepthStencilView));

	//Configure viewport
	D3D11_VIEWPORT viewPort;
	viewPort.Width = 800;
	viewPort.Height = 600;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1u, &viewPort);

	//Bind depth stencil view to OM
	m_pDeviceContext->OMSetRenderTargets(1u, &m_pRenderTargetView, m_pDepthStencilView);

}

CGraphics::~CGraphics()
{
	//Release pointers

	if (m_pDevice != nullptr)
		m_pDevice->Release();

	if (m_pSwapChain != nullptr)
		m_pSwapChain->Release();

	if (m_pDeviceContext != nullptr)
		m_pDeviceContext->Release();

	if (m_pRenderTargetView != nullptr)
		m_pRenderTargetView->Release();

	if (m_pDepthStencilView != nullptr)
		m_pDepthStencilView->Release();

	if (m_pDepthStencilState != nullptr)
		m_pDepthStencilState->Release();

	if (m_pDepthStencilTexture != nullptr)
		m_pDepthStencilTexture->Release();
}


void CGraphics::EndFrame()
{

	HRESULT hr;

	//Call Set() every frame in CDXGIInfoManager if we're in debug mode, otherwise ignore it
#ifndef NDEBUG
	m_InfoManager.Set();
#endif // !NDEBUG

	//Switches front and backbuffer
	if (FAILED(hr = m_pSwapChain->Present(1u,	//Specifies how to synchronise presentation with the vertical bank (???)
		0u)))	//Swapchain presentation options, flags and stuff
	{
		//Special error checking for Present since it can get the DXGI_ERROR_DEVICE_REMOVED error, 
		//which contains additional information which is worth extracting
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			GFX_DEVICE_REMOVED_ASSERT(m_pDevice->GetDeviceRemovedReason());
		}
		else
		{
			GFX_ASSERT(hr);
		}
	}
}

void CGraphics::BeginFrame(float r, float g, float b) noexcept
{

	const float color[] = { r, g, b, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	//Gotta clear the depth stencil every frame as well
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void CGraphics::DrawIndexed(unsigned int indexCount)
{
	GFX_ASSERT_INFO_ONLY(m_pDeviceContext->DrawIndexed(indexCount, 0, 0));
}

/*
//Graphics exception stuff
CGraphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file),
	m_hr(hr)
{
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

	DisplayError();
}

const char* CGraphics::HrException::what() const noexcept
{

	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;

	//Output additional information if there is any
	if (m_sInfo.empty() == false)
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();

	m_sWhatBuffer = oss.str();

	return m_sWhatBuffer.c_str();
}

//Both of these functions (GetErrorString and GetErrorDescription) use functions from dxerr.h, shamelessly stolen from ChiliTomatoNoodle
//The stolen files are as follows: dxerr.h, dxerr.cpp, DXGetErrorDescription.inl, DXGetErrorString.inl and DXTrace.inl

std::string CGraphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorStringA(m_hr);
}

std::string CGraphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescriptionA(m_hr, buf, sizeof(buf));
	return buf;
}

//For draw calls

CGraphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	//Join all info messages with newline into a single string
	for (const auto& m : infoMsgs)
	{
		m_sInfo += m;
		m_sInfo.push_back('\n');
	}
	//Remove final newline if it exists
	if (m_sInfo.empty() == false)
	{
		m_sInfo.pop_back();
	}

	DisplayError();
}

const char* CGraphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	m_sWhatBuffer = oss.str();

	return m_sWhatBuffer.c_str();
}
*/