#pragma once
#include "Renderer.h"
#include "EssentialRenderData.h"
#include "Math/Matrix.h"
//#include "../RenderCallbacks/IRenderCallback.h"

class IRenderCallback;
class IBindable;

//TODO: Redesign
class IRenderable
{

public:

	IRenderable() = default;
	IRenderable(const IRenderable&) = delete;
	virtual ~IRenderable() = default;
	
	virtual Matrix GetWorldMatrix() const /*noexcept*/ = 0;
	void Render(CRenderer* pRenderer);// const; //noexcept; // IS DEBUG
	virtual void Update(float deltaTime) /*noexcept*/ = 0;

	void SetRenderCallback(IRenderCallback* pRenderCallback) { m_pRenderCallback = pRenderCallback; }

protected:

	RenderData* m_pRenderData = nullptr;

	IRenderCallback* m_pRenderCallback = nullptr;

private:

	const CIndexBuffer* m_pIndexBuffer = nullptr;
};