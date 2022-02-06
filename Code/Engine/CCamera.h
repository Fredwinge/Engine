#pragma once
#include "Math\Matrix.h"
#include "Math/Vectors.h"
#include "CKeyboard.h"

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

private:

	Vector3 m_vPos = { 0.0f, 0.0f, -20.0f };


	float pitch = 0.0f;
	float yaw = 0.0f;

	Matrix m_viewMatrix;
	Matrix m_projMatrix;
};