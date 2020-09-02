#pragma once
#include "IBindable.h"

class CSampler : public IBindable
{
public:

	CSampler(CGraphics& gfx);

	void Bind(CGraphics& gfx) noexcept override;

private:

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
};