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

void IRenderable::AddBind(std::unique_ptr<IBindable> bind) //noexcept
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(CIndexBuffer));
	m_Binds.push_back(std::move(bind));
}

void IRenderable::AddIndexBuffer(std::unique_ptr<CIndexBuffer> ibuf) /*noexcept*/
{
	assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
	m_pIndexBuffer = ibuf.get();
	m_Binds.push_back(std::move(ibuf));
}