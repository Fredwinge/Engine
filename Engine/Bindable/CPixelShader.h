#pragma once
#include "IBindable.h"

class CPixelShader : public IBindable
{
public:

	CPixelShader(CGraphics& gfx, const char* shaderName);
	CPixelShader(ID3D11PixelShader* pPixelShader);

	void Bind(CGraphics& gfx) noexcept override;

protected:

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
};