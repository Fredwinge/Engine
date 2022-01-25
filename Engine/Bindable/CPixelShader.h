#pragma once
#include "IBindable.h"

class CPixelShader : public IBindable
{
public:

	CPixelShader(CRenderer* pRenderer, const char* shaderName);
	CPixelShader(ID3D11PixelShader* pPixelShader);

	void Bind(CRenderer* pRenderer) noexcept override;

protected:

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
};