#pragma once
#include "IBindable.h"
#include "../GraphicsAssertMacros.h"

//TODO: Figure out what to do with this class
//TODO: Remove template, figure out if update size needs to be the same as the original size,
//if so, then there's little point to having it templateified, just use void* pData and store size,
//so we can assert if the update size is correct

template <typename C>
class CConstantBuffer : public IBindable
{
public:

	//Functions have to be in the header since templates work that way (?)
	void Update(CRenderer* pRenderer, const C& consts)
	{
		GET_INFOMANAGER(pRenderer);

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		GFX_ASSERT_INFO(pRenderer->GetDeviceContext()->Map(m_pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource));
		memcpy(mappedSubresource.pData, &consts, sizeof(consts));

		pRenderer->GetDeviceContext()->Unmap(m_pConstantBuffer.Get(), 0u);
	}

	CConstantBuffer(CRenderer* pRenderer, const C& consts)
	{
		GET_INFOMANAGER(pRenderer);

		D3D11_BUFFER_DESC cbufferDesc = {};
		cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbufferDesc.MiscFlags = 0u;
		cbufferDesc.ByteWidth = sizeof(C);
		cbufferDesc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;

		GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateBuffer(&cbufferDesc, &csd, &m_pConstantBuffer));
	}
	CConstantBuffer(CRenderer* pRenderer)
	{
		GET_INFOMANAGER(pRenderer);

		D3D11_BUFFER_DESC cbufferDesc = {};
		cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbufferDesc.MiscFlags = 0u;
		cbufferDesc.ByteWidth = sizeof(C);
		cbufferDesc.StructureByteStride = 0u;

		GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateBuffer(&cbufferDesc, nullptr, &m_pConstantBuffer));
	}

protected:

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};

//Two classes with individual bind functions are neccessary since vertex and pixel
//constant buffers are bound differently //maybe this can be achieved more optimally???

template <typename C>
class CVertexConstantBuffer : public CConstantBuffer<C>
{
	//This is needed due to some inheritance issues with templates
	using CConstantBuffer<C>::m_pConstantBuffer;

public:

	using CConstantBuffer<C>::CConstantBuffer;

	void Bind(CRenderer* pRenderer) noexcept override
	{
		pRenderer->GetDeviceContext()->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}
};

template <typename C>
class CPixelConstantBuffer : public CConstantBuffer<C>
{
	using CConstantBuffer<C>::m_pConstantBuffer;
	
public:

	using CConstantBuffer<C>::CConstantBuffer;

	void Bind(CRenderer* pRenderer) noexcept override
	{
		pRenderer->GetDeviceContext()->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}
};