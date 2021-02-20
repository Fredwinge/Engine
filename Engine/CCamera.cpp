#include "CCamera.h"

namespace dx = DirectX;

CCamera::CCamera()
{
	m_Matrix = dx::XMMatrixIdentity();
}

void CCamera::Reset() noexcept
{

	m_vPos = { 0.0f, 0.0f, -20.0f };
	//phi = 0.0f;
	//theta = 0.0f;

	pitch = 0.0f;
	yaw = 0.0f;
	//roll = 0.0f;

	//m_mProjectionMatrix = GetMatrix();
}

//Based on: https://www.braynzarsoft.net/viewtutorial/q16390-29-free-look-camera
void CCamera::MoveCamera(CKeyboard* pKbd, Vector2 deltaMove, float deltaTime)
{
	if (pKbd == nullptr)
		return;

	pitch += deltaMove.y * 0.001f;
	yaw += deltaMove.x * 0.001f;

	const float moveDelta = 10.0f * deltaTime;

	Vector3 MoveDir = 0;
	if (pKbd->KeyIsPressed('A'))
		MoveDir.x -= moveDelta;
	if (pKbd->KeyIsPressed('D'))
		MoveDir.x += moveDelta;

	if (pKbd->KeyIsPressed('W'))
		MoveDir.z += moveDelta;
	if (pKbd->KeyIsPressed('S'))
		MoveDir.z -= moveDelta;

	using namespace dx;

	XMVECTOR DefaultForward = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);

	dx::XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);
	dx::XMVECTOR camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix );
	camTarget = XMVector3Normalize(camTarget);

	//XMMATRIX RotateYTempMatrix;
	//RotateYTempMatrix = XMMatrixRotationY(yaw);

	XMVECTOR camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	
	XMVECTOR camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	XMVECTOR camUp = XMVector3Cross(camForward, camRight);

	XMVECTOR camPosition = dx::XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 1.0f);
	camPosition += MoveDir.x * camRight;
	camPosition += MoveDir.z * camForward;

	camTarget = camPosition + camTarget;    

	m_Matrix = XMMatrixLookAtLH( camPosition, camTarget, camUp );

	//TODO: STOP USING DIRECTXMATH, JESUS
	m_vPos.x = XMVectorGetX(camPosition);
	m_vPos.y = XMVectorGetY(camPosition);
	m_vPos.z = XMVectorGetZ(camPosition);

}