#pragma once

#include "Shape.h"

class Sword;

class CharacterBase : public ObjectBase
{
protected:


public:

	CharacterBase();
	virtual ~CharacterBase();


	virtual bool init(float worldGravity)		{ return true; }
	virtual void update()	{}
	virtual void render()	{}
	virtual void cleanup();

	virtual void addPosition(const Vector3& position)
	{
		_position += position;
	}



	Vector3 getOldPosition() { return _oldPosition; }

	void setVelocity(float velocity) { _velocity = velocity; }
	void setHitObjectType(ObjectType ot);

	void addExtrusion(const Vector3& extrusion);

	//===========================================================================
	// �Q�b�^�[
	//===========================================================================
	bool isDead() { return _isDead; }

	//===========================================================================
	// �Z�b�^�[
	//===========================================================================
	void setHit(bool flag)	 { _isHit = flag; }

	void addHitObjectType( ObjectType objectType );

	//! �r�X�V
	virtual void armUpdate();
	//! �r�`��
	virtual void armRender(const Matrix* parentMatrix, Vector3& armRote, const Vector3& armPos);

	Matrix getArmMatrix() { return _armMatrix; }
	Vector3 getArmPosition() { return _armPosition; }
	Sword* getWeaponInfo()
	{
		return _weapon;
	}


	std::vector<ObjectType>& getHitObject()
	{
		return _hitObjectType;
	}

protected:
	Sword*  _weapon;

	bool	_isHit;
	bool	_isDead;

	float	_gravity;
	float	_velocity;

	int  _hitPoint;
	int  _invincibleFlameCount; // ���G����
	bool _isInvincible; // ���G���



	// �r
	Box*		_armBox;
	Matrix		_armMatrix;
	Vector3		_armPosition;
	Vector3		_armRotation;




	Vector3 _oldPosition;

	// �����o����
	std::vector<Vector3> _extrusion;
};
