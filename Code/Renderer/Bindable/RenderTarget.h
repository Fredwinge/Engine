#pragma once
#include "IBindable.h"

//TODO: Include depthstencil?
class CRenderTarget
{
public:

	CRenderTarget(CRenderer* pRenderer, Vec2 vSize, DXGI_FORMAT eFormat = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
	~CRenderTarget();

	void Bind(CRenderer* pRenderer, uint8 uSlot);

	void ClearRenderTarget(CRenderer* pRenderer, Vec4 rgba = { 0.0f, 0.0f, 0.0f, 1.0f});

	ID3D11ShaderResourceView* GetShaderResourceView() { return m_pShaderResourceView; }

private:


	//Render target view
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	//Render target texture
	ID3D11Texture2D* m_pTexture;
	//Render target SRV
	ID3D11ShaderResourceView* m_pShaderResourceView = nullptr;

	//Depth stencil view
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
	//Depth stencil texture
	ID3D11Texture2D* m_pDepthStencilTexture = nullptr;

	//TODO: Keep separate?
	//Depth stencil state
	//ID3D11DepthStencilState* m_pDepthStencilState = nullptr;
};