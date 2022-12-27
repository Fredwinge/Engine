#include "DepthStencil.h"
#include "GraphicsAssertMacros.h"

CDepthStencil::CDepthStencil(CRenderer* pRenderer, bool bTestDepth, D3D11_DEPTH_WRITE_MASK depthMask, D3D11_COMPARISON_FUNC depthFunc)
{
	GET_INFOMANAGER(pRenderer);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

	depthStencilDesc.DepthEnable = bTestDepth;
	depthStencilDesc.DepthWriteMask = depthMask;
	depthStencilDesc.DepthFunc = depthFunc;

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState));
}

//TODO: Add ability to bind more than one state
void CDepthStencil::Bind(CRenderer* pRenderer) noexcept
{
	pRenderer->GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState, 1u);
}