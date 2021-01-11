#pragma once
#include <DirectXMath.h>
#include "Maths/Vectors.h"
#include "CKeyboard.h"

//TODO: rework

class CCamera
{

public:

	DirectX::XMMATRIX GetMatrix() const noexcept;

	void Reset() noexcept;

	//TODO: MOVE SOMEWHERE ELSE
	void MoveCamera(CKeyboard* pKbd, float deltaTime);

private:

	//DirectX::XMMATRIX m_mProjectionMatrix;

	Vector3 m_vPos = { 0.0f, 0.0f, -20.0f };
	//float phi = 0.0f;
	//float theta = 0.0f;

	float pitch = 0.0f;
	float yaw = 0.0f;
	//float roll = 0.0f;
};