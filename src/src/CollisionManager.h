#pragma once

class Player;
class EnemyManager;
class WallManager;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	bool init( Player* pPlayer						= nullptr, 
			   EnemyManager* pEnemyManager			= nullptr,
			   MapObjectManager* pMapObjectManager	= nullptr,
			   WeaponManager* pWorldWeaponManager	= nullptr,
			   ItemManager* pItemManager			= nullptr);

	void update();
	void cleanup();


	void	debugDisp();
	void	debugKoutenCircle();



private:
	Player*			_pPlayer;
	EnemyManager*	_pEnemyManager;
	MapObjectManager*	_pMapObjectManager;
	WeaponManager*	_pWorldWeaponManager;
	ItemManager*	_pItemManager;

	Vector3 _debugKouten;
	bool	_debugFlag;

	//!< �v���C���[�ƃm�[�}���G�l�~�[
	void playerCollisionNomalEnemy(const Capsule& capsule);

	//!< �m�[�}���G�l�~�[�ƃm�[�}���G�l�~�[
	void nomalEnemyCollisionNomalEnemy();

	//!< �v���C���[�Ɨ����Ă��镐��
	void playerCollisionWalrdWeapon(const Capsule& capsule);

	//!< �v���C���[�̕���ƃm�[�}���G�l�~�[
	void playerWeaponCollisionNomalEnemy();

	//!< �m�[�}���G�l�~�[�̕���ƃv���C���[
	void nomalEnemyWeaponCollisionPlayer(const Capsule& capsule);

	//!< �{�X�G�l�~�[�̍U���ƃv���C���[
	void bossEnemyAttackCollisionPlayer(const Capsule& capsule);

	//!< �v���C���[�ƕ�
	void playerCollisionWall();

	//!< �G�l�~�[�ƕ�
	void enemyCollisionWall();

	//! �v���C���[�ƕ�
	void playerCollisionTower(const Capsule& capsule);

	//!< �v���C���[�ƃL�[�A�C�e��
	void playerCollisionKeyAitem(const Capsule& capsule);

	//!< �v���C���[�Ə�
	void playerCollisionFloor(const Capsule& capsule);

	void playerCollisionDumyWall2(const Capsule& capsule);

	

	Sound* _damageSound;
	

	std::vector<Triangle*> _hitTriangle;
};

void DebugStringValue(const std::vector<std::string> text, const Vector3& num, const Vector3& pos, const Vector3& space);
void DebugString(const std::vector<std::string> text, const Vector3& pos, const Vector3& space);

void render_string(float x, float y, std::string const& str);