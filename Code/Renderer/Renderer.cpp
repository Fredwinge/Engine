#include "Renderer.h"
#include "dxerr/dxerr.h"
#include <sstream>
#include "GraphicsAssertMacros.h"
#include <assert.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


CRenderer::CRenderer(const HWND hWnd, const Vector2 wndSize)
	:
	m_uDirtyBinds(0u)
{
	ZeroMemory(m_aBoundRenderTargets, sizeof(m_aBoundRenderTargets));
	ZeroMemory(m_aBoundDepthStencilViews, sizeof(m_aBoundDepthStencilViews));

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
	
	OutputDebugString("\nCreated swapchain desc");

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
	
	OutputDebugString("\nSet boxes device, context and swapchain");

	ID3D11Resource* pBackBuffer = nullptr;



	//Get the backbuffer
	GFX_ASSERT_INFO(m_pSwapChain->GetBuffer(
		0,								//Index of the buffer we want to get, 0 is the backbuffer
		__uuidof(ID3D11Resource),		//uuid of the interface which we want to recieve onto our subobject(???)
		(LPVOID*)&pBackBuffer));		//** to the object we want to store the backbuffers address in
	
	OutputDebugString("\nGet backbuffer");

	//Create render target view
	GFX_ASSERT_INFO(m_pDevice->CreateRenderTargetView(
		pBackBuffer,					//The resource which holds the render target we want a view on
		nullptr,						//Description for Render target view configuration. nullptr gives us a default one we will use for now
		&m_pRenderTargetView));			//The *ID3D11RenderTargetView which stores the rendertarget
	
	OutputDebugString("\nCreate render target view");

	//Release pBackBuffer since it's no longer needed
	pBackBuffer->Release();

	OutputDebugString("\nReleased backbuffer");

	//Create Depth Stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

	depthStencilDesc.DepthEnable = true;							//Enable depth testing
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//Decides if the depth stencil can be written to, there are only two options: ZERO and ALL
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;				//Determines what kind of comparison is going to be done to decide which pixels are drawn or not

	GFX_ASSERT_INFO(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState));

	OutputDebugString("\nCreated depth stencil state");

	//Bind depth stencil state to the pipeline
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1u);

	D3D11_TEXTURE2D_DESC depthStencilTxtDesc = {};
	
	depthStencilTxtDesc.SampleDesc.Count = 1;
	depthStencilTxtDesc.SampleDesc.Quality = 0;
	depthStencilTxtDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //For depth stencil we would want DXGI_FORMAT_D24_UNORM_S8_UINT, since it's split between depth and stencil
	depthStencilTxtDesc.Width = wndSize.x;
	depthStencilTxtDesc.Height = wndSize.y;
	depthStencilTxtDesc.ArraySize = 1;					//The amount of textures, we only need 1
	depthStencilTxtDesc.MipLevels = 1;
	depthStencilTxtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTxtDesc.Usage = D3D11_USAGE_DEFAULT;

	GFX_ASSERT_INFO(m_pDevice->CreateTexture2D(&depthStencilTxtDesc, nullptr, &m_pDepthStencilTexture));

	OutputDebugString("\nCreated depthstencil texture");

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;

	GFX_ASSERT_INFO(m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture, &depthStencilViewDesc, &m_pDepthStencilView));

	OutputDebugString("\nCreated depthstencil view");

	//TODO: Classify?
	//Configure viewport
	D3D11_VIEWPORT viewPort;
	viewPort.Width = wndSize.x;
	viewPort.Height = wndSize.y;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1u, &viewPort);

	OutputDebugString("\nConfigured viewport");

	//Bind depth stencil view to OM
	m_pDeviceContext->OMSetRenderTargets(1u, &m_pRenderTargetView, m_pDepthStencilView);

	OutputDebugString("\nSet render target");

}

CRenderer::~CRenderer()
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


void CRenderer::EndFrame()
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

void CRenderer::BeginFrame(float r, float g, float b) noexcept
{

	const float color[] = { r, g, b, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	//Gotta clear the depth stencil every frame as well
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0u);

	//TODO: temporary reset, should handle this elsewhere
	m_pDeviceContext->OMSetRenderTargets(1u, &m_pRenderTargetView, m_pDepthStencilView);
}

void CRenderer::DrawIndexed(unsigned int indexCount)
{
	if (m_uDirtyBinds != BIND_NONE)
		ResolveDirtyBinds();

	GFX_ASSERT_INFO_ONLY(m_pDeviceContext->DrawIndexed(indexCount, 0, 0));
}

void CRenderer::SetDefaultRenderTarget()
{
	m_pDeviceContext->OMSetRenderTargets(1u, &m_pRenderTargetView, m_pDepthStencilView);
	//BindRenderTarget(0, m_pRenderTargetView, m_pDepthStencilView);
}

//TODO: Move elsewhere?
void CRenderer::ResolveDirtyBinds()
{
	//TODO: FIX ALL THE WARNINGS CAUSED BY THIS
	if (m_uDirtyBinds & BIND_RENDERTARGET)
	{
		m_pDeviceContext->OMSetRenderTargets(MAX_RENDERTARGET_COUNT, m_aBoundRenderTargets, *m_aBoundDepthStencilViews);
	}

	if (m_uDirtyBinds & BIND_SHADERRESOURCE)
	{
		//TODO: Configure startslot?
		m_pDeviceContext->PSSetShaderResources(0, MAX_SHADERESOURCE_COUNT, m_aBoundShaderResources);
	}

	m_uDirtyBinds = BIND_NONE;
}

void CRenderer::BindRenderTarget(uint8 uSlot, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
{
	assert(uSlot < MAX_RENDERTARGET_COUNT);
	m_aBoundRenderTargets[uSlot] = pRTV;
	m_aBoundDepthStencilViews[uSlot] = pDSV;

	m_uDirtyBinds |= BIND_RENDERTARGET;
}

void CRenderer::BindShaderResource(uint8 uSlot, ID3D11ShaderResourceView* pSRV)
{
	assert(uSlot < MAX_SHADERESOURCE_COUNT);
	m_aBoundShaderResources[uSlot] = pSRV;

	m_uDirtyBinds |= BIND_SHADERRESOURCE;
}