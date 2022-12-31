#include "BlendState.h"
#include "GraphicsAssertMacros.h"

CBlendState::CBlendState(CRenderer* pRenderer, bool bAlphaToCoverageEnabled, D3D11_RENDER_TARGET_BLEND_DESC RTBlendDesc)
{
	GET_INFOMANAGER(pRenderer);

	D3D11_BLEND_DESC blendDesc = {};

	blendDesc.AlphaToCoverageEnable = bAlphaToCoverageEnabled;
	blendDesc.IndependentBlendEnable = false; //TODO: Allow multiple different blend states to be used.
	//We only set the blendstate for slot 0 since IndependentBlendEnable is false anyways.
	blendDesc.RenderTarget[0] = RTBlendDesc;


	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateBlendState(&blendDesc, &m_pBlendState));
}

void CBlendState::Bind(CRenderer* pRenderer, uint32 sampleMask)
{
	float blendFactor[4] = { 0.0f };
	pRenderer->GetDeviceContext()->OMSetBlendState(m_pBlendState, blendFactor, sampleMask);
}