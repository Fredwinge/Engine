#include "CInputLayout.h"
#include "../GraphicsThrowMacros.h"

CInputLayout::CInputLayout(CGraphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
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