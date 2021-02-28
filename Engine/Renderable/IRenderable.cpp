#include "IRenderable.h"
#include "../GraphicsAssertMacros.h"
#include "../Bindable/CIndexBuffer.h"
#include <cassert>
#include <typeinfo> //Checkup

#include "../RenderCallbacks/IRenderCallback.h"

void IRenderable::Render(CGraphics& gfx) /*const*/ //noexcept
{
	/*
	for (auto& b : m_Binds)
	{
		b->Bind(gfx);
	}

	gfx.DrawIndexed(m_pIndexBuffer->GetCount());*/

	m_pRenderData->mWorldMatrix = GetWorldMatrix();

	if (m_pRenderCallback != nullptr)
		m_pRenderCallback->RenderCallback(gfx, m_pRenderData);

}