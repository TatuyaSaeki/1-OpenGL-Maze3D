#pragma once

class WeaponBase : public ObjectBase
{
public:
	//! �R���X�g���N�^
	WeaponBase();
	//! ���@�[�`�����f�X�g���N�^
	virtual ~WeaponBase();

	//! ������
	virtual	bool init()	{ return true ;}
	//! �X�V
	virtual void update();

	virtual void update( const Matrix& matParent );

	//! �`��
	virtual void render() {};
	virtual void render(const Matrix& matrix);
	//! ���
	virtual void cleanup();

	void	setHaveWeapon(bool flag);
	bool	getHaveWeapon();

	void	isHit(bool flag);

	Capsule* getCollision() { return _pCollision; }

protected:
	bool		_isHit;
	bool		_haveWeapon;
	float		_attackPower;

	Model*		_swordModel;
	Capsule*	_pCollision;
};