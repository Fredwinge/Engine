#pragma once
#include "IBindable.h"

class CPixelShader : public IBindable
{
public:

	CPixelShader(CGraphics& gfx, LPCWSTR shaderLink);

	void Bind(CGraphics& gfx) noexcept override;

protected:

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
};