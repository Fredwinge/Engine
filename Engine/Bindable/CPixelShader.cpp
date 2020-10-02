#include "CPixelShader.h"
#include "../GraphicsAssertMacros.h"

CPixelShader::CPixelShader(CGraphics& gfx, LPCWSTR shaderLink)
{
	GET_INFOMANAGER(gfx);

	ID3DBlob* pBlob;

	//D3DReadFileToBlob reads CSO files
	GFX_ASSERT_INFO(D3DReadFileToBlob(shaderLink, &pBlob));

	GFX_ASSERT_INFO(gfx.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader));

	pBlob->Release();
}

CPixelShader::CPixelShader(ID3D11PixelShader* pPixelShader)
	:
	m_pPixelShader(pPixelShader)
{

}

void CPixelShader::Bind(CGraphics& gfx) noexcept
{
	gfx.GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}