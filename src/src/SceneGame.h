#pragma once

class SceneGame : public SceneBase
{
public:
	//! デフォルトコンストラクタ
	SceneGame();
	//! デストラクタ
	~SceneGame();

	//! 初期化
	bool init() override;
	//! 更新
	void update() override;
	//! 描画
	void render() override;
	//! 解放
	void cleanup() override;

	// 一時
	int getPlayerHitPoint() { _player->getHitPoint(); }

	//bool sceneEnd() { return _sceneEnd; }

private:
	Camera *			_pCamera;
	Player*				_player;
	EnemyManager*		_enemyManager;
	MapObjectManager*	_mapObjectManager;
	WeaponManager*		_weaponManager;
	CollisionManager*	_collisionManager;
	ItemManager*		_itemManager;


	bool playerInit();
	bool enemyManagerInit();
	bool mapObjectInit();
	bool weaponManagerInit();
	bool itemManagerInit();
	bool collisionManagerInit();
	bool cameraInit();


	bool _startBlackOut;

	Texture* _warpEffect01;


	void cameraUpdata();
	
	KeyInformation* _keyInformation;

	Sound* _gameBgm;

	bool _objectiveNext;
	bool _flag;
	bool _flag2;
};