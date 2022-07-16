#include "IndexBuffer.h"
#include "../GraphicsAssertMacros.h"

CIndexBuffer::CIndexBuffer(CRenderer* pRenderer, const std::vector<tIndex>& indices)
	:
	m_IndexCount(indices.size())
{
	GET_INFOMANAGER(pRenderer);

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = m_IndexCount * sizeof(tIndex);
	indexBufferDesc.StructureByteStride = sizeof(tIndex);
	D3D11_SUBRESOURCE_DATA indexBufferSD = {};
	indexBufferSD.pSysMem = indices.data();

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateBuffer(&indexBufferDesc, &indexBufferSD, &m_pIndexBuffer));

}

CIndexBuffer::CIndexBuffer(ID3D11Buffer* pIndexBuffer)
	:
	m_pIndexBuffer(pIndexBuffer)
{

}

void CIndexBuffer::Bind(CRenderer* pRenderer) noexcept
{
	pRenderer->GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}