#pragma once
#include "IRenderable.h"
#include <random>


class CBox : public IRenderable
{
public:

	CBox(CGraphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>&adist,
		std::uniform_real_distribution<float>&ddist,
		std::uniform_real_distribution<float>&odist,
		std::uniform_real_distribution<float>&rdist,
		std::uniform_real_distribution<float>&bdist);

	void Update(float deltaTime) /*noexcept*/ override;
	DirectX::XMMATRIX GetWorldMatrix() const /*noexcept*/ override;

private:
	//Positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	//speed delta/s
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;

	//model transform
	DirectX::XMFLOAT3X3 mt;
};