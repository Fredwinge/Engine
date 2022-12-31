#pragma once
#include "Renderable/IRenderable.h"
#include "Math/Vectors.h"
#include "Math/Matrix.h"

//This class is purely meant for debugging point lights.
class CPointLight : public IRenderable
{
public:

	CPointLight(CRenderer* pRenderer, Vec4 vColor, Vec3 vPos, float fRadius);

	void Update(float deltaTime) override;
	const Matrix GetWorldMatrix() const override { return m_WorldMatrix; }
	void SetWorldMatrix(Matrix m) override { m_WorldMatrix = m; };

	void RenderInternal(CRenderer* pRenderer) override;

	const Vec4 GetColor()	{ return m_vColor; }
	const float GetRadius() { return m_fRadius; }

private:

	//TODO: Inherit as base from somewhere
	Matrix m_WorldMatrix;

	Vec4 m_vColor;
	float m_fRadius;

	CConstantBuffer* m_pVertexCBuffer;
	CConstantBuffer* m_pPixelCBuffer;

};