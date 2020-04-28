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

	//!< プレイヤーとノーマルエネミー
	void playerCollisionNomalEnemy(const Capsule& capsule);

	//!< ノーマルエネミーとノーマルエネミー
	void nomalEnemyCollisionNomalEnemy();

	//!< プレイヤーと落ちている武器
	void playerCollisionWalrdWeapon(const Capsule& capsule);

	//!< プレイヤーの武器とノーマルエネミー
	void playerWeaponCollisionNomalEnemy();

	//!< ノーマルエネミーの武器とプレイヤー
	void nomalEnemyWeaponCollisionPlayer(const Capsule& capsule);

	//!< ボスエネミーの攻撃とプレイヤー
	void bossEnemyAttackCollisionPlayer(const Capsule& capsule);

	//!< プレイヤーと壁
	void playerCollisionWall();

	//!< エネミーと壁
	void enemyCollisionWall();

	//! プレイヤーと壁
	void playerCollisionTower(const Capsule& capsule);

	//!< プレイヤーとキーアイテム
	void playerCollisionKeyAitem(const Capsule& capsule);

	//!< プレイヤーと床
	void playerCollisionFloor(const Capsule& capsule);

	void playerCollisionDumyWall2(const Capsule& capsule);

	

	Sound* _damageSound;
	

	std::vector<Triangle*> _hitTriangle;
};

void DebugStringValue(const std::vector<std::string> text, const Vector3& num, const Vector3& pos, const Vector3& space);
void DebugString(const std::vector<std::string> text, const Vector3& pos, const Vector3& space);

void render_string(float x, float y, std::string const& str);