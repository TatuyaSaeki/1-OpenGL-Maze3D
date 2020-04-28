#pragma once
#include "Model.h"

class NomalEnemy : public EnemyBase
{
public:
	//! デフォルトコンストラクタ
	NomalEnemy();

	//! デストラクタ
	~NomalEnemy();

	//! 初期化
	bool init(float worldGravity) override { return true; }

	bool init(float worldGravity, const Vector3& position, const Vector3& rot, const int hp = 50);
	//! 更新
	void update() override;
	//! 描画
	void render() override;
	//! 解放
	void cleanup() override;

	void armUpdate() override;

	void debugSearc();

	void attack();
	void attackPreliminary();
	void coolTime();
	void nockback();

	//-----------------
	void deadMotionInit() override;
	void deadMotionUpdate() override;
	void deadMotionRender() override;

	// 聞く
	// 参照渡し | ポインタで渡したい(const付けれない)
	Vector3 getDeathPosition() const
	{
		return _dropPosition;
	}

	void state(const EnemyState es)
	{
		_state = es;
	}

	void hpRender();

	Vector3 getLastPosition()
	{
		return _lastPosition;
	}
	Vector3 getNewPosition()
	{
		return _newPosition;
	}

	void setPosition(const Vector3& position) override
	{
		_position = position;
		_matrix = Matrix::identity();
		_matrix = Matrix::wolrdMatrixTransform(_rotation, _position);
	}

	bool flag;
	bool getFlag()
	{
		return flag;
	}
private:
	Vector3 _cross[3];
	int		_attackCooltimeCount;
	Vector3 _firstPosition;
	Vector3 _firstRotation;
	float	_count;
	//void deadMotion();



	// 消えるモーションに入る直前の座標(ドロップアイテムのため)
	Vector3 _dropPosition;

	Matrix _sowrdMatrix;

	//------------------------------
	Matrix		_bodyMatrix;
	Matrix		_armrMatrix[2];
	Matrix		_handMatrix[2];
	Matrix		_legMatrix[2];
	Vector3		_bodyPosition;
	Vector3		_armrPosition[2];
	Vector3		_handPosition[2];
	Vector3		_legPosition[2];
	Box*		_body;
	Box*		_arm[2];
	Box*		_hand[2];
	Box*		_leg[2];

	Vector3		_armrRotation[2];
	Vector3		_handRotation[2];
	Vector3		_legRotation[2];
	float		_legAnimetionSpeed[2];

	Vector3 _lastPosition;
	Vector3 _newPosition;

	void a();
	void b();
	void c();

	bool _isAttack;

	float _weaponAlpha;
	int _warpCounter;

	
};