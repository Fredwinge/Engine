#include "CTopology.h"

CTopology::CTopology(CGraphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	:
	m_TopologyType(type)
{

}

void CTopology::Bind(CGraphics& gfx) noexcept
{
	gfx.GetDeviceContext()->IASetPrimitiveTopology(m_TopologyType);
}