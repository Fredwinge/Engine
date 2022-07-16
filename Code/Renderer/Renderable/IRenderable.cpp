#include "IRenderable.h"
#include "GraphicsAssertMacros.h"
#include "Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo> //Checkup

void IRenderable::Render(CRenderer* pRenderer) /*const*/ //noexcept
{

	RenderInternal(pRenderer);

	/*
	for (auto& b : m_Binds)
	{
		b->Bind(gfx);
	}

	gfx.DrawIndexed(m_pIndexBuffer->GetCount());*/

	pRenderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (m_pRenderMesh)
		m_pRenderMesh->BindBuffers(pRenderer);

	if (m_pMaterial)
		m_pMaterial->BindMaterial(pRenderer);

	pRenderer->DrawIndexed(m_pRenderMesh->GetIdxCount());

}