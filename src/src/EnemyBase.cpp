#include "stdafx.h"


//======================================================================================
//! コンストラクタ
//======================================================================================
EnemyBase::EnemyBase()
:_pCollision(nullptr)
,_model(nullptr)
, _deadSphereMax(15)
{
	// ObjectBase
	_objectType = ObjectType::Enemy;
	_position = Vector3::ZERO;
	_rotation = Vector3::ZERO;
	_color = Color::BLACK;

	// CharacterBase
	_state = EnemyState::Wait;

	_weapon = nullptr;

	_isHit = false;
	_isDead = false;

	_gravity = 0.0f;
	_velocity = 0.0f;


	_searchRange = 0.0f;
	_isSearc = false;

	// Debug

	_playerPosition = Vector3::ZERO;

	_scale = Vector3::ZERO;

	for (int i = 0; i < _deadSphereMax; i++) {
		_deadMotionSphere[i] = nullptr;
	}

	_isDeadMotion = false;
	_deadMotionCounter = 0.0f;
	_deadMotionModelAlpha = 300.0f;
}

//======================================================================================
//! デストラクタ
//======================================================================================
EnemyBase::~EnemyBase()
{
}

//======================================================================================
//! 初期化
//======================================================================================
bool EnemyBase::init(float worldGravity)
{
	

	return true;
}

//======================================================================================
//! 更新
//======================================================================================
void EnemyBase::update()
{

}

//======================================================================================
//! 描画
//======================================================================================
void EnemyBase::render()
{
	_matrix = Matrix::identity();

	//---- 平行移動

	_matrix = Matrix::scale(_scale) * Matrix::wolrdMatrixTransform(_rotation, _position + Vector3(0.0f, -2.0f, 0.0f));

	glLoadMatrixf((GLfloat*)&_matrix);

	// 本体
	//drawCapsule(_capsule, _color);
	_model->render();



	// モデル以外のための行列初期化( 中心点や索敵範囲 )
	_matrix = Matrix::identity();

	//---- 平行移動

	_matrix = Matrix::wolrdMatrixTransform(_rotation, _position);

	glLoadMatrixf((GLfloat*)&_matrix);

	auto instance = Render::getInstance();
	// 一時
	instance->drawSphere(Vector3(0.0f, 0.0f, 0.0f), 2.5f, _color);

	// 中心点
	instance->drawSphere(Vector3(0.0f, 0.0f, 0.0f), 0.1f, _color);

	
	// 索敵範囲
	instance->drawPlaneCircle(Vector3(0.0f, 0.0f, 0.0f), _searchRange, _color);

	armRender(&_matrix, _armRotation, _armPosition);

	_weapon->render();
}

//======================================================================================
//! 解放
//======================================================================================
void EnemyBase::cleanup()
{
	SAFE_DELETE(_pCollision);
	SAFE_DELETE(_model);
	for (int i = 0; i < _deadSphereMax; i++) {
		SAFE_DELETE(_deadMotionSphere[i]);
	}
	CharacterBase::cleanup();
}

//======================================================================================
//! 腕更新
//======================================================================================
void EnemyBase::armUpdate()
{
	if (_weapon != nullptr)		_weapon->update(_armMatrix);
}

Vector3 EnemyBase::getDir()
{
	return _position - _oldPosition;
}