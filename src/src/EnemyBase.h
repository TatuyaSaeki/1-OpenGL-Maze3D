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
	//! デフォルトコンストラクタ
	EnemyBase();

	//! デストラクタ
	~EnemyBase();

	//! 初期化
	virtual bool init(float worldGravity);
	//! 更新
	virtual void update();
	//! 描画
	virtual void render();
	//! 解放
	virtual void cleanup();

	//! ゲッター
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
	// 名前適当
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