#pragma once
#include "Math/Matrix.h"
#include "Math/Vectors.h"
#include "Input/Keyboard.h"

//TODO: rework

class CCamera
{

public:

	CCamera();
	~CCamera() = default;

	const Matrix GetView() const			{ return m_viewMatrix; }
	const Matrix GetProjection() const		{ return m_projMatrix; }
	const Matrix GetViewProjection() const	{ return m_viewMatrix * m_projMatrix; }

	void SetView(const Matrix view)			{ m_viewMatrix = view; }
	void SetProjection(const Matrix proj)	{ m_projMatrix = proj; }

	void Reset() noexcept;

	//TODO: MOVE SOMEWHERE ELSE
	void MoveCamera(CKeyboard* pKbd, Vector2 deltaMove, float deltaTime);

	const Vec3 GetPosition() const					{ return m_vPos; }

	void SetPosition(const Vector3 pos)				{ m_vPos = pos; }
	void SetRotation(const Vector3 PitchYawRoll)	{ m_vPitchYawRoll = PitchYawRoll; }

private:

	Vector3 m_vPos = { 0.0f, 0.0f, -20.0f };
	Vector3 m_vPitchYawRoll = Vector3::Zero;

	Matrix m_viewMatrix;
	Matrix m_projMatrix;

	Matrix m_worldMatrix;
};