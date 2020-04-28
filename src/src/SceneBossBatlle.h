#pragma once

class SceneBossBattle : public SceneBase
{
public:
	//! デフォルトコンストラクタ
	SceneBossBattle(int playerHitPoint, int bossHitPoint);
	//! デストラクタ
	~SceneBossBattle();

	//! 初期化
	bool init() override;
	//! 更新
	void update() override;
	//! 描画
	void render() override;
	//! 解放
	void cleanup() override;



	//bool sceneEnd() { return _sceneEnd; }
	void cameraUpdata();
private:
	Camera*				_pCamera;
	Player*				_player;
	EnemyManager*		_enemyManager;
	MapObjectManager*	_mapObjectManager;
	WeaponManager*		_weaponManager;
	CollisionManager*	_collisionManager;
	ItemManager*		_itemManager;
	Sound*				_bgm;

	int _playerHitPoint;
	int _bossHitPoint;
	int			_blackoutTimer;

	bool playerInit();
	bool enemyManagerInit();
	bool mapObjectInit();
	bool weaponManagerInit();
	bool itemManagerInit();
	bool collisionManagerInit();
	bool cameraInit();
	bool _startBlackOut = false;
	KeyInformation* _keyInformation;
	bool _bossDead;
};