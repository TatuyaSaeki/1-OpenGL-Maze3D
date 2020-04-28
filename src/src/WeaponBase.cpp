#include "stdafx.h"

WeaponBase::WeaponBase()
: _isHit( false )
, _haveWeapon( false )
, _attackPower( 0.0f )
, _pCollision(nullptr)
{
	_swordModel= new Model();
	if (_swordModel != nullptr) {
		if (_swordModel->load("data/Model/sword027/sword027.mqo", 0.17f) == false) {
			MessageBox(nullptr, "cart.mqo ファイルの読み込みに失敗しました.", "エラー", MB_OK);
			return;
		}
	}
}

WeaponBase::~WeaponBase()
{
}

void WeaponBase::update()
{

}

void WeaponBase::update( const Matrix& matParent)
{

}

void WeaponBase::render(const Matrix& matrix)
{

}

void WeaponBase::cleanup()
{
	SAFE_DELETE(_pCollision);
	SAFE_DELETE(_swordModel);
}

void WeaponBase::setHaveWeapon(bool flag)
{
	_haveWeapon = flag; 
}

bool WeaponBase::getHaveWeapon()
{
	return _haveWeapon; 
}
	 
void WeaponBase::isHit(bool flag)
{
	_isHit = flag; 
}
