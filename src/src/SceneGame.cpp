#include "stdafx.h"
#include <time.h>
#if 0
// 0.0 ~ 1.0
inline f32 frand_0_1(void)
{
	static const f32 INV_MAX = 1.0f / 32767.0f;
	return static_cast<f32>( rand() ) * INV_MAX;
}

// -1.0 ~ +1.0
inline f32 frand_1_1(void)
{
	return frand_0_1() * 2.0f - 1.0f;
}
#endif

//======================================================================================
//! デフォルトコンストラクタ
//======================================================================================
SceneGame::SceneGame()
	: _pCamera(nullptr)
	, _player(nullptr)
	, _enemyManager(nullptr)
	, _mapObjectManager(nullptr)
	, _weaponManager(nullptr)
	, _collisionManager(nullptr)
	, _itemManager(nullptr)
	, _keyInformation(nullptr)
	, _gameBgm(nullptr)
{
	//scene().sceneJump(SceneType::SceneBossBattle);
	_sceneEnd = false;
}

//======================================================================================
//! デストラクタ
//======================================================================================
SceneGame::~SceneGame()
{
}

//======================================================================================
//! 初期化
//======================================================================================
bool SceneGame::init()
{
	srand((unsigned)time(NULL));

	cameraInit();

	mapObjectInit();

	weaponManagerInit();
	itemManagerInit();

	playerInit();
	enemyManagerInit();

	collisionManagerInit();



	_startBlackOut = false;

	_blackoutTexture = new Texture();
	if (_blackoutTexture == nullptr) return false;
	_blackoutTexture->setColor(Color(0, 0, 0, 255));

	_warpEffect01 = new Texture();
	_warpEffect01->load("data/Texture/Objective/warpEffect.png");

	_objectiveNum = OBJECTIVE_NUMBER::WEAPON_PICKUP;

	_keyInformation = new KeyInformation();
	_keyInformation->setPlaeyr(*_player);
	_keyInformation->setMapObject(*_mapObjectManager);

	_gameBgm = new Sound();
	if (_gameBgm == nullptr) return false;
	_gameBgm->loadFile(L".\\data\\Sound\\z48.mp3");
	_gameBgm->volumeDown(3000);
	
	_objectiveNext = false;
	_flag = false;
	_flag2 = false;

	return true;
}

//======================================================================================
//! 更新
//======================================================================================
void SceneGame::update()
{
	//---------------------------------------------
	// よくわかんない
	// わかれ
	//---------------------------------------------
	// 3Dに切り替え
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// リセット

									// 視野角画角, アスペクト比, near, far
	gluPerspective(45.0f, 16.0f / 9.0f, 0.01f, 10000.0f);
/*
	// 視点設定
	gluLookAt(2.0f, 10.0f, 10.0f,		// Position 視点位置
		0.0f, 0.0f, 0.0f,			// Look at  注視点
		0.0f, 1.0f, 0.0f);		// World up 世界の上方向のベクトル
*/


	// 視線の上下回転
	//glRotatef(30, 1,0,0);

	cameraUpdata();


	if (GetKeyState(VK_ESCAPE) & 0x8000)
	{
		_itemManager->getLastKeyItem()->setStartUp();
	}


	if (!_startBlackOut) {
		_gameBgm->volumeUp(1);
		if (!_isBlackOut) {
			_blackoutTexture->setAlpha(255);
			_isBlackOut = true;
		}
		_blackoutTexture->addAlpha(-3);
		if (_blackoutTexture->getColor()._a <= 0) {
			_blackoutTexture->setAlpha(255);
			_startBlackOut = true;
			_isBlackOut = false;
		}
		//return;
	}


	if (_player->getState() == PlayerState::Dead) {
		_gameBgm->volumeDown(20);
		if (!_isBlackOut) {
			_blackoutTexture->setAlpha(0);
			_isBlackOut = true;
		}

		_blackoutTexture->addAlpha(2);

		if (_blackoutTexture->getColor()._a >= 255) {
			scene().jump(SCENE_TYPE::GAME_OVER);
		}
	}


	auto a = _itemManager->getLastKeyItemIsActive();
	if (_itemManager->getLastKeyItemIsStartUp()  && _startBlackOut) {
		_gameBgm->volumeDown(20);
		if (!_isBlackOut) {
			_blackoutTexture->setAlpha(0);
			_isBlackOut = true;
		}
	
		_blackoutTexture->addAlpha(2);
	
		if (_blackoutTexture->getColor()._a >= 255) {
			scene().setPlayerHitPoint(_player->getHitPoint());
			scene().jump(SCENE_TYPE::BOSS_BATTLE);
		}
	//	return;
	}






	//-------------------------------------------
	// モデル
	//-------------------------------------------
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	_gameBgm->playRoop();

	if(_startBlackOut) { 
		_player->update();
	}

	if (_player->getWeapon()) {
		_mapObjectManager->setFlag(true);
	}

	_enemyManager->update(_itemManager);

	_itemManager->update();

	// 武器
	_weaponManager->update();

	_mapObjectManager->update();

	

	if(!_isBlackOut) { 
		_collisionManager->update();

		_keyInformation->update();
		_player->lateUpdate();
	}

	

	if(!_player->getWeapon())	{
		_objectiveNum = OBJECTIVE_NUMBER::WEAPON_PICKUP;
	}
	else {
		if (!_flag ) {
			_objectives[0]->addAlpha(-2);
			if (_objectives[0]->getColor()._a <= 0) {
				_objectiveNext = true;
				_objectives[1]->setAlpha(0);
				_flag = true;
			}
		}
	}

	if(!_itemManager->getLastKeyItemIsActive()) {
		if( _objectiveNext) {
			_objectives[1]->addAlpha(2);
			_objectiveNum = OBJECTIVE_NUMBER::FOUR_OF_GIMMICKS;
		}
	}
	else {
		if (_flag && !_flag2) {
			_objectives[1]->addAlpha(-2);
			if (_objectives[1]->getColor()._a <= 0) {
				_objectives[2]->setAlpha(0);
				_objectiveNext = false;
				_flag2 = true;
			}
		}
	}

	if (_itemManager->getLastKeyItem()->isActive() && _flag2) {
		_objectives[2]->addAlpha(2);
		_objectiveNum = OBJECTIVE_NUMBER::CENTER_GIMMICK;
	}
}

//======================================================================================
//! 描画
//======================================================================================
void SceneGame::render()
{


	RenderState::setDelthTest(true);
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

	// 壁_mapObjectManager
	_mapObjectManager->render();
	//
	_itemManager->render();

	// 武器
	_weaponManager->render();

	// 敵
	_enemyManager->render();

	// プレイヤー
	_player->render();

	
	// 2DRenderStart
	glLoadIdentity();
	Render::getInstance()->begin2D(1280, 720);


	_keyInformation->render();
	_player->render2D();

	SceneBase::render();

	// 順番謎素
	if (_isBlackOut) {
		Color color = _blackoutTexture->getColor();
		Render::getInstance()->drawRect2D( Vector3(0,0, 0), Vector2(1280,720), color );
		//drawScreenTexture(*_blackoutTexture, _blackoutTexture->getColor());
	}


}


//======================================================================================
//! 解放
//======================================================================================
void SceneGame::cleanup()
{
	SAFE_DELETE_CLEANUP(_gameBgm);
	SAFE_DELETE_CLEANUP(_keyInformation);
	SAFE_DELETE(_warpEffect01);
	SAFE_DELETE_CLEANUP(_itemManager);
	SAFE_DELETE_CLEANUP(_collisionManager);
	SAFE_DELETE_CLEANUP(_weaponManager);
	SAFE_DELETE_CLEANUP(_mapObjectManager);
	SAFE_DELETE_CLEANUP(_enemyManager);
	SAFE_DELETE_CLEANUP(_player);
	SAFE_DELETE_CLEANUP(_pCamera);
	SceneBase::cleanup();
}

bool SceneGame::playerInit()
{
	_player = new Player();
	if (_player == nullptr)				return false;

	_player->setCamera(_pCamera);
	if (!_player->init(WORLD_GRAVITY))	return false;

	_player->setPosition(Vector3(22.0f, 0.0f, 22.5f));

	_player->matrixUpdate();

	return true;
}

bool SceneGame::enemyManagerInit()
{
	_enemyManager = new EnemyManager();
	if (_enemyManager == nullptr)	return false;
	if (!_enemyManager->init())		return false;

	// ノーマルエネミ--------------------------------------------------
	{
		Vector3 nomalEnemyPosition[] =
		{
			{ -564, 0, -301 },
			{ -489, 0, -373 },
			{ -264 , 0, -277 },
			{ -358, 0, 6.8f },
			{ -220, 0, 2 },
			{-590,0,350},
			{ -468,0,370 },
			{ -321,0,319 },

		{ 250,0,490 },
		{ 250,0,430 },
		{ -129,0,262 },
		{ 260,0,268 },

		{ 553, 0,-280 },
		{460, 0,-300 },
		{ 510, 0,358 },
		{ 426, 0,417 },
		{ 229, 0,3 },
		{ 100,0,-250 },
		{ -82,0,-223 },
		{ 0	,0,183 },
		};

		Vector3 nomalEnemyRotation[] =
		{
			{ 0.0f, 62, 0.0f },
			{ 0.0f, 35, 0.0f },
		{ 0.0f, -133, 0.0f },

		{ 0.0f, 95, 0.0f },
		{ 0.0f, 90, 0.0f },

		{ 0.0f, 117, 0.0f },
		{ 0.0f, 162, 0.0f },
		{ 0.0f, 90, 0.0f },
		{ 0.0f, -90, 0.0f },
		{ 0.0f, -90, 0.0f },
		{ 0.0f, -5, 0.0f },
		{ 0.0f, -80, 0.0f },
		{ 0.0f, -280, 0.0f },
		{ 0.0f, -300, 0.0f },
		{ 0.0f, -100, 0.0f },
		{ 0.0f, -140, 0.0f },
		{ 0.0f, -90, 0.0f },

		{ 0.0f, -40, 0.0f },
		{ 0.0f, 5, 0.0f },
		{ 0.0f, -180, 0.0f },
		};

		const int positionsSize = sizeof(nomalEnemyPosition) / sizeof(nomalEnemyPosition[0]);

		for (int i = 0; i < positionsSize; ++i) {
			NomalEnemy* nomalEnemy = new NomalEnemy();
			if (nomalEnemy == nullptr) return false;
			nomalEnemy->init(WORLD_GRAVITY, nomalEnemyPosition[i], nomalEnemyRotation[i]);
			_enemyManager->addNomalEnemy(nomalEnemy);
		}
	}

	return true;
}

bool SceneGame::mapObjectInit()
{
	_mapObjectManager = new MapObjectManager();
	if (_mapObjectManager == nullptr)	return false;
	if (!_mapObjectManager->init())		return false;

	_mapObjectManager->addMapModel("data/Model/map/gameMap1.mqo", 0.5f);
	_mapObjectManager->addMapModel("data/Model/map/gameMap2.mqo", 0.5f);
	//_mapObjectManager->addMapModel("data/Model/map/test.mqo", 0.5f);
	return true;
}

bool SceneGame::weaponManagerInit()
{
	_weaponManager = new WeaponManager();
	if (_weaponManager == nullptr)	return false;
	if (!_weaponManager->init())	return false;

	// ワールドに落ちている武器の初期値
	Vector3 positions[] =
	{
		{ Vector3(+20.0f, 0.0f, 0.0f) },
	};

	Vector3 rotetion[] =
	{
		{ Vector3(0.0f,0.0f,90.0f) },
	};

	const int positionsSize = sizeof(positions) / sizeof(positions[0]);

	Line line = Line(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 3.5f, 0.0f));

	for (int i = 0; i < positionsSize; i++) {
		Sword* sword = new Sword();
		sword->init(ObjectType::WardlWeapon, line);
		sword->setPosition(positions[i]);
		sword->setRotetion(rotetion[i]);
		_weaponManager->addWeapon(sword);
	}

	return true;
}

bool SceneGame::itemManagerInit()
{
	_itemManager = new ItemManager();
	if (_itemManager == nullptr)	return false;
	if (!_itemManager->init())		return false;

	//--------------------------------------------------------
	{
		Vector3 keyItemPosition[] =
		{
			{ -557.6f, 115.0f,  425.8f },
			{ -571.0f, 115.0f,  -356.2f },
			{  515.0f, 115.0f,  -360.4f },
			{  525.1f, 115.0f,  425.4f },
		};
		const int positionsArraySize = sizeof(keyItemPosition) / sizeof(keyItemPosition[0]);

		for (int i = 0; i < positionsArraySize; ++i) {
			KeyItem* keyItem = new KeyItem();
			if (keyItem == nullptr) return false;
			if (!keyItem->init())	return false;
			keyItem->setPosition(keyItemPosition[i]);
			_itemManager->addKeyItem(keyItem);
		}
	}

#if 0
	//--------------------------------------------------------
	{
		Vector3 dummyKeyItemPosition[] =
		{
			{ -320.7f,  400.0f, -204.3f },
			{ -320.2f,  400.0f,  200.0f },
			{ 311.2f,	400.0f,  204.0f },
			{ 311.5f,	400.0f, -204.5f },
		};
		const int positionsArraySize = sizeof(dummyKeyItemPosition) / sizeof(dummyKeyItemPosition[0]);

		for (int i = 0; i < positionsArraySize; ++i) {
			DummyKeyItem* keyItem = new DummyKeyItem();
			if (keyItem == nullptr) return false;
			if (!keyItem->init())	return false;
			keyItem->setPosition(dummyKeyItemPosition[i]);
			_itemManager->addDummyKeyItem(keyItem);
		}
	}
#endif

	//--------------------------------------------------------
	{
		Vector3 lastKeyItemPosition = Vector3(0.0f, 8.0f, 0.0f);
		LastKeyItem* _lastKeyItem = new LastKeyItem();
		if (_lastKeyItem == nullptr) return false;
		if (!_lastKeyItem->init())   return false;
		_lastKeyItem->setPosition(lastKeyItemPosition);
		_itemManager->addLastKeyItem(_lastKeyItem);
		// わからん　メモリエラー原因(多分)
		//SAFE_DELETE(_lastKeyItem);
	}

	{
		Vector3 dorpItemPos[] =
		{
			{230, 0, 460},
		{330,0,320},
		{96,0,115},
		{391,0,-165},
		{ 416 ,0,-485 },
		{ -409.0f, 0.0f,  243.4f },
		{ -554.0f, 0.0f,  545.4f },
		{ -380  , 0.0f,  -230.4f },
		{ -112   , 0.0f,  -290 },
		};

		const int positionsSize = sizeof(dorpItemPos) / sizeof(dorpItemPos[0]);

		for(int i = 0; i < positionsSize; i++) {
			DropItem* dropItem = new DropItem(1.0f);
			dropItem->init();
			dropItem->setPosition(dorpItemPos[i]);
			dropItem->setColor(Color(0, 255, 0));
			_itemManager->addDropItem(dropItem);
		}
	}

	return true;
}

bool SceneGame::collisionManagerInit()
{
	_collisionManager = new CollisionManager();
	if (_collisionManager == nullptr)	return false;

	if (!_collisionManager->init(_player,
		_enemyManager,
		_mapObjectManager,
		_weaponManager,
		_itemManager))
	{
		return false;
	}

	return true;
}

bool SceneGame::cameraInit()
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


void SceneGame::cameraUpdata()
{
	if(_player->getState2() == PlayerState::PickUp){
		auto a = _player->getPickUpRotation();
		glRotatef(a._x,1,0,0);
	}

	{
		const auto mat = _player->getMatrix();
		Vector3 position = _player->getHeadPosition();
		

		Vector3 cameraPosition = position;


		Matrix  matWorld = _pCamera->getMatWorld();
		Vector3	forward  = matWorld.axisZ() * -1.0f;
		Vector3	right    = matWorld.axisX();

		Vector3 cameraLookAt = cameraPosition + forward;

		Vector3	viewDir = forward;

		_pCamera->setPosition(cameraPosition);
		_pCamera->setLookAt(cameraPosition + viewDir);

		_pCamera->update();


		Matrix matView = _pCamera->getMatView();
		glMultMatrixf((GLfloat*)&matView);
	}
}