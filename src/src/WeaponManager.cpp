#include "stdafx.h"

WeaponManager::WeaponManager()
{

}

WeaponManager::~WeaponManager()
{
}

bool WeaponManager::init()
{
	return true;
}

void WeaponManager::update()
{
	auto itr = _worldWeapons.begin();
	while( itr != _worldWeapons.end() ){
		if( (*itr)->getHaveWeapon() ){
			(*itr)->cleanup();
			delete (*itr);
			itr = _worldWeapons.erase( itr );
		}else{
			Matrix matrix = Matrix::rotateX( DEG_TO_RAD( (*itr)->getRotation()._x ) )
						  * Matrix::rotateY( DEG_TO_RAD( (*itr)->getRotation()._y ) )
						  * Matrix::rotateZ( DEG_TO_RAD( (*itr)->getRotation()._z ) )
						  * Matrix::translate( (*itr)->getPosition() );

			(*itr)->update( matrix );
			++itr;
		}
	}
}

void WeaponManager::render()
{
	for ( auto weapon : _worldWeapons ) {
		weapon->render();
	}
}

void WeaponManager::cleanup()
{
	for ( auto& weapon : _worldWeapons ) {
		SAFE_DELETE_CLEANUP(weapon);
	}
	_worldWeapons.clear();
}

//======================================================================================
//! ïêäÌí«â¡
//======================================================================================
void WeaponManager::addWeapon( WeaponBase* weapon )
{
	_worldWeapons.push_back( weapon );
}