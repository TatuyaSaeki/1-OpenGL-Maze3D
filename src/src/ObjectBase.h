#pragma once

struct BaseCollisionType;
enum class CollisionType;

enum class ObjectType : int
{
	None = 0,	// ObjectBase �R���X�g���N�^�̏������p
	Player,
	Enemy,
	Wall,
	WardlWeapon,
	PlayerWeapon,
	EnemyWeapon,
	KeyItem,
	HeelItem,

	// �ꎞ
	BossShortAttack,
	BossLongAttack,
	BossLongAttackMagic,
};


class ObjectBase
{
public:
	//! �R���X�g���N�^
	ObjectBase();
	//! �f�X�g���N�^
	virtual	~ObjectBase(){};

	//! ������
	virtual	bool init()    { return true; }
	//! �X�V
	virtual void update()  {};
	//! �`��
	virtual void render()  {};
	//! ���
	virtual void cleanup() {};

	//! �Q�b�^�[
	ObjectType	getObjectType()		const { return _objectType; }

	Matrix		getMatrix()			const { return _matrix ; }
	Vector3		getPosition()		const { return _position; }
	Vector3		getRotation()		const { return _rotation; }
	Vector3		getScale()			const { return _scale; }

	//! �Z�b�^�[
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