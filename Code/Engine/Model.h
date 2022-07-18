#pragma once
#include "Renderable/IRenderable.h"
#include "Math/Vectors.h"
#include "Math/Matrix.h"

class CModel : public IRenderable
{
public:

	CModel(CRenderer* pRenderer, const char* path);

	//TODO: Temporary constructor, figure out if this should stay
	CModel(CRenderer* pRenderer, CRenderMesh* pMesh);

	void Update(float deltaTime) override;
	const Matrix GetWorldMatrix() const override	{ return m_WorldMatrix; }
	void SetWorldMatrix(Matrix m) override			{ m_WorldMatrix = m; };

	void RenderInternal(CRenderer* pRenderer) override;

private:

	std::vector<SVertexData> m_VertexBuffer;

	//TODO: Inherit as base from somewhere
	Matrix m_WorldMatrix;

};