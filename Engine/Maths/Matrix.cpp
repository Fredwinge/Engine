#include "Matrix.h"

const Matrix Matrix::Identity;

////////////////////
//MATRIX OPERATORS
////////////////////

//MATRIX + MATRIX
const Matrix Matrix::operator+(const Matrix& mt) const
{
	Matrix mat;

	mat.Left = Left + mt.Left;
	mat.Up   = Up   + mt.Up;
	mat.At   = At   + mt.At;
	mat.Pos  = Pos  + mt.Pos;

	return mat;
}

const Matrix Matrix::operator-(const Matrix& mt) const
{
	Matrix mat;

	mat.Left = Left - mt.Left;
	mat.Up   = Up   - mt.Up;
	mat.At   = At   - mt.At;
	mat.Pos  = Pos  - mt.Pos;

	return mat;
}

const Matrix Matrix::operator*(const Matrix& mt) const
{
	//Turn multiplication matrix to column major
	Matrix mulMat = mt.GetTransposed();
	
	Vector4 vLeft(Left.Dot(mulMat.Left), Left.Dot(mulMat.Up), Left.Dot(mulMat.At), Left.Dot(mulMat.Pos));
	Vector4 vUp	 (Up.Dot(mulMat.Left)  , Up.Dot(mulMat.Up)  , Up.Dot(mulMat.At)  , Up.Dot(mulMat.Pos));
	Vector4 vAt	 (At.Dot(mulMat.Left)  , At.Dot(mulMat.Up)  , At.Dot(mulMat.At)  , At.Dot(mulMat.Pos));
	Vector4 vPos (Pos.Dot(mulMat.Left) , Pos.Dot(mulMat.Up) , Pos.Dot(mulMat.At) , Pos.Dot(mulMat.Pos));

	return Matrix(vLeft, vUp, vAt, vPos);
}

//MATRIX += MATRIX
Matrix Matrix::operator+=(const Matrix& mt)
{
	Left += mt.Left;
	Up   += mt.Up;
	At   += mt.At;
	Pos  += mt.Pos;
	return *this;
}

Matrix Matrix::operator-=(const Matrix& mt)
{
	Left -= mt.Left;
	Up   -= mt.Up;
	At   -= mt.At;
	Pos  -= mt.Pos;
	return *this;
}

Matrix Matrix::operator*=(const Matrix& mt)
{
	//Turn multiplication matrix to column major
	Matrix mulMat = mt.GetTransposed();

	Left = Vector4(Left.Dot(mulMat.Left), Left.Dot(mulMat.Up), Left.Dot(mulMat.At), Left.Dot(mulMat.Pos));
	Up   = Vector4(Up.Dot(mulMat.Left)  , Up.Dot(mulMat.Up)  , Up.Dot(mulMat.At)  , Up.Dot(mulMat.Pos));
	At	 = Vector4(At.Dot(mulMat.Left)  , At.Dot(mulMat.Up)  , At.Dot(mulMat.At)  , At.Dot(mulMat.Pos));
	Pos  = Vector4(Pos.Dot(mulMat.Left) , Pos.Dot(mulMat.Up) , Pos.Dot(mulMat.At) , Pos.Dot(mulMat.Pos));

	return *this;
}

//MATRIX + FLOAT
const Matrix Matrix::operator+(const float f) const
{
	Matrix mat;

	mat.Left = Left + f;
	mat.Up   = Up   + f;
	mat.At   = At   + f;
	mat.Pos  = Pos  + f;

	return mat;
}

const Matrix Matrix::operator-(const float f) const
{
	Matrix mat;

	mat.Left = Left - f;
	mat.Up   = Up   - f;
	mat.At   = At   - f;
	mat.Pos  = Pos  - f;

	return mat;
}

const Matrix Matrix::operator*(const float f) const
{
	Matrix mat;

	mat.Left = Left * f;
	mat.Up   = Up   * f;
	mat.At   = At   * f;
	mat.Pos  = Pos  * f;

	return mat;
}

const Matrix Matrix::operator/(const float f) const
{
	Matrix mat;

	mat.Left = Left / f;
	mat.Up   = Up   / f;
	mat.At   = At   / f;
	mat.Pos  = Pos  / f;

	return mat;
}

//MATRIX += FLOAT
Matrix Matrix::operator+=(const float f)
{
	Left += f;
	Up   += f;
	At   += f;
	Pos  += f;

	return *this;
}

Matrix Matrix::operator-=(const float f)
{
	Left -= f;
	Up   -= f;
	At   -= f;
	Pos  -= f;

	return *this;
}

Matrix Matrix::operator*=(const float f)
{
	Matrix mat;

	Left *= f;
	Up   *= f;
	At   *= f;
	Pos  *= f;

	return *this;
}

Matrix Matrix::operator/=(const float f)
{
	Left /= f;
	Up   /= f;
	At   /= f;
	Pos  /= f;

	return *this;
}

bool Matrix::operator==(const Matrix& mt)
{
	return Left == mt.Left && Up == mt.Up && At == mt.At && Pos == mt.Pos;
}

////////////////////
//MATRIX FUNCTIONS
////////////////////
void Matrix::Transpose()
{
	Vector4 vLeft(Left.x, Up.x, At.x, Pos.x);
	Vector4 vUp  (Left.y, Up.y, At.y, Pos.y);
	Vector4 vAt  (Left.z, Up.z, At.z, Pos.z);
	Vector4 vPos (Left.w, Up.w, At.w, Pos.w);

	Left = vLeft;
	Up   = vUp;
	At   = vAt;
	Pos  = vPos;
}

const Matrix Matrix::GetTransposed() const
{
	Vector4 vLeft(Left.x, Up.x, At.x, Pos.x);
	Vector4 vUp  (Left.y, Up.y, At.y, Pos.y);
	Vector4 vAt  (Left.z, Up.z, At.z, Pos.z);
	Vector4 vPos (Left.w, Up.w, At.w, Pos.w);

	return Matrix(vLeft, vUp, vAt, vPos);
}

//Based on: https://semath.info/src/inverse-cofactor-ex4.html
//TODO: Is this the most performant / optimal way to do this?
void Matrix::Inverse()
{

	float determinant = GetDeterminant();

	Matrix adjugate;
	//LEFT
	adjugate.Left.x =	( (Up.y   * At.z * Pos.w) + (Up.z * At.w * Pos.y)   + (Up.w * At.y * Pos.z)
						- (Up.w   * At.z * Pos.y) - (Up.z * At.y * Pos.w)   - (Up.y * At.w * Pos.z));

	adjugate.Left.y =	(-(Left.y * At.z * Pos.w) - (Left.z * At.w * Pos.y) - (Left.w * At.y * Pos.z)
						+ (Left.w * At.z * Pos.y) + (Left.z * At.y * Pos.w) + (Left.y * At.w * Pos.z));

	adjugate.Left.z =	( (Left.y * Up.z * Pos.w) + (Left.z * Up.w * Pos.y) + (Left.w * Up.y * Pos.z)
						- (Left.w * Up.z * Pos.y) - (Left.z * Up.y * Pos.w) - (Left.y * Up.w * Pos.z));

	adjugate.Left.w =	(-(Left.y * Up.z * At.w)  - (Left.z * Up.w * At.y)  - (Left.w * Up.y * At.z)
						+ (Left.w * Up.z * At.y)  + (Left.z * Up.y * At.w)  + (Left.y * Up.w * At.z));

	//UP
	adjugate.Up.x =		(-(Up.x * At.z * Pos.w)   - (Up.z * At.w * Pos.x)   - (Up.w * At.x * Pos.z)
						+ (Up.w * At.z * Pos.x)   + (Up.z * At.x * Pos.w)   + (Up.x * At.w * Pos.z));

	adjugate.Up.y =		( (Left.x * At.z * Pos.w) + (Left.z * At.w * Pos.x) + (Left.w * At.x * Pos.z)
						- (Left.w * At.z * Pos.x) - (Left.z * At.x * Pos.w) - (Left.x * At.w * Pos.z));

	adjugate.Up.z =		(-(Left.x * Up.z * Pos.w) - (Left.z * Up.w * Pos.x) - (Left.w * Up.x * Pos.z)
						+ (Left.w * Up.z * Pos.x) + (Left.z * Up.x * Pos.w) + (Left.x * Up.w * Pos.z));

	adjugate.Up.w =		( (Left.x * Up.z * At.w)  + (Left.z * Up.w * At.x)  + (Left.w * Up.x * At.z)
						- (Left.w * Up.z * At.x)  - (Left.z * Up.x * At.w)  - (Left.x * Up.w * At.z));

	//AT
	adjugate.At.x =		( (Up.x * At.y * Pos.w)   + (Up.y * At.w * Pos.x)   + (Up.w * At.x * Pos.y)
						- (Up.w * At.y * Pos.x)   - (Up.y * At.x * Pos.w)   - (Up.x * At.w * Pos.y));

	adjugate.At.y =		(-(Left.x * At.y * Pos.w) - (Left.y * At.w * Pos.x) - (Left.w * At.x * Pos.y)
						+ (Left.w * At.y * Pos.x) + (Left.y * At.x * Pos.w) + (Left.x * At.w * Pos.y));

	adjugate.At.z =		( (Left.x * Up.y * Pos.w) + (Left.y * Up.w * Pos.x) + (Left.w * Up.x * Pos.y)
						- (Left.w * Up.y * Pos.x) - (Left.y * Up.x * Pos.w) - (Left.x * Up.w * Pos.y));

	adjugate.At.w =		(-(Left.x * Up.y * At.w)  - (Left.y * Up.w * At.x)  - (Left.w * Up.x * At.y)
						+ (Left.w * Up.y * At.x)  + (Left.y * Up.x * At.w)  + (Left.x * Up.w * At.y));

	//POS
	adjugate.Pos.x =	(-(Up.x * At.y * Pos.z)   - (Up.y * At.z * Pos.x)   - (Up.z * At.x * Pos.y)
						+ (Up.z * At.y * Pos.x)   + (Up.y * At.x * Pos.z)   + (Up.x * At.z * Pos.y));

	adjugate.Pos.y =	( (Left.x * At.y * Pos.z) + (Left.y * At.z * Pos.x) + (Left.z * At.x * Pos.y)
						- (Left.z * At.y * Pos.x) - (Left.y * At.x * Pos.z) - (Left.x * At.z * Pos.y));

	adjugate.Pos.z =	(-(Left.x * Up.y * Pos.z) - (Left.y * Up.z * Pos.x) - (Left.z * Up.x * Pos.y)
						+ (Left.z * Up.y * Pos.x) + (Left.y * Up.x * Pos.z) + (Left.x * Up.z * Pos.y));

	adjugate.Pos.w =	( (Left.x * Up.y * At.z)  + (Left.y * Up.z * At.x)  + (Left.z * Up.x * At.y)
						- (Left.z * Up.y * At.x)  - (Left.y * Up.x * At.z)  - (Left.x * Up.z * At.y));

	*this = adjugate * (1.0f / determinant);

}

const Matrix Matrix::GetInverted() const
{
	float determinant = GetDeterminant();

	Matrix adjugate;
	//LEFT
	adjugate.Left.x =	( (Up.y   * At.z * Pos.w) + (Up.z * At.w * Pos.y)   + (Up.w * At.y * Pos.z)
						- (Up.w   * At.z * Pos.y) - (Up.z * At.y * Pos.w)   - (Up.y * At.w * Pos.z));

	adjugate.Left.y =	(-(Left.y * At.z * Pos.w) - (Left.z * At.w * Pos.y) - (Left.w * At.y * Pos.z)
						+ (Left.w * At.z * Pos.y) + (Left.z * At.y * Pos.w) + (Left.y * At.w * Pos.z));

	adjugate.Left.z =	( (Left.y * Up.z * Pos.w) + (Left.z * Up.w * Pos.y) + (Left.w * Up.y * Pos.z)
						- (Left.w * Up.z * Pos.y) - (Left.z * Up.y * Pos.w) - (Left.y * Up.w * Pos.z));

	adjugate.Left.w =	(-(Left.y * Up.z * At.w)  - (Left.z * Up.w * At.y)  - (Left.w * Up.y * At.z)
						+ (Left.w * Up.z * At.y)  + (Left.z * Up.y * At.w)  + (Left.y * Up.w * At.z));

	//UP
	adjugate.Up.x =		(-(Up.x * At.z * Pos.w)   - (Up.z * At.w * Pos.x)   - (Up.w * At.x * Pos.z)
						+ (Up.w * At.z * Pos.x)   + (Up.z * At.x * Pos.w)   + (Up.x * At.w * Pos.z));

	adjugate.Up.y =		( (Left.x * At.z * Pos.w) + (Left.z * At.w * Pos.x) + (Left.w * At.x * Pos.z)
						- (Left.w * At.z * Pos.x) - (Left.z * At.x * Pos.w) - (Left.x * At.w * Pos.z));

	adjugate.Up.z =		(-(Left.x * Up.z * Pos.w) - (Left.z * Up.w * Pos.x) - (Left.w * Up.x * Pos.z)
						+ (Left.w * Up.z * Pos.x) + (Left.z * Up.x * Pos.w) + (Left.x * Up.w * Pos.z));

	adjugate.Up.w =		( (Left.x * Up.z * At.w)  + (Left.z * Up.w * At.x)  + (Left.w * Up.x * At.z)
						- (Left.w * Up.z * At.x)  - (Left.z * Up.x * At.w)  - (Left.x * Up.w * At.z));

	//AT
	adjugate.At.x =		( (Up.x * At.y * Pos.w)   + (Up.y * At.w * Pos.x)   + (Up.w * At.x * Pos.y)
						- (Up.w * At.y * Pos.x)   - (Up.y * At.x * Pos.w)   - (Up.x * At.w * Pos.y));

	adjugate.At.y =		(-(Left.x * At.y * Pos.w) - (Left.y * At.w * Pos.x) - (Left.w * At.x * Pos.y)
						+ (Left.w * At.y * Pos.x) + (Left.y * At.x * Pos.w) + (Left.x * At.w * Pos.y));

	adjugate.At.z =		( (Left.x * Up.y * Pos.w) + (Left.y * Up.w * Pos.x) + (Left.w * Up.x * Pos.y)
						- (Left.w * Up.y * Pos.x) - (Left.y * Up.x * Pos.w) - (Left.x * Up.w * Pos.y));

	adjugate.At.w =		(-(Left.x * Up.y * At.w)  - (Left.y * Up.w * At.x)  - (Left.w * Up.x * At.y)
						+ (Left.w * Up.y * At.x)  + (Left.y * Up.x * At.w)  + (Left.x * Up.w * At.y));

	//POS
	adjugate.Pos.x =	(-(Up.x * At.y * Pos.z)   - (Up.y * At.z * Pos.x)   - (Up.z * At.x * Pos.y)
						+ (Up.z * At.y * Pos.x)   + (Up.y * At.x * Pos.z)   + (Up.x * At.z * Pos.y));

	adjugate.Pos.y =	( (Left.x * At.y * Pos.z) + (Left.y * At.z * Pos.x) + (Left.z * At.x * Pos.y)
						- (Left.z * At.y * Pos.x) - (Left.y * At.x * Pos.z) - (Left.x * At.z * Pos.y));

	adjugate.Pos.z =	(-(Left.x * Up.y * Pos.z) - (Left.y * Up.z * Pos.x) - (Left.z * Up.x * Pos.y)
						+ (Left.z * Up.y * Pos.x) + (Left.y * Up.x * Pos.z) + (Left.x * Up.z * Pos.y));

	adjugate.Pos.w =	( (Left.x * Up.y * At.z)  + (Left.y * Up.z * At.x)  + (Left.z * Up.x * At.y)
						- (Left.z * Up.y * At.x)  - (Left.y * Up.x * At.z)  - (Left.x * Up.z * At.y));

	return adjugate * (1.0f / determinant);
}

const float Matrix::GetDeterminant() const
{
	return  (Left.x * ((Up.y	* At.z * Pos.w) + (Up.z   * At.w * Pos.y) + (Up.w	* At.y * Pos.z)
					- (	Up.w	* At.z * Pos.y) - (Up.z   * At.y * Pos.w) - (Up.y	* At.w * Pos.z)))
			- (Up.x * ((Left.y	* At.z * Pos.w) + (Left.z * At.w * Pos.y) + (Left.w * At.y * Pos.z)
					- (	Left.w	* At.z * Pos.y) - (Left.z * At.y * Pos.w) - (Left.y * At.w * Pos.z)))
			+ (At.x * ((Left.y	* Up.z * Pos.w) + (Left.z * Up.w * Pos.y) + (Left.w * Up.y * Pos.z)
					- (	Left.w	* Up.z * Pos.y) - (Left.z * Up.y * Pos.w) - (Left.y * Up.w * Pos.z)))
		   - (Pos.x * ((Left.y	* Up.z * At.w)	+ (Left.z * Up.w * At.y)  + (Left.w * Up.y * At.z)
					- (	Left.w	* Up.z * At.y)	- (Left.z * Up.y * At.w)  - (Left.y * Up.w * At.z)));
}