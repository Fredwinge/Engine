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

	void AddBind(std::unique_ptr<IBindable> bind); //noexcept; // IS DEBUG
	void AddIndexBuffer(std::unique_ptr<class CIndexBuffer> ibuf) /*noexcept*/;


private:

	//TODO: Rework
	//Pure virtual functions since CDrawable needs access to the static binds of its child classes
	virtual const std::vector<std::unique_ptr<IBindable>>* GetStaticBinds() const noexcept = 0;

	const CIndexBuffer* m_pIndexBuffer = nullptr;
	std::vector <std::unique_ptr<IBindable>> m_Binds;
};