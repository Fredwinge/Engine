#pragma once
#include "IBindable.h"
#include "../CImage.h"

class CTexture : public IBindable
{
public:

	CTexture(CGraphics& gfx, const wchar_t* fileName);

	void Bind(CGraphics& gfx) noexcept override;

private:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
};