#include "stdafx.h"

DropItem::DropItem()
:_isActive(true)
,_collisionSphere(nullptr)
,_radius(0.0f)
{
}

DropItem::DropItem(float radius)
	:_isActive(true)
	, _collisionSphere(nullptr)
	, _radius(3)
{
}

DropItem::~DropItem()
{
}

bool DropItem::init()
{
	_collisionSphere = new Sphere(Vector3::ZERO, 4);

	// ObjectBase
	_objectType = ObjectType::HeelItem;
	_position	= Vector3::ZERO;
	_rotation	= Vector3::ZERO;

	_color		= Color(0, 0, 0);

	return true;
}

void DropItem::update()
{

}

void DropItem::render()
{
	glPushMatrix();

	Vector3 pos = _position;
	pos._y += 2.0f;
	_matrix = Matrix::wolrdMatrixTransform(_rotation, _position);
	glLoadMatrixf((GLfloat*)&_matrix);



	glColor3ub(_color._r, _color._g, _color._b);
	glutSolidSphere(1, 10, 10);

	glPopMatrix();
}

void DropItem::cleanup()
{
	SAFE_DELETE(_collisionSphere);
	ItemBase::cleanup();
}
