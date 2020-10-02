#pragma once
#include "IRenderable.h"
#include "../Maths/Vectors.h"

class CModel : public IRenderable
{
public:

	//TODO: Replace vectors with just ptrs?
	CModel(CGraphics& rGfx, const char* path);

	void Update(float deltaTime) override;
	DirectX::XMMATRIX GetTransformXM() const override;

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
	DirectX::XMFLOAT3X3 mt;

};