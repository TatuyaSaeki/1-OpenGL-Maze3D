//================================================================================
//!
//! @file	Shape.h
//! @file	形状定義
//! @auther
//!
//================================================================================
#pragma once

enum class CollisionType : int
{
	None = 0,	//!< 使われていない
	Sphare,		//!< 球
	Capsule,	//!< カプセル
	Line,		//!< 線
	Plane,		//!< 平面
	Triangle,	//!< 三角ポリゴン
	Rectangle,	//!< 矩形面
	Box			//!< 箱
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
//! 球	Sphere
//================================================================================
struct Sphere : public BaseCollisionType
{
	Vector3		_position;
	f32			_radius;	//!< 半径

	//! デフォルトコンストラクタ
	Sphere() {}

	//! コンストラクタ
	Sphere(const Vector3& position, f32 radius)
	: _radius(radius)
	{
		_position = position;
		_collisionType = CollisionType::Sphare;
	}
};

//================================================================================
//! 直線 Line / Ray / Line segment
//================================================================================
struct  Line
{
	Vector3 _position[2];

	//! デフォルトコンストラクタ
	Line() {}

	//! コンストラクタ
	Line(const Vector3& a, const Vector3& b)
	{
		_position[0] = a;
		_position[1] = b;
	}

	// パラメーターtと渡して実際の座標を求める
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
//! カプセル
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
//! 平面 Plane
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

	//! デフォルトコンストラクタ
	Plane() {}

	//! コピーコストラクタ
	Plane(const Plane& other)
	{
		_normal = other._normal;
		_distance = other._distance;
	}

	//! コンストラクタ (法線ベクトル と 代表点1点)
	Plane(const Vector3& normal, const Vector3& p)
	{
		initialize(normal, p);
	}

	//! コンストラクタ (法線ベクトル と 代表点1点)
	void initialize(const Vector3& normal, const Vector3& p)
	{
		// ABC = 面法線ベクトル
		_normal = normal.normalize();

		// Dを求める
		// Ax + By + Cz + D = 0
		//				  D = -(Ax + By + Cz)
		_d = -Vector3::dot(_normal, p);
	}

	//! コンストラクタ (代表点3点)
	Plane(const Vector3& a, const Vector3& b, const Vector3& c) 
	{
		Vector3	dirA = c - b;
		Vector3	dirB = a - b;

		Vector3	normal = Vector3::cross(dirA, dirB);

		initialize(normal, a);
	}
};

//================================================================================
// 三角形
//================================================================================
struct Triangle : public Plane
{
	Vector3		_position[3];		//!< 頂点座標

	//! デフォルトコンストラクタ
	Triangle() {}

	//! コピーコンストラクタ
	Triangle(const Triangle& other)
		: Plane(other)
	{
		_position[0] = other._position[0];
		_position[1] = other._position[1];
		_position[2] = other._position[2];
	}

	//! コンストラクタ (代表点3点)
	Triangle(const Vector3& a, const Vector3& b, const Vector3& c)
	: Plane(a, b, c)
	{
		_position[0] = a;
		_position[1] = b;
		_position[2] = c;
	}
};

//================================================================================
// 四角形
//================================================================================
struct rRectangle : public BaseCollisionType {
	Vector3		_vertexPosition[4];		//!< 頂点座標

									//! デフォルトコンストラクタ
	rRectangle() {}

	//! コンストラクタ (代表点4点)
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