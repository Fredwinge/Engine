#pragma once
#include <math.h>

const double PI = 3.14159265359;

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
inline T DegreesToRadians(T degree)
{
	return (degree * (PI / 180));
}

template<typename T>
inline T RadiansToDegrees(T radian)
{
	return (radian * (180 / PI));
}