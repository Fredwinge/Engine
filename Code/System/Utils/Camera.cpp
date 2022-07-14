#include "Camera.h"
#include "Math\Matrix.h"
#include "Math/CommonMath.h"

CCamera::CCamera()
{
	m_viewMatrix = Matrix::Identity;
	m_projMatrix = Matrix::Identity;

	m_worldMatrix = Matrix::Identity;
	m_worldMatrix.Pos.z = -20.0f;
}

void CCamera::Reset() noexcept
{
	m_vPos = { 0.0f, 0.0f, -20.0f };
	m_vPitchYawRoll = Vector3::Zero;
}

//Based on: https://www.braynzarsoft.net/viewtutorial/q16390-29-free-look-camera
void CCamera::MoveCamera(CKeyboard* pKbd, Vector2 deltaMove, float deltaTime)
{
	if (pKbd == nullptr)
		return;

	m_vPitchYawRoll.x += deltaMove.y * 0.1f *deltaTime;
	m_vPitchYawRoll.y += deltaMove.x * 0.1f * deltaTime;

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

	Vec4 forward = { 0.0f, 0.0f, 1.0f, 0.0f };
	Vector4 right = { 1.0f, 0.0f, 0.0f, 0.0f };


	//Identical to transposed camRotationMatrix with this
	Matrix rot = Matrix::Identity;

	//Rotate function makes camera chug
	rot.Rotate(m_vPitchYawRoll);

	Vector4 target = forward * rot;
	target.Normalize();

	forward *= rot;
	right *= rot;

	Matrix mI = Matrix::Identity;
	mI *= rot;

	m_vPos += (Vector4(right.GetXYZ(), 1.0f) * MoveDir.x).GetXYZ();
	m_vPos += (Vector4(forward.GetXYZ(), 1.0f) * MoveDir.z).GetXYZ();

	target.x += m_vPos.x;
	target.y += m_vPos.y;
	target.z += m_vPos.z;

	Vector3 zaxis = (target.GetXYZ() - m_vPos).GetNormalized();
	Vector3 xaxis = (Vector3(0.0f, 1.0f, 0.0f).Cross(zaxis)).GetNormalized();
	Vector3 yaxis = zaxis.Cross(xaxis);

	Matrix viewMatrix = {
		Vector4(       xaxis.x,            yaxis.x,            zaxis.x,      0.0f ),
		Vector4(       xaxis.y,            yaxis.y,            zaxis.y,      0.0f ),
		Vector4(       xaxis.z,            yaxis.z,            zaxis.z,      0.0f ),
		Vector4( -xaxis.Dot(m_vPos ), -yaxis.Dot( m_vPos ), -zaxis.Dot( m_vPos ), 1.0f )
	};

	SetView(viewMatrix);

}