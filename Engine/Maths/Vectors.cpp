#include "Vectors.h"

//////////////////////////
//////////////////////////
//VECTOR4
//////////////////////////
//////////////////////////

const Vector4 Vector4::Zero = 0.0f;

//VECTOR4 + VECTOR4
const Vector4 Vector4::operator+(const Vector4& v) const
{
	Vector4 Vec;
	Vec.x = x + v.x;
	Vec.y = y + v.y;
	Vec.z = z + v.z;
	Vec.w = w + v.w;
	return Vec;
}

const Vector4 Vector4::operator-(const Vector4& v) const
{
	Vector4 Vec;
	Vec.x = x - v.x;
	Vec.y = y - v.y;
	Vec.z = z - v.z;
	Vec.w = w - v.w;
	return Vec;
}

const Vector4 Vector4::operator*(const Vector4& v) const
{
	Vector4 Vec;
	Vec.x = x * v.x;
	Vec.y = y * v.y;
	Vec.z = z * v.z;
	Vec.w = w * v.w;
	return Vec;
}

const Vector4 Vector4::operator/(const Vector4& v) const
{
	Vector4 Vec;
	Vec.x = x / v.x;
	Vec.y = y / v.y;
	Vec.z = z / v.z;
	Vec.w = w / v.w;
	return Vec;
}

//VECTOR4(THIS) += VECTOR4
Vector4 Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vector4 Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vector4 Vector4::operator*=(const Vector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

Vector4 Vector4::operator/=(const Vector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

//VECTOR4 + FLOAT
const Vector4 Vector4::operator+(const float f) const
{
	Vector4 Vec;
	Vec.x = x + f;
	Vec.y = y + f;
	Vec.z = z + f;
	Vec.w = w + f;
	return Vec;
}

const Vector4 Vector4::operator-(const float f) const
{
	Vector4 Vec;
	Vec.x = x - f;
	Vec.y = y - f;
	Vec.z = z - f;
	Vec.w = w - f;
	return Vec;
}

const Vector4 Vector4::operator*(const float f) const
{
	Vector4 Vec;
	Vec.x = x * f;
	Vec.y = y * f;
	Vec.z = z * f;
	Vec.w = w * f;
	return Vec;
}

const Vector4 Vector4::operator/(const float f) const
{
	Vector4 Vec;
	Vec.x = x / f;
	Vec.y = y / f;
	Vec.z = z / f;
	Vec.w = w / f;
	return Vec;
}

//VECTOR4(THIS) += FLOAT
Vector4 Vector4::operator+=(const float f)
{
	x += f;
	y += f;
	z += f;
	w += f;
	return *this;
}

Vector4 Vector4::operator-=(const float f)
{
	x -= f;
	y -= f;
	z -= f;
	w -= f;
	return *this;
}

Vector4 Vector4::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

Vector4 Vector4::operator/=(const float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

//////////////////////////
//////////////////////////
//VECTOR3
//////////////////////////
//////////////////////////

const Vector3 Vector3::Zero = 0.0f;

//VECTOR3 + VECTOR3
const Vector3 Vector3::operator+(const Vector3& v) const
{
	Vector3 Vec;
	Vec.x = x + v.x;
	Vec.y = y + v.y;
	Vec.z = z + v.z;
	return Vec;
}

const Vector3 Vector3::operator-(const Vector3& v) const
{
	Vector3 Vec;
	Vec.x = x - v.x;
	Vec.y = y - v.y;
	Vec.z = z - v.z;
	return Vec;
}

const Vector3 Vector3::operator*(const Vector3& v) const
{
	Vector3 Vec;
	Vec.x = x * v.x;
	Vec.y = y * v.y;
	Vec.z = z * v.z;
	return Vec;
}

const Vector3 Vector3::operator/(const Vector3& v) const
{
	Vector3 Vec;
	Vec.x = x / v.x;
	Vec.y = y / v.y;
	Vec.z = z / v.z;
	return Vec;
}

//VECTOR3(THIS) += VECTOR3
Vector3 Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vector3 Vector3::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

//VECTOR3 + FLOAT
const Vector3 Vector3::operator+(const float f) const
{
	Vector3 Vec;
	Vec.x = x + f;
	Vec.y = y + f;
	Vec.z = z + f;
	return Vec;
}

const Vector3 Vector3::operator-(const float f) const
{
	Vector3 Vec;
	Vec.x = x - f;
	Vec.y = y - f;
	Vec.z = z - f;
	return Vec;
}

const Vector3 Vector3::operator*(const float f) const
{
	Vector3 Vec;
	Vec.x = x * f;
	Vec.y = y * f;
	Vec.z = z * f;
	return Vec;
}

const Vector3 Vector3::operator/(const float f) const
{
	Vector3 Vec;
	Vec.x = x / f;
	Vec.y = y / f;
	Vec.z = z / f;
	return Vec;
}

//VECTOR3(THIS) += FLOAT
Vector3 Vector3::operator+=(const float f)
{
	x += f;
	y += f;
	z += f;
	return *this;
}

Vector3 Vector3::operator-=(const float f)
{
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

Vector3 Vector3::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vector3 Vector3::operator/=(const float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

//////////////////////////
//////////////////////////
//VECTOR2
//////////////////////////
//////////////////////////

const Vector2 Vector2::Zero = 0.0f;

//VECTOR2 + VECTOR2
const Vector2 Vector2::operator+(const Vector2& v) const
{
	Vector2 Vec;
	Vec.x = x + v.x;
	Vec.y = y + v.y;
	return Vec;
}

const Vector2 Vector2::operator-(const Vector2& v) const
{
	Vector2 Vec;
	Vec.x = x - v.x;
	Vec.y = y - v.y;
	return Vec;
}

const Vector2 Vector2::operator*(const Vector2& v) const
{
	Vector2 Vec;
	Vec.x = x * v.x;
	Vec.y = y * v.y;
	return Vec;
}

const Vector2 Vector2::operator/(const Vector2& v) const
{
	Vector2 Vec;
	Vec.x = x / v.x;
	Vec.y = y / v.y;
	return Vec;
}

//VECTOR2(this) += VECTOR2
Vector2 Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2 Vector2::operator*=(const Vector2& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

Vector2 Vector2::operator/=(const Vector2& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

//VECTOR2 + FLOAT
const Vector2 Vector2::operator+(const float f) const
{
	Vector2 Vec;
	Vec.x = x + f;
	Vec.y = y + f;
	return Vec;
}

const Vector2 Vector2::operator-(const float f) const
{
	Vector2 Vec;
	Vec.x = x - f;
	Vec.y = y - f;
	return Vec;
}

const Vector2 Vector2::operator*(const float f) const
{
	Vector2 Vec;
	Vec.x = x * f;
	Vec.y = y * f;
	return Vec;
}

const Vector2 Vector2::operator/(const float f) const
{
	Vector2 Vec;
	Vec.x = x / f;
	Vec.y = y / f;
	return Vec;
}

//VECTOR2(THIS) += FLOAT
Vector2 Vector2::operator+=(const float f)
{
	x += f;
	y += f;
	return *this;
}

Vector2 Vector2::operator-=(const float f)
{
	x -= f;
	y -= f;
	return *this;
}

Vector2 Vector2::operator*=(const float f)
{
	x *= f;
	y *= f;
	return *this;
}

Vector2 Vector2::operator/=(const float f)
{
	x /= f;
	y /= f;
	return *this;
}