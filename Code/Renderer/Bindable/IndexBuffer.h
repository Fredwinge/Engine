#pragma once
#include "IBindable.h"

typedef uint16 tIndex;

class CIndexBuffer : public IBindable
{
public:

	CIndexBuffer(CRenderer* pRenderer, const std::vector<tIndex>& indices);
	CIndexBuffer(ID3D11Buffer* pIndexBuffer);

	void Bind(CRenderer* pRenderer) noexcept override;

	tIndex GetCount() const noexcept { return m_IndexCount; }

protected:

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	tIndex m_IndexCount;
};