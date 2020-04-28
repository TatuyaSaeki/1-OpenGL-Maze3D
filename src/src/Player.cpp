#include "stdafx.h"
#include "Player.h"


//======================================================================================
//! コンストラクタ
//======================================================================================
Player::Player()
{
	// Player
	_matrix = Matrix::IDENTITY;
	_isJump = false;
	_isShot = false;
	_pCollision = nullptr;

	// ObjectBase
	_objectType = ObjectType::Player;
	_position = Vector3::ZERO;
	_color = Color(255, 0, 0);

	// CharacterBase
	_state = PlayerState::Wait;
	_state2 = PlayerState::Wait;
	_state3 = PlayerState::Wait;
	_hitPoint = 50;

	_haveWeapon = false;
	_weapon = nullptr;

	_isHit = false;
	_isDead = false;

	_gravity = 0.0f;
	_velocity = 0.0f;

	_armAngle = 0;

	//-------------------------
	// サウンドテスト
	//-------------------------
	_moveSound = nullptr;
	_isMove = false;

	_pickUpSound = nullptr;

	_swordMatrix = Matrix::identity();
	_damageEffect = nullptr;
	_damageEffectCounter = 0;

	_heelTexture = nullptr;
	_isHeel = false;

	_pickUpCameraRotation = Vector3::ZERO;

	_lastPosition = Vector3::ZERO;
	_newPosition = Vector3::ZERO;

	_moveCorrection = 1.0f;

	_damageSound1 = new Sound();
	_damageSound1->loadFile(L".\\data\\Sound\\damage1.mp3");
	//_damageSound1->volumeUp(1000);
	_damageSound1->volumeDown(3000);	
}

//======================================================================================
//! デストラクタ
//======================================================================================
Player::~Player()
{

}

//======================================================================================
//! 初期化
//======================================================================================
bool Player::init(float worldGravity)
{
	// Player
	_matrix = Matrix::IDENTITY;
	_isJump = false;
	_isShot = false;

	// ObjectBase
	_objectType = ObjectType::Player;
	_position = Vector3(22.0f, 0.0f, 22.5f);
	_rotation = Vector3(0.0f, 0.0f, 0.0f);

	_color = Color(255, 0, 0);

	// CharacterBase
	_state = PlayerState::Wait;

	Vector3 lines[] = {
		Vector3(0.0f,  0.0f, 0.0f),
		Vector3(0.0f, +8.0f, 0.0f)
		//Vector3(0.0f, +40.0f, 0.0f)
	};
	//_pCollision = new Capsule(lines[0], lines[1], 0.0f);
	_pCollision = new Capsule(lines[0], lines[1], 1.0f);

	_haveWeapon = false;
	_weapon = nullptr;

	_isHit = false;
	_isDead = false;

	_gravity = worldGravity;
	_velocity = 0.0f;

	// 腕
	_armMatrix = Matrix::IDENTITY;
	_armPosition = Vector3(2.0f, 7.0f, -3.0f);
	_armRotation = Vector3::ZERO;


	// 中心点の場所		// 中心点からの最小サイズ	// 中心点からの最大サイズ
	_armBox = new Box(Vector3(-0.25f, -0.5f, -0.25f), Vector3(0.25f, 1.5f, 0.25f));
	if (_armBox == nullptr) return false;

	_motionCount = 0.0f;
	_coolTimeCount = 0.0f;
	_isCoolTime = false;

	//-------------------------
	// サウンドテスト
	//-------------------------
	if(!soundInit()) return false;

	_weaponFlag = false;

	_damageEffect = new Texture();
	_damageEffect->load("data/Texture/Effect/damageEffect01.png");
	_damageEffect->setColor(Color(255, 255, 255, 255));

	_enemyWeaponIsHit = false;

	_heelTexture = new Texture;
	_heelTexture->load("data/Texture/Effect/HeelEffect01.png");
	_heelTextureTv0 = 0.0f;
	_heelTextureTv1 = 0.0f;

	_damageEffect2 = new Texture;
	_damageEffect2->load("data/Texture/Effect/fire.png");

	matrixUpdate();

	_headQuakePower   = 0.0f;
	_headPositionBase = Vector3(0.0f, 9.0f, 0.0f);
	_headPosition	  = Vector3(0.0f, 9.0f, 0.0f);
	
	_startUpMotionCount = 0;
	_invincibleCount = 0;
	//_pushKey[0]->load();
	_bossAttack = false;
	_firstAttack = false;
	_firstAvoidance = false;
	_firstMove = false;
	_kaihiDir  = Vector3::ZERO;

	return true;
}

//======================================================================================
//! 更新
//======================================================================================
void Player::update()
{
	_lastPosition = _position;

	if (_hitPoint <= 0) {
		_hitPoint = 0;
		_state = PlayerState::Dead;
		return;
	}

	if (_isInvincible) {
		_invincibleFlameCount++;
		if (_invincibleFlameCount > _invincibleCount) {
			_invincibleFlameCount = 0;
			_invincibleCount = 30;
			_isInvincible = false;
		}
	}

#if 0
	if (GetKeyState(VK_SPACE) & 0x8000) {
		_position._y += 1.0f;
		_velocity = 0.0f;
	}
#endif


	switch (_state)
	{
	case PlayerState::Wait:
#if 0
		if (GetKeyState(' ') & 0x8000) {
			_state = PlayerState::Jump;
		}
		break;
#endif

	case PlayerState::Move:
		//move();
		break;
	case PlayerState::BackStep:
		backStep();
		break;
	case PlayerState::CoolTime:
		coolTime();
		break;
	case PlayerState::Dead:
		break;
	}

	switch (_state2)
	{
	case PlayerState::PickUp:
		pickUp();
		break;
	case PlayerState::StartUp:
		startUp();
		break;
	case PlayerState::NockBack:
		nockback();
		break;
	case PlayerState::NockBack1:
		nockback1();
		break;
	default:
		break;
	}

	switch (_state3)
	{
	case PlayerState::Attack:
		attack();
		break;
	}

	if (_state != PlayerState::BackStep || _state2 != PlayerState::NockBack || _state2 != PlayerState::NockBack1) {
		move();
	}



	updateHead();

	matrixUpdate2();

	//---- 法線の計算
	Vector3 AB = _pCamera->getActiveCamera()->getMatWorld().axisX() - Vector3::ZERO;
	Vector3 AC = _pCamera->getActiveCamera()->getMatWorld().axisY() - _pCamera->getActiveCamera()->getMatWorld().axisX();
	_cross = Vector3::cross(AC, AB).normalize() * 3;

	_cross._y = 0.0f;



	_newPosition = _position;
}

//======================================================================================
//! 当たり判定後の更新
//======================================================================================
void Player::lateUpdate()
{
	_moveCorrection = 1.0f;

	// 当たっているオブジェクト通知が来ていたら
	for (auto hitObject : _hitObjectType) {
		switch (hitObject)
		{
			// エネミーと衝突している場合
		case ObjectType::Enemy:
		case ObjectType::Wall:

			if (hitObject == ObjectType::Enemy) _hitPoint -= 1;

			for (int i = 0; i < (signed)_extrusion.size(); i++) {
				_position += _extrusion[i];
			}

			// 無理やりすぎ
			_extrusion.clear();
			break;

			// 落ちている武器と衝突している場合
		case ObjectType::WardlWeapon:
			if (!_haveWeapon) {
				if (pushKeyState(0x45, E)) {
					_state2 = PlayerState::PickUp;
				}
			}
			break;
		case ObjectType::KeyItem:
			if (pushKeyState(0x45, E)) {
				_state2 = PlayerState::StartUp;
			}
			break;
		case ObjectType::HeelItem:
			if (_hitPoint < 50) _hitPoint += 5;
			_isHeel = true;
			_heelTexture->setAlpha(255);
			break;
		case ObjectType::EnemyWeapon:
			if (!_isInvincible) {
				_invincibleCount = 20;
				//_armRotation._x = _armRotation._z = 0.0f;
				//_armAngle = 0.0f;
				_isInvincible = true;
				_hitPoint -= 5;
				_state2 = PlayerState::NockBack1;
				_damageSound1->playRoop();
				_enemyWeaponIsHit = true;
				_damageEffect->setAlpha(0);
				_damageEffectCounter = 0;
			}
			break;

		case ObjectType::BossShortAttack:
			_invincibleCount = 60;
			if (!_isInvincible) {
				_armAngle = 0.0f;
				_isInvincible = true;
				_hitPoint -= 5;
				_damageSound1->playRoop();
				_state2 = PlayerState::NockBack1;
			}
			break;

		case ObjectType::BossLongAttack:
			_invincibleCount = 120;
			if (!_isInvincible) {
				_isInvincible = true;
				_hitPoint -= 7;
				if (_state3 != PlayerState::Attack) {
					_armRotation._x = _armRotation._z = 0.0f;
					_bossAttack = true;
					_damageEffect2->setAlpha(0);
					_damageEffectCounter = 0;
					_damageSound1->playRoop();
				}
				_state2 = PlayerState::NockBack;
			}
			break;

		case ObjectType::BossLongAttackMagic:
			_moveCorrection = 0.5f;
			break;
		default:
			
			break;
		}
	}

	if (_enemyWeaponIsHit) {
		_damageEffectCounter++;
		if (_damageEffectCounter < 45) {
			_damageEffect->addAlpha(3);
		}
		else {
			_damageEffect->addAlpha(-3);
			if (_damageEffect->getColor()._a <= 0) {
				_enemyWeaponIsHit = false;
			}
		}
	}

	if(_bossAttack) {
		_damageEffectCounter++;
		if (_damageEffectCounter < 45) {
			_damageEffect2->addAlpha(3);
		}
		else {
			_damageEffect2->addAlpha(-3);
			if (_damageEffect2->getColor()._a <= 0) {
				_bossAttack = false;
			}
		}
	}

	if (_haveWeapon) {
		if (pushKeyState(VK_LBUTTON, MOUSE_LEFT) &&
			_state2 != PlayerState::PickUp && _state2 != PlayerState::StartUp && _state3 != PlayerState::Attack) {
			_state3 = PlayerState::Attack;
			if(!_firstAttack) { _firstAttack  = true; }
		}
	}



	// 通知削除
	_hitObjectType.clear();

	soundUpdate();

	
}



//======================================================================================
//! 描画
//======================================================================================
void Player::render()
{
# if 0
	glPushMatrix();
	Capsule capsule = *_pCollision;
	capsule._line._position[0] = capsule._line._position[0].transform(_matrix);
	capsule._line._position[1] = capsule._line._position[1].transform(_matrix);

	// 法線描画
	Matrix m = Matrix::notRot(_matrix);
	glLoadMatrixf((GLfloat*)&m);

	glLoadMatrixf((GLfloat*)&_armMatrix);
	//Render::getInstance()->drawAABB(_armBox->_min, _armBox->_max, Color(255,0,0,0));

	if (_weapon != nullptr) {
		_weapon->render();
	}

	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();

	Render::getInstance()->drawArrorMatrix(_matrix, 10.0f);
	Render::getInstance()->drawArrorMatrix(_armMatrix, 2.0f);

	glPopMatrix();
#endif
	
	glPushMatrix();
	// 法線描画
	Matrix m = Matrix::notRot(_matrix);
	glLoadMatrixf((GLfloat*)&m);

	glLoadMatrixf((GLfloat*)&_armMatrix);


	if (_weapon != nullptr) {
		_weapon->render();
	}

	{
		auto	matCamera = _pCamera->getMatWorld();

		// プレイヤーの左右の向きはカメラと同じ(Y軸は固定)
		_matrix.axisY() = Vector3(0, 1, 0);
		_matrix.axisX() = Vector3::cross(Vector3(0, 1, 0), matCamera.axisZ()).normalize();
		_matrix.axisZ() = Vector3::cross(matCamera.axisX(), Vector3(0, 1, 0)).normalize();

		_matrix.translate() = _position;
	}

	_armMatrix = Matrix::wolrdMatrixTransform(_armRotation, _armPosition) * _matrix;

	// 武器を持っていたら
	if (_weapon != nullptr) {
		auto a = _weapon->getPosition();
		Matrix m = Matrix::translate(_weapon->getPosition()) * _armMatrix;
		_weapon->update(m);
	}
#if 0
	//---------------------------------
	std::vector<std::string> text;
	text.push_back("PlayerHp = ");
	glColor3ub(0, 0, 255);
	DebugStringValue(text, Vector3(_hitPoint, 0, 0), Vector3(10, 200, 0), Vector3(100.0f, 0.0f, 0.0f));

	std::vector<std::string> text1;
	text1.push_back("X = ");
	text1.push_back("Y = ");
	text1.push_back("Z = ");
	glColor3ub(0, 255, 0);
	DebugStringValue(text1, _position, Vector3(10, 230, 0), Vector3(150.0f, 0.0f, 0.0f));

	std::vector<std::string> text2;
	text2.push_back("X = ");
	text2.push_back("Y = ");
	text2.push_back("Z = ");
	glColor3ub(0, 255, 0);
	DebugStringValue(text2, _rotation, Vector3(10, 270, 0), Vector3(150.0f, 0.0f, 0.0f));
#endif
}

void Player::render2D()
{
	if(_enemyWeaponIsHit) {
		//drawScreenTexture(*_damageEffect, _damageEffect->getColor());
		Render::getInstance()->drawTexture2D(*_damageEffect, Vector2(0,0), Vector2(1280, 720), _damageEffect->getColor());
	}

	if(_bossAttack) {
		static Vector2 v = Vector2(0, 0);
		v._x += 0.01;
		v._y += 0.04;
		static Vector2 v1 = Vector2(1, 1);
		v1._x += 0.01;
		v1._y += 0.04;
		_damageEffect2->setTextureUV(v, v1);
		Render::getInstance()->drawScreenTextureUV(
			*_damageEffect2,
			_damageEffect2->getColor(),
			Vector2(0, 0), Vector2(1280, 720),
			_damageEffect2->getTextureUV1(),
			_damageEffect2->getTextureUV2());
	}

	if(_isHeel) { 
		Vector2 pos0 = Vector2(-100.0f, 0.0f);
		Vector2 pos1 = Vector2(1400.0f, 720.0f);
		float tu0 = 0.0f;
		float tu1 = 1.0f;

		_heelTextureTv0 += 0.03f;
		_heelTextureTv1 += 0.03f;

		if (_heelTextureTv0 > 1.0f || _heelTextureTv1 > 1.0f) {
			_heelTextureTv0 = 0.0f;
			_heelTextureTv1 = 0.0f;
			_isHeel = false;
		}

		_heelTexture->addAlpha(1);

		Render::getInstance()->drawScreenTexture(*_heelTexture, _heelTexture->getColor(), pos0, pos1, tu0, _heelTextureTv0, tu1, _heelTextureTv1);
	}
	hpRender(Color(0, 0, 128));


}


//======================================================================================
//! 解放
//======================================================================================
void Player::cleanup()
{
	// アドレスを参照しているだけなので delete 不要.
	_pCamera = nullptr;

	SAFE_DELETE_CLEANUP(_damageSound1);
	SAFE_DELETE(_damageEffect2);
	SAFE_DELETE(_heelTexture);
	SAFE_DELETE(_damageEffect);
	SAFE_DELETE_CLEANUP(_weapon);
	SAFE_DELETE(_pCollision);
	SAFE_DELETE(_armBox);

	soundCleanup();
}

//======================================================================================
//! 移動
//======================================================================================
void Player::move()
{
	if (_state == PlayerState::BackStep) return;

	Matrix matCamera = _pCamera->getMatWorld();

	if(_state2 == PlayerState::NockBack || _state2 == PlayerState::NockBack1) { 
		return;
	}

	// 移動ベクトル
	Vector3				dir(0.0f, 0.0f, 0.0f);
	//dir = Vector3::ZERO;

	// カメラの向きに移動する用
	Vector3 frontdir = matCamera.axisZ() * -1.0f;
	frontdir._y = 0.0f;
	frontdir = frontdir.normalize();

	Vector3 rightdir = matCamera.axisX();
	rightdir._y = 0.0f;
	rightdir = rightdir.normalize();

	// 移動
	_isMove = false;
	_kaihiDir = Vector3::ZERO;

	if (GetKeyState('W') & 0x8000) { dir += frontdir; _isMove = true; _kaihiDir += frontdir; }
	if (GetKeyState('S') & 0x8000) { dir -= frontdir; _isMove = true; _kaihiDir -= frontdir; }
	if (GetKeyState('A') & 0x8000) { dir -= rightdir; _isMove = true; _kaihiDir -= rightdir; }
	if (GetKeyState('D') & 0x8000) { dir += rightdir; _isMove = true; _kaihiDir += rightdir; }
	if (_isMove && !_firstMove) { _firstMove = true; }

	if (pushKeyState(0x43, C)) {
		if (_state2 != PlayerState::NockBack && _state2 != PlayerState::NockBack1) {
			if (!_isCoolTime) {
				dir = Vector3::ZERO;
				_armAngle = 0.0f;
				if(!_isMove) _kaihiDir -= frontdir;
				_kaihiDir = _kaihiDir.normalize() * 1.5f;
				_isInvincible = true;
				_invincibleCount = 10;
				_state = PlayerState::BackStep;

				if (!_firstAvoidance) _firstAvoidance = true;
			}
		}
	}

	
	if (_state == PlayerState::BackStep) {
		dir = Vector3::ZERO;
	}

#if 1
	
	_velocity -= _gravity;
	if(_velocity >= 20.0f) {
		_velocity = 20.0f;	
	}// 重力
	dir = dir.normalize() * 1.15f;							// 正規化
	//dir = dir.normalize() * 0.9f;							// 正規化
	//dir = dir.normalize() * 2.0f;							// 正規化
	_position += dir * _moveCorrection;
	_position._y += _velocity;

	if (_position._y < 0) {
		_position._y = 0;
		_velocity = 0;
	}

	_rotation._y = RAD_TO_DEG(_pCamera->getRotationY());

#else 

	_velocity -= _gravity;										// 重力
																//_position._y += _velocity;										// 重力
																//dir = dir.normalize() * 1.0f;							// 正規化
	dir = dir.normalize() * 0.3f;							// 正規化

	_position = _position;
	_position = _position;

	_position += dir;
	_position._y += _velocity;


	if (_position._y < 0) {
		_position._y = 0;
		_velocity = 0;
		_isJump = false;
	}

											// 移動ベクトル加算
#endif

}

//@@@ ノックバック
void Player::nockback()
{
	Vector3 dir = (_cross * -1).normalize() * 1.9;
	dir._y = sinf(DEG_TO_RAD(150.0f));

	if (_motionCount > 15) {
		dir._y *= -1.0f;
	}

	_position += dir;
	
	//_pCamera->setLookAt(Vector3(0,0, cosf(DEG_TO_RAD(num))));
	//num *= -0.5;

	_motionCount++;
	if (_motionCount > 30) {
		_motionCount = 0;
		//_isCoolTime = true;
		_state2 = PlayerState::Wait;
	}
}

void Player::nockback1()
{
	Vector3 dir = (_cross * -1).normalize() * 1.0f;
	dir._y = sinf(DEG_TO_RAD(10.0f));

	
	if (_motionCount > 10) {
		dir._y *= -1.0f;
	}
	_position += dir;

	_motionCount++;
	if (_motionCount > 20) {
		_motionCount = 0;
		//_isCoolTime = true;
		_state2 = PlayerState::Wait;
	}

	
}

void Player::backStep()
{
	_position += _kaihiDir;

	_motionCount++;
	if (_motionCount > 12) {
		_kaihiDir = Vector3::ZERO;
		_motionCount = 0;
		_isCoolTime = true;
		_state = PlayerState::CoolTime;
	}
}

void Player::coolTime()
{
	_coolTimeCount++;

	if (_coolTimeCount > 45) {
		_coolTimeCount = 0;
		_isCoolTime = false;
		_state = PlayerState::Wait;
	}
}


#if 0
//======================================================================================
//! ジャンプ
//======================================================================================
void Player::jump()
{
	// ジャンプ
	if (!_isJump) {
		_isJump = true;
		_velocity = 0.5f;
	}
}
#endif

void Player::pickUp()
{
	if (!_haveWeapon) {
		_haveWeapon = true;
	}

	if(_pickUpCameraRotation._x < 45.0f && !_weaponFlag) {
		_pickUpCameraRotation._x += 2.0f;
		if(_pickUpCameraRotation._x >= 45.0f) {
			createWeapon();		
			_weaponFlag = true;
		}
	}
	else if(_weaponFlag) {
		_pickUpCameraRotation._x -= 2.0f;
		if(_pickUpCameraRotation._x <= 0) { 
			_pickUpCameraRotation._x = 0.0f;
			_state2 = PlayerState::Wait;
		}
	}
}

void Player::attack()
{
	_armPosition._x  = 1.0f;
	_armRotation._x -= 9.0f;
	_armRotation._z  = -32.0f;
	
	if (_armRotation._x < -140.0f) {
		_armPosition._x = 2.0f;
		_armRotation._x = 0.0f;
		_armRotation._z = 0.0f;
		_state3 = PlayerState::Wait;
	}
}

Vector3 Player::getDirection()
{
	return _position - _oldPosition;
}

void Player::hpRender(const Color& color)
{
	//-------------------------------------------------------------
	// 2D描画
	//-------------------------------------------------------------
	glMatrixMode(GL_PROJECTION);

	// ピクセル座標をスクリーン座標に変換する
	//	f32		x2 = (f32)x  * ( 2.0f / screenW ) - 1;
	//	f32		y2 = (f32)y  * (-2.0f / screenH ) + 1;
	s32		screenW = 1280;
	s32		screenH = 720;

	Matrix	matPixelFit = Matrix::scale(Vector3(2.0f / screenW, -2.0f / screenH, 1.0f)) *
							Matrix::translate(Vector3(-1.0f, +1.0f, 0.0f)); // 原点(左上)をスクリーンの左上に移動
	glLoadMatrixf((GLfloat*)&matPixelFit);

	float x0 = 10.0f;
	float y0 = 10.0f;
	float x1 = 310.0f;
	float y1 = 60.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//------------------------------
	// 枠
	//------------------------------
	glBegin(GL_LINES);
	glColor3f(color._r, color._b, color._g);
	// 横
	glVertex3f(x0, y0, 0.0f);
	glVertex3f(x1, y0, 0.0f);

	glVertex3f(x0, y1, 0.0f);
	glVertex3f(x1, y1, 0.0f);

	// 縦
	glVertex3f(x0, y0, 0.0f);
	glVertex3f(x0, y1, 0.0f);

	glVertex3f(x1, y0, 0.0f);
	glVertex3f(x1, y1, 0.0f);
	glEnd();

	//------------------------------
	// HP
	//------------------------------
	int hp = _hitPoint * 6 + 10;

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(color._r, color._b, color._g);
	// 横
	glVertex3f(x0,			  y0, 0.0f);
	glVertex3f(hp, y0, 0.0f);

	glVertex3f(x0,			  y1, 0.0f);
	glVertex3f(hp, y1, 0.0f);

	// 縦
	glVertex3f(x0, y0, 0.0f);
	glVertex3f(x0, y1, 0.0f);

	glVertex3f(hp, y0, 0.0f);
	glVertex3f(hp, y1, 0.0f);

	glEnd();
}



bool Player::soundInit()
{
	_moveSound = new Sound();
	if (_moveSound == nullptr) return false;
	_moveSound->loadFile(L".\\data\\Sound\\moveSound17.mp3");
	_moveSound->volumeDown(2200);

	_pickUpSound = new Sound();
	if (_pickUpSound == nullptr) return false;
	_pickUpSound->loadFile(L".\\data\\Sound\\pickUp.mp3");
	_pickUpSound->volumeDown(2000);

	_attackSound = new Sound();
	if (_attackSound == nullptr) return false;
	_attackSound->loadFile(L".\\data\\Sound\\attack.mp3");
	//_attackSound->volumeUp(2000);
	_attackSound->volumeDown(4000);

	_backStepSound = new Sound();
	if (_backStepSound == nullptr) return false;
	_backStepSound->loadFile(L".\\data\\Sound\\backStep.wav");
	//_backStepSound->volumeUp(500);
	_backStepSound->volumeDown(2000);
	
	return true;
}

void Player::soundUpdate()
{
	// 歩く
	if (_isMove) {
		_moveSound->playRoop();
	}

	// 拾う
	if(_state2 == PlayerState::PickUp) {
		_pickUpSound->play();
	}

	// 攻撃
	if(_state3 == PlayerState::Attack) {
		_attackSound->play();
	}
	else {
		_attackSound->stop();
	}

	// バックステップ
	if(_state == PlayerState::BackStep) {
		_backStepSound->playRoop();
	}
}

void Player::soundCleanup()
{
	SAFE_DELETE_CLEANUP(_moveSound);
	SAFE_DELETE_CLEANUP(_pickUpSound);
	SAFE_DELETE_CLEANUP(_attackSound);
	SAFE_DELETE_CLEANUP(_backStepSound);
}

void Player::startUp()
{
	_startUpMotionCount++;
	if(_startUpMotionCount > 60){
		_startUpMotionCount = 0;
		_state2 = PlayerState::Wait;
	}
}