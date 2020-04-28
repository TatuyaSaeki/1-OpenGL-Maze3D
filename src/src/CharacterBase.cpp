#include "stdafx.h"

CharacterBase::CharacterBase()
: _weapon( nullptr )
, _isHit( false )
, _isDead( false )
, _gravity(0.0f)
, _velocity(0.0f)
, _hitPoint(0)
,_invincibleFlameCount(0)
,_isInvincible(false)
{
	// ˜r
	_armBox			= nullptr;
	_armMatrix		= Matrix::IDENTITY;
	_armPosition	= Vector3::ZERO;
	_armRotation	= Vector3::ZERO;
}

CharacterBase::~CharacterBase()
{
	if (_weapon != nullptr) {
		delete _weapon;
		_weapon = nullptr;
	}
}

void CharacterBase::addHitObjectType( ObjectType objectType)
{
	_hitObjectType.push_back(objectType);
}



//======================================================================================
//! ˜rXV
//======================================================================================
void CharacterBase::armUpdate()
{
}

//======================================================================================
//! ˜r•`‰æ
//======================================================================================
void CharacterBase::armRender(const Matrix* parentMatrix, Vector3& armRote, const Vector3& armPos)
{
	// ˜r
	_armMatrix = Matrix::wolrdMatrixTransform(armRote, armPos)  * (*parentMatrix);
	
	glLoadMatrixf((GLfloat*)&_armMatrix);
	Render::getInstance()->drawAABB(_armBox->_min, _armBox->_max, _color);
}

void CharacterBase::setHitObjectType(ObjectType ot)
{
	_hitObjectType.push_back(ot);
}

void CharacterBase::addExtrusion(const Vector3& extrusion)
{
	_extrusion.push_back(extrusion);
}


void CharacterBase::cleanup()
{
	SAFE_DELETE_CLEANUP(_weapon);
	SAFE_DELETE(_armBox);
}