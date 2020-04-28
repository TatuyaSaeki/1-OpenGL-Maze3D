#include "stdafx.h"
#include "Shape.h"

ObjectBase::ObjectBase()
: _objectType( ObjectType::None )
, _matrix( Matrix::IDENTITY )
, _position( Vector3::ZERO )
, _rotation( Vector3::ZERO )
, _scale( Vector3::ONE )
, _color( Color::BLACK )
{
}

Vector3 ObjectBase::getTranslate()
{
	Vector3 result;
	result._x = _matrix._m[0][3];
	result._y = _matrix._m[1][3];
	result._z = _matrix._m[2][3];

	return result;
}

Vector3 ObjectBase::getRotation()
{
	return _rotation;
}
