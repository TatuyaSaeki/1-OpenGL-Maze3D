#include "stdafx.h"
#include <GL/glut.h>
#include <vector>

#if 0
Vector3 cameraPosition	= Vector3(0,10,10);
Vector3 cameraLookAt	= Vector3(0,0,0);
Matrix  matCameraWorld;
Matrix  matView;


void updateCamera()
{
	Vector3 axisZ = (cameraPosition - cameraLookAt).normalize();
	Vector3 axisX = Vector3::cross(Vector3(0,1,0), axisZ).normalize();
	Vector3 axisY = Vector3::cross(axisZ, axisX).normalize();

	//----------------------------------------------------------
	// ���[���h�ɃJ������z�u
	//----------------------------------------------------------
	matCameraWorld._m[0][0] = axisX._x;
	matCameraWorld._m[0][1] = axisX._y;
	matCameraWorld._m[0][2] = axisX._z;
	matCameraWorld._m[0][3] = 0.0f;

	matCameraWorld._m[1][0] = axisY._x;
	matCameraWorld._m[1][1] = axisY._y;
	matCameraWorld._m[1][2] = axisY._z;
	matCameraWorld._m[1][3] = 0.0f;

	matCameraWorld._m[2][0] = axisZ._x;
	matCameraWorld._m[2][1] = axisZ._y;
	matCameraWorld._m[2][2] = axisZ._z;
	matCameraWorld._m[2][3] = 0.0f;

	matCameraWorld._m[3][0] = cameraPosition._x;
	matCameraWorld._m[3][1] = cameraPosition._y;
	matCameraWorld._m[3][2] = cameraPosition._z;
	matCameraWorld._m[3][3] = 1;

	//----------------------------------------------------------
	// �J��������̎��E�ɕύX
	//----------------------------------------------------------
	matView._m[0][0] = axisX._x;
	matView._m[0][1] = axisY._x;
	matView._m[0][2] = axisZ._x;
	matView._m[0][3] = 0;

	matView._m[1][0] = axisX._y;
	matView._m[1][1] = axisY._y;
	matView._m[1][2] = axisZ._y;
	matView._m[1][3] = 0;
	
	matView._m[2][0] = axisX._z;
	matView._m[2][1] = axisY._z;
	matView._m[2][2] = axisZ._z;
	matView._m[2][3] = 0;
	
	matView._m[3][0] = -Vector3::dot(axisX, cameraPosition);
	matView._m[3][1] = -Vector3::dot(axisY, cameraPosition);
	matView._m[3][2] = -Vector3::dot(axisZ, cameraPosition);
	matView._m[3][3] = 1.0f;

	glMultMatrixf((GLfloat*)&matView);
}

POINT	gPrevCursorPos;
bool	gInitialized = false;

#if 0
void	updateDebugCamera()
{
	//-------------------------------------------------------------
	// ���s�ړ�
	//-------------------------------------------------------------
	Vector3	moveTo(0.0f, 0.0f, 0.0f);

	Vector3	forward = matCameraWorld.axisZ() * -1.0f;
	Vector3	right = matCameraWorld.axisX();

	if (GetKeyState('W') & 0x8000) {
		moveTo += forward * 0.2f;
	}
	if (GetKeyState('S') & 0x8000) {
		moveTo -= forward * 0.2f;
	}

	if (GetKeyState('A') & 0x8000) {
		moveTo -= right * 0.2f;
	}
	if (GetKeyState('D') & 0x8000) {
		moveTo += right * 0.2f;
	}

	cameraPosition += moveTo;
	cameraLookAt += moveTo;

	//-------------------------------------------------------------
	// ���_��]
	//-------------------------------------------------------------
	POINT	cursor;				// ���݈ʒu
	GetCursorPos(&cursor);

	if (gInitialized == false) {
		gPrevCursorPos = cursor;	// ������s���͈ʒu�𑵂���
		gInitialized = true;
	}

	// �}�E�X�J�[�\���̈ړ������l
	//float	xdiff = (cursor.x - gPrevCursorPos.x) * -0.004f;
	//float	ydiff = (cursor.y - gPrevCursorPos.y) * -0.004f;

	// �����x�N�g���̉�]
	if (GetKeyState(VK_LBUTTON) & 0x8000) {
		Vector3	viewDir = cameraLookAt - cameraPosition;

		viewDir = viewDir.transformNormal(Matrix::rotateY(xdiff));

		// �㉺��U��F�J�����̉E�����̃x�N�g�����S�ɉ�]
		Vector3	axisX = matCameraWorld.axisX();
		viewDir = viewDir.transformNormal(Matrix::rotateAxis(axisX, ydiff));

		cameraLookAt = cameraPosition + viewDir;
	}

	gPrevCursorPos = cursor;	// ���̃t���[���̂��߂ɕۑ����Ă���
}
#endif



void	updatePlayerCamera(const Vector3& position)
{
	Vector3		cameraDir = cameraLookAt - cameraPosition;

	cameraPosition = position;
	cameraPosition._y += 2;
	//cameraPosition._z += 2;

	cameraLookAt = cameraPosition + cameraDir;
	//	cameraLookAt._y += 1;
	//	cameraLookAt._z -= 1;

	//-------------------------------------------------------------
	// ���_��]
	//-------------------------------------------------------------
	POINT	cursor;				// ���݈ʒu
	GetCursorPos(&cursor);

	if (gInitialized == false) {
		gPrevCursorPos = cursor;	// ������s���͈ʒu�𑵂���
		gInitialized = true;
	}

	// �}�E�X�J�[�\���̈ړ������l
	float	xdiff = (cursor.x - gPrevCursorPos.x) * -0.004f;
	float	ydiff = (cursor.y - gPrevCursorPos.y) * -0.004f;

	// �����x�N�g���̉�]
		Vector3	viewDir = cameraLookAt - cameraPosition;

		viewDir = viewDir.transformNormal(Matrix::rotateY(xdiff));

		// �㉺��U��F�J�����̉E�����̃x�N�g�����S�ɉ�]
		Vector3	axisX = matCameraWorld.axisX();
		viewDir = viewDir.transformNormal(Matrix::rotateAxis(axisX, ydiff));

		cameraLookAt = cameraPosition + viewDir;
	

	gPrevCursorPos = cursor;	// ���̃t���[���̂��߂ɕۑ����Ă���
}

Matrix getCameraMatrix()
{
	return matCameraWorld;
}

Vector3 getMatCameraWorldAxisX()
{
	return matCameraWorld.axisX();
}

Vector3 getMatCameraWorldAxisZ()
{
	return matCameraWorld.axisZ();
}
#endif








//Matrix Camera::_tmpWorldMatrix;
//Matrix Camera::_tmpViewMatrix;
Camera*	Camera::_activeCamera;

//---------------------------------------------------------------------------
//	@brief  �R���X�g���N�^
//---------------------------------------------------------------------------
Camera::Camera(void)
: _matWorld		(Matrix::IDENTITY)
, _matView		(Matrix::IDENTITY)
, _matProj		(Matrix::IDENTITY)
, _position		(Vector3::ZERO)
, _rotation		(Vector3::ZERO)
, _lookAt		(Vector3::AXIS_Z)
, _worldUp		(Vector3::AXIS_Y)
, _aspectRatio	(16.0f/9.0f)
, _fov			(60.0f)
, _nearZ		(0.1f)
, _farZ			(1000.0f)
{
	if (_activeCamera == nullptr) {
		_activeCamera = this;
	}
}

//---------------------------------------------------------------------------
//	@brief  �f�X�g���N�^
//---------------------------------------------------------------------------
Camera::~Camera(void)
{
	if (_activeCamera == this) {
		_activeCamera = nullptr;
	}
}

//---------------------------------------------------------------------------
//	@brief  ������
//---------------------------------------------------------------------------
bool Camera::init(void)
{
	return true;
}

//---------------------------------------------------------------------------
//	@brief  �X�V
//---------------------------------------------------------------------------
void Camera::update(void)
{
	//Vector3 axisZ = -(_position - _lookAt);
	Vector3 axisZ = (_position - _lookAt);

	Vector3 axisX = Vector3::cross(_worldUp, axisZ);
	Vector3 axisY = Vector3::cross(axisZ, axisX);

	axisX = axisX.normalize();
	axisY = axisY.normalize();
	axisZ = axisZ.normalize();

	//----------------------------------------------------------
	// ���[���h�ɃJ������z�u
	//----------------------------------------------------------
	_matWorld._m[0][0] = axisX._x;
	_matWorld._m[0][1] = axisX._y;
	_matWorld._m[0][2] = axisX._z;
	_matWorld._m[0][3] = 0.0f;

	_matWorld._m[1][0] = axisY._x;
	_matWorld._m[1][1] = axisY._y;
	_matWorld._m[1][2] = axisY._z;
	_matWorld._m[1][3] = 0.0f;

	_matWorld._m[2][0] = axisZ._x;
	_matWorld._m[2][1] = axisZ._y;
	_matWorld._m[2][2] = axisZ._z;
	_matWorld._m[2][3] = 0.0f;

	_matWorld._m[3][0] = _position._x;
	_matWorld._m[3][1] = _position._y;
	_matWorld._m[3][2] = _position._z;
	_matWorld._m[3][3] = 1.0f;

	//----------------------------------------------------------
	// �J��������̎��E�ɕύX
	//----------------------------------------------------------
	_matView._m[0][0] = axisX._x;
	_matView._m[0][1] = axisY._x;
	_matView._m[0][2] = axisZ._x;
	_matView._m[0][3] = 0.0f;

	_matView._m[1][0] = axisX._y;
	_matView._m[1][1] = axisY._y;
	_matView._m[1][2] = axisZ._y;
	_matView._m[1][3] = 0.0f;

	_matView._m[2][0] = axisX._z;
	_matView._m[2][1] = axisY._z;
	_matView._m[2][2] = axisZ._z;
	_matView._m[2][3] = 0.0f;

	_matView._m[3][0] = -Vector3::dot(axisX, _position);
	_matView._m[3][1] = -Vector3::dot(axisY, _position);
	_matView._m[3][2] = -Vector3::dot(axisZ, _position);
	_matView._m[3][3] = 1.0f;

	//----------------------------------
//	_tmpWorldMatrix = _matWorld;
//	_tmpViewMatrix  = _matView;
}

//---------------------------------------------------------------------------
//	@brief  �`��
//---------------------------------------------------------------------------
void Camera::render(void)
{
}

//---------------------------------------------------------------------------
//	@brief  ���
//---------------------------------------------------------------------------
void Camera::cleanup(void)
{
}


//---------------------------------------------------------------------------
//	@brief  �R���X�g���N�^
//---------------------------------------------------------------------------
CameraFPS::CameraFPS(void)
	: _oldPoint(0,0)
{
}

//---------------------------------------------------------------------------
//	@brief  �f�X�g���N�^
//---------------------------------------------------------------------------
CameraFPS::~CameraFPS(void)
{
}

//---------------------------------------------------------------------------
//	@brief  ������
//---------------------------------------------------------------------------
bool CameraFPS::init(void)
{
	if (!Camera::init()) {
		return false;
	}

	//SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	SetCursorPos(960, 530);

	POINT	cursor;
	GetCursorPos(&cursor);
	_oldPoint._x = static_cast<f32>(cursor.x);
	_oldPoint._y = static_cast<f32>(cursor.y);

	return true;
}

//---------------------------------------------------------------------------
//	@brief  �X�V
//---------------------------------------------------------------------------
void CameraFPS::update(void)
{
	POINT	cursor;
	GetCursorPos(&cursor);

	// �}�E�X�J�[�\���̈ړ������l
	const f32 scale = -0.001f;
	f32 xdiff = (cursor.x - _oldPoint._x) * scale;
	f32 ydiff = (cursor.y - _oldPoint._y) * scale;

	// �����x�N�g���̉�]
	Vector3	viewDir = _lookAt - _position;
	viewDir = viewDir.transformNormal(Matrix::rotateY(xdiff));

	// �㉺��U��F�J�����̉E�����̃x�N�g�����S�ɉ�]
	Vector3	axisX = _matWorld.axisX();
	//viewDir = viewDir.transformNormal(Matrix::rotateAxis(axisX, ydiff));
	Vector3 upDown = viewDir.transformNormal(Matrix::rotateAxis(axisX, ydiff));
	f32 dot = Vector3::dot(upDown, Vector3::AXIS_Y);
	if (std::abs(dot) < 0.95f) {
		viewDir = upDown;
	}

	_lookAt = _position + viewDir;

	// ���̃t���[���̂��߂ɕۑ����Ă���
	//_oldPoint._x = cursor.x;
	//_oldPoint._y = cursor.y;
	SetCursorPos((int)_oldPoint._x, (int)_oldPoint._y);

	// �s��X�V
	Camera::update();
}

//---------------------------------------------------------------------------
//	@brief  �`��
//---------------------------------------------------------------------------
void CameraFPS::render(void)
{
	Camera::render();
}

//---------------------------------------------------------------------------
//	@brief  ���
//---------------------------------------------------------------------------
void CameraFPS::cleanup(void)
{
	Camera::cleanup();
}
