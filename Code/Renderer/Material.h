#pragma once
#include "Bindable\BindableBase.h"

class CMaterial
{
public:
	//TODO: We should be loading from a material file or something instead.
	CMaterial() = delete;
	CMaterial(CRenderer* pRenderer, const char* sVertexShader, const char* sPixelShader);

	~CMaterial();

	void BindMaterial(CRenderer* pRenderer);

private:
	CVertexShader* m_pVertexShader;
	CPixelShader* m_pPixelShader;

	CInputLayout* m_pInputLayout;

};