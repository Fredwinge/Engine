#pragma once
#include "../CGraphics.h"
#include <DirectXMath.h>
#include "EssentialRenderData.h"
//#include "../RenderCallbacks/IRenderCallback.h"

class IRenderCallback;
class IBindable;

class IRenderable
{

public:

	IRenderable() = default;
	IRenderable(const IRenderable&) = delete;
	virtual ~IRenderable() = default;
	
	virtual DirectX::XMMATRIX GetTransformXM() const /*noexcept*/ = 0;
	void Render(CGraphics& gfx);// const; //noexcept; // IS DEBUG
	virtual void Update(float deltaTime) /*noexcept*/ = 0;

	void SetRenderCallback(IRenderCallback* pRenderCallback) { m_pRenderCallback = pRenderCallback; }

protected:

	void AddBind(std::unique_ptr<IBindable> bind); //noexcept;
	void AddIndexBuffer(std::unique_ptr<class CIndexBuffer> ibuf) /*noexcept*/;

	RenderData* m_pRenderData = nullptr;

	IRenderCallback* m_pRenderCallback = nullptr;

private:

	const CIndexBuffer* m_pIndexBuffer = nullptr;
	std::vector <std::unique_ptr<IBindable>> m_Binds;
};