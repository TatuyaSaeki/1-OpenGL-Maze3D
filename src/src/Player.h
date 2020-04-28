#pragma once

enum class PlayerState
{
	Wait,
	Move,
	Jump,
	Attack,
	BackStep,
	NockBack,
	NockBack1,
	HitWeapon,
	PickUp,
	StartUp,
	CoolTime,
	Dead,
};

class Player : public CharacterBase
{
protected:


public:
	//! �f�t�H���g�R���X�g���N�^
	Player();
	//! �f�X�g���N�^
	virtual ~Player();

	//! ������
	bool init(float worldGravity) override;
	//! �X�V
	void update() override;
	//! �����蔻���̍X�V
	void lateUpdate();
	//! �`��
	void render() override;

	//! �`��
	void render2D();

	//! ���
	void cleanup() override;

	//! �ړ�
	void move();
	//@@@ �m�b�N�o�b�N
	void nockback();
	// �o�b�N�X�e�b�v
	void backStep();
	//! �W�����v
	void jump();
	void coolTime();
	void attack();

	//! ����������Ă��邩
	void setWeapon(bool flag) { _haveWeapon = flag; }
	bool getWeapon() { return _haveWeapon; }

	// �ꎞ
	Vector3 getCross() { return _cross; }

	void setVelocity(float v) { _velocity = v; }
	float getVelocity() { return _velocity; }

	void pickUp();


	void setCamera(Camera* pCamera) { _pCamera = pCamera; }

	Capsule* getCollision() { return _pCollision; }

	void setState(PlayerState state) { _state = state; }
	PlayerState getState() { return _state; }
	PlayerState getState2() { return _state2; }
	PlayerState getState3() { return _state3; }

	Vector3 getDirection();

	//-------------------------
	void hpRender(const Color& color);

	int getHitPoint() const { return _hitPoint; }
	void setHitPoint(int hitPoint) { _hitPoint = hitPoint ;}

	void addPosition(const Vector3& position)override
	{
		_position += position;
		matrixUpdate2();
	}

	void setPosition(const Vector3& position)override
	{
		_position = position;
		//_lastPosition = position;
		//_newPosition  = position;
		//matrixUpdate();
	}

	bool createWeapon()
	{
		if(_weapon == nullptr) {
			_haveWeapon = true;
			Line line = Line(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 9.0f, 0.0f));
			_weapon = new Sword();
			_weapon->init(ObjectType::PlayerWeapon, line);
			if(_weapon == nullptr) return false;
			_weapon->setPosition(Vector3::ZERO);

			// ����������Ă�����
			if (_weapon != nullptr) {
				_armMatrix = Matrix::wolrdMatrixTransform(_armRotation, _armPosition) * _matrix;
				Matrix m = Matrix::translate(_weapon->getPosition()) * _armMatrix;
				_weapon->update(m);
			}

			return true;
		}
		return false;
	}

	void updateHead()
	{
		_headPosition = _headPositionBase;

		if (_headQuakePower > 0.0f) {
			_headPosition._x += frand_1_1() * _headQuakePower;
			_headPosition._y += frand_1_1() * _headQuakePower;
			_headPosition._z += frand_1_1() * _headQuakePower;

			_headQuakePower *= 0.8f;
			if (_headQuakePower < 0.01f) {
				_headQuakePower = 0.0f;
			}
		}

		_headPosition = _headPosition.transform(_matrix);
	}

	Vector3 getHeadPosition()
	{
		updateHead();

		return _headPosition;
	}

	Matrix* getCamera()
	{
		return &_matrix;
	}

	void startUp();

	Vector3 getPickUpRotation() const
	{
		return _pickUpCameraRotation;	
	}

	Vector3 getLastPosition()
	{
		return _lastPosition;
	}
	Vector3 getNewPosition()
	{
		return _newPosition;
	}
	void matrixUpdate()
	{
		_matrix = Matrix::identity();
		_matrix = Matrix::wolrdMatrixTransform(_rotation, _position);

		// ����������Ă�����
		if (_weapon != nullptr) {
			Matrix m = Matrix::translate(_weapon->getPosition()) * _armMatrix * _matrix;
			_weapon->update(m);
		}
	}

	bool firstAttack()
	{
		return _firstAttack;
	}
	bool firstAvoidance()
	{
		return _firstAvoidance;
	}
	bool firstMove()
	{
		return _firstMove;
	}

	void 	firstInit()
	{
		_firstAttack = true;
		_firstAvoidance = true;
		_firstMove = true;
	}
private:
	Vector3 _lastPosition;
	Vector3 _newPosition;
	//-------------
	bool soundInit();
	void soundUpdate();
	void soundCleanup();


private:
	
	PlayerState		_state;
	PlayerState		_state2;
	PlayerState		_state3;

	Camera * _pCamera;
	//Matrix		_matrix;
	float		_rotY;			// y����]�̒l

	bool		_haveWeapon;	// ����������Ă��邩

	// �W�����v
	bool	_isJump;
	bool	_isShot;

	Vector3 _cross;
	Vector3 _center;
	
	f32 _armAngle;


	Capsule* _pCollision;

	// �ړ��x�N�g��
	Vector3				_dir;

	float _motionCount;
	float _coolTimeCount;
	bool  _isCoolTime;;

	bool _enemyWeaponIsHit;



	void matrixUpdate2()
	{	
		//---- �`��
		_matrix = Matrix::identity();
		Matrix  matCamera = _pCamera->getMatWorld();
		matCamera._m[3][0] = matCamera._m[3][1] = matCamera._m[3][2] = 0.0f;
		
		matCamera._m[0][1] = matCamera._m[1][0] = 0.0f;
		matCamera._m[1][1] = 1.0f;
		matCamera._m[1][2] = matCamera._m[2][1] = 0.0f;
		
		_matrix = matCamera * Matrix::wolrdMatrixTransform(_rotation, _position);
	}
	
private:
	//-------------------------
	// �T�E���h�e�X�g
	//-------------------------
	// ����
	Sound* _moveSound;
	bool   _isMove;

	// �E��
	Sound* _pickUpSound;

	// �U��
	Sound* _attackSound;
	// ���
	Sound* _backStepSound;

	Matrix _swordMatrix;

	bool _weaponFlag;

	Texture* _damageEffect;
	int _damageEffectCounter;

	Texture* _heelTexture;
	bool _isHeel;
	float _heelTextureTv0;
	float _heelTextureTv1;

	Vector3	_headPositionBase;
	Vector3	_headPosition;
	f32		_headQuakePower;

	int _startUpMotionCount;
	Vector3 _pickUpCameraRotation;

	void nockback1();


	float _moveCorrection;
	int   _invincibleCount;

	Texture* _damageEffect2;
	bool _bossAttack;

	bool _firstAttack;
	bool _firstAvoidance;
	bool _firstMove;

	Vector3 _kaihiDir;

	Sound* _damageSound1;
};

