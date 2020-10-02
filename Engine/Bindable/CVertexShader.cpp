#include "CVertexShader.h"
#include "../GraphicsAssertMacros.h"

CVertexShader::CVertexShader(CGraphics& gfx, LPCWSTR shaderLink)
{
	GET_INFOMANAGER(gfx);

	//D3DReadFileToBlob reads CSO files
	GFX_ASSERT_INFO(D3DReadFileToBlob(shaderLink, &m_pBytecodeBlob));

	GFX_ASSERT_INFO(gfx.GetDevice()->CreateVertexShader(m_pBytecodeBlob->GetBufferPointer(), m_pBytecodeBlob->GetBufferSize(), nullptr, &m_pVertexShader));
}

CVertexShader::CVertexShader(ID3D11VertexShader* pVertexShader)
	:
	m_pVertexShader(pVertexShader)
{

}

void CVertexShader::Bind(CGraphics& gfx) noexcept
{
	gfx.GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
}