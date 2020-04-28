#pragma once

class SceneBossBattle : public SceneBase
{
public:
	//! �f�t�H���g�R���X�g���N�^
	SceneBossBattle(int playerHitPoint, int bossHitPoint);
	//! �f�X�g���N�^
	~SceneBossBattle();

	//! ������
	bool init() override;
	//! �X�V
	void update() override;
	//! �`��
	void render() override;
	//! ���
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