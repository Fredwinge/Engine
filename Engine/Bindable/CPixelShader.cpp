#include "CPixelShader.h"
#include "../GraphicsAssertMacros.h"

CPixelShader::CPixelShader(CGraphics& gfx, LPCWSTR shaderLink)
{
	GET_INFOMANAGER(gfx);

	ID3DBlob* pBlob;

	//D3DReadFileToBlob reads CSO files
	GFX_ASSERT_INFO(D3DReadFileToBlob(shaderLink, &pBlob));

	GFX_ASSERT_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader));

	pBlob->Release();
}

void CPixelShader::Bind(CGraphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}