#pragma once
#include "IBindable.h"
#include "../CImage.h"

class CTexture : public IBindable
{
public:

	CTexture(CRenderer* pRenderer, const wchar_t* fileName);

	//TODO: Create a CShaderResourceView? rework this into a CShaderResourceView?
	CTexture(ID3D11ShaderResourceView* pTexture);

	void Bind(CRenderer* pRenderer) noexcept override;

private:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
};