#include "CTexture.h"
#include "../GraphicsAssertMacros.h"

CTexture::CTexture(CGraphics& gfx, const wchar_t* fileName)
{

	GET_INFOMANAGER(gfx);

	CImage* image = nullptr;

	if (CImage::MakeFromFile(fileName, &image) == CImage::LOAD_SUCCESSFUL)
	{

		//Create texture Resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = image->GetWidth();
		textureDesc.Height = image->GetHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = image->GetFormat();
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = image->GetBuffer();
		sd.SysMemPitch = image->GetBufferSize() / image->GetWidth();

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture2D;

		GFX_ASSERT_INFO(gfx.GetDevice()->CreateTexture2D(&textureDesc, &sd, &m_pTexture2D));

		//Create shader resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		GFX_ASSERT_INFO(DirectX::CreateShaderResourceView(gfx.GetDevice(), image->GetScratch()->GetImage(0, 0, 0), 1u, image->GetScratch()->GetMetadata(), &m_pTextureView));
	}
	else
	{
		//Create texture Resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = 1;
		textureDesc.Height = 1;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		UINT pixelColor = 0x0ee641;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &pixelColor;
		sd.SysMemPitch = sizeof(UINT);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture2D;

		GFX_ASSERT_INFO(gfx.GetDevice()->CreateTexture2D(&textureDesc, &sd, &m_pTexture2D));

		//Create shader resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		GFX_ASSERT_INFO(gfx.GetDevice()->CreateShaderResourceView(m_pTexture2D.Get(), &srvDesc, &m_pTextureView));
	}

	//Don't leave empty image ptrs floating around
	if(image != nullptr)
		delete image;
}

CTexture::CTexture(ID3D11ShaderResourceView* pShaderResource)
	:
	m_pTextureView(pShaderResource)
{

}

void CTexture::Bind(CGraphics& gfx) noexcept
{
	gfx.GetDeviceContext()->PSSetShaderResources(0u, 1u, m_pTextureView.GetAddressOf());
}