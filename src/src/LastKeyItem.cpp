#include "stdafx.h"

LastKeyItem::LastKeyItem()
	:_keyItemAllActive(false)
{
}

LastKeyItem::~LastKeyItem()
{
}

bool LastKeyItem::init() 
{
	_dumyCollision = new Sphere(Vector3::ZERO, 7.0f);
	_collisionSphere = new Sphere(Vector3::ZERO, 13.0f);
	_isActive		 = false;

	// ObjectBase
	_objectType = ObjectType::KeyItem;
	_position	= Vector3(0.0f, 5.0f, 0.0f);
	_rotation	= Vector3::ZERO;

	_color				= Color(255, 0, 0,255);
	_keyItemAllActive	= false;
	_isHit = false;
	_isStartUp = false;
	_activeSound = new Sound();
	_activeSound->loadFile(L".\\data\\Sound\\se_maoudamashii_effect07.mp3");
	_activeSound->volumeDown(1500);
	_count = 0;
	_flag = false;

	return true;
}

void LastKeyItem::update() 
{
	//------	何かに当たっていたら
	// 当たっているオブジェクト通知が来ていたら
	if (_keyItemAllActive && !_flag) {
		_flag = true;
		_isHit = true;
		_isActive = true;
		_color = Color(0, 255, 0);
	}

	
	if(_isHit && _flag) {
		_count++;
		if (_count > 300) {
			_activeSound->playRoop();
			_isHit = false;
		}	
	}

	if (_hitObjectType == ObjectType::Player) {
		_isStartUp = true;
	}

	_hitObjectType = ObjectType::None;
}

void LastKeyItem::render() 
{
	glLoadIdentity();

	if (_isActive) {
		glColor4ubv((GLubyte*)&_color);
		glLoadIdentity();

		Vector3 pos = _position;
		pos._y = 122.0f;
		Render::getInstance()->drawLine(pos, _position, _color);

		_matrix = Matrix::wolrdMatrixTransform(_rotation, _position);
		glLoadMatrixf((GLfloat*)&_matrix);
		glutSolidSphere(3.0f, 20, 20);
	}

	_matrix = Matrix::wolrdMatrixTransform(_rotation, _position);

	glLoadIdentity();
	glColor4ubv((GLubyte*)&_color);
	Matrix matrix = Matrix::translate(Vector3(0.0f, 115.0f, 0.0f)) * _matrix;
	glLoadMatrixf((GLfloat*)&matrix);
	glutSolidSphere(3.0f, 20, 20);
}

void LastKeyItem::cleanup()
{
	SAFE_DELETE_CLEANUP(_activeSound);
	KeyItem::cleanup();
}