//===========================================================================
//!
//!	@file	Vector.h
//!	@brief	ベクトル
//!
//===========================================================================
#pragma once

// プロトタイプ宣言
struct Vector4;
struct Matrix;

//===========================================================================
//!	３要素ベクトル構造体
//===========================================================================
struct Vector3
{
	// ゼロベクトル
	static const Vector3 ZERO;
	// 1ベクトル
	static const Vector3 ONE;

	static const Vector3 AXIS_X;
	static const Vector3 AXIS_Y;
	static const Vector3 AXIS_Z;
	

	//------------------------------------------------------------
	//!	@name 初期化
	//------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Vector3();

	//! コンストラクタ (xyz指定)
	Vector3(f32 x, f32 y, f32 z);

	//! コンストラクタ
	Vector3(const Vector3& v);

	//! コンストラクタ
	Vector3(const Vector4& v);

	//@}
	//-------------------------------------------------------------
	//!	@name 代入
	//-------------------------------------------------------------
	//@{

	//! 代入
	Vector3&	operator = (const Vector3& v);

	//! 代入
	Vector3&	operator = (const Vector4& v);

	//@}
	//-------------------------------------------------------------
	//!	@name 単項演算子
	//-------------------------------------------------------------
	//@{

	//! 正の値
	Vector3		operator + () const;

	//! 負の値
	Vector3		operator - () const;

	//@}
	//------------------------------------------------------------
	//!	@name 演算
	//------------------------------------------------------------
	//@{

	//! 加算
	Vector3		operator + (const Vector3& v) const;

	//! 減算
	Vector3		operator - (const Vector3& v) const;

	//! 乗算
	Vector3		operator * (const Vector3& v) const;

	//! スケーリング
	Vector3		operator * (f32 scale) const;

	//! スケーリング
	Vector3		operator / (f32 scale) const;
	
	//! 加算
	Vector3&	operator += (const Vector3& v);

	//! 減算
	Vector3&	operator -= (const Vector3& v);

	//! スケーリング
	Vector3&	operator *= (f32 scale);

	//! スケーリング
	Vector3&	operator /= (f32 scale);

	//! 長さの２乗
	f32			lengthSq() const;

	//! ベクトルの長さ
	f32			length() const;

	//! 正規化
	Vector3		normalize() const;

	//@}

	//! 座標変換(平行移動あり)
	Vector4		transform(const Matrix& matrix);

	//! 座標変換(ベクトルの回転。平行移動なし)
	Vector3		transformNormal(const Matrix& matrix);

	//!	内積
	//!	@param	a	ベクトルa
	//!	@param	b	ベクトルb
	//!	@return	内積値
	static f32		dot(Vector3 a, Vector3 b);
	static f32		fdot(float a, float b);

	//	外積
	//!	@param	a	ベクトルa
	//!	@param	b	ベクトルb
	//!	@return	外積値
	static Vector3	cross(Vector3 a, Vector3 b);

	f32			_x;
	f32			_y;
	f32			_z;
};


//===========================================================================
//!	4要素ベクトル
//===========================================================================
struct Vector4
{
	//! デフォルトコンストラクタ
	Vector4()
	{
	}

	//! コンストラクタ(4要素)
	Vector4(f32 x, f32 y, f32 z, f32 w)
	{
		_x = x;
		_y = y;
		_z = z;
		_w = w;
	}

	//! コンストラクタ(Vector3 + W値)
	Vector4(const Vector3& v, f32 w)
	{
		_x = v._x;
		_y = v._y;
		_z = v._z;
		_w = w;
	}

	union {
		struct {
			f32		_x;
			f32		_y;
			f32		_z;
			f32		_w;
		};
		struct {
			f32		_r;
			f32		_g;
			f32		_b;
			f32		_a;
		};
	};
};

//===========================================================================
//	EOF
//===========================================================================
