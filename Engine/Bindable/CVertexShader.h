#pragma once
#include "IBindable.h"

class CVertexShader : public IBindable
{
public:

	CVertexShader(CGraphics& gfx, LPCWSTR link);

	void Bind(CGraphics& gfx) noexcept override;
	ID3DBlob* GetBytecode() const noexcept { return m_pBytecodeBlob.Get(); }

protected:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;

	//Blob needs to be saved as a variable so we can access it for the input layout
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBytecodeBlob;
};