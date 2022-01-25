#pragma once
#include "IBindable.h"

class CTopology : public IBindable
{
public:

	CTopology(CRenderer* pRenderer, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(CRenderer* pRenderer) noexcept override;

protected:

	D3D11_PRIMITIVE_TOPOLOGY m_TopologyType;
};