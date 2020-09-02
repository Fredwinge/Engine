#pragma once
#include "IBindable.h"

class CTopology : public IBindable
{
public:

	CTopology(CGraphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(CGraphics& gfx) noexcept override;

protected:

	D3D11_PRIMITIVE_TOPOLOGY m_TopologyType;
};