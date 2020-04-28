#pragma once

class WeaponManager
{
public:
	WeaponManager();
	~WeaponManager();

	bool init();
	void update();
	void render();
	void cleanup();

	//! •Ší’Ç‰Á
	void addWeapon(WeaponBase* weapon);

	std::vector<WeaponBase*>& getWorldWeapons() { return _worldWeapons; }

private:
	std::vector<WeaponBase*> _worldWeapons;
	//std::vector<WeaponBase*> _fafWeapons;
};