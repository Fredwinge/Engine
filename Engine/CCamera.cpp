#include "CCamera.h"

namespace dx = DirectX;

dx::XMMATRIX CCamera::GetMatrix() const noexcept
{

	const auto pos = dx::XMVectorSet(x, y, z, 0.0f);/*dx::XMVector3Transform(dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
		dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f));*/

	/*return dx::XMMatrixLookAtLH(pos, dx::XMVectorZero(), 
		dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * dx::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);*/

	dx::XMVECTOR fwdBase = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	const auto lookVec = dx::XMVector3Transform(fwdBase, dx::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f));

	dx::XMFLOAT4 vec1;
	dx::XMFLOAT4 vec2;
	dx::XMStoreFloat4(&vec1, pos);
	dx::XMStoreFloat4(&vec2, lookVec);

	dx::XMFLOAT4 vec3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z, vec1.w + vec2.w);

	const auto target = dx::XMLoadFloat4(&vec3);

	return dx::XMMatrixLookAtLH(pos, target, dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void CCamera::Reset() noexcept
{
	x = 0.0f;
	y = 0.0f;
	z = -20.0f;
	//phi = 0.0f;
	//theta = 0.0f;

	pitch = 0.0f;
	yaw = 0.0f;
	//roll = 0.0f;

	//m_mProjectionMatrix = GetMatrix();
}