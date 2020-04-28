#pragma once
#include "Model.h"

class BossEnemy : public EnemyBase
{
public:
	//! デフォルトコンストラクタ
	BossEnemy();

	//! デストラクタ
	~BossEnemy();

	//! 初期化
	bool init(float worldGravity) override { return true; }

	bool init(float worldGravity, const Vector3& position, const int bossHitPoint = 100);
	//! 更新
	void update() override;

	void lateUpdate();

	//! 描画
	void render() override;
	//! 解放
	void cleanup() override;

	//void armUpdate() override;

	

	void longAttack();
	void longAttack2();
	void shortAttack();
	//void coolTime();
	void warp();
	Vector3 warpPositionInit();
	void coolTime();

	void searc();

	Capsule* longAttackCapsule();
	Sphere* shortAttackSphere();

	Vector3 getDeathPosition() const
	{
		return _dropPosition;
	}

	EnemyState getState2()
	{
		return _state2;
	}

	
	void deadMotionInit() override;
	void deadMotionUpdate() override;
	void deadMotionRender() override;

private:
	Vector3 _cross[3];
	//int		_count;
	int		_attackCooltimeCount;
	Vector3 _firstPosition;

	bool _isShortAttack;
	bool _isLongAttack;
	bool _isLongAttack2;

	int _attackCount;

	// 一時

	bool _flag;
	Vector3 _playerPosition1;
	float _count;
	float _sinCount;

	float _searc1;
	float _searc2;
	float _searc3;

	Capsule* _longAttackCapsule;
	Sphere* _shortAttackSphere;

	int _c; // 数秒以上ShortAttack範囲にいたらShortAttack開始

	Vector3 	_dropPosition;

	Sound* _damageSound;

	Texture* _longAttackMagic;

	Sphere* _warpSphere;

	int _warpAlpha;
	int _warpWeitCount;
	bool _warpFlag;
	bool _isWarp;
	bool _warpFlag1;

	int _modelAlpha;

	struct LongAttack2
	{
		Sphere _longAttack2Sphere;
		float  _longAttack2Alpha;
		Vector3 _longAttack2MagicPosition;
		float	_magicAlpha;
		bool	_isAlive;
		bool	_isMagic;
		float   _magicRadius;

		LongAttack2()
		{
			//_longAttack2Sphere;
			//_longAttack2MagicPosition = nullptr;
			_magicAlpha = 0.0f;
			_longAttack2Alpha = 0.0f;
			_magicRadius = 0.0f;
			_isAlive = false;
			_isMagic = false;
		}

		~LongAttack2()
		{
			//SAFE_DELETE(_longAttack2Sphere);
			//SAFE_DELETE(_longAttack2MagicPosition);
		}
	};
	public:
		std::vector<LongAttack2*>& getLongAttack2()
		{
			return _longAttack2;
		}

private:
	bool _isLongAttackEnd;
	bool _isLongAttack000;
	std::vector<LongAttack2*> _longAttack2;
	Vector3 _playerPosition2;

	const float _longAttackSphereRadius;
	int _longAttackSphereNum;

	EnemyState _state2;
	EnemyState _state3;

	void a();
	
	float _deadMotionSphereSpeed[15];

	void BossEnemy::hpRender()
	{
		glLoadIdentity();
		Vector3 offsetPosition(5.0f, 25.0f, 0.0f);
		Matrix m = Matrix::translate(offsetPosition) * _matrix;
		glLoadMatrixf((GLfloat*)&m);
		// 一時static
		Vector3 size(_hitPoint * 0.05f, -0.7f, 0.0f);

		Render::getInstance()->drawParticleBillboard(Vector3::ZERO, size, Color(0, 255, 0, 255));
	}

	Sound* _warpSound;
	Sound* _magicSound[8];
};