#include "stdafx.h"

//======================================================================================
//! コンストラクタ
//======================================================================================
NomalEnemy::NomalEnemy()
{
	// 腕
	_armMatrix = Matrix::IDENTITY;
	_armBox = nullptr;
	_armPosition = Vector3::ZERO;
	_armRotation = Vector3::ZERO;


	_lastPosition = Vector3::ZERO;
	_newPosition = Vector3::ZERO;
}

//======================================================================================
//! デストラクタ
//======================================================================================
NomalEnemy::~NomalEnemy()
{

}

//======================================================================================
//! 初期化
//======================================================================================
bool NomalEnemy::init(float worldGravity, const Vector3& position, const Vector3& rot, const int hp)
{
	// ObjectBase
	_objectType = ObjectType::Enemy;
	_position = position;

	_rotation = Vector3(0.0f, 0.0f, 0.0f);
	_color = Color(0, 0, 255);

	Vector3 lines[] = {
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 10.0f, 0.0f)
	};
	_pCollision = new Capsule(lines[0], lines[1], 2.0f);

	// CharacterBase
	_state = EnemyState::Wait;


#if 0
	Line line = Line(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 8.0f, 0.0f));

	_weapon = new Sword(ObjectType::EnemyWeapon, line);

	if (_weapon == nullptr)	return false;
	_weapon->setPosition(Vector3(0.0f, 0.0f, 0.0f));
#endif

	_isHit = false;
	_isDead = false;

	_gravity = worldGravity;
	_velocity = 0.0f;
	_warpCounter = 0;

	// NomalEnemy
	_firstPosition = position;
	_firstRotation = rot;

	// 腕
	_armBox = new Box(Vector3(-0.15f, -0.5f, -0.25f), Vector3(0.15f, 1.0f, 0.25f));
	if (_armBox == nullptr) return false;

	_armMatrix = Matrix::IDENTITY;
	//_armPosition = Vector3(-1.0f, 8.0f, 0.5f);
	//_armRotation = Vector3(0.0f, 0.0f, 0.0f);

	_searchRange = 100.0f;
	_isSearc = false;

	_scale = Vector3(0.25f, 0.25f, 0.25f);

	_hitPoint = hp;
	_count = 0;

	_attackCooltimeCount = 0;

	_model = new Model();
	if (_model == nullptr)	return false;
	if (_model->load("data/Model/knight/knight.mqo", 0.23f) == false) {
		MessageBox(nullptr, "cart.mqo ファイルの読み込みに失敗しました.", "エラー", MB_OK);
		return false;
	}



	//---------------------------
	Vector3 bodySize[2] =
	{
		Vector3(-1.0f, 0.0f, -0.8f),
		Vector3(+1.0f, 4.0f, +0.8f)
	};
	Vector3 armSize[2] =
	{
		Vector3(-0.5f, 0.0f, -0.5f),
		Vector3(+0.5f, -3.0f, +0.5f)
	};
	Vector3 handSize[2] =
	{
		Vector3(-0.25f,  0.0f, -0.25f),
		Vector3(+0.25f, -0.5f, +0.25f)
	};
	Vector3 legSize[2] =
	{
		Vector3(-0.5f, 0.0f, -0.5f),
		Vector3(+0.5f, -4.0f, +0.5f)
	};

	_bodyMatrix    = Matrix::identity();
	_armrMatrix[0] = Matrix::identity();
	_armrMatrix[1] = Matrix::identity();
	_handMatrix[0] = Matrix::identity();
	_handMatrix[1] = Matrix::identity();
	_legMatrix[0]  = Matrix::identity();
	_legMatrix[1]  = Matrix::identity();


	_body    = new Box(bodySize[0], bodySize[1]);
	_arm[0]  = new Box(armSize[0], armSize[1]);
	_arm[1]  = new Box(armSize[0], armSize[1]);
	_hand[0] = new Box(handSize[0], handSize[1]);
	_hand[1] = new Box(handSize[0], handSize[1]);
	_leg[0]  = new Box(legSize[0], legSize[1]);
	_leg[1]  = new Box(legSize[0], legSize[1]);
	_bodyPosition	  = Vector3( 0.0f, 4.0f, 0.0f);

	_armrPosition[0]  = Vector3(-2.0f, 3.5f, 0.0f);
	_armrPosition[1]  = Vector3(+2.0f, 3.5f, 0.0f);

	_handPosition[0] = Vector3(0.0f, -3.0f, 0.0f);
	_handPosition[1] = Vector3(0.0f, -3.0f, 0.0f);

	_legPosition[0]   = Vector3(+1.0f, 0.0f, 0.0f);
	_legPosition[1]   = Vector3(-1.0f, 0.0f, 0.0f);

#if 0
	_armrRotation[0]  = Vector3(-60, 0,   40);//(-45.0f,0.0f,0.0f);
	_armrRotation[1]  = Vector3(-60, 0, -40);
	_handRotation[0] = Vector3::ZERO;
	_handRotation[1] = Vector3::ZERO;
#endif
	_armrRotation[0] = Vector3::ZERO; //(-45.0f,0.0f,0.0f);
	_armrRotation[1] = Vector3::ZERO;
	_handRotation[0] = Vector3::ZERO;
	_handRotation[1] = Vector3::ZERO;

	_legRotation[0]	  = Vector3::ZERO;
	_legRotation[1]	  = Vector3::ZERO;


	_legAnimetionSpeed[0] = 3.0f;
	_legAnimetionSpeed[1] = 3.0f;

	Line line = Line(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 120.0f, 0.0f));

	_weapon = new Sword();
	_weapon->init(ObjectType::EnemyWeapon, line);


	if (_weapon == nullptr)	return false;
	_weapon->setPosition(Vector3(0.0f, -3.0f, 0.0f));
	//_weapon->setRotetion(Vector3(0.0f, 90.0f, 0.0f));

	_sowrdMatrix = Matrix::identity();
	_isAttack = false;

	_weaponAlpha = 255.0f;
	
	_lastPosition = position;
	_newPosition = position;

	flag = false;

	return true;
}

//======================================================================================
//! 更新
//======================================================================================
void NomalEnemy::update()
{
	_lastPosition = _position;

	if(_state == EnemyState::Wait) {
		_armrRotation[0] = Vector3(60.0f, 180.0f, -40.0f); //(-45.0f,0.0f,0.0f);
		_armrRotation[1] = Vector3(-60.0f, 0.0f,  -40.0f);
		_handRotation[0] = Vector3::ZERO;
		_handRotation[1] = Vector3::ZERO;
		_legRotation[0] = Vector3::ZERO;
 		_legRotation[1] = Vector3::ZERO;
	}

	if (_hitPoint <= 0) {
		_state = EnemyState::Dead;
	}

	if (_state == EnemyState::Dead) {
		if (!_isDeadMotion) {
			_deadMotionCounter = 300.0f;	// 泡が消える時間
			_dropPosition = _pCollision->_line.getPosition(0.0f).transform(_matrix);
			deadMotionInit();
			_isDeadMotion = true;
		}


		deadMotionUpdate();
		return;
	}


	//------	何かに当たっていたら
	// 当たっているオブジェクト通知が来ていたら
	for (auto hitObject : _hitObjectType) {
		switch (hitObject)
		{
			// エネミーと衝突している場合
		case ObjectType::Enemy:
			for (int i = 0; i < (signed)_extrusion.size(); i++) {
				_position += _extrusion[i];
			}

			// 無理やりすぎ
			_extrusion.clear();

			break;
		case ObjectType::Player:
			_isAttack = false;
			_state = EnemyState::BackStep;
			break;

		case ObjectType::Wall:
			break;


		case ObjectType::PlayerWeapon:
			if (!_isInvincible) {
				_isInvincible = true;

				_isAttack = false;
				_state = EnemyState::BackStep;
				

				_color = Color(255,0,0);
				_hitPoint -= 10;
			}
			break;

		default:
			break;
		}
	}
	_extrusion.clear();
	_hitObjectType.clear();



	if (_isInvincible) {
		_invincibleFlameCount++;
		if (_invincibleFlameCount > 30) {
			_invincibleFlameCount = 0;
			_isInvincible = false;
			_color = Color(0,0,255);
		}
	}



	_velocity -= _gravity;	// 重力
	_position._y += _velocity;

	if (_position._y < 0.0f) {
		_position._y = 0.0f;
		_velocity = 0.0f;
	}

	switch (_state)
	{
	case EnemyState::Attack:
		attack();
		break;
	case EnemyState::AttackPreliminary:
		attackPreliminary();
		break;

	case EnemyState::CoolTime:
		coolTime();
		break;
	case EnemyState::BackStep:
		nockback();
		break;
	case EnemyState::Dead:
		//deadMotion();
		break;
	}


	debugSearc();

#if 0
	//---- 法線の計算
	//Vector3 AB = (Vector3(0.0f, 0.0f, 1.0f) - Vector3(0.0f, 1.0f, 1.0f));
	//Vector3 AC = (Vector3(1.0f, 1.0f, 1.0f) - Vector3(0.0f, 0.0f, 1.0f));

	// X 
	Vector3 AB = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 AC = Vector3(0.0f, 0.0f, -1.0f);
	_cross[0] = Vector3::cross(AC, AB).normalize() * 3;

	// Z
	AC = Vector3(1.0f, 0.0f, 0.0f);
	_cross[1] = Vector3::cross(AC, AB).normalize() * 3;

	// Y
	AB = Vector3(0.0f, 0.0f, 1.0f);
	_cross[2] = Vector3::cross(AB, AC).normalize() * 3;
#endif

	armUpdate();

	//_rotetion._y++;
	//if( _rotetion._y > 360.0f )	_rotetion._y = 0.0f;


	_matrix = Matrix::wolrdMatrixTransform(_rotation, _position);

	Vector3 offset = Vector3::ZERO;
	Vector3 rot    = Vector3::ZERO;
	switch(_state)
	{
	case EnemyState::Attack:
		//offset = Vector3(0.75f, -1.0f, +0.0f);
		rot._x = -0.0f;
		rot._z = -140.0f;
		break;
	case EnemyState::AttackPreliminary:
		rot._x = -100.0f;
		rot._z = -140.0f;
		break;
	case EnemyState::CoolTime:
		_armrRotation[0] = Vector3(60.0f, 180.0f, -40.0f); //(-45.0f,0.0f,0.0f);
		_armrRotation[1] = Vector3(-60.0f, 0.0f, -40.0f);
		_handRotation[0] = Vector3::ZERO;
		_handRotation[1] = Vector3::ZERO;

		rot._x = -120.0f;
		rot._z = -140.0f;
		break;
	default:
		offset = Vector3(0.0f, -0.0f, +0.0f);

		_armrRotation[0] = Vector3(60.0f, 180.0f, -40.0f); //(-45.0f,0.0f,0.0f);
		_armrRotation[1] = Vector3(-60.0f, 0.0f, -40.0f);
		_handRotation[0] = Vector3::ZERO;
		_handRotation[1] = Vector3::ZERO;

		rot._x = -120.0f;
		rot._y = 90.0f;
		rot._z = -140.0f;
		break;
	}

	_sowrdMatrix = Matrix::rotateY(rot._y) * Matrix::rotateX(DEG_TO_RAD(rot._x)) * Matrix::rotateZ(DEG_TO_RAD(rot._z)) * Matrix::translate(offset) * _handMatrix[0];

	_weapon->update(_sowrdMatrix);

	_newPosition = _position;
}

//======================================================================================
//! 描画
//======================================================================================
void NomalEnemy::render()
{
	a();

	if (_state == EnemyState::Dead && _isDeadMotion) {
		deadMotionRender();
		_weapon->render(_weaponAlpha);
		return;
	}

#if 0
	glPushMatrix();
	Vector3 position(_position._x, _position._y + 7.0f, _position._z);
	Matrix m = Matrix::wolrdMatrixTransform(_rotation, position);
	glLoadMatrixf((GLfloat*)&m);

	// 法線
	drawArrow(Vector3(0.0f, 3.0f, 0.0f), _cross[0], Color(255, 0, 0));	// x
	drawArrow(Vector3(0.0f, 3.0f, 0.0f), _cross[1], Color(0, 255, 0));	// y
	drawArrow(Vector3(0.0f, 3.0f, 0.0f), _cross[2], Color(0, 0, 255));	// z
	glPopMatrix();
#endif

	glPushMatrix();
	glLoadMatrixf((GLfloat*)&_sowrdMatrix);
	_weapon->render();
	glPopMatrix();

	if(_isSearc) { 
		hpRender();
	}


#if 0
	std::vector<std::string> text1;
	text1.push_back("X = ");
	text1.push_back("Y = ");
	text1.push_back("Z = ");
	glColor3ub(0, 255, 0);
	DebugStringValue(text1, _armrRotation[0], Vector3(10, 500, 0), Vector3(150.0f, 0.0f, 0.0f));

	std::vector<std::string> text2;
	text2.push_back("X = ");
	text2.push_back("Y = ");
	text2.push_back("Z = ");
	glColor3ub(0, 255, 0);
	DebugStringValue(text2, _position, Vector3(10, 600, 0), Vector3(150.0f, 0.0f, 0.0f));

#endif
}

//======================================================================================
//! 解放
//======================================================================================
void NomalEnemy::cleanup()
{
	for (int i = 0; i < _deadSphereMax; ++i) {
		SAFE_DELETE(_deadMotionSphere[i]);
	}

	c();



	EnemyBase::cleanup();
}

//======================================================================================
//! 腕更新
//======================================================================================
void NomalEnemy::armUpdate()
{
	if (_weapon != nullptr) {
		_weapon->update(_armrMatrix[0]);
	}
}

void NomalEnemy::debugSearc()
{ 
	if (_isSearc) {
		flag = true;
		if (_state == EnemyState::Wait || _state == EnemyState::Move) {
			_state = EnemyState::AttackPreliminary;
		}

		float length = abs((_playerPosition - _position).length());
		// プレイヤーに向かうベクトルを求める
		Vector3	dir = _playerPosition - _position;
		dir.normalize();

		//			float rot = atan2(DEG_TO_RAD(dir._x), DEG_TO_RAD(dir._z));
		float rot = atan2(dir._x, dir._z);

		if (_state != EnemyState::Attack && _state != EnemyState::CoolTime) {
			_rotation._y = rot * (180.0f / 3.14f);
		}

		if (length < 11.0f) {
			if (_state != EnemyState::CoolTime && _state != EnemyState::Attack) {
				_legRotation[0]._x = 0.0f;
				_legRotation[1]._x = 0.0f;
			}
		}
		else {
			if (_state != EnemyState::CoolTime && _state != EnemyState::Attack) {
				dir = dir.normalize();

				_position._x += dir._x * 0.6f;
				_position._z += dir._z * 0.6f;

				// 足アニメーション(仮)
				if (DEG_TO_RAD(_legRotation[0]._x) > PI * 0.2f ||
					(DEG_TO_RAD(_legRotation[0]._x) < PI * -0.2f)) {
					_legAnimetionSpeed[0] *= -1;
				}

				if (DEG_TO_RAD(_legRotation[1]._x) > PI * 0.2f ||
					(DEG_TO_RAD(_legRotation[1]._x) < PI * -0.2f)) {
					_legAnimetionSpeed[1] *= -1;
				}
				_legRotation[0]._x += _legAnimetionSpeed[0];
				_legRotation[1]._x -= _legAnimetionSpeed[1];

			}
			else {
				//if (_state == EnemyState::Attack) {
				//	_armRotation._x += 1.5f;
				//	if (_armRotation._x > 140.0f) {
				//		_armRotation._x = 0.0f;
				//		_armRotation._z = 0.0f;
				//		_state = EnemyState::CoolTime;
				//	}
				//}
			}
		}
	}
	else {
		if (_state == EnemyState::Attack) {
			_armrRotation[0]._x += 3.0f;
			_armrRotation[1]._x += 3.0f;
			if (_armrRotation[0]._x > 20) {
				_armrRotation[0] = Vector3(-60, 0, 40);//(-45.0f,0.0f,0.0f);
				_armrRotation[1] = Vector3(-60, 0, -40);
				_state = EnemyState::Wait;
			}
		}
		else {
			_state = EnemyState::Wait;
		}

		// 初期座標に向かうベクトルを求める
		Vector3	dir = (_firstPosition - _position).normalize();

		if (abs((_firstPosition - _position).length()) > 2.0f) {
			float rot = atan2(dir._x, dir._z);

			_rotation._y = rot * (180.0f / 3.14f);

			_position._x += dir._x * 0.7f;
			_position._z += dir._z * 0.7f;

			// 足アニメーション(仮)
			if (DEG_TO_RAD(_legRotation[0]._x) > PI * 0.2f ||
				(DEG_TO_RAD(_legRotation[0]._x) < PI * -0.2f)) {
				_legAnimetionSpeed[0] *= -1;
			}

			if (DEG_TO_RAD(_legRotation[1]._x) > PI * 0.2f ||
				(DEG_TO_RAD(_legRotation[1]._x) < PI * -0.2f)) {
				_legAnimetionSpeed[1] *= -1;
			}
			_legRotation[0]._x += _legAnimetionSpeed[0];
			_legRotation[1]._x -= _legAnimetionSpeed[1];
			_state = EnemyState::BackMove;
			_warpCounter++;
			if(_warpCounter > 800) { 
				_position = _firstPosition;
				_rotation = _firstRotation;
				_state = EnemyState::Wait;
				_warpCounter = 0;
				flag = false;
			}
		}
		else {
			_position = _firstPosition;
			_rotation = _firstRotation;
			_state = EnemyState::Wait;
			_warpCounter = 0;
			flag = false;
		}
	}
}


void NomalEnemy::attack()
{
	_armrRotation[0]._x -= 10.5f;
	_armrRotation[1]._x += 10.5f;
	//_armrRotation[0]._x -= 1.0f;
	//_armrRotation[1]._x += 1.0f;

	if (_armrRotation[0]._x < 30.0f) {
		_armrRotation[0]._x = 60.0f;
		_armrRotation[1]._x = -60.0f;
		_state = EnemyState::CoolTime;
		_isAttack = false;
	}
}

// 攻撃予備動作
void NomalEnemy::attackPreliminary()
{
	_armrRotation[0]._x += 9.0f;
	_armrRotation[1]._x -= 9.0f;
	//_armrRotation[0]._x += 1.0f;
	//_armrRotation[1]._x -= 1.0f;
	if (_armrRotation[0]._x > 180.0f) {
		//_armrRotation[0] = Vector3(-60, 0, 40);//(-45.0f,0.0f,0.0f);
		//_armrRotation[1] = Vector3(-60, 0, -40);//(-45.0f,0.0f,0.0f);

		_armrRotation[0]._x =  180.0f;
		_armrRotation[1]._x = -180.0f;
		if(abs((_playerPosition - _position).length()) < 14.0f ) { 
			_state = EnemyState::Attack;
		}
	}
}

void NomalEnemy::coolTime()
{
	_attackCooltimeCount++;
	if (_attackCooltimeCount > 60) {
		_state = EnemyState::Wait;
		_attackCooltimeCount = 0;

	}
	_armRotation._x = _armRotation._z = 0.0f;
}

void NomalEnemy::nockback()
{
	//_armrRotation[0]._x = _armrRotation[0]._z = 0.0f;

	Vector3 dir = (_position - _playerPosition).normalize() * 1.5f;
	dir._y = sinf(0.2f);

	_count++;
	if (_count >= 20) {
		_count = 0;
		_state = EnemyState::Wait;
	}

	_position   += dir;
}

void NomalEnemy::deadMotionInit()
{
	for (int i = 0; i < _deadSphereMax; i++) {
		_deadMotionSphere[i] = new Sphere();
		if (_deadMotionSphere[i] == nullptr) continue;

		int randNum = rand() % 360;
		_deadMotionSphere[i]->_position._x = _position._x + (sinf(DEG_TO_RAD(randNum)) * 4.0f);
		_deadMotionSphere[i]->_position._z = _position._z + (cosf(DEG_TO_RAD(randNum)) * 4.0f);
		_deadMotionSphere[i]->_position._y = _position._y + (rand() % 15) - 15;
		_deadMotionSphere[i]->_radius = 1.1f * (rand() * 0.00002f);
	}

	_armrRotation[0] = Vector3(60.0f, 180.0f, -40.0f);//(-45.0f,0.0f,0.0f);
	_armrRotation[1] = Vector3(-60.0f, 0.0f, -40.0f);
	_handRotation[0] = Vector3::ZERO;
	_handRotation[1] = Vector3::ZERO;
	_legRotation[0] = Vector3::ZERO;
	_legRotation[1] = Vector3::ZERO;
}

void NomalEnemy::deadMotionUpdate()
{
	for (int i = 0; i < _deadSphereMax; i++) {
		_deadMotionSphere[i]->_position._y += 0.08f;
	}

	if (_deadMotionCounter <= 0) {
		_isDead = true;
		return;
	}

	_deadMotionCounter -= 2.0f;
	_position._y -= 0.03f;
	_weaponAlpha -= 3.0f;


	_matrix = Matrix::wolrdMatrixTransform(_rotation, _position);

	Vector3 offset = Vector3::ZERO;
	Vector3 rot = Vector3::ZERO;

	_armrRotation[0] = Vector3(60.0f, 180.0f, -40.0f); //(-45.0f,0.0f,0.0f);
	_armrRotation[1] = Vector3(-60.0f, 0.0f, -40.0f);
	_handRotation[0] = Vector3::ZERO;
	_handRotation[1] = Vector3::ZERO;

	rot._x = -120.0f;
	rot._y = 90.0f;
	rot._z = -140.0f;
	_sowrdMatrix = Matrix::rotateY(rot._y) * Matrix::rotateX(DEG_TO_RAD(rot._x)) * Matrix::rotateZ(DEG_TO_RAD(rot._z)) * Matrix::translate(offset) * _handMatrix[0];
	_weapon->update(_sowrdMatrix);

}

void NomalEnemy::deadMotionRender()
{
	glLoadIdentity();

	_color = Color(0, 0, 255);
	float c = _deadMotionCounter;
	if (c > 255) c = 255;

	// 半透明合成ON
	glEnable(GL_BLEND);
	// 通常ブレンド
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < _deadSphereMax; i++) {
		Render::getInstance()->drawSphereFan(*_deadMotionSphere[i], Color(0, 0, 0), c);
	}

	Render::getInstance()->drawPlaneCircleTriangleFan(_dropPosition, 5.0f, Color(0, 0, 0), c);

	// 半透明合成OFF
	glDisable(GL_BLEND);
}

void NomalEnemy::a()
{
	float alpha				   = _deadMotionModelAlpha;
	if (alpha <= 0.0f)   alpha = 0.0f;
	if (alpha > 255.0f)  alpha = 255.0f;
	Color c(_color._r, _color._g, _color._b, alpha);

	if (_isDeadMotion) {
		// ポリゴンオフセット機能でデプスをずらす
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);

		// アルファテスト有効化
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1f);	// 比較条件　A > 0.5のときに描画する

										// 半透明合成ON
		glEnable(GL_BLEND);
		// 通常ブレンド
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_deadMotionModelAlpha -= 2.5f;
	}

	glPushMatrix();

	// body
	_bodyMatrix = Matrix::translate(_bodyPosition) * _matrix;
	glLoadMatrixf((GLfloat*)&_bodyMatrix);
	Render::getInstance()->drawAABB(_body->_min, _body->_max, c);
	
	glPushMatrix();
	// arm[1]
	//_armrMatrix[0] = Matrix::rotateZ(DEG_TO_RAD(_armrRotation[0]._z)) * Matrix::rotateX(DEG_TO_RAD(_armrRotation[0]._x)) * Matrix::translate(_armrPosition[0]) * _bodyMatrix;
	_armrMatrix[0] = Matrix::rotateZ(DEG_TO_RAD(_armrRotation[0]._z)) * Matrix::rotateX(DEG_TO_RAD(_armrRotation[0]._x)) * Matrix::rotateY(DEG_TO_RAD(_armrRotation[0]._y)) * Matrix::translate(_armrPosition[0]) * _bodyMatrix;
	glLoadMatrixf((GLfloat*)&_armrMatrix[0]);
	Render::getInstance()->drawAABB(_arm[0]->_min, _arm[0]->_max, c);

	// arm[1]
	_armrMatrix[1] = Matrix::rotateZ(DEG_TO_RAD(_armrRotation[1]._z)) * Matrix::rotateX(DEG_TO_RAD(_armrRotation[1]._x)) * Matrix::rotateY(DEG_TO_RAD(_armrRotation[1]._y)) * Matrix::translate(_armrPosition[1]) * _bodyMatrix;
	glLoadMatrixf((GLfloat*)&_armrMatrix[1]);
	Render::getInstance()->drawAABB(_arm[1]->_min, _arm[1]->_max, c);
	

	// hand[0]
	_handMatrix[0] = Matrix::wolrdMatrixTransform(_handRotation[0], _handPosition[0]) * _armrMatrix[0];
	glLoadMatrixf((GLfloat*)&_handMatrix[0]);
	Render::getInstance()->drawAABB(_hand[0]->_min, _hand[0]->_max, c);

	// hand[1]
	_handMatrix[1] = Matrix::wolrdMatrixTransform(_handRotation[1], _handPosition[1]) * _armrMatrix[1];
	glLoadMatrixf((GLfloat*)&_handMatrix[1]);
	Render::getInstance()->drawAABB(_hand[1]->_min, _hand[1]->_max, c);

	// leg
	_legMatrix[0] = Matrix::wolrdMatrixTransform(_legRotation[0], _legPosition[0]) * _bodyMatrix;
	glLoadMatrixf((GLfloat*)&_legMatrix[0]);
	Render::getInstance()->drawAABB(_leg[0]->_min, _leg[0]->_max, c);
	// leg
	_legMatrix[1] = Matrix::wolrdMatrixTransform(_legRotation[1], _legPosition[1]) * _bodyMatrix;
	glLoadMatrixf((GLfloat*)&_legMatrix[1]);
	Render::getInstance()->drawAABB(_leg[1]->_min, _leg[1]->_max, c);

	if(_isDeadMotion) {
		glDisable(GL_BLEND);
		// アルファテスト無効化
		glDisable(GL_ALPHA_TEST);

		// ポリゴンオフセット無効化
		glDisable(GL_POLYGON_OFFSET_FILL);
	}



	glPopMatrix();
}

void NomalEnemy::b()
{
	//glLoadMatrixf((GLfloat*)&_armMatrix);
//	drawAABB(_body->_min, _body->_max, _color);
	//drawBox(&_armrPosition[0]);
	//drawBox(&_armrPosition[1]);
	//drawBox(&_legPosition[0]);
	//drawBox(&_legPosition[2]);
}

void NomalEnemy::c()
{
	SAFE_DELETE(_body);
	SAFE_DELETE(_arm[0]);
	SAFE_DELETE(_arm[1]);
	SAFE_DELETE(_hand[0]);
	SAFE_DELETE(_hand[1]);
	SAFE_DELETE(_leg[0]);
	SAFE_DELETE(_leg[1]);
}

void NomalEnemy::hpRender()
{
	Vector3 offsetPosition(2.5f, 8.0f, 0.0f);
	Matrix m = Matrix::translate(offsetPosition) * _matrix;
	glLoadMatrixf((GLfloat*)&m);
	// 一時static
	Vector3 size(_hitPoint * 0.05f, -0.7, 0.0f);

	Render::getInstance()->drawParticleBillboard(Vector3::ZERO, size, Color(0,255,0,255));
}