#pragma once
#include "CDrawableBase.h"
#include "../Vectors.h"

class CModel : public CDrawableBase<CModel>
{
public:

	//TODO: put somewhere better?
	struct VertexData
	{
		Vector3 Position;
		Vector2 TexCoord;
		Vector3 Normal;
	};

	//TODO: Replace vectors with just ptrs?
	CModel(CGraphics& rGfx, std::vector<VertexData>* pVertexBuffer, std::vector<unsigned short>* pIndices);

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