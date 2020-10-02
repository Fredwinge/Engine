#include "CTransformCBuf.h"

std::unique_ptr<CVertexConstantBuffer<DirectX::XMMATRIX>> CTransformCBuf::m_pVCBuf;

CTransformCBuf::CTransformCBuf(CGraphics& gfx, const IRenderable& parent)
	:
	m_rParent(parent)
{
	if (m_pVCBuf == nullptr)
	{
		m_pVCBuf = std::make_unique<CVertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void CTransformCBuf::Bind(CGraphics& gfx) noexcept
{
	m_pVCBuf->Update(gfx, DirectX::XMMatrixTranspose(m_rParent.GetTransformXM() * gfx.GetCamera() * gfx.GetProjection()));
	m_pVCBuf->Bind(gfx);
}
