#pragma once
#include "IBindable.h"

class CDepthStencil : public IBindable
{
public:

	CDepthStencil(CRenderer* pRenderer, bool bTestDepth, D3D11_DEPTH_WRITE_MASK depthMask, D3D11_COMPARISON_FUNC depthFunc);

	~CDepthStencil()
	{
		if (m_pDepthStencilState)
			m_pDepthStencilState->Release();
	}

	//TODO: Add ability to bind more than one state
	void Bind(CRenderer* pRenderer) noexcept override;

private:

	ID3D11DepthStencilState* m_pDepthStencilState = nullptr;
};