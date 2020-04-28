#include "stdafx.h"
#include <time.h>

//======================================================================================
//! �f�t�H���g�R���X�g���N�^
//======================================================================================
SceneBossBattle::SceneBossBattle(int playerHitPoint, int bossHitPoint)
	: _pCamera(nullptr)
	, _player(nullptr)
	, _enemyManager(nullptr)
	, _mapObjectManager(nullptr)
	, _weaponManager(nullptr)
	, _collisionManager(nullptr)
	, _itemManager(nullptr)
	, _keyInformation(nullptr)
{
	_sceneEnd = false;

	_playerHitPoint = playerHitPoint;
	_bossHitPoint = bossHitPoint;

	if(_playerHitPoint > 50) {
		_playerHitPoint = 50;
	}

	if (_bossHitPoint <= 20) {
		_bossHitPoint = 20;
	}

	_bgm = new Sound();
	_bgm->loadFile(L".\\data\\Sound\\n137.mp3");
	_bgm->volumeDown(6000);
	_bossDead = false;
}

//======================================================================================
//! �f�X�g���N�^
//======================================================================================
SceneBossBattle::~SceneBossBattle()
{
	
}

//======================================================================================
//! ������
//======================================================================================
bool SceneBossBattle::init()
{
	srand((unsigned)time(NULL));
	
	cameraInit();
	playerInit();
	enemyManagerInit();
	mapObjectInit();
	weaponManagerInit();
	itemManagerInit();
	collisionManagerInit();


	_blackoutTexture = new Texture();
	if (_blackoutTexture == nullptr) return false;
	if (_blackoutTexture->load("data/Texture/Blackout/�_�E�����[�h.png") == false) {
		MessageBox(nullptr, "�_�E�����[�h.png �t�@�C���̓ǂݍ��݂Ɏ��s���܂���.", "�G���[", MB_OK);
		return false;
	}
	_blackoutTexture->setColor(Color(0,0,0,255));

	blackoutInit("data/Texture/Blackout/�_�E�����[�h.png", 0);
	_blackoutTimer = -200;

	_alpha = -100;

	_objectiveNum = OBJECTIVE_NUMBER::DEFEAT_BOSS;

	_keyInformation = new KeyInformation();
	_keyInformation->setPlaeyr(*_player);
	_keyInformation->setMapObject(*_mapObjectManager);

	return true;
}

//======================================================================================
//! �X�V
//======================================================================================
void SceneBossBattle::update()
{
	//---------------------------------------------
	// �悭�킩��Ȃ�
	// �킩��
	//---------------------------------------------
	// 3D�ɐ؂�ւ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// ���Z�b�g

									// ����p��p, �A�X�y�N�g��, near, far
	gluPerspective(45.0f, 16.0f / 9.0f, 0.01f, 10000.0f);

#if 0
	// ���_�ݒ�
	gluLookAt(2.0f, 10.0f, 10.0f,		// Position ���_�ʒu
		0.0f, 0.0f, 0.0f,			// Look at  �����_
		0.0f, 1.0f, 0.0f);		// World up ���E�̏�����̃x�N�g��
#else
	cameraUpdata();
#endif

	if (!_startBlackOut) {
		_bgm->volumeUp(1);
		if (!_isBlackOut) {
			_blackoutTexture->setAlpha(255);
			_isBlackOut = true;
		}
		_blackoutTexture->addAlpha(-2);
		if (_blackoutTexture->getColor()._a <= 0) {
			_blackoutTexture->setAlpha(255);
			_startBlackOut = true;
			_isBlackOut = false;
		}
		//return;
	}

	if (_player->getState() == PlayerState::Dead) {
		_bgm->volumeDown(20);
		if (!_isBlackOut) {
			_blackoutTexture->setAlpha(0);
			_isBlackOut = true;
		}

		_blackoutTexture->addAlpha(3);

		if (_blackoutTexture->getColor()._a >= 255) {
			scene().jump(SCENE_TYPE::GAME_OVER_BOSS_BATTLE);
		}
	}


	if (_itemManager->getBossKeyItem() != nullptr) {
		if (_itemManager->getBossKeyItemIsActive()) {
			if (!_isBlackOut) {
				_blackoutTexture->setAlpha(0);
				_isBlackOut = true;
			}

			_blackoutTexture->addAlpha(3);

			if (_blackoutTexture->getColor()._a >= 255) {
				scene().jump(SCENE_TYPE::GAME_CLEAR);
			}
		}
	}

	if (!_bossDead) {
		if (_enemyManager->getDeadState() == EnemyState::Dead) {
			_bossDead = true;
		}
	}
	else {
		_bgm->volumeDown(10);
	}


	_bgm->playRoop();
	
	//-------------------------------------------
	// ���f��
	//-------------------------------------------
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	if (_startBlackOut) {
		_player->update();
	}

	_enemyManager->update(_itemManager);

	_itemManager->update();

	// ����
	_weaponManager->update();

	_mapObjectManager->update();

	if (!_isBlackOut) {
		_collisionManager->update();
		_keyInformation->update();
		_player->lateUpdate();
	}

	_enemyManager->lateUpdate();



	if (_objectiveNum != OBJECTIVE_NUMBER::CENTER_GIMMICK) {
		if (_enemyManager->bossEnemyIsDead()) {
			_objectiveNum = OBJECTIVE_NUMBER::CENTER_GIMMICK;
		}
	}
}

//======================================================================================
//! �`��
//======================================================================================
void SceneBossBattle::render()
{
#if 0
	if (_itemManager->getLastKeyItemIsActive()) {
		static int alpha = -100;
		blackout(_blackoutTexture, alpha);
		alpha++;
		if (alpha >= 255) {
			alpha = 255;
		}
	}
#endif
	RenderState::setDelthTest(true);

	// ��_mapObjectManager
	_mapObjectManager->render();

	_itemManager->render();

	// ����
	_weaponManager->render();

	// �G
	_enemyManager->render();

	// �v���C���[
	_player->render();


	RenderState::setDelthTest(false);
	glLoadIdentity();
	Render::getInstance()->begin2D(1280, 720);
	//RenderState::setDelthTest(false);

	_player->render2D();

	SceneBase::render();
	_keyInformation->render();
	//_collisionManager->debugDisp();
	if (_isBlackOut) {
		Color color = _blackoutTexture->getColor();
		Render::getInstance()->drawRect2D(Vector3(0, 0, 0), Vector2(1280, 720), color);
	}

	


	//_blackoutTexture->setAlpha(100);
	//drawScreenTexture(_blackoutTexture);
}

//======================================================================================
//! ���
//======================================================================================
void SceneBossBattle::cleanup()
{
	SAFE_DELETE_CLEANUP(_bgm);
	SAFE_DELETE_CLEANUP(_keyInformation);
	SAFE_DELETE_CLEANUP(_itemManager);
	SAFE_DELETE_CLEANUP(_collisionManager);
	SAFE_DELETE_CLEANUP(_weaponManager);
	SAFE_DELETE_CLEANUP(_mapObjectManager);
	SAFE_DELETE_CLEANUP(_enemyManager);
	SAFE_DELETE_CLEANUP(_player);
	SAFE_DELETE_CLEANUP(_pCamera);
	SceneBase::cleanup();
}

bool SceneBossBattle::playerInit()
{
	_player = new Player();
	if (_player == nullptr)				return false;

	_player->setCamera(_pCamera);
	if (!_player->init(WORLD_GRAVITY))	return false;
	_player->setPosition(Vector3(0.0f, 0.0f, 150.0f));
	_player->firstInit();

	_player->setHitPoint(_playerHitPoint);
	if(!_player->createWeapon()) return false;
	_player->matrixUpdate();

	return true;
}

bool SceneBossBattle::enemyManagerInit()
{
	_enemyManager = new EnemyManager();
	if (_enemyManager == nullptr)	return false;
	if (!_enemyManager->init())		return false;

	// �m�[�}���G�l�~--------------------------------------------------
	{
		Vector3 nomalEnemyPosition[] =
		{
			{  150.0f,  0.0f, -10.0f },
			{ -150.0f,  0.0f, -10.0f },
			{	0.0f,  0.0f, -10.0f },
		};

		Vector3 nomalEnemyRotation[] =
		{
			{0,0,0},
		{ 0,0,0 },
		{ 0,0,0 },
		};
		const int positionsSize = sizeof(nomalEnemyPosition) / sizeof(nomalEnemyPosition[0]);

		for (int i = 0; i < positionsSize; ++i) {
			NomalEnemy* nomalEnemy = new NomalEnemy();
			if (nomalEnemy == nullptr) return false;
			nomalEnemy->init(WORLD_GRAVITY, nomalEnemyPosition[i], nomalEnemyRotation[i], 30);
			nomalEnemy->setSearchRange(70.0f);
			_enemyManager->addNomalEnemy(nomalEnemy);
		}
	}
	

	// �ڂ����˂݁[--------------------------------------------------
	{
		Vector3 bossPotiion[] =
		{
			{ 0.0f, 0.0f, -100.0f },
		};
		const int positionsSize = sizeof(bossPotiion) / sizeof(bossPotiion[0]);

		for (int i = 0; i < positionsSize; ++i) {
			BossEnemy* bossEnemy = new BossEnemy();
			if (bossEnemy == nullptr) return false;
			bossEnemy->init(WORLD_GRAVITY, bossPotiion[i], _bossHitPoint);
			_enemyManager->addBossEnemy(bossEnemy);
		}
	}

	return true;
}

bool SceneBossBattle::mapObjectInit()
{
	_mapObjectManager = new MapObjectManager();
	if (_mapObjectManager == nullptr)	return false;
	if (!_mapObjectManager->init())		return false;
	_mapObjectManager->addMapModel("data/Model/map/bossBattle.mqo", 0.5f);

	return true;
}

bool SceneBossBattle::weaponManagerInit()
{
	_weaponManager = new WeaponManager();
	if (_weaponManager == nullptr)	return false;
	if (!_weaponManager->init())	return false;

	return true;
}

bool SceneBossBattle::itemManagerInit()
{
	_itemManager = new ItemManager();
	if (_itemManager == nullptr)	return false;
	if (!_itemManager->init())		return false;

	return true;
}

bool SceneBossBattle::collisionManagerInit()
{
	_collisionManager = new CollisionManager();
	if (_collisionManager == nullptr)	return false;

	if (!_collisionManager->init(	_player,
									_enemyManager,
									_mapObjectManager,
									_weaponManager,
									_itemManager)) 
	{
		return false;
	}
	
	return true;
}

bool SceneBossBattle::cameraInit()
{
	_pCamera = new CameraFPS();
	if (_pCamera == nullptr) return false;
	_pCamera->init();
	_pCamera->setPosition(Vector3::ZERO);
	_pCamera->setLookAt(Vector3::AXIS_Z);
	_pCamera->setWorldUp(Vector3::AXIS_Y);
	Camera::setActiveCamera(_pCamera);

	return true;
}

void SceneBossBattle::cameraUpdata()
{
	{
		const auto mat = _player->getMatrix();
		Vector3 position = _player->getHeadPosition();


		Vector3 cameraPosition = position;


		Matrix  matWorld = _pCamera->getMatWorld();
		Vector3	forward = matWorld.axisZ() * -1.0f;
		Vector3	right = matWorld.axisX();

		Vector3 cameraLookAt = cameraPosition + forward;

		Vector3	viewDir = forward;

		_pCamera->setPosition(cameraPosition);
		_pCamera->setLookAt(cameraPosition + viewDir);

		_pCamera->update();


		Matrix matView = _pCamera->getMatView();
		glMultMatrixf((GLfloat*)&matView);
	}
}