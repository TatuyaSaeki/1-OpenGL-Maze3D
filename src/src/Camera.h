#pragma once

/*
void	updateCamera();
void	updateDebugCamera();
void	updatePlayerCamera(const Vector3& position);

Matrix getCameraMatrix();

Vector3 getMatCameraWorldAxisX();
Vector3 getMatCameraWorldAxisZ();

float getXdiff();
float getYdiff();
*/




class Camera
{
public:
	//! @brief  �R���X�g���N�^
	Camera(void);

	//! @brief  �f�X�g���N�^
	virtual ~Camera(void);

	//! @brief  ������
	virtual bool init(void);

	//! @brief  �X�V
	virtual void update(void);
	//void updateDebugCamera();

	//! @brief  �`��
	virtual void render(void);

	//! @brief  ���
	virtual void cleanup(void);

	Matrix getMatWorld(void) const { return _matWorld; }
	Matrix getMatView(void) const { return _matView; }
	Matrix getMatProj(void) const { return _matProj; }

	void setPosition(const Vector3& position) { _position = position; }
	Vector3 getPosition(void) { return _position; }

	void setLookAt(const Vector3& lookAt) { _lookAt = lookAt; }
	Vector3 getLookAt(void) { return _lookAt; }

	void setWorldUp(const Vector3& worldUp) { _worldUp = worldUp; }
	Vector3 getWorldUp(void) { return _worldUp; }

	void setRotation(const Vector3& rotation) { _rotation = rotation; }
	Vector3 getRotation(void) { return _rotation; }

//	static Matrix getTmpMatWorld()
//	{
//		return _tmpWorldMatrix;
//	}

	static void setActiveCamera(Camera* camera)
	{
		_activeCamera = camera;
	}

	static Camera* getActiveCamera()
	{
		return _activeCamera;
	}

	float getRotationY()
	{
		Vector3 result = (_lookAt - _position).normalize();

		return atan2(result._x, result._z);
	}
protected:
	Matrix _matWorld;
	Matrix _matView;
	Matrix _matProj;

	Vector3 _position;
	Vector3 _rotation;
	Vector3 _lookAt;
	Vector3 _worldUp;

	f32		_aspectRatio;
	f32		_fov;
	f32		_nearZ;
	f32		_farZ;

	static Camera*	_activeCamera;
//	static Matrix _tmpWorldMatrix;
//	static Matrix _tmpViewMatrix;
};


class CameraFPS : public Camera
{
public:
	//! @brief  �R���X�g���N�^
	CameraFPS(void);

	//! @brief  �f�X�g���N�^
	virtual ~CameraFPS(void);

	//! @brief  ������
	virtual bool init(void) override;

	//! @brief  �X�V
	virtual void update(void) override;

	//! @brief  �`��
	virtual void render(void) override;

	//! @brief  ���
	virtual void cleanup(void) override;

protected:
	Vector2 _oldPoint;
};