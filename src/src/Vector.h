//===========================================================================
//!
//!	@file	Vector.h
//!	@brief	�x�N�g��
//!
//===========================================================================
#pragma once

// �v���g�^�C�v�錾
struct Vector4;
struct Matrix;

//===========================================================================
//!	�R�v�f�x�N�g���\����
//===========================================================================
struct Vector3
{
	// �[���x�N�g��
	static const Vector3 ZERO;
	// 1�x�N�g��
	static const Vector3 ONE;

	static const Vector3 AXIS_X;
	static const Vector3 AXIS_Y;
	static const Vector3 AXIS_Z;
	

	//------------------------------------------------------------
	//!	@name ������
	//------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Vector3();

	//! �R���X�g���N�^ (xyz�w��)
	Vector3(f32 x, f32 y, f32 z);

	//! �R���X�g���N�^
	Vector3(const Vector3& v);

	//! �R���X�g���N�^
	Vector3(const Vector4& v);

	//@}
	//-------------------------------------------------------------
	//!	@name ���
	//-------------------------------------------------------------
	//@{

	//! ���
	Vector3&	operator = (const Vector3& v);

	//! ���
	Vector3&	operator = (const Vector4& v);

	//@}
	//-------------------------------------------------------------
	//!	@name �P�����Z�q
	//-------------------------------------------------------------
	//@{

	//! ���̒l
	Vector3		operator + () const;

	//! ���̒l
	Vector3		operator - () const;

	//@}
	//------------------------------------------------------------
	//!	@name ���Z
	//------------------------------------------------------------
	//@{

	//! ���Z
	Vector3		operator + (const Vector3& v) const;

	//! ���Z
	Vector3		operator - (const Vector3& v) const;

	//! ��Z
	Vector3		operator * (const Vector3& v) const;

	//! �X�P�[�����O
	Vector3		operator * (f32 scale) const;

	//! �X�P�[�����O
	Vector3		operator / (f32 scale) const;
	
	//! ���Z
	Vector3&	operator += (const Vector3& v);

	//! ���Z
	Vector3&	operator -= (const Vector3& v);

	//! �X�P�[�����O
	Vector3&	operator *= (f32 scale);

	//! �X�P�[�����O
	Vector3&	operator /= (f32 scale);

	//! �����̂Q��
	f32			lengthSq() const;

	//! �x�N�g���̒���
	f32			length() const;

	//! ���K��
	Vector3		normalize() const;

	//@}

	//! ���W�ϊ�(���s�ړ�����)
	Vector4		transform(const Matrix& matrix);

	//! ���W�ϊ�(�x�N�g���̉�]�B���s�ړ��Ȃ�)
	Vector3		transformNormal(const Matrix& matrix);

	//!	����
	//!	@param	a	�x�N�g��a
	//!	@param	b	�x�N�g��b
	//!	@return	���ϒl
	static f32		dot(Vector3 a, Vector3 b);
	static f32		fdot(float a, float b);

	//	�O��
	//!	@param	a	�x�N�g��a
	//!	@param	b	�x�N�g��b
	//!	@return	�O�ϒl
	static Vector3	cross(Vector3 a, Vector3 b);

	f32			_x;
	f32			_y;
	f32			_z;
};


//===========================================================================
//!	4�v�f�x�N�g��
//===========================================================================
struct Vector4
{
	//! �f�t�H���g�R���X�g���N�^
	Vector4()
	{
	}

	//! �R���X�g���N�^(4�v�f)
	Vector4(f32 x, f32 y, f32 z, f32 w)
	{
		_x = x;
		_y = y;
		_z = z;
		_w = w;
	}

	//! �R���X�g���N�^(Vector3 + W�l)
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
