#include "CInputLayout.h"
#include "../GraphicsAssertMacros.h"

CInputLayout::CInputLayout(CRenderer* pRenderer, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
{
	GET_INFOMANAGER(pRenderer);

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateInputLayout(
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

void CInputLayout::Bind(CRenderer* pRenderer) noexcept
{
	pRenderer->GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());
}