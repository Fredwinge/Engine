#include "CIndexBuffer.h"
#include "../GraphicsAssertMacros.h"

CIndexBuffer::CIndexBuffer(CGraphics& gfx, const std::vector<unsigned short>& indices)
	:
	m_IndexCount(indices.size())
{
	GET_INFOMANAGER(gfx);

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = m_IndexCount * sizeof(unsigned short);
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indexBufferSD = {};
	indexBufferSD.pSysMem = indices.data();

	GFX_ASSERT_INFO(gfx.GetDevice()->CreateBuffer(&indexBufferDesc, &indexBufferSD, &m_pIndexBuffer));

}

CIndexBuffer::CIndexBuffer(ID3D11Buffer* pIndexBuffer)
	:
	m_pIndexBuffer(pIndexBuffer)
{

}

void CIndexBuffer::Bind(CGraphics& gfx) noexcept
{
	gfx.GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}