#include "CVertexBuffer.h"
#include "../GraphicsAssertMacros.h"

CVertexBuffer::CVertexBuffer(ID3D11Buffer* pVertexBuffer)
	:
	m_pVertexBuffer(pVertexBuffer)
{

}

void CVertexBuffer::Bind(CGraphics& gfx) noexcept
{
	const UINT offset = 0u;
	gfx.GetDeviceContext()->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &m_stride, &offset);
}