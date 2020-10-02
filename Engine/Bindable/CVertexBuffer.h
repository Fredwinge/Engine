#pragma once
#include "IBindable.h"

class CVertexBuffer : public IBindable
{
public:
	//Constructor needs to be in header for whatever reason
	template<class V>
	CVertexBuffer(CGraphics& gfx, const std::vector<V>& vertices)
		:
		m_stride(sizeof(V))
	{
		GET_INFOMANAGER(gfx);

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.CPUAccessFlags = 0u;
		vertexBufferDesc.MiscFlags = 0u;
		vertexBufferDesc.ByteWidth = UINT(sizeof(V) * vertices.size());
		vertexBufferDesc.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
		vertexSubresourceData.pSysMem = vertices.data();

		GFX_ASSERT_INFO(gfx.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &m_pVertexBuffer));
	}

	CVertexBuffer(ID3D11Buffer* pVertexBuffer);

	void Bind(CGraphics& gfx) noexcept override;

protected:

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;

	//TODO: Remove?
	unsigned int m_stride;
};