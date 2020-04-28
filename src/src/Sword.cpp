#include "stdafx.h"

//======================================================================================
//! コンストラクタ
//======================================================================================
Sword::Sword()
: _rot(0.0f)
{

}

//======================================================================================
//! デストラクタ
//======================================================================================
Sword::~Sword()
{

}


//======================================================================================
//! デストラクタ
//======================================================================================
bool Sword::init(const ObjectType objType, const Line& line)
{
	// Sword
	Vector3 size[]
	{
		Vector3(-0.05f, 0.0f, -0.05f),
		Vector3(0.05f, 3.0f,  0.05f),
	};

	// ObjectBase
	_objectType = objType;
	_position = Vector3(0.0f, 0.0f, 0.0f);
	_color = Color(0, 255, 0);
	_isHit = false;
	_haveWeapon = false;


	_pCollision = new Capsule(line._position[0], line._position[1], 5.25f);

	if (_pCollision == nullptr) return false;


	//_scale = Vector3(0.1f, 0.07f, 0.1f);
	_scale = Vector3(0.1f, 0.07f, 0.1f);

	// WeaponBase

	_isHit = false;
	_attackPower = 1.0f;

	return true;
}

//======================================================================================
//! 更新
//======================================================================================
void Sword::update( const Matrix& matParent )
{
	_matrix		= Matrix::scale(_scale) * matParent;

	if( !_haveWeapon ) return;
}

//======================================================================================
//! 描画
//======================================================================================
void Sword::render()
{
	glPushMatrix();

	glLoadMatrixf((GLfloat*)&_matrix);

	_swordModel->render();

	glPopMatrix();

#if 0
	glLoadMatrixf((GLfloat*)&_matrix);
	Render::getInstance()->drawCapsule(_pCollision, Color(255,255,0));
	Render::getInstance()->drawSphere( Vector3::ZERO, _pCollision->_radius + 0.25f, Color(255,0,0) );

	Vector3 cross[3];
	// X 
	Vector3 AB = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 AC = Vector3(0.0f, 0.0f, -1.0f);
	cross[0] = Vector3::cross(AC, AB).normalize() * 15;

	// Z
	AC = Vector3(1.0f, 0.0f, 0.0f);
	cross[1] = Vector3::cross(AC, AB).normalize() * 15;

	// Y
	AB = Vector3(0.0f, 0.0f, 1.0f);
	cross[2] = Vector3::cross(AB, AC).normalize() * 15;

	// 法線
	glPushMatrix();
	glLoadIdentity();
	Render::getInstance()->drawArrorMatrix(_matrix, 10.0f);
	glPushMatrix();
#endif
}

void Sword::render(int alpha)
{
	glLoadIdentity();
	glLoadMatrixf((GLfloat*)&_matrix);
	_swordModel->addAlpha(alpha);
	_swordModel->render();

#if 0
	glLoadIdentity();

	glLoadMatrixf((GLfloat*)&_matrix);
	drawCapsule(_pCollision, Color(255, 255, 0));
	drawSphere(Vector3::ZERO, _pCollision->_radius + 0.25f, Color(255, 0, 0));

	Vector3 cross[3];
	// X 
	Vector3 AB = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 AC = Vector3(0.0f, 0.0f, -1.0f);
	cross[0] = Vector3::cross(AC, AB).normalize() * 3;

	// Z
	AC = Vector3(1.0f, 0.0f, 0.0f);
	cross[1] = Vector3::cross(AC, AB).normalize() * 3;

	// Y
	AB = Vector3(0.0f, 0.0f, 1.0f);
	cross[2] = Vector3::cross(AB, AC).normalize() * 3;
	// 法線
	drawArrow(Vector3(0.0f, 3.0f, 0.0f), cross[0], Color(255, 0, 0));	// x
	drawArrow(Vector3(0.0f, 3.0f, 0.0f), cross[1], Color(0, 255, 0));	// y
	drawArrow(Vector3(0.0f, 3.0f, 0.0f), cross[2], Color(0, 0, 255));	// z
#endif
}
//======================================================================================
//! 解放
//======================================================================================
void Sword::cleanup()
{
	WeaponBase::cleanup();
}
