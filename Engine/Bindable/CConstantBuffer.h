#pragma once
#include "IBindable.h"
#include "../GraphicsAssertMacros.h"

template <typename C>
class CConstantBuffer : public IBindable
{
public:

	//Functions have to be in the header since templates work that way (?)
	void Update(CGraphics& gfx, const C& consts)
	{
		INFOMAN(gfx);

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		GFX_ASSERT_INFO(GetContext(gfx)->Map(m_pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource));
		memcpy(mappedSubresource.pData, &consts, sizeof(consts));

		GetContext(gfx)->Unmap(m_pConstantBuffer.Get(), 0u);
	}

	CConstantBuffer(CGraphics& gfx, const C& consts)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbufferDesc = {};
		cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbufferDesc.MiscFlags = 0u;
		cbufferDesc.ByteWidth = sizeof(C);
		cbufferDesc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;

		GFX_ASSERT_INFO(GetDevice(gfx)->CreateBuffer(&cbufferDesc, &csd, &m_pConstantBuffer));
	}
	CConstantBuffer(CGraphics& gfx)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbufferDesc = {};
		cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbufferDesc.MiscFlags = 0u;
		cbufferDesc.ByteWidth = sizeof(C);
		cbufferDesc.StructureByteStride = 0u;

		GFX_ASSERT_INFO(GetDevice(gfx)->CreateBuffer(&cbufferDesc, nullptr, &m_pConstantBuffer));
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
	//for the bind function to work, this->GetContext(gfx)... also works
	using CConstantBuffer<C>::m_pConstantBuffer;
	using IBindable::GetContext;

public:

	using CConstantBuffer<C>::CConstantBuffer;

	void Bind(CGraphics& gfx) noexcept override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}
};

template <typename C>
class CPixelConstantBuffer : public CConstantBuffer<C>
{
	using CConstantBuffer<C>::m_pConstantBuffer;
	using IBindable::GetContext;
	
public:

	using CConstantBuffer<C>::CConstantBuffer;

	void Bind(CGraphics& gfx) noexcept override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}
};