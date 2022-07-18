#pragma once
#include "Renderer.h"
#include "VertexData.h"
#include "Math/Matrix.h"
#include "RenderMesh.h"
#include "Material.h"

class IBindable;

//TODO: Redesign
class IRenderable
{

public:

	IRenderable() = default;
	IRenderable(const IRenderable&) = delete;
	virtual ~IRenderable() = default;
	
	//TODO: We shouldn't have the set here.
	virtual const Matrix GetWorldMatrix() const = 0;
	virtual void SetWorldMatrix(Matrix m) = 0;
	void Render(CRenderer* pRenderer);
	virtual void Update(float deltaTime) = 0;

	virtual void RenderInternal(CRenderer* pRenderer) = 0; //For updating rendering stuff, separate for different types of IRenderables

protected:

	CRenderMesh* m_pRenderMesh;
	CMaterial* m_pMaterial;

private:

	const CIndexBuffer* m_pIndexBuffer = nullptr;
};