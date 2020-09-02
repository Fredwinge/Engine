#include "CPixelShader.h"
#include "../GraphicsThrowMacros.h"

CPixelShader::CPixelShader(CGraphics& gfx, LPCWSTR shaderLink)
{
	INFOMAN(gfx);

	ID3DBlob* pBlob;

	//D3DReadFileToBlob reads CSO files
	GFX_THROW_INFO(D3DReadFileToBlob(shaderLink, &pBlob));

	GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader));

	pBlob->Release();
}

void CPixelShader::Bind(CGraphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}