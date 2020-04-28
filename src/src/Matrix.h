//===========================================================================
//!
//!	@file	Matrix.h
//!	@brief	行列
//!
//===========================================================================
#pragma once

struct Matrix
{
	// 単位行列
	static const Matrix	IDENTITY;

	// デフォルトコンストラクタ
	Matrix() {}

	//! コンストラクタ
	Matrix(float m11, float m12, float m13, float m14, 
		   float m21, float m22, float m23, float m24,
		   float m31, float m32, float m33, float m34,
		   float m41, float m42, float m43, float m44)
	{
		_m[0][0] = m11;
		_m[0][1] = m12;
		_m[0][2] = m13;
		_m[0][3] = m14;

		_m[1][0] = m21;
		_m[1][1] = m22;
		_m[1][2] = m23;
		_m[1][3] = m24;

		_m[2][0] = m31;
		_m[2][1] = m32;
		_m[2][2] = m33;
		_m[2][3] = m34;
	
		_m[3][0] = m41;
		_m[3][1] = m42;
		_m[3][2] = m43;
		_m[3][3] = m44;
	}

	// 行列 x 行列
	Matrix	operator * (const Matrix& other)
	{
		Matrix	result;
	
		for( int i=0; i<4; ++i ) {
			for( int j=0; j<4; ++j ) {
				result._m[i][j] = (_m[i][0] * other._m[0][j]) +
								 (_m[i][1] * other._m[1][j]) +
								 (_m[i][2] * other._m[2][j]) +
								 (_m[i][3] * other._m[3][j]);
			}
		}

		return result;
	}



	// 単位行列
	static Matrix	identity()
	{
		return Matrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	// 平行移動
	static Matrix	translate(const Vector3& v)
	{
		return Matrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			v._x, v._y, v._z, 1.0f);
	}

	// 平行移動
	static Matrix	translate(f32 x, f32 y, f32 z)
	{
		return Matrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			   x,    y,    z, 1.0f);
	}

	// 拡大縮小
	static Matrix	scale(const Vector3& s)
	{
		return Matrix(
			s._x, 0.0f, 0.0f, 0.0f,
			0.0f, s._y, 0.0f, 0.0f,
			0.0f, 0.0f, s._z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	// X軸中心の回転
	static Matrix	rotateX(float radian)
	{
		float	s = sinf(radian);
		float	c = cosf(radian);

		return Matrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,    s, 0.0f,
			0.0f,   -s,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	// Y軸中心の回転
	static Matrix	rotateY(float radian)
	{
		float	s = sinf(radian);
		float	c = cosf(radian);
	
		return Matrix(
			   c, 0.0f,   -s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			   s, 0.0f,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	// Z軸中心の回転
	static Matrix	rotateZ(float radian)
	{
		float	s = sinf(radian);
		float	c = cosf(radian);
	
		return Matrix(
			   c,    s, 0.0f, 0.0f,
			  -s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	// 任意軸中心の回転
	static Matrix	rotateAxis(
			const Vector3&	v,		// 中心軸
			float			radian)	// 回転角
	{
		float	s = sinf(radian);
		float	c = cosf(radian);

		Vector3	axis = v.normalize();
		float	x = axis._x;
		float	y = axis._y;
		float	z = axis._z;
	
		Matrix	matrix;
		matrix._m[0][0] = x * x * (1.0f - c) + c;
		matrix._m[0][1] = x * y * (1.0f - c) + z * s;
		matrix._m[0][2] = x * z * (1.0f - c) - y * s;
		matrix._m[0][3] = 0.0f;

		matrix._m[1][0] = x * y * (1.0f - c) - z * s;
		matrix._m[1][1] = y * y * (1.0f - c) + c;
		matrix._m[1][2] = y * z * (1.0f - c) + x * s;
		matrix._m[1][3] = 0.0f;

		matrix._m[2][0] = x * z * (1.0f - c) + y * s;
		matrix._m[2][1] = y * z * (1.0f - c) - x * s;
		matrix._m[2][2] = z * z * (1.0f - c) + c;
		matrix._m[2][3] = 0.0f;

		matrix._m[3][0] = 0.0f;
		matrix._m[3][1] = 0.0f;
		matrix._m[3][2] = 0.0f;
		matrix._m[3][3] = 1.0f;

		return matrix;
	}	

	// 渡した行列の回転抜きを返す
	static Matrix	notRot(const Matrix& matrix)
	{
		Matrix result = matrix;

		result._m[0][1] = result._m[0][2] = 0.0f;
		result._m[1][0] = result._m[1][2] = 0.0f;
		result._m[2][0] = result._m[2][1] = 0.0f;
		result._m[0][0] = result._m[1][1] = result._m[2][2] = 1.0f;

		return result;
	}

	Vector3&	axisX()
	{
		return *((Vector3*)&_m[0][0]);
	}

	Vector3&	axisY()
	{
		return *((Vector3*)&_m[1][0]);
	}

	Vector3&	axisZ()
	{
		return *((Vector3*)&_m[2][0]);
	}

	Vector3&	translate()
	{
		return *((Vector3*)&_m[3][0]);
	}

	const Vector3	axisX() const
	{
		return Vector3(_m[0][0], _m[0][1], _m[0][2]);
	}

	const Vector3	axisY() const
	{
		return Vector3(_m[1][0], _m[1][1], _m[1][2]);
	}

	const Vector3	axisZ() const
	{
		return Vector3(_m[2][0], _m[2][1], _m[2][2]);
	}

	const Vector3	translate() const
	{
		return Vector3(_m[3][0], _m[3][1], _m[3][2]);
	}

	float	_m[4][4];


	static Matrix wolrdMatrixTransform(const Vector3& rote, const Vector3& position)
	{
		Matrix result;

		result =  Matrix::rotateX(DEG_TO_RAD(rote._x))
				* Matrix::rotateY(DEG_TO_RAD(rote._y))
				* Matrix::rotateZ(DEG_TO_RAD(rote._z))
				* Matrix::translate(position);
		
		return result;
	}

	// 姿勢行列を求める
	static Matrix posture(const Vector3& position, const Vector3& lookAt, const Vector3& worldUp)
	{
		Vector3 axisZ = (position - lookAt);
		Vector3 axisX = Vector3::cross(worldUp, axisZ);
		Vector3 axisY = Vector3::cross(axisZ, axisX);

		axisX = axisX.normalize();
		axisY = axisY.normalize();
		axisZ = axisZ.normalize();

		Matrix matrix;
		matrix._m[0][0] = axisX._x;
		matrix._m[0][1] = axisX._y;
		matrix._m[0][2] = axisX._z;
		matrix._m[0][3] = 0.0f;

		matrix._m[1][0] = axisY._x;
		matrix._m[1][1] = axisY._y;
		matrix._m[1][2] = axisY._z;
		matrix._m[1][3] = 0.0f;

		matrix._m[2][0] = axisZ._x;
		matrix._m[2][1] = axisZ._y;
		matrix._m[2][2] = axisZ._z;
		matrix._m[2][3] = 0.0f;

		matrix._m[3][0] = position._x;
		matrix._m[3][1] = position._y;
		matrix._m[3][2] = position._z;
		matrix._m[3][3] = 1.0f;

		return matrix;
	}

	// 直交座標系の逆行列を求める
	Matrix inverseCoord(void) const
	{
		Vector3 x = axisX();
		Vector3 y = axisY();
		Vector3 z = axisZ();
		Vector3 p = translate();

		f32 a = -Vector3::dot( p, x );
		f32 b = -Vector3::dot( p, y );
		f32 c = -Vector3::dot( p, z );

		return Matrix(
			x._x, y._x, z._x, 0.0f,
			x._y, y._y, z._y, 0.0f,
			x._z, y._z, z._z, 0.0f,
			   a,    b,    c, 1.0f
		);
	}

};

//===========================================================================
//	EOF
//===========================================================================
