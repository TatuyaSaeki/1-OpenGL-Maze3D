#pragma once

class SceneGame : public SceneBase
{
public:
	//! �f�t�H���g�R���X�g���N�^
	SceneGame();
	//! �f�X�g���N�^
	~SceneGame();

	//! ������
	bool init() override;
	//! �X�V
	void update() override;
	//! �`��
	void render() override;
	//! ���
	void cleanup() override;

	// �ꎞ
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