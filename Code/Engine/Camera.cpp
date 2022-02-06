#include "Camera.h"
#include "Math\Matrix.h"
#include "Math/CommonMath.h"

CCamera::CCamera()
{
	m_viewMatrix = Matrix::Identity;
	m_projMatrix = Matrix::Identity;
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

	pitch += deltaMove.y * 0.1f *deltaTime;
	yaw += deltaMove.x * 0.1f * deltaTime;

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

	Vec4 forward = { 0,0,1, 0 };
	Vector4 right = { 1,0,0,0  };


	//Identical to transposed camRotationMatrix with this
	Matrix rot = Matrix::Identity;

	//Rotate function makes camera chug
	rot.Rotate(Vector3((pitch), (yaw), 0));

	Vector4 target = forward * rot;
	target.Normalize();

	forward *= rot;
	right *= rot;

	m_vPos += (Vector4(right.GetXYZ(), 1) * MoveDir.x).GetXYZ();
	m_vPos += (Vector4(forward.GetXYZ(), 1) * MoveDir.z).GetXYZ();

	target.x += m_vPos.x;
	target.y += m_vPos.y;
	target.z += m_vPos.z;

	Vector3 up = forward.GetXYZ().Cross(right.GetXYZ());

	Vector3 zaxis = (target.GetXYZ() - m_vPos).GetNormalized();
	Vector3 xaxis = (Vector3(0, 1, 0).Cross(zaxis)).GetNormalized();
	Vector3 yaxis = zaxis.Cross(xaxis);

	Matrix viewMatrix = {
		Vector4(       xaxis.x,            yaxis.x,            zaxis.x,      0 ),
		Vector4(       xaxis.y,            yaxis.y,            zaxis.y,      0 ),
		Vector4(       xaxis.z,            yaxis.z,            zaxis.z,      0 ),
		Vector4( -xaxis.Dot(m_vPos ), -yaxis.Dot( m_vPos ), -zaxis.Dot( m_vPos ), 1 )
	};

	SetView(viewMatrix);

}