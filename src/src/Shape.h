//================================================================================
//!
//! @file	Shape.h
//! @file	�`���`
//! @auther
//!
//================================================================================
#pragma once

enum class CollisionType : int
{
	None = 0,	//!< �g���Ă��Ȃ�
	Sphare,		//!< ��
	Capsule,	//!< �J�v�Z��
	Line,		//!< ��
	Plane,		//!< ����
	Triangle,	//!< �O�p�|���S��
	Rectangle,	//!< ��`��
	Box			//!< ��
};

struct BaseCollisionType
{
	BaseCollisionType()
	: _collisionType(CollisionType::None)
	{
	}

	virtual ~BaseCollisionType() {};

	CollisionType getCollisionType()
	{
		return _collisionType;
	}

protected:
	CollisionType _collisionType;
};

//================================================================================
//! ��	Sphere
//================================================================================
struct Sphere : public BaseCollisionType
{
	Vector3		_position;
	f32			_radius;	//!< ���a

	//! �f�t�H���g�R���X�g���N�^
	Sphere() {}

	//! �R���X�g���N�^
	Sphere(const Vector3& position, f32 radius)
	: _radius(radius)
	{
		_position = position;
		_collisionType = CollisionType::Sphare;
	}
};

//================================================================================
//! ���� Line / Ray / Line segment
//================================================================================
struct  Line
{
	Vector3 _position[2];

	//! �f�t�H���g�R���X�g���N�^
	Line() {}

	//! �R���X�g���N�^
	Line(const Vector3& a, const Vector3& b)
	{
		_position[0] = a;
		_position[1] = b;
	}

	// �p�����[�^�[t�Ɠn���Ď��ۂ̍��W�����߂�
	Vector3	getPosition(f32 t) const
	{
		return (_position[1] - _position[0]) * t + _position[0];
	}

	f32 getDistance()
	{
		return 0.0f;
	}
};

//================================================================================
//! �J�v�Z��
//================================================================================
struct Capsule : public BaseCollisionType
{
public:
	Capsule()
	{
	}

	Capsule(const Vector3& p0, const Vector3& p1, f32 radius)
		: _line(p0, p1)
		, _radius(radius)
	{
	}

	Capsule(const Line& line, f32 radius)
		: _line(line)
		, _radius(radius)
	{
	}

	virtual ~Capsule()
	{
	}

	Line	_line;
	f32		_radius;
};





//================================================================================
//! ���� Plane
//	Ax + By + Cz + D = 0
//================================================================================
struct Plane : public BaseCollisionType
{
	union {
		struct {
			f32		_a;
			f32		_b;
			f32		_c;
			f32		_d;
		};
		struct {
			Vector3	_normal;
			f32		_distance;
		};
	};

	//! �f�t�H���g�R���X�g���N�^
	Plane() {}

	//! �R�s�[�R�X�g���N�^
	Plane(const Plane& other)
	{
		_normal = other._normal;
		_distance = other._distance;
	}

	//! �R���X�g���N�^ (�@���x�N�g�� �� ��\�_1�_)
	Plane(const Vector3& normal, const Vector3& p)
	{
		initialize(normal, p);
	}

	//! �R���X�g���N�^ (�@���x�N�g�� �� ��\�_1�_)
	void initialize(const Vector3& normal, const Vector3& p)
	{
		// ABC = �ʖ@���x�N�g��
		_normal = normal.normalize();

		// D�����߂�
		// Ax + By + Cz + D = 0
		//				  D = -(Ax + By + Cz)
		_d = -Vector3::dot(_normal, p);
	}

	//! �R���X�g���N�^ (��\�_3�_)
	Plane(const Vector3& a, const Vector3& b, const Vector3& c) 
	{
		Vector3	dirA = c - b;
		Vector3	dirB = a - b;

		Vector3	normal = Vector3::cross(dirA, dirB);

		initialize(normal, a);
	}
};

//================================================================================
// �O�p�`
//================================================================================
struct Triangle : public Plane
{
	Vector3		_position[3];		//!< ���_���W

	//! �f�t�H���g�R���X�g���N�^
	Triangle() {}

	//! �R�s�[�R���X�g���N�^
	Triangle(const Triangle& other)
		: Plane(other)
	{
		_position[0] = other._position[0];
		_position[1] = other._position[1];
		_position[2] = other._position[2];
	}

	//! �R���X�g���N�^ (��\�_3�_)
	Triangle(const Vector3& a, const Vector3& b, const Vector3& c)
	: Plane(a, b, c)
	{
		_position[0] = a;
		_position[1] = b;
		_position[2] = c;
	}
};

//================================================================================
// �l�p�`
//================================================================================
struct rRectangle : public BaseCollisionType {
	Vector3		_vertexPosition[4];		//!< ���_���W

									//! �f�t�H���g�R���X�g���N�^
	rRectangle() {}

	//! �R���X�g���N�^ (��\�_4�_)
	rRectangle(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d)
	{
		_vertexPosition[0] = a;
		_vertexPosition[1] = b;
		_vertexPosition[2] = c;
		_vertexPosition[3] = d;

		_collisionType = CollisionType::Rectangle;
	}
	
};


struct Box : public BaseCollisionType
{
	Box() {};
	Box(const Vector3& min, const Vector3& max)
	: _min(min)
	, _max(max)
	{
		_collisionType = CollisionType::Box;
	}

	Vector3 _min;
	Vector3 _max;
};

//================================================================================