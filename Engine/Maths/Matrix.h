#pragma once
#include "Vectors.h"

//Row major
struct Matrix
{

	Matrix()
		:
		Left(1.0f, 0.0f, 0.0f, 0.0f),
		Up(0.0f, 1.0f, 0.0f, 0.0f),
		At(0.0f, 0.0f, 1.0f, 0.0f),
		Pos(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	Matrix(Vector4 vLeft, Vector4 vUp, Vector4 vAt, Vector4 vPos)
		:
		Left(vLeft),
		Up(vUp),
		At(vAt),
		Pos(vPos)
	{}

	Matrix(float xx, float xy, float xz, float xw,
		float yx, float yy, float yz, float yw,
		float zx, float zy, float zz, float zw,
		float wx, float wy, float wz, float ww)
		:
		Left(xx, xy, xz, xw),
		Up(yx, yy, yz, yw),
		At(zx, zy, zz, zw),
		Pos(wx, wy, wz, ww)
	{}

	//Operators
	const Matrix operator+(const Matrix& mt) const;
	const Matrix operator-(const Matrix& mt) const;
	const Matrix operator*(const Matrix& mt) const;

	Matrix operator+=(const Matrix& mt);
	Matrix operator-=(const Matrix& mt);
	Matrix operator*=(const Matrix& mt);

	const Matrix operator+(const float f) const;
	const Matrix operator-(const float f) const;
	const Matrix operator*(const float f) const;
	const Matrix operator/(const float f) const;

	Matrix operator+=(const float f);
	Matrix operator-=(const float f);
	Matrix operator*=(const float f);
	Matrix operator/=(const float f);

	bool operator==(const Matrix & mt);

	Vector4& operator[] (const size_t i);

	//Functions
	void			Transpose();
	const Matrix	GetTransposed() const;

	void			Invert();
	const Matrix	GetInverted() const;

	const float		GetDeterminant() const;
	const bool		IsInvertible() const	{ return GetDeterminant() != 0.0f; }

	const Matrix	GetAdjugate() const;

	void Rotate(const Vector3 rollPitchYaw);
	void SetRotation(const Vector3 rollPitchYaw);

	Vector4 Left;
	Vector4 Up;
	Vector4 At;
	Vector4 Pos;

	static const Matrix Identity;
};