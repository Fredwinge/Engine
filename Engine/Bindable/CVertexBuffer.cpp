#include "CVertexBuffer.h"
#include "../GraphicsAssertMacros.h"

CVertexBuffer::CVertexBuffer(ID3D11Buffer* pVertexBuffer)
	:
	m_pVertexBuffer(pVertexBuffer)
{

}

void CVertexBuffer::Bind(CRenderer* pRenderer) noexcept
{
	const UINT offset = 0u;
	pRenderer->GetDeviceContext()->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &m_stride, &offset);
}