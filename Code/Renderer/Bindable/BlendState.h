#pragma once
#include "IBindable.h"

class CBlendState
{
public:

	CBlendState(CRenderer* pRenderer, bool bAlphaToCoverageEnabled, D3D11_RENDER_TARGET_BLEND_DESC RTBlendDesc);

	~CBlendState()
	{
		if (m_pBlendState)
			m_pBlendState->Release();
	}

	//TODO: Add ability to bind more than one state
	void Bind(CRenderer* pRenderer, uint32 sampleMask = 0xffffffff);

private:

	ID3D11BlendState* m_pBlendState;
};