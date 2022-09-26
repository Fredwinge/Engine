#include "RenderTarget.h"
#include "GraphicsAssertMacros.h"

CRenderTarget::CRenderTarget(CRenderer* pRenderer, Vec2 vSize, DXGI_FORMAT eFormat)
{
	GET_INFOMANAGER(pRenderer);

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = vSize.x;
	textureDesc.Height = vSize.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = eFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_pTexture));

	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc = {};
	RTVDesc.Format = textureDesc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateRenderTargetView(m_pTexture, &RTVDesc, &m_pRenderTargetView));

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = textureDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateShaderResourceView(m_pTexture, &SRVDesc, &m_pShaderResourceView));



	D3D11_TEXTURE2D_DESC depthStencilTxtDesc = {};

	depthStencilTxtDesc.SampleDesc.Count = 1;
	depthStencilTxtDesc.SampleDesc.Quality = 0;
	depthStencilTxtDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //For depth stencil we would want DXGI_FORMAT_D24_UNORM_S8_UINT, since it's split between depth and stencil
	depthStencilTxtDesc.Width = vSize.x;
	depthStencilTxtDesc.Height = vSize.y;
	depthStencilTxtDesc.ArraySize = 1;					//The amount of textures, we only need 1
	depthStencilTxtDesc.MipLevels = 1;
	depthStencilTxtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTxtDesc.Usage = D3D11_USAGE_DEFAULT;

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateTexture2D(&depthStencilTxtDesc, nullptr, &m_pDepthStencilTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateDepthStencilView(m_pDepthStencilTexture, &depthStencilViewDesc, &m_pDepthStencilView));
}

CRenderTarget::~CRenderTarget()
{
	if (m_pRenderTargetView)
		m_pRenderTargetView->Release();

	if (m_pTexture)
		m_pTexture->Release();

	if (m_pShaderResourceView)
		m_pShaderResourceView->Release();

	if (m_pDepthStencilView != nullptr)
		m_pDepthStencilView->Release();

	if (m_pDepthStencilTexture != nullptr)
		m_pDepthStencilTexture->Release();
}

void CRenderTarget::Bind(CRenderer* pRenderer, uint8 uSlot)
{
	pRenderer->BindRenderTarget(uSlot, m_pRenderTargetView, m_pDepthStencilView);
}

void CRenderTarget::ClearRenderTarget(CRenderer* pRenderer, Vec4 rgba)
{
	//TODO: Convert rgba better?
	pRenderer->GetDeviceContext()->ClearRenderTargetView(m_pRenderTargetView, (float*)&rgba);

	pRenderer->GetDeviceContext()->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}