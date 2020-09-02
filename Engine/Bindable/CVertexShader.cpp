#include "CVertexShader.h"
#include "../GraphicsThrowMacros.h"

CVertexShader::CVertexShader(CGraphics& gfx, LPCWSTR shaderLink)
{
	INFOMAN(gfx);

	//D3DReadFileToBlob reads CSO files
	GFX_THROW_INFO(D3DReadFileToBlob(shaderLink, &m_pBytecodeBlob));

	GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(m_pBytecodeBlob->GetBufferPointer(), m_pBytecodeBlob->GetBufferSize(), nullptr, &m_pVertexShader));
}

void CVertexShader::Bind(CGraphics& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
}