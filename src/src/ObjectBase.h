#pragma once

struct BaseCollisionType;
enum class CollisionType;

enum class ObjectType : int
{
	None = 0,	// ObjectBase コンストラクタの初期化用
	Player,
	Enemy,
	Wall,
	WardlWeapon,
	PlayerWeapon,
	EnemyWeapon,
	KeyItem,
	HeelItem,

	// 一時
	BossShortAttack,
	BossLongAttack,
	BossLongAttackMagic,
};


class ObjectBase
{
public:
	//! コンストラクタ
	ObjectBase();
	//! デストラクタ
	virtual	~ObjectBase(){};

	//! 初期化
	virtual	bool init()    { return true; }
	//! 更新
	virtual void update()  {};
	//! 描画
	virtual void render()  {};
	//! 解放
	virtual void cleanup() {};

	//! ゲッター
	ObjectType	getObjectType()		const { return _objectType; }

	Matrix		getMatrix()			const { return _matrix ; }
	Vector3		getPosition()		const { return _position; }
	Vector3		getRotation()		const { return _rotation; }
	Vector3		getScale()			const { return _scale; }

	//! セッター
	virtual void setPosition( const Vector3& position ) { _position = position; }
	virtual void setRotetion( const Vector3& rotetion )	{ _rotation = rotetion; }
	virtual void setScale	( const Vector3& scale )	{ _scale	= scale; }

	Vector3 getTranslate();
	Vector3 getRotation();

protected:
	ObjectType	_objectType;

	Matrix		_matrix;
	Vector3		_position;
	Vector3		_rotation;
	Vector3		_scale;

	Color		_color;

	std::vector<ObjectType> _hitObjectType;
};