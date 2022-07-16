#pragma once
#include "Renderer.h"
#include "EssentialRenderData.h"
#include "Math/Matrix.h"
//#include "../RenderCallbacks/IRenderCallback.h"
#include "RenderMesh.h"
#include "Material.h"

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

	virtual void RenderInternal(CRenderer* pRenderer) = 0; //For updating rendering stuff, separate for different types of IRenderables

	void SetRenderCallback(IRenderCallback* pRenderCallback) { m_pRenderCallback = pRenderCallback; }

protected:

	RenderData* m_pRenderData = nullptr;

	IRenderCallback* m_pRenderCallback = nullptr;

	CRenderMesh* m_pRenderMesh;
	CMaterial* m_pMaterial;

private:

	const CIndexBuffer* m_pIndexBuffer = nullptr;
};