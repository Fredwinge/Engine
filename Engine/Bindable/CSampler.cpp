#include "CSampler.h"
#include "../GraphicsAssertMacros.h"

CSampler::CSampler(CGraphics& gfx)
{
	GET_INFOMANAGER(gfx);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_ASSERT_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &m_pSampler));

}

void CSampler::Bind(CGraphics& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());
}