#pragma once
#include "Model.h"

enum class EnemyState
{
	Wait,
	Move,
	Jump,
	Attack,
	AttackPreliminary,
	ShortAttack,
	LongAttack,
	LongAttack2,
	BackStep,
	CoolTime,
	BackMove,

	Dead,
};

class EnemyBase : public CharacterBase
{
protected:


public:
	//! �f�t�H���g�R���X�g���N�^
	EnemyBase();

	//! �f�X�g���N�^
	~EnemyBase();

	//! ������
	virtual bool init(float worldGravity);
	//! �X�V
	virtual void update();
	//! �`��
	virtual void render();
	//! ���
	virtual void cleanup();

	//! �Q�b�^�[
	void  setSearchRange(float range) { _searchRange = range; }
	float getSearchRange() { return _searchRange; }
	void  isSearc(bool flag) { _isSearc = flag; }
	bool  getIsSearc() { return _isSearc; }

	void dead(bool flag) { _isDead = flag; }

	virtual void armUpdate();

	// Dubeg
	void setPlayerPosition(const Vector3& v)
	{
		_playerPosition = v;
	}

	Capsule* getCollision() { return _pCollision; }

	EnemyState getState() { return _state; }

	Vector3 getDir();

	virtual void deadMotionInit(){};
	virtual void deadMotionUpdate() {};
	virtual void deadMotionRender() {};

protected:

	EnemyState _state;

	Vector3 _cross[3];

	float _searchRange;
	// ���O�K��
	bool  _isSearc;

	// Debug
	Vector3 _playerPosition;

	// static
	Model* _model;

	Capsule* _pCollision;

	const int _deadSphereMax;
	Sphere* _deadMotionSphere[15];
	float   _deadMotionSphereAlpha[15];
	bool	_isDeadMotion;
	float	_deadMotionCounter;
	float	_deadMotionModelAlpha;


};