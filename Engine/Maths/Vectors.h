#pragma once

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

	float x,y,z,w;
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


	float x,y,z;
};

struct Vector2
{
	Vector2() = default;
	Vector2(float xVal, float yVal) 
		:
		x(xVal),
		y(yVal)
	{}

	float x,y;
};