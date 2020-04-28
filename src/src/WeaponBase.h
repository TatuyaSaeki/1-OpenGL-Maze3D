#pragma once

class WeaponBase : public ObjectBase
{
public:
	//! コンストラクタ
	WeaponBase();
	//! ヴァーチャルデストラクタ
	virtual ~WeaponBase();

	//! 初期化
	virtual	bool init()	{ return true ;}
	//! 更新
	virtual void update();

	virtual void update( const Matrix& matParent );

	//! 描画
	virtual void render() {};
	virtual void render(const Matrix& matrix);
	//! 解放
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