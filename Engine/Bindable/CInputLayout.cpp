#include "CInputLayout.h"
#include "../GraphicsAssertMacros.h"

CInputLayout::CInputLayout(CGraphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
{
	GET_INFOMANAGER(gfx);

	GFX_ASSERT_INFO(GetDevice(gfx)->CreateInputLayout(
		layout.data(), 
		layout.size(), 
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&m_pInputLayout));
}

void CInputLayout::Bind(CGraphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(m_pInputLayout.Get());
}