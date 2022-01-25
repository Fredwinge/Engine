#include "CTopology.h"

CTopology::CTopology(CRenderer* pRenderer, D3D11_PRIMITIVE_TOPOLOGY type)
	:
	m_TopologyType(type)
{

}

void CTopology::Bind(CRenderer* pRenderer) noexcept
{
	pRenderer->GetDeviceContext()->IASetPrimitiveTopology(m_TopologyType);
}