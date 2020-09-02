#pragma once
#include <DirectXMath.h>

class CCamera
{

public:

	DirectX::XMMATRIX GetMatrix() const noexcept;

	void Reset() noexcept;

private:

	DirectX::XMMATRIX m_mProjectionMatrix;

	float x = 0.0f;
	float y = 0.0f;
	float z = -20.0f;
	//float phi = 0.0f;
	//float theta = 0.0f;

	float pitch = 0.0f;
	float yaw = 0.0f;
	//float roll = 0.0f;
};