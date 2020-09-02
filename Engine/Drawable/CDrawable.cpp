#include "CDrawable.h"
#include "../GraphicsThrowMacros.h"
#include "../Bindable/CIndexBuffer.h"
#include <cassert>
#include <typeinfo> //Checkup

void CDrawable::Draw(CGraphics& gfx) const //noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	for (auto& b : GetStaticBinds())
	{
		b->Bind(gfx);
	}

	gfx.DrawIndexed(m_pIndexBuffer->GetCount());
}

void CDrawable::AddBind(std::unique_ptr<IBindable> bind) //noexcept
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(CIndexBuffer));
	binds.push_back(std::move(bind));
}

void CDrawable::AddIndexBuffer(std::unique_ptr<CIndexBuffer> ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
	m_pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}