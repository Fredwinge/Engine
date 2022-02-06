#pragma once
#include "IBindable.h"

class CSampler : public IBindable
{
public:

	CSampler(CRenderer* pRenderer);
	CSampler(ID3D11SamplerState* pSampler);

	void Bind(CRenderer* pRenderer) noexcept override;

private:

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
};