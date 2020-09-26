#pragma once
#include "../CGraphics.h"
#include <DirectXMath.h>

class IBindable;

class CDrawable
{
	//CDrawableBase is a friend so it has access to the private variables
	template<class T>
	friend class CDrawableBase;

public:

	CDrawable() = default;
	CDrawable(const CDrawable&) = delete;
	virtual ~CDrawable() = default;
	
	virtual DirectX::XMMATRIX GetTransformXM() const /*noexcept*/ = 0;
	void Draw(CGraphics& gfx) const; //noexcept; // IS DEBUG
	virtual void Update(float deltaTime) /*noexcept*/ = 0;

protected:

	void AddBind(std::unique_ptr<IBindable> bind); //noexcept;
	void AddIndexBuffer(std::unique_ptr<class CIndexBuffer> ibuf) /*noexcept*/;


private:

	const CIndexBuffer* m_pIndexBuffer = nullptr;
	std::vector <std::unique_ptr<IBindable>> m_Binds;
};