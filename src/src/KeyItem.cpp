#include "stdafx.h"

KeyItem::KeyItem()
	:_isActive(false)
	,_collisionSphere(nullptr)
	,_endPosition(Vector3::ZERO)
	,_endPosition2(nullptr)
	, _sound(nullptr)
	, _dumyCollision(nullptr)
{
}

KeyItem::~KeyItem()
{
	//SAFE_DELETE(_collisionSphere);
}


bool KeyItem::init()
{
	_dumyCollision = new Sphere(Vector3::ZERO, 7.0f);
	_collisionSphere = new Sphere(Vector3::ZERO, 22.0f);
	_isActive = false;
	
	_hitObjectType   = ObjectType::None;

	// ObjectBase
	_objectType = ObjectType::KeyItem;
	_position = Vector3::ZERO;
	_rotation = Vector3::ZERO;
	_endPosition = Vector3::ZERO;
	_endPosition2 = nullptr;
	_flag = false;

	_color = Color(255,0,0, 255);
	_dumyItem = Color(255, 0, 0, 255);
	flag  = false;

	_sound = new Sound();
	_sound->loadFile(L".\\data\\Sound\\light_saber1.mp3");
	_sound->volumeUp(5000);

	return true;
}

void KeyItem::update()
{

	//------	何かに当たっていたら
	// 当たっているオブジェクト通知が来ていたら
	if(_hitObjectType == ObjectType::Player){
		_sound->play();

		_isActive = true;
	}

	_hitObjectType = ObjectType::None;
}

void KeyItem::render()
{
	glLoadIdentity();
	_matrix = Matrix::wolrdMatrixTransform(_rotation, _position);
	glLoadMatrixf((GLfloat*)&_matrix);
	//Vector3 position = _matrix.translate();

	//drawSphere(Vector3::ZERO, _dumyCollision->_radius, _color);
	//drawSphere(Vector3::ZERO, _collisionSphere->_radius, _color);
	if(!_isActive) { 
		glColor4ubv((GLubyte*)&Color(255, 0, 0, 255));
	}else {
		glColor4ubv((GLubyte*)&Color(0, 0, 255, 255));
	}
	glutSolidSphere(3.0f, 20, 20);

	glLoadIdentity();
	if (_isActive) {
		if (_endPosition._y < 250.0f) {
			_endPosition._y += 0.5f;
		}
		else {
			_endPosition._y = 250.0f;
			if (_endPosition2 == nullptr) {
				_endPosition2 = new Vector3(Vector3(0.0f, 122.0f, 0.0f) - _endPosition);
				*_endPosition2 = _endPosition2->normalize();
			}

			if(!flag) { 
				_dumyItem = Color(0,0,255,255);
				flag = true;
			}

			Vector3 len = Vector3(0.0f, 122.0f, 0.0f) - (_endPosition + *_endPosition2);
			if (len.length() > 8.5f) {
				*_endPosition2 *= 1.02f;
				Render::getInstance()->drawLine(_endPosition, _endPosition + (*_endPosition2), Color(0, 0, 255, 255));
			}
			else {
				Render::getInstance()->drawLine(_endPosition, Vector3(0.0f, 122.0f, 0.0f), Color(0,0,255,255));
			}
		}
		Render::getInstance()->drawLine(_position, _endPosition, Color(0, 0, 255, 255));
	}

	glLoadIdentity();
	glColor4ubv((GLubyte*)&_dumyItem);
	Matrix matrix = Matrix::translate(Vector3(0.0f, 135.0f, 0.0f)) * _matrix;
	glLoadMatrixf((GLfloat*)&matrix);
	glutSolidSphere(3.0f, 20, 20);
}


void KeyItem::cleanup()
{
	SAFE_DELETE(_dumyCollision);
	SAFE_DELETE_CLEANUP(_sound);
	SAFE_DELETE(_endPosition2);
	SAFE_DELETE(_collisionSphere);
}