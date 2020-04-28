#include "stdafx.h"

BossKeyItem::BossKeyItem()
:_alpha(0.0f)
{
	_collisionSphere = nullptr;
	_objectType = ObjectType::KeyItem;
}

BossKeyItem::~BossKeyItem()
{
}

bool BossKeyItem::init()
{
	_position = Vector3::ZERO;
	_alpha	  = 0;
	_dumyCollision = new Sphere(Vector3::ZERO, 7.0f);
	_collisionSphere = new Sphere(Vector3::ZERO, +20.0f);
	_isActive = false;
	_hitObjectType = ObjectType::None;

	// ObjectBase
	_objectType = ObjectType::KeyItem;
	_position = Vector3::ZERO;

	_color = Color::BLACK;

	return true;
}

void BossKeyItem::update()
{
	//------	何かに当たっていたら
	// 当たっているオブジェクト通知が来ていたら
	if (_hitObjectType == ObjectType::Player && !_isActive) {
		_isActive = true;
		_hitObjectType = ObjectType::None;
	}

	if(_position._y < 8.0f) { 
		_position._y += 0.05f;
	}
	else { 
		_position._y = 8.0f;
	}

	_alpha += 0.5f;

	_matrix = Matrix::translate(_position);
}

void BossKeyItem::render()
{
	glPushMatrix(); 

	glLoadMatrixf((GLfloat*)&_matrix);

	int a = (int) _alpha;
	if(_alpha < 0) a = 0;
	if(_alpha > 255) a = 255;
	
	glColor4ubv((GLubyte*)&Color(255, 255, 0, a));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutSolidSphere(3.0f, 30, 30);
	glDisable(GL_BLEND);
	//drawSphere(_position, _collisionSphere->_radius, _color);

	glPopMatrix();
}

void BossKeyItem::cleanup()
{
	KeyItem::cleanup();
}