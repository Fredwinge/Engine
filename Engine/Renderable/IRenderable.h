#pragma once
#include "../CGraphics.h"
#include <DirectXMath.h>

class IBindable;

class IRenderable
{

public:

	IRenderable() = default;
	IRenderable(const IRenderable&) = delete;
	virtual ~IRenderable() = default;
	
	virtual DirectX::XMMATRIX GetTransformXM() const /*noexcept*/ = 0;
	void Render(CGraphics& gfx) const; //noexcept; // IS DEBUG
	virtual void Update(float deltaTime) /*noexcept*/ = 0;

protected:

	void AddBind(std::unique_ptr<IBindable> bind); //noexcept;
	void AddIndexBuffer(std::unique_ptr<class CIndexBuffer> ibuf) /*noexcept*/;


private:

	const CIndexBuffer* m_pIndexBuffer = nullptr;
	std::vector <std::unique_ptr<IBindable>> m_Binds;
};