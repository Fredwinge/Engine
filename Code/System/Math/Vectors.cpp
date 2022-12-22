#include "Vectors.h"
#include "Matrix.h"
#include "CommonMath.h"
#include <assert.h>

//////////////////////////
//////////////////////////
//VECTOR4
//////////////////////////
//////////////////////////

const Vector4 Vector4::Zero = 0.0f;

//CONSTRUCTOR
Vector4::Vector4(Vector3 xyz, float w)
	:
	x(xyz.x),
	y(xyz.y),
	z(xyz.z),
	w(w)
{
}

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

//VECTOR4 * MATRIX
const Vector4 Vector4::operator*(const Matrix& mt) const
{
	Vector4 vec;
	vec.x = mt.Left.x * x	+ mt.Left.y * y + mt.Left.z * z + mt.Left.w * w;
	vec.y = mt.Up.x * x		+ mt.Up.y * y	+ mt.Up.z * z	+ mt.Up.w * w;
	vec.z = mt.At.x * x		+ mt.At.y * y	+ mt.At.z * z	+ mt.At.w * w;
	vec.w = mt.Pos.x * x	+ mt.Pos.y * y	+ mt.Pos.z * z	+ mt.Pos.w * w;
	return vec;
}

Vector4 Vector4::operator*=(const Matrix& mt)
{
	Vector4 vec;
	vec.x = mt.Left.x * x	+ mt.Left.y * y + mt.Left.z * z + mt.Left.w * w;
	vec.y = mt.Up.x * x		+ mt.Up.y * y	+ mt.Up.z * z	+ mt.Up.w * w;
	vec.z = mt.At.x * x		+ mt.At.y * y	+ mt.At.z * z	+ mt.At.w * w;
	vec.w = mt.Pos.x * x	+ mt.Pos.y * y	+ mt.Pos.z * z	+ mt.Pos.w * w;
	*this = vec;
	return *this;
}

//VECTOR4 == VECTOR4
bool Vector4::operator==(const Vector4& v)
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

//VECTOR4 != VECTOR4
bool Vector4::operator!=(const Vector4& v)
{
	return !(x == v.x && y == v.y && z == v.z && w == v.w);
}

//VECTOR4[]
float& Vector4::operator[](const size_t i)
{
	assert(i < 4);
	return *(&x + i);
}

//VECTOR4 FUNCTIONS
const float Vector4::Dot(const Vector4& v) const
{
	return (x * v.x) + (y * v.y) + (z * v.z) + (w * v.w);
}

const float Vector4::Dot(const Vector4& v1, const Vector4& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

void Vector4::Normalize()
{
	*this /= GetLength();
}

const Vector4 Vector4::GetNormalized() const
{
	return *this / GetLength();
}

const float Vector4::GetLength() const
{
	//TODO: More effiecent sqrt function?
	return sqrtf(x * x + y * y + z * z + w * w);
}

const float Vector4::GetDistanceTo(const Vector4& v) const
{
	Vector4 diff = *this - v;
	return diff.GetLength();
}

const Vector3 Vector4::GetXYZ() const				
{ 
	return Vector3(x, y, z); 
}
void Vector4::SetXYZ(const Vector3& xyz)			
{ 
	x = xyz.x; 
	y = xyz.y; 
	z = xyz.z; 
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

//VECTOR3 * MATRIX
const Vector3 Vector3::operator*(const Matrix& mt) const
{
	Vector4 mulVector(x, y, z, 1.0f);
	mulVector *= mt;
	return Vector3(mulVector.x, mulVector.y, mulVector.z) / mulVector.w;
}

Vector3 Vector3::operator*=(const Matrix& mt)
{
	Vector4 mulVector(x, y, z, 1.0f);
	mulVector *= mt;
	*this = Vector3(mulVector.x, mulVector.y, mulVector.z) / mulVector.w;
	return *this;
}

//VECTOR3 == VECTOR3
bool Vector3::operator==(const Vector3& v)
{
	return x == v.x && y == v.y && z == v.z;
}

//VECTOR3 != VECTOR3
bool Vector3::operator!=(const Vector3& v)
{
	return !(x == v.x && y == v.y && z == v.z);
}

//VECTOR3[]
float& Vector3::operator[](const size_t i)
{
	assert(i < 3);
	return *(&x + i);
}

//VECTOR3 FUNCTIONS
const float Vector3::Dot(const Vector3& v) const
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

const float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

const Vector3 Vector3::Cross(const Vector3& v) const
{
	Vector3 cross;
	cross.x = y * v.z - z * v.y;
	cross.y = z * v.x - x * v.z;
	cross.z = x * v.y - y * v.x;

	return cross;
}

void Vector3::Normalize()
{
	*this /= GetLength();
}

const Vector3 Vector3::GetNormalized() const
{
	return *this / GetLength();
}

const float Vector3::GetLength() const
{
	//TODO: More effiecent sqrt function?
	return sqrtf(x * x + y * y + z * z);
}

const float Vector3::GetDistanceTo(const Vector3& v) const
{
	Vector3 diff = *this - v;
	return diff.GetLength();
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

//VECTOR2 == VECTOR2
bool Vector2::operator==(const Vector2& v)
{
	return x == v.x && y == v.y;
}

//VECTOR2 != VECTOR2
bool Vector2::operator!=(const Vector2& v)
{
	return !(x == v.x && y == v.y);
}

//VECTOR2[]
float& Vector2::operator[](const size_t i)
{
	assert(i < 2);
	return *(&x + i);
}

//VECTOR2 FUNCTIONS
const float Vector2::Dot(const Vector2& v) const
{
	return (x * v.x) + (y * v.y);
}

const float Vector2::Dot(const Vector2& v1, const Vector2& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

void Vector2::Normalize()
{
	*this /= GetLength();
}

const Vector2 Vector2::GetNormalized() const
{
	return *this / GetLength();
}

const float Vector2::GetLength() const
{
	//TODO: More effiecent sqrt function?
	return sqrtf(x * x + y * y);
}

const float Vector2::GetDistanceTo(const Vector2& v) const
{
	Vector2 diff = *this - v;
	return diff.GetLength();
}