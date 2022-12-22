#pragma once
struct Matrix;
struct Vector3;

//TODO: Figure out
//1. Is it worth keeping operators for Vector + Float?
//They aren't necessary for functionality but might be more performant

//2. Should the vectors be split up into multiple .h's and .cpp's?
//It might become too much of a clutterfest otherwise

//3. Templateify this?

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
	Vector4(Vector3 xyz, float w);

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

	const Vector4 operator*(const Matrix& mt) const;
	Vector4 operator*=(const Matrix& mt);

	bool operator==(const Vector4& v);
	bool operator!=(const Vector4& v);

	float& operator[] (const size_t i);

	//Functions
	const float			Dot(const Vector4& v) const;
	static const float	Dot(const Vector4& v1, const Vector4& v2);

	void				Normalize();
	const Vector4		GetNormalized() const;

	const float			GetLength() const;
	const float			GetDistanceTo(const Vector4& v) const;

	const Vector3		GetXYZ() const;
	void				SetXYZ(const Vector3& xyz);

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

	const Vector3 operator*(const Matrix& mt) const;
	Vector3 operator*=(const Matrix& mt);

	Vector3 operator-() const { return Vector3(-x, -y, -z); };

	bool operator==(const Vector3& v);
	bool operator!=(const Vector3& v);

	float& operator[] (const size_t i);

	//Functions
	const float			Dot(const Vector3& v) const;
	static const float	Dot(const Vector3& v1, const Vector3& v2);

	const Vector3		Cross(const Vector3& v) const;

	void				Normalize();
	const Vector3		GetNormalized() const;
	
	//TODO: Change to Length() and Distance()?
	const float			GetLength() const;
	const float			GetDistanceTo(const Vector3& v) const;

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

	bool operator==(const Vector2& v);
	bool operator!=(const Vector2& v);

	float& operator[] (const size_t i);

	//Functions
	const float			Dot(const Vector2& v) const;
	static const float	Dot(const Vector2& v1, const Vector2& v2);

	void				Normalize();
	const Vector2		GetNormalized() const;

	const float			GetLength() const;
	const float			GetDistanceTo(const Vector2& v) const;

	float x,y;

	static const Vector2 Zero;
};

typedef Vector4 Vec4;
typedef Vector3 Vec3;
typedef Vector2 Vec2;