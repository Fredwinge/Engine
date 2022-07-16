#pragma once
#include <math.h>
#include "Matrix.h"
#include "Vectors.h"

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template<typename T>
inline T lerp(T a, T b, T t)
{
	return a + (b - a) * t;
}

template<typename T>
inline T min(T a, T b)
{
	return a < b ? a : b;
}

template<typename T>
inline T max(T a, T b)
{
	return a > b ? a : b;
}

template<typename T>
inline T clamp(T x, T low, T high)
{
	return max(low, min(x, high));
}

template<typename T>
inline T to_rad(T degree)
{
	return (degree * (PI / 180.0f));
}

template<typename T>
inline T to_deg(T radian)
{
	return (radian * (180.0f / PI));
}