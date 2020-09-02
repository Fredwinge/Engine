#pragma once
#include "IBindable.h"

class CIndexBuffer : public IBindable
{
public:

	CIndexBuffer(CGraphics& gfx, const std::vector<unsigned short>& indices);

	void Bind(CGraphics& gfx) noexcept override;

	unsigned int GetCount() const noexcept { return m_IndexCount; }

protected:

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	unsigned int m_IndexCount;
};