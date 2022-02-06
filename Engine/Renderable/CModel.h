#pragma once
#include "IRenderable.h"
#include "../Maths/Vectors.h"
#include "../Maths/Matrix.h"

class CModel : public IRenderable
{
public:

	//TODO: Replace vectors with just ptrs?
	CModel(CRenderer* pRenderer, const char* path);

	void Update(float deltaTime) override;
	Matrix GetWorldMatrix() const override { return m_ModelMatrix; }

private:

	std::vector<VertexData> m_VertexBuffer;

	//TODO: MAKE A BETTER SOLUTION
	//Positional
	float r;
	float roll;
	float pitch;
	float yaw;
	float theta;
	float phi;
	float chi;

	//model transform
	//DirectX::XMFLOAT3X3 mt;

	//TODO: Inherit as base from somewhere
	Matrix m_ModelMatrix;

};