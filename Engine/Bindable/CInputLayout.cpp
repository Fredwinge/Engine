#include "CInputLayout.h"
#include "../GraphicsAssertMacros.h"

CInputLayout::CInputLayout(CGraphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
{
	GET_INFOMANAGER(gfx);

	GFX_ASSERT_INFO(gfx.GetDevice()->CreateInputLayout(
		layout.data(), 
		layout.size(), 
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&m_pInputLayout));
}

CInputLayout::CInputLayout(ID3D11InputLayout* pInputLayout)
	:
	m_pInputLayout(pInputLayout)
{

}

void CInputLayout::Bind(CGraphics& gfx) noexcept
{
	gfx.GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());
}