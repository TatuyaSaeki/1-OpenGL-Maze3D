#include "stdafx.h"

//======================================================================================
//! コンストラクタ
//======================================================================================
BossEnemy::BossEnemy()
	: _longAttackMagic(nullptr)
	, _shortAttackSphere(nullptr)
	, _longAttackSphereRadius(25.0f)

{
	for (int i = 0; i < 3; i++) {
		_cross[i] = Vector3::ZERO;
	}
}

//======================================================================================
//! デストラクタ
//======================================================================================
BossEnemy::~BossEnemy()
{

}

//======================================================================================
//! 初期化
//======================================================================================
bool BossEnemy::init(float worldGravity, const Vector3& position, const int bossHitPoint)
{
	// ObjectBase
	_objectType		= ObjectType::Enemy;
	_position		= position;
	_firstPosition	= position;

	_rotation		= Vector3(0.0f, 0.0f, 0.0f);
	_color			= Color(0, 255, 0);

	Vector3 lines[] = {
		Vector3(0.0f, -3.0f, 0.0f),
		Vector3(0.0f, 20.0f, 0.0f)
	};
	_pCollision		= new Capsule(lines[0], lines[1], 10.0f);

	_hitPoint		= 50;
	
	
	_isSearc		= false;

	_scale			= Vector3(0.25f, 0.25f, 0.25f);

	_isLongAttack	= false;
	_isShortAttack	= false;
	_attackCount	= 0;

	// 直す
	_flag			= false;
	_playerPosition1 = Vector3::ZERO;

	_count			= 0.0f;
	_sinCount		= 0.0f;

	_model			= new Model();
	if (_model == nullptr)	return false;
	if (_model->load("data/Model/wizard003/wizard003.mqo", 0.5f) == false) {
		MessageBox(nullptr, "cart.mqo ファイルの読み込みに失敗しました.", "エラー", MB_OK);
		return false;
	}

	_searchRange = 250.0f;
	_searc1 = 250.0f;
	_searc2 = 220.0f;
	_searc3 = 29.0f;

	_longAttackCapsule = nullptr;
	_shortAttackSphere = nullptr;

	_c = 0;

	_dropPosition = Vector3::ZERO;

	_damageSound = new Sound();
	if (_damageSound == nullptr) return false;
	_damageSound->loadFile(L".\\data\\Sound\\damage4.mp3");
	_damageSound->volumeDown(1500);

	_longAttackMagic = new Texture();
	_longAttackMagic->load("data/Texture/Effect/LongAttackMagic.png");

	_warpWeitCount = 0;


	_warpFlag = false;
	_warpSphere = nullptr;
	_isWarp = false;

	_modelAlpha = 255;
	_warpFlag1 = false;
	_isLongAttack2 = false;
	_isLongAttack000 = false;
	_longAttackSphereNum = 0;
	_state2 = EnemyState::Wait;
	_state3 = EnemyState::Wait;
	_isLongAttackEnd = false;

	_model->setColor(128, 128, 128, 255);

	_isDeadMotion = false;

	_deadMotionCounter = 500;

	_warpAlpha = 255;

	_warpSound = new Sound();
	_warpSound->loadFile(L".\\data\\Sound\\magic-worp1.mp3");
	_warpSound->volumeDown(2500);
		
	for(int i = 0; i < 8; i++) {
		_magicSound[i] = new Sound();
		_magicSound[i]->loadFile(L".\\data\\Sound\\magic_wave4.mp3");
		_magicSound[i]->volumeDown(3800);
	}
	

	return true;
}

//======================================================================================
//! 更新
//======================================================================================
void BossEnemy::update()
{

	if(_state != EnemyState::Dead) { 
	// 当たっているオブジェクト通知が来ていたら
	for (auto hitObject : _hitObjectType) {
		switch (hitObject)
		{
		case ObjectType::Wall:
			for (int i = 0; i < (signed)_extrusion.size(); i++) {
				_position += _extrusion[i];
			}
			// 無理やりすぎ
			_extrusion.clear();
			break;

		case ObjectType::PlayerWeapon:
			if (!_isInvincible) {

				if (_state3 != EnemyState::BackStep) {
					_isLongAttack = false;
					SAFE_DELETE(_longAttackCapsule);
					_isInvincible = true;
					_state3 = EnemyState::BackStep;
					_damageSound->playRoop();
					_hitPoint -= 10;
					_model->setColor(Color(255, 0, 0, _model->getColor()._a));
				}
			}
			if (_hitPoint <= 0) {
				//_dropPosition = _pCollision->_line.getPosition(0.5f).transform(_matrix);
				_state = EnemyState::Dead;
			}
			break;

		default:
			break;
		}
	}}



	_hitObjectType.clear();

	if(_state != EnemyState::Dead) { 
		searc();
	}

	

	if (_isInvincible) {
		_invincibleFlameCount++;
		if (_invincibleFlameCount > 10) {
			_invincibleFlameCount = 0;
			_isInvincible		  = false;
			_model->setColor(128,128,128, _model->getColor()._a);
		}
	}

	_sinCount++;
	if(_sinCount >= 360.0f) _sinCount = 0.0001f;
	_position._y += sinf(DEG_TO_RAD(_sinCount)) / 60;


	




	//---- 法線の計算
	//Vector3 AB = (Vector3(0.0f, 0.0f, 1.0f) - Vector3(0.0f, 1.0f, 1.0f));
	//Vector3 AC = (Vector3(1.0f, 1.0f, 1.0f) - Vector3(0.0f, 0.0f, 1.0f));

	// X 
	Vector3 AB = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 AC = Vector3(0.0f, 0.0f, -1.0f);
	_cross[0]  = Vector3::cross(AC, AB).normalize() * 3;

	// Z
	AC		   = Vector3(1.0f, 0.0f, 0.0f);
	_cross[1]  = Vector3::cross(AC, AB).normalize() * 3;

	// Y
	AB		   = Vector3(0.0f, 0.0f, 1.0f);
	_cross[2]  = Vector3::cross(AB, AC).normalize() * 3;

	_matrix    = Matrix::wolrdMatrixTransform(_rotation, _position);
}

void BossEnemy::lateUpdate()
{
	if (_state == EnemyState::Dead) { 
		a();
	}

	switch (_state)
	{
	case EnemyState::ShortAttack:
		shortAttack();
		break;
	default:
		_isShortAttack = false;
		SAFE_DELETE(_shortAttackSphere);
	}

	switch (_state2)
	{
	case EnemyState::LongAttack2:
		longAttack2();
		break;

	case EnemyState::CoolTime:
		coolTime();
		_isLongAttack2 = false;
		break;
	default:
		_isLongAttack2 = false;
	}

	switch (_state3)
	{
	case EnemyState::BackStep:
		warp();
		break;
	}
}



//======================================================================================
//! 描画
//======================================================================================
void BossEnemy::render()
{
	// 本体
	Matrix matrix = Matrix::scale(_scale) * _matrix;
	glLoadMatrixf((GLfloat*)&matrix);
	{
		_model->render();
	}
	
	{	
		glPushMatrix();
		Vector3 offset1(-37.0f, 85.0f, 0.0f);
		Matrix m = Matrix::translate(offset1);
		glMultMatrixf((GLfloat*)&m);
		glColor4ubv((GLubyte*)&Color(255, 0, 0, _model->getColor()._a));
		RenderState::setBlendMode(true);
		glutSolidSphere(5.0f, 30, 30);
		RenderState::setBlendMode(false);
		glPopMatrix();
	}
	{
		glPushMatrix();
		Vector3 offset2(+37.0f, 85.0f, 0.0f);
		Matrix m = Matrix::translate(offset2);
		glMultMatrixf((GLfloat*)&m);
		glColor4ubv((GLubyte*)&Color(255, 0, 0, _model->getColor()._a));
		RenderState::setBlendMode(true);
		glutSolidSphere(5.0f, 30, 30);
		RenderState::setBlendMode(false);
		glPopMatrix();
	}
	
#if 0
	// モデル以外のための行列初期化( 中心点や索敵範囲 )
	matrix.identity();

	//Vector3 position = _matrix.translate();

	Capsule capsule = *_pCollision;
	//capsule._line._position[0] = capsule._line._position[0].transform(_matrix);
	//capsule._line._position[1] = capsule._line._position[1].transform(_matrix);

	glLoadMatrixf((GLfloat*)&_matrix);

	auto instance = Render::getInstance();
	// 中心点
	instance->drawSphere(capsule._line.getPosition(0.5f), 0.1f, _color);

	// 索敵範囲
	instance->drawPlaneCircle(capsule._line.getPosition(0.4f), _searchRange, _color);
	instance->drawPlaneCircle(capsule._line.getPosition(0.4f), _searc1, Color(0, 0, 255));
	instance->drawPlaneCircle(capsule._line.getPosition(0.4f), _searc3, Color(0, 0, 255));


	// 当たり判定用の円柱
	instance->drawCapsule(&capsule, _color);

	// 法線
	instance->drawArrow(Vector3(0.0f, 10.0f, 0.0f), _cross[0], Color(255, 0, 0));	// x
	instance->drawArrow(Vector3(0.0f, 10.0f, 0.0f), _cross[1], Color(0, 255, 0));	// y
	instance->drawArrow(Vector3(0.0f, 10.0f, 0.0f), _cross[2], Color(0, 0, 255));	// z

	glLoadIdentity();
#endif

	auto instance = Render::getInstance();

	if (_isShortAttack) {
		if (_shortAttackSphere != nullptr) {
			glPushMatrix();
			auto p = _shortAttackSphere->_position;
			p._y += 2.0f;
			Matrix m = Matrix::translate(p);
			glLoadMatrixf((GLfloat*)&m);
			glColor4ubv((GLubyte*)&Color(0, 0, 255, 255));
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glutSolidSphere(_shortAttackSphere->_radius, 20, 20);
			glDisable(GL_BLEND);
			glPopMatrix();
		}
	}


	if (_state3 == EnemyState::BackStep) {
		if (_warpSphere != nullptr) {
			glPushMatrix();
			Matrix m = Matrix::translate(_warpSphere->_position);
			glLoadMatrixf((GLfloat*)&m);
			glColor4ubv((GLubyte*)&Color(0, 0, 0, _warpAlpha));
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glutSolidSphere(_warpSphere->_radius, 20, 20);
			glDisable(GL_BLEND);

			glPopMatrix();
		}
	}

	if (_state == EnemyState::Dead) {
		deadMotionRender();
	}

	if (_isLongAttack2) {
		RenderState::setBlendMode(true);
		auto itr1 = _longAttack2.begin();
		auto itr2 = _longAttack2.end();

		
		for (; itr1 != itr2;) {
		glPushMatrix();
		
			Matrix m = Matrix::translate((*itr1)->_longAttack2MagicPosition);
			glLoadMatrixf((GLfloat*)&m);
			auto a = (*itr1)->_magicAlpha;
			glColor4ubv((GLubyte*)&Color(255, 0, 0, (*itr1)->_magicAlpha));
			Vector3 positionDifference(30.0f, 1.5f, 30.0f);
			

			Texture::set(_longAttackMagic);
			instance->drawParticleRect(Vector3::ZERO, positionDifference, Color(255,0,0, (*itr1)->_magicAlpha));
			

			Texture::set(nullptr);
		//	instance->drawSphere(Sphere(Vector3::ZERO, (*itr1)->_magicRadius), Color(0, 255, 0));
			
		glPopMatrix();

#if 1
			//if((*itr1)->_isAlive && (*itr1)->_longAttack2Sphere != nullptr) { 
		if ((*itr1)->_isAlive) {
				glPushMatrix();
					Matrix m = Matrix::translate((*itr1)->_longAttack2Sphere._position);
					glLoadMatrixf((GLfloat*)&m);
					glColor4ubv((GLubyte*)&Color(128, 0, 0, 255));// (*itr1)->_longAttack2Alpha));

					glutSolidSphere((*itr1)->_longAttack2Sphere._radius, 30, 30);

				glPopMatrix();
			}

#endif
			RenderState::setBlendMode(false);
			++itr1;
		}

		
	}

	glPushMatrix();
	if (_isSearc) {
		hpRender();
	}
	glPopMatrix();

#if 0
	Vector3 positionDifference(20.0f, 0.01f, 20.0f);
	static float angle = 0.0f;
	angle += 0.1f;

	positionDifference._x = positionDifference._x * sinf(DEG_TO_RAD(angle)) + positionDifference._z * cosf(DEG_TO_RAD(angle));
	positionDifference._z = positionDifference._z * cosf(DEG_TO_RAD(angle)) - positionDifference._x * sinf(DEG_TO_RAD(angle));
	drawParticleRect(_position, positionDifference, _effectTextrue);
#endif

#if 0
	glPushMatrix();
	std::vector<std::string> text0;
	text0.push_back("X = ");
	text0.push_back("Y = ");
	text0.push_back("Z = ");
	glColor3ub(255, 0, 0);
	DebugStringValue(text0, _position, Vector3(10, 230, 0), Vector3(100.0f, 0.0f, 0.0f));
	glPopMatrix();

	if(_shortAttackSphere != nullptr) {
		std::vector<std::string> text0;
		text0.push_back("X = ");
		text0.push_back("Y = ");
		text0.push_back("Z = ");
		glColor3ub(255, 0, 0);
		DebugStringValue(text0, _shortAttackSphere->_position, Vector3(10, 270, 0), Vector3(100.0f, 0.0f, 0.0f));
	}else
	{
		std::vector<std::string> text0;
		text0.push_back("X = ");
		text0.push_back("Y = ");
		text0.push_back("Z = ");
		glColor3ub(255, 0, 0);
		DebugStringValue(text0, Vector3::ZERO, Vector3(10, 270, 0), Vector3(100.0f, 0.0f, 0.0f));
	}
	
	
	// プレイヤーに向かうベクトルを求める
	Vector3	dir = _playerPosition - _position;
	float length = abs(dir.length());
	std::vector<std::string> text1;
	text1.push_back("len = ");
	glColor3ub(255, 0, 0);
	DebugStringValue(text1, Vector3(length, 0, 0), Vector3(10, 350, 0), Vector3(100.0f, 0.0f, 0.0f));
#endif


}

//======================================================================================
//! 解放
//======================================================================================
void BossEnemy::cleanup()
{
	for(auto& longAttack2 : _longAttack2) { 
		SAFE_DELETE(longAttack2);
	}
	_longAttack2.clear();

	for(int i = 0; i < 8;i++) {
		SAFE_DELETE_CLEANUP(_magicSound[i]);
	}
	
	SAFE_DELETE_CLEANUP(_warpSound);
	SAFE_DELETE(_warpSphere);
	SAFE_DELETE(_longAttackMagic);
	SAFE_DELETE(_shortAttackSphere);
	SAFE_DELETE(_longAttackCapsule);
	SAFE_DELETE_CLEANUP(_damageSound);
	EnemyBase::cleanup();
}


void BossEnemy::searc()
{
	// プレイヤーに向かうベクトルを求める
	Vector3	dir  = _playerPosition - _position;
	float rot	 = atan2(DEG_TO_RAD(dir._x), DEG_TO_RAD(dir._z));

	_rotation._y = rot * (180.0f / 3.14f);

	if (_isSearc) {
		if (_state == EnemyState::ShortAttack) return;
		if (_state3 == EnemyState::BackStep)    return;

		float length = abs(dir.length());

		if (length < _searc1) {						// Move
			if (length < _searc2) {					// LongAttack
				if (length < _searc3) {				// ShortAttack or BackStep
					_c++;
					int randNum = rand() % 700;

					if (randNum < 15 || _c > 60 * 3.0f) {
						_c = 0;
						SAFE_DELETE(_longAttackCapsule);
					_state = EnemyState::ShortAttack;
						return;
					}
					if(randNum < 10) {
					//	SAFE_DELETE(_longAttackCapsule);
					//	_state = EnemyState::BackStep;
						return;
					}
				}
				else {									// LongAttack
					if (_state == EnemyState::ShortAttack) return;

					int randNum = rand() % 100;
					if (randNum < 5) {
						//_state = EnemyState::LongAttack;
						if (_state2 == EnemyState::CoolTime)   return;
						if(_state3 != EnemyState::BackStep || _state != EnemyState::ShortAttack) { 
							_state2 = EnemyState::LongAttack2;
						}
						return;
					}
				}
			}					
		}											// Move
		else {
			if (_state2 == EnemyState::LongAttack2)  return;
			if (_state2 != EnemyState::LongAttack2 && _state != EnemyState::ShortAttack) {
				dir.normalize();
				_position._x += dir._x * 0.001f;
				_position._z += dir._z * 0.001f;
			}
		}
	}
//	else {
//		if (_state == EnemyState::LongAttack || _state == EnemyState::BackStep) return;
//		_state = EnemyState::Move;
//
//		// 初期座標に向かうベクトルを求める
//		Vector3	dir = _firstPosition - _position;
//		dir.normalize();
//
//
//		float rot = atan2(DEG_TO_RAD(dir._x), DEG_TO_RAD(dir._z));
//
//		_rotation._y = rot * (180.0f / 3.14f);
//
//		//_position._x += dir._x * 0.01f;
//		//_position._z += dir._z * 0.01f;
//	}

	_isSearc = false;
}

void BossEnemy::longAttack()
{
	_isShortAttack = false;
	_isLongAttack = true;
	_attackCount++;

	if (!_flag) {
		_playerPosition1 = _playerPosition;
		_flag = true;
	}

	
	if (_longAttackCapsule == nullptr) {
		Line line(_playerPosition1, _playerPosition1);
		_longAttackCapsule = new Capsule(line, 25.0f);
	}
	
	_longAttackMagic->addAlpha(1);
	
	if (_attackCount > 60 * 2.0f) {
		_longAttackCapsule->_line._position[1]._y += 0.3f;

		if (_longAttackCapsule->_line._position[1]._y > 50.0f) {
			_isLongAttack = false;
			_flag = false;
			_attackCount = 0;
			_state = EnemyState::CoolTime;
			_longAttackMagic->setAlpha(0);
			SAFE_DELETE(_longAttackCapsule);
		}
	}
}

void BossEnemy::longAttack2()
{
	_isLongAttack2 = true;
	
	static Vector3 dir = Vector3::ZERO;


	// 一度だけ
	if (!_flag) {
		_playerPosition2 = _playerPosition;
		_flag = true;
		dir = _playerPosition2 - _position;
		float length = abs((dir).length());

		// 距離に応じて最大個数を変化
		_longAttackSphereNum = (int)length / (int)(_longAttackSphereRadius * 2) + 1;

		Vector3 position0(_position);
		Vector3 position1(Vector3(0.0f, 8.0f, 0.0f));
		// 魔法人の位置を設定
		position1._y = 0.3f;

		// 一定時間ごとかつ最大数を超えていなかったら
		for (int i = 0; i < _longAttackSphereNum; i++) {
			//LongAttack2 magic = new LongAttack2();
			LongAttack2* magic = new LongAttack2();
			magic->_isMagic = false;
			magic->_magicAlpha = 0;
			//magic->_longAttack2Sphere = nullptr;
			//magic->_longAttack2MagicPosition = new Vector3(position0 + position1 + dir.normalize() * (_longAttackSphereRadius * 2 * (i + 1)));
			magic->_longAttack2MagicPosition =  Vector3(position0 + position1 + dir.normalize() * (_longAttackSphereRadius * 2 * (i + 1)));
			_longAttack2.push_back(magic);
			//_longAttack2.emplace_back(magic);
		}

		//_magicSound->playFromStart();
		
		_magicSound[0]->playRoop();
		_longAttack2[0]->_isMagic = true;
	}

	// 一時
	// サイズは変更されるから、カウンターを所持
	static int count = 0;
	static bool fff = false;
#if 1
	if (!fff) {
		for (int i = 0; i < _longAttack2.size(); i++) {
			if (_longAttack2[i]->_isMagic) {
				_longAttack2[i]->_magicAlpha += 5.0f;
				if (_longAttack2[i]->_magicAlpha > 255) {
					_longAttack2[i]->_magicAlpha = 255;
				}
				if (_longAttack2[i]->_magicAlpha > 70) {
					_longAttack2[i]->_magicRadius = 25.0f;

					if (_longAttack2[i]->_magicAlpha != 255) {
						if (i + 1 < _longAttack2.size()) {
							_longAttack2[i + 1]->_isMagic = true;
							
							_magicSound[i+1]->playRoop();
						}
						else {
							Vector3 offset(Vector3(0.0f, 8.0f, 0.0f));
							auto pos = _longAttack2[0]->_longAttack2MagicPosition;
							pos += offset;
							_longAttack2[0]->_longAttack2Sphere =  Sphere(pos, 0.0f);
							_longAttack2[0]->_isAlive = true;

							fff = true;
						}
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < _longAttack2.size(); i++) {
			if (_longAttack2[i]->_isAlive) {
				_longAttack2[i]->_longAttack2Sphere._radius += 1.0f;

				if (_longAttack2[i]->_longAttack2Sphere._radius > 30) {
					_longAttack2[i]->_isAlive = false;
					_longAttack2[i]->_isMagic = false;
					count++;
				}

				if (_longAttack2[i]->_longAttack2Sphere._radius > 15) {
					if (i + 1 < _longAttack2.size() && !_longAttack2[i + 1]->_isAlive) {
						Vector3 offset(Vector3(0.0f, 8.0f, 0.0f));
						auto pos = _longAttack2[i + 1]->_longAttack2MagicPosition;
						pos += offset;
						_longAttack2[i + 1]->_longAttack2Sphere = Sphere(pos, 0.0f);
						_longAttack2[i + 1]->_isAlive = true;
						break;
					}
				}
			}
		}
	}


	if(count == _longAttackSphereNum) {
		for(auto& longAttack : _longAttack2) {
			SAFE_DELETE(longAttack);
		}
		_longAttack2.clear();

		_isLongAttack2 = false;
		_flag = false;
		fff = false;
		_attackCount = 0;
		_state2 = EnemyState::CoolTime;
		_longAttackSphereNum = 0;
		_isLongAttack000 = false;
		count = 0;

		for(int i = 0; i < 5;i++){
			_magicSound[i]->stop();
		}
	}
	

#else 
	// 一定時間ごとかつ最大数を超えていなかったら
	if (_attackCount % 30 == 0 && count < _longAttackSphereNum) {
		auto longAttack2 = new LongAttack2();
		// 球の場所を設定									プレイヤーの向きに直径 * サイズ
		Vector3 position0(_position);
		Vector3 position1(Vector3(0.0f, 8.0f, 0.0f));
		position0._y = 0.0f;
		// 級の位置を
		longAttack2->_longAttack2Sphere = new Sphere(position0 + position1 + dir.normalize() * (_longAttackSphereRadius * 2 * (count + 1)), 0.0f);
		// 球のアルファ値を設定
		longAttack2->_longAttack2Alpha = 0.0f;
		// 魔法人の位置を設定
		position1._y = 0.3f;
		longAttack2->_longAttack2MagicPosition = new Vector3(position0 + position1 + dir.normalize() * (_longAttackSphereRadius * 2 * (count + 1)));
		// 追加
		_longAttack2.push_back(longAttack2);
		_attackCount = 0;

		count++;
	}
	_attackCount++;

	for (auto& longAttack2 : _longAttack2) {
		longAttack2->_count++;
		longAttack2->_magicAlpha += 0.3f;
		if (longAttack2->_magicAlpha > 255.0f) {
			longAttack2->_magicAlpha = 255.0f;
		}

		if (longAttack2->_count > 20 && !longAttack2->_isMagic) {
			longAttack2->_isMagic = true;
		}

		if (!longAttack2->_isAlive && longAttack2->_isMagic) {
			longAttack2->_isAlive = true;
		}

		if (longAttack2->_isAlive) {
			longAttack2->_longAttack2Sphere->_radius += 0.5f;
			// AddAlpha
			longAttack2->_longAttack2Alpha += 0.1f;

			// アルファ値が超えたら 255 に固定
			if (longAttack2->_longAttack2Alpha > 255.0f) {
				longAttack2->_longAttack2Alpha = 255.0f;
			}

			if (longAttack2->_longAttack2Sphere->_radius > _longAttackSphereRadius) {
				longAttack2->_isAlive = false;
			}
		}
	}

	if (_longAttack2.size() != 0) {
		auto itr = _longAttack2.begin();
		while (itr != _longAttack2.end()) {
			if (!(*itr)->_isAlive && (*itr)->_isMagic) {
				delete *itr;
				itr = _longAttack2.erase(itr);
				continue;
			}
			++itr;
		}
		if (_longAttack2.size() == 0) {
			_isLongAttack000 = true;
		}
	}

	if (_isLongAttack000) {
		_isLongAttack2 = false;
		_flag = false;
		_attackCount = 0;
		_state2 = EnemyState::CoolTime;
		_longAttackSphereNum = 0;
		_longAttack2.clear();
		_isLongAttack000 = false;
		count = 0;
	}

#endif
}

void BossEnemy::shortAttack()
{	
	_isShortAttack = true;
	//_isLongAttack2 = false;

	if (_shortAttackSphere == nullptr) {
		_shortAttackSphere = new Sphere(_position + _pCollision->_line.getPosition(0.5f), 0.0f);
	}

	_shortAttackSphere->_radius += 1.0f;

	if (_shortAttackSphere->_radius > 30.0f) {

		//SAFE_DELETE(_shortAttackSphere);

		// プレイヤーに向かうベクトルを求める
		Vector3	dir  = _playerPosition - _position;
		float length = abs(dir.length());
		if (length < 38.0f) {
			_state3 = EnemyState::BackStep;
		}
		_state = EnemyState::Wait;
		_isShortAttack = false;
		SAFE_DELETE(_shortAttackSphere);
	}
}

void BossEnemy::warp()
{
#if 0
	// 原点を中心にランダムで壁の外に出ないようにワープ
	if (_warpWeitCount > 60 && !_warpFlag) {
		_warpFlag = true;
	}
	else if (_warpWeitCount == 0) {
		_warpSphere = new Sphere();
	}
	else if (_warpFlag) {
		_warpSphere->_radius--;
	}
	else {
		_warpSphere->_radius++;
	}

	if (_warpSphere->_radius < 0.0f && _warpFlag) {
		Vector3 warpPosition = warpPositionInit();
		Vector3 dir = warpPosition - _playerPosition;
		float length = abs(dir.length());
		if (length < 50) {
			warp();
		}
		_position = warpPosition;
		_state = EnemyState::Wait;
		_warpWeitCount = 0;
		_warpFlag = false;
	}
#endif

	if (!_warpFlag && !_isWarp) {
		_warpSound->playRoop();
		_model->addAlpha(-10);
		if (_model->getColor()._a <= 0) {
			_warpFlag = true;
		}
	}

	if (_warpFlag && !_isWarp) {
		Vector3 warpPosition = warpPositionInit();
		Vector3 dir =  _playerPosition - warpPosition;
		float length = abs(dir.length());
		if (length < 110.0f) {
			warp();
		}
		_isWarp = true;

		if (_warpSphere == nullptr) {
			_warpSphere = new Sphere(warpPosition + _pCollision->_line.getPosition(0.8f), 0.0f);
			_position = warpPosition;
			_warpFlag = false;
			_model->setAlpha(0);
			_warpSound->playRoop();
		}
	}

	if (_isWarp) {
		_warpSphere->_radius += 0.5f;
		if (_warpSphere->_radius > 22.0f) {
			_warpFlag = true;
		}
	}


	if (_isWarp && _warpFlag) {
		_model->setAlpha(255);
		_warpFlag = false;
		_isWarp = false;
		SAFE_DELETE(_warpSphere);
		_state3 = EnemyState::Wait;
	}
}

Vector3 BossEnemy::warpPositionInit()
{
	Vector3 v = Vector3::ZERO;
	v._x = rand() % (150 * 2) - 150;
	v._z = rand() % (130 * 2) - 130;

	return v;
}

void BossEnemy::coolTime()
{
	_count++;
	if (_count > 60 * 2.0f) {
		_state2 = EnemyState::Wait;
		_count = 0.0f;
	}
}

Capsule* BossEnemy::longAttackCapsule()
{
	if(_longAttackCapsule != nullptr){
		return _longAttackCapsule;
	}
//	return nullptr;
}


Sphere* BossEnemy::shortAttackSphere()
{
	if (_shortAttackSphere != nullptr) {
		return _shortAttackSphere;
	}
	//return nullptr;
}


void BossEnemy::a()
{
	if(!_isDeadMotion) { 
		deadMotionInit();
	}

	deadMotionUpdate();


}

void BossEnemy::deadMotionInit()
{
	for (int i = 0; i < _deadSphereMax; i++) {
		_deadMotionSphere[i] = new Sphere();
		if (_deadMotionSphere[i] == nullptr) continue;

		int randNum = rand() % 360;
		_deadMotionSphere[i]->_position._x = _position._x + (sinf(DEG_TO_RAD(randNum)) * 15.0f);
		_deadMotionSphere[i]->_position._z = _position._z + (cosf(DEG_TO_RAD(randNum)) * 15.0f);
		_deadMotionSphere[i]->_position._y = _position._y + (rand() % 15) - 10;
		_deadMotionSphere[i]->_radius = 1.1f * (rand() * 0.00004f);

		_deadMotionSphereAlpha[i] = 255.0f;
		
		{
			int randNum = rand() % 15 + 5;
			_deadMotionSphereSpeed[i] = randNum * 0.005f;
		}
	}

	_state3 = EnemyState::Wait;
	_isDeadMotion = true;

	_model->setAlpha(255);
	_warpAlpha = 0;

	SAFE_DELETE(_shortAttackSphere);
	SAFE_DELETE(_warpSphere);
	if (_warpSphere == nullptr) {
		_warpSphere = new Sphere(_position + _pCollision->_line.getPosition(0.8f), 0.0f);
	}
}

void BossEnemy::deadMotionUpdate()
{
	for (int i = 0; i < _deadSphereMax; i++) {
		int a = rand() % 10;
		_deadMotionSphere[i]->_position._y += _deadMotionSphereSpeed[i];
		_deadMotionSphereAlpha[i] -= 1.0f;
	}

	_deadMotionCounter -= 2.0f;
	_position._y -= 0.025f;
	

	_model->addAlpha(-1);

	_warpAlpha++;

	if (_warpAlpha > 255) {
		_warpAlpha = 255;
	}

	if (_deadMotionCounter <= 0) {
		_isDead = true;
	}
}

void BossEnemy::deadMotionRender()
{
	if (_warpSphere != nullptr) {
		glPushMatrix();
		Matrix m = Matrix::translate(_warpSphere->_position);
		glLoadMatrixf((GLfloat*)&m);
		glColor4ubv((GLubyte*)&Color(128, 0, 0, _warpAlpha));
		// 半透明合成ON
		RenderState::setBlendMode(true);
		glutSolidSphere(_warpSphere->_radius, 20, 20);
		// 半透明合成OFF
		RenderState::setBlendMode(false);
		glPopMatrix();
	}



	for (int i = 0; i < _deadSphereMax; i++) {
		glPushMatrix();
		Matrix m = Matrix::translate(_deadMotionSphere[i]->_position);
		glLoadMatrixf((GLfloat*)&m);
		glColor4ubv((GLubyte*)&Color(128, 0, 0, _deadMotionSphereAlpha[i]));
		// 半透明合成ON
		RenderState::setBlendMode(true);
		glutSolidSphere(_deadMotionSphere[i]->_radius, 20, 20);
		// 半透明合成OFF
		RenderState::setBlendMode(false);
		glPopMatrix();
	}
}