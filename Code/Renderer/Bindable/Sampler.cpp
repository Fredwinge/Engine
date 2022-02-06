#include "Sampler.h"
#include "../GraphicsAssertMacros.h"

CSampler::CSampler(CRenderer* pRenderer)
{
	GET_INFOMANAGER(pRenderer);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateSamplerState(&samplerDesc, &m_pSampler));

}

CSampler::CSampler(ID3D11SamplerState* pSampler)
	:
	m_pSampler(pSampler)
{

}

void CSampler::Bind(CRenderer* pRenderer) noexcept
{
	pRenderer->GetDeviceContext()->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());
}