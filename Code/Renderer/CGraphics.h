#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <vector>
#include "CDXGIInfoManager.h"
#include <memory>
#include "GraphicsAssertMacros.h"
#include "Math\Vectors.h"
#include "Core/Core.h"

class CCamera;

class CRenderer
{
	//Allow access to private variables for IBindable
	//friend class IBindable;
public:
	
	//We need a handle to our window to construct a Windows device object
	CRenderer(HWND hWnd, Vector2 wndSize);
	CRenderer(const CRenderer&) = delete;
	CRenderer& operator=(const CRenderer&) = delete;
	~CRenderer();

	void EndFrame();
	void BeginFrame(float r = 0.7f, float g = 0.7f, float b = 1.0f) noexcept;
	void DrawIndexed(unsigned int indexCount);

	//Get - Set Camera
	const CCamera*	GetCamera()						{ return m_pCurrentCamera; }
	void			SetCamera(CCamera* pCam)	{ m_pCurrentCamera = pCam; }


	//Get functions for device & device context in case one wants to create bindables
	//that don't directly correspond to the IBindable classes
	ID3D11Device*			GetDevice()						{ return m_pDevice; }
	ID3D11DeviceContext*	GetDeviceContext()				{ return m_pDeviceContext; }

#ifndef NDEBUG
	CDXGIInfoManager& GetInfoManager() { return m_InfoManager; }
#endif // !NDEBUG

private:

#ifndef NDEBUG
	CDXGIInfoManager m_InfoManager;
#endif // !NDEBUG


	//Used for creating and allocating resources
	ID3D11Device* m_pDevice = nullptr;
	//Used for handling frame buffers
	IDXGISwapChain* m_pSwapChain = nullptr;
	//Used for issuing rendering commands and configuring the pipeline
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

	//Render target view
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	//Depth stencil view
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
	//Depth stencil texture
	ID3D11Texture2D* m_pDepthStencilTexture = nullptr;
	//Depth stencil state
	ID3D11DepthStencilState* m_pDepthStencilState = nullptr;

	//TODO: Should this really be here?
	CCamera* m_pCurrentCamera = nullptr;
};