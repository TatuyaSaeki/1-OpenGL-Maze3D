#include "stdafx.h"

DummyKeyItem::DummyKeyItem()
{
}

DummyKeyItem::~DummyKeyItem()
{
	//SAFE_DELETE(_collisionSphere);
}


bool DummyKeyItem::init()
{
	if(KeyItem::init()) return false;

	return true;
}

void DummyKeyItem::update()
{


}

void DummyKeyItem::render()
{
	glLoadIdentity();
	_matrix = Matrix::wolrdMatrixTransform(_rotation, _position);

	//Vector3 position = _matrix.translate();

	Render::getInstance()->drawSphere(_position, _collisionSphere->_radius, _color);

	if (_isActive) {
		Render::getInstance()->drawLine(_position, Vector3(0.0f, 8.0f, 0.0f), _color);
	}
}

void DummyKeyItem::cleanup()
{
	KeyItem::cleanup();
}