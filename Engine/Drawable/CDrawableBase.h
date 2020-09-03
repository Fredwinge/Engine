#pragma once
#include "CDrawable.h"
#include "../Bindable/CIndexBuffer.h"

//The purpose of CDrawableBase is to hold a static vector with required values for drawable objects
//which we intend to initialize more than one of, this way we can reduce the amount of initializations done in the constructor
//improving performance in the process

template<class T>
class CDrawableBase : public CDrawable
{
protected:

	//If there are no Bindables in s_StaticBinds then we assume it hasn't been initialized
	//Not optimal for the most optimized systems
	static bool IsStaticInitialized() noexcept	{ return !s_StaticBinds.empty(); };

	static void AddStaticBind(std::unique_ptr<IBindable> pBindable)
	{
		assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*pBindable) != typeid(m_pIndexBuffer));
		s_StaticBinds.push_back(std::move(pBindable));
	}

	void AddStaticIndexBuffer(std::unique_ptr<CIndexBuffer> pIndexBuffer)
	{
		assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
		m_pIndexBuffer = pIndexBuffer.get();
		s_StaticBinds.push_back(std::move(pIndexBuffer));
	}

	void SetIndexFromStatic()
	{
		//asserts to make sure you dont accidentally add multiple index buffers like a complete baboon
		assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
		for (const auto& b : s_StaticBinds)
		{
			if (const auto p = dynamic_cast<CIndexBuffer*>(b.get()))
			{
				m_pIndexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && m_pIndexBuffer == nullptr);
	}

private:

	const std::vector<std::unique_ptr<IBindable>>& GetStaticBinds() const noexcept { return s_StaticBinds; }

	static std::vector<std::unique_ptr<IBindable>> s_StaticBinds;

};

//s_StaticBinds needs to be declared
template<class T>
std::vector<std::unique_ptr<IBindable>> CDrawableBase<T>::s_StaticBinds;