#pragma once
#include "IBindable.h"
#include "../GraphicsAssertMacros.h"

class CConstantBuffer : public IBindable
{
public:

	enum EConstBufferType
	{
		ETYPE_VERTEX = 1,
		ETYPE_PIXEL = 2
	};

	CConstantBuffer(CRenderer* pRenderer, const EConstBufferType eType, const void* pConsts, const uint32 uSize)
		:
		m_eType(eType),
		m_uBufferSize(uSize)
	{
		GET_INFOMANAGER(pRenderer);

		D3D11_BUFFER_DESC cbufferDesc = {};
		cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbufferDesc.MiscFlags = 0u;
		cbufferDesc.ByteWidth = m_uBufferSize;
		cbufferDesc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = pConsts;

		GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateBuffer(&cbufferDesc, &csd, &m_pConstantBuffer));
	}
	CConstantBuffer(CRenderer* pRenderer, const EConstBufferType eType, const uint32 uSize)
		:
		m_eType(eType),
		m_uBufferSize(uSize)
	{
		GET_INFOMANAGER(pRenderer);

		D3D11_BUFFER_DESC cbufferDesc = {};
		cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbufferDesc.MiscFlags = 0u;
		cbufferDesc.ByteWidth = m_uBufferSize;
		cbufferDesc.StructureByteStride = 0u;

		GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateBuffer(&cbufferDesc, nullptr, &m_pConstantBuffer));
	}

	void Update(CRenderer* pRenderer, const void* pConsts)
	{
		GET_INFOMANAGER(pRenderer);

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		GFX_ASSERT_INFO(pRenderer->GetDeviceContext()->Map(m_pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource));
		memcpy(mappedSubresource.pData, pConsts, m_uBufferSize);

		pRenderer->GetDeviceContext()->Unmap(m_pConstantBuffer.Get(), 0u);
	}

	void Bind(CRenderer* pRenderer) noexcept override
	{
		//TODO: We need to be able to bind different slots
		switch (m_eType)
		{
		case ETYPE_VERTEX:
			pRenderer->GetDeviceContext()->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
			break;
		case ETYPE_PIXEL:
			pRenderer->GetDeviceContext()->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
			break;
		}
	}

protected:

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;

	const EConstBufferType m_eType;
	const uint32 m_uBufferSize;
};