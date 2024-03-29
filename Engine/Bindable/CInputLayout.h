#pragma once
#include "IBindable.h"

class CInputLayout : public IBindable
{
public:

	CInputLayout(CGraphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);
	CInputLayout(ID3D11InputLayout* pInputLayout);

	void Bind(CGraphics& gfx) noexcept override;

protected:

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};