#pragma once
#include <Windows.h>
#include "IException.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include "CDXGIInfoManager.h"

#include <memory>
#include <random>

class CGraphics
{
	//Allow access to private variables for IBindable
	friend class IBindable;
public:

	class Exception : public IException
	{
		using IException::IException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;

		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		const char* GetType() const noexcept override	{ return "Graphics Exception"; }
		HRESULT GetErrorCode() const noexcept			{ return m_hr; }
		std::string GetErrorInfo() const noexcept		{ return m_sInfo; }

	private:
		HRESULT m_hr;
		std::string m_sInfo;
	};
	//DeviceRemovedException is meant to extract information from DXGI_ERROR_DEVICE_REMOVED errors purely
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

	public:
		const char* GetType() { return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)"; }

	private:
		std::string m_sReason;
	};
	//InfoException gives us information from draw calls purely
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override	{ return "Graphics Info Exception"; }
		std::string GetErrorInfo() const noexcept		{ return m_sInfo; }
	private:
		std::string m_sInfo;
	};

	//We need a handle to our window to construct a Windows device object
	CGraphics(HWND hWnd);
	CGraphics(const CGraphics&) = delete;
	CGraphics& operator=(const CGraphics&) = delete;
	~CGraphics();

	void EndFrame();
	void BeginFrame(float r = 0.7f, float g = 0.7f, float b = 1.0f) noexcept;
	void DrawIndexed(unsigned int indexCount);

	//Get - Set Matrixes
	void SetProjection(DirectX::FXMMATRIX proj) noexcept	{ m_mProjectionMatrix = proj; }
	DirectX::XMMATRIX GetProjection() const noexcept		{ return m_mProjectionMatrix; }

	void SetCamera(DirectX::FXMMATRIX cam) noexcept			{ m_mCameraMatrix = cam; }
	DirectX::XMMATRIX GetCamera() const noexcept			{ return m_mCameraMatrix; }

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

	//Projection matrix
	DirectX::XMMATRIX m_mProjectionMatrix;

	//Camera matrix
	DirectX::XMMATRIX m_mCameraMatrix;
};