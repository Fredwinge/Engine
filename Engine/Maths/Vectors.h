#pragma once

//TODO: Figure out
//1. Is it worth keeping operators for Vector + Float?
//They aren't necessary for functionality but might be more performant

//2. Should the vectors be split up into multiple .h's and .cpp's?
//It might become too much of a clutterfest otherwise

struct Vector4
{
	Vector4() = default;
	Vector4(float xVal, float yVal, float zVal, float wVal)
		:
		x(xVal),
		y(yVal),
		z(zVal),
		w(wVal)
	{}
	Vector4(float val)
		:
		x(val),
		y(val),
		z(val),
		w(val)
	{}

	//Operators
	const Vector4 operator+(const Vector4& v) const;
	const Vector4 operator-(const Vector4& v) const;
	const Vector4 operator*(const Vector4& v) const;
	const Vector4 operator/(const Vector4& v) const;

	Vector4 operator+=(const Vector4& v);
	Vector4 operator-=(const Vector4& v);
	Vector4 operator*=(const Vector4& v);
	Vector4 operator/=(const Vector4& v);

	const Vector4 operator+(const float f) const;
	const Vector4 operator-(const float f) const;
	const Vector4 operator*(const float f) const;
	const Vector4 operator/(const float f) const;

	Vector4 operator+=(const float f);
	Vector4 operator-=(const float f);
	Vector4 operator*=(const float f);
	Vector4 operator/=(const float f);

	float x,y,z,w;

	static const Vector4 Zero;
};

struct Vector3
{
	Vector3() = default;
	Vector3(float xVal, float yVal, float zVal)
		:
		x(xVal),
		y(yVal),
		z(zVal)
	{}
	Vector3(float val)
		:
		x(val),
		y(val),
		z(val)
	{}

	//Operators
	const Vector3 operator+(const Vector3& v) const;
	const Vector3 operator-(const Vector3& v) const;
	const Vector3 operator*(const Vector3& v) const;
	const Vector3 operator/(const Vector3& v) const;

	Vector3 operator+=(const Vector3& v);
	Vector3 operator-=(const Vector3& v);
	Vector3 operator*=(const Vector3& v);
	Vector3 operator/=(const Vector3& v);

	const Vector3 operator+(const float f) const;
	const Vector3 operator-(const float f) const;
	const Vector3 operator*(const float f) const;
	const Vector3 operator/(const float f) const;

	Vector3 operator+=(const float f);
	Vector3 operator-=(const float f);
	Vector3 operator*=(const float f);
	Vector3 operator/=(const float f);

	float x,y,z;

	static const Vector3 Zero;
};

struct Vector2
{
	Vector2() = default;
	Vector2(float xVal, float yVal) 
		:
		x(xVal),
		y(yVal)
	{}
	Vector2(float val)
		:
		x(val),
		y(val)
	{}

	//Operators
	const Vector2 operator+(const Vector2& v) const;
	const Vector2 operator-(const Vector2& v) const;
	const Vector2 operator*(const Vector2& v) const;
	const Vector2 operator/(const Vector2& v) const;

	Vector2 operator+=(const Vector2& v);
	Vector2 operator-=(const Vector2& v);
	Vector2 operator*=(const Vector2& v);
	Vector2 operator/=(const Vector2& v);

	const Vector2 operator+(const float f) const;
	const Vector2 operator-(const float f) const;
	const Vector2 operator*(const float f) const;
	const Vector2 operator/(const float f) const;

	Vector2 operator+=(const float f);
	Vector2 operator-=(const float f);
	Vector2 operator*=(const float f);
	Vector2 operator/=(const float f);

	float x,y;

	static const Vector2 Zero;
};