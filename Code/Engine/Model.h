#pragma once
#include "Renderable/IRenderable.h"
#include "Math/Vectors.h"
#include "Math/Matrix.h"

class CModel : public IRenderable
{
public:

	//TODO: Replace vectors with just ptrs?
	CModel(CRenderer* pRenderer, const char* path);

	void Update(float deltaTime) override;
	const Matrix GetWorldMatrix() const override { return m_WorldMatrix; }

	void RenderInternal(CRenderer* pRenderer) override;

private:

	std::vector<SVertexData> m_VertexBuffer;

	//TODO: Inherit as base from somewhere
	Matrix m_WorldMatrix;

};