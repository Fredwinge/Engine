#pragma once
#include <DirectXMath.h>
#include "Maths/Vectors.h"
#include "CKeyboard.h"

//TODO: rework

class CCamera
{

public:

	CCamera();
	~CCamera() = default;

	DirectX::XMMATRIX GetMatrix() const { return m_Matrix; };

	void Reset() noexcept;

	//TODO: MOVE SOMEWHERE ELSE
	void MoveCamera(CKeyboard* pKbd, Vector2 deltaMove, float deltaTime);

private:

	//DirectX::XMMATRIX m_mProjectionMatrix;

	DirectX::XMMATRIX m_Matrix;

	Vector3 m_vPos = { 0.0f, 0.0f, -20.0f };
	//float phi = 0.0f;
	//float theta = 0.0f;

	float pitch = 0.0f;
	float yaw = 0.0f;
	//float roll = 0.0f;
};