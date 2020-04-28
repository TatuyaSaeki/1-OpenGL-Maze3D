#include "stdafx.h"

//======================================================================================
//! 初期化
//======================================================================================
bool SceneTitle::init()
{
	// 一時
	_isBlackOut = false;


	_camera = new Camera();
	if (_camera == nullptr) return false;
	_camera->init();
	_camera->setPosition(Vector3(0.0f, 300.0f, 0.0f));
	_camera->setLookAt(Vector3(0.0f, 50.0f, 0.0f));
	_camera->setWorldUp(Vector3::AXIS_Y);

	_mapObjectManager = new MapObjectManager();
	if (_mapObjectManager->init() == false) {
		MessageBox(nullptr, "cart.mqo ファイルの読み込みに失敗しました.", "エラー", MB_OK);
		return false;
	}

	_mapObjectManager->addMapModel("data/Model/map/gameMap1.mqo", 0.5f);
	//_mapObjectManager->addMapModel("data/Model/map/gameMap2.mqo", 0.5f);

	if (!SceneBase::blackoutInit("data/Texture/Blackout/ダウンロード.png", -100)) return false;

	_titleBgm = new Sound();
	if (_titleBgm == nullptr) return false;
	//_titleBgm->loadFile(L".\\data\\Sound\\titleBgm.mp3");
	_titleBgm->loadFile(L".\\data\\Sound\\titleBGM.mp3");
	_titleBgm->volumeDown(3000);
	//_titleBgm->volumeUp(100);

	_blackoutTexture->setAlpha(255);


	_itemManager = new ItemManager();
	if (_itemManager == nullptr)	return false;
	if (!_itemManager->init())		return false;

	//--------------------------------------------------------
	{
		Vector3 keyItemPosition[] =
		{
			{ -557.6f, 115.0f,  425.8f },
		{ -571.0f, 115.0f,  -356.2f },
		{ 515.0f, 115.0f,  -360.4f },
		{ 525.1f, 115.0f,  425.4f },
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

	return true;
}

//======================================================================================
//! 更新
//======================================================================================
void SceneTitle::update()
{
	_titleBgm->playRoop();

	if (_isBlackOut) {
		_titleBgm->volumeDown(10);
		_blackoutTexture->addAlpha(3);

		if (_blackoutTexture->getColor()._a >= 255) {
			scene().jump(SCENE_TYPE::GAME);
		}
	}
	else {
		_blackoutTexture->addAlpha(-3);

		if (_blackoutTexture->getColor()._a <= 0) {
			_blackoutTexture->setAlpha(0);
		}
	}

	SceneBase::menuSelection(_menuSelectionPosition[0][0], _menuSelectionPosition[0][1], MENU_SELECTION::GAME_START);
	SceneBase::menuSelection(_menuSelectionPosition[3][0], _menuSelectionPosition[3][1], MENU_SELECTION::QUIT_GAME);



	//---------------------------------------------
	// よくわかんない
	// わかれ
	//---------------------------------------------
	// 3Dに切り替え
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// リセット

									// 視野角画角, アスペクト比, near, far
	gluPerspective(45.0f, 16.0f / 9.0f, 0.01f, 10000.0f);

	Vector3 cameraPosition = _camera->getPosition();
	Vector3 cameraRotation = _camera->getRotation();
	Vector3 cameraLookAt = _camera->getLookAt();
	Matrix  matWorld = _camera->getMatWorld();
	Vector3	forward = matWorld.axisZ() * -1.0f;

	// 角度
	cameraRotation._y += DEG_TO_RAD(0.15f);// * (PI * 2.0f);

	if (cameraRotation._y > PI * 2.0f) { cameraRotation._y = 0.0f; }

	// ポジション移動
	cameraPosition._x = cameraLookAt._x + sinf(cameraRotation._y) * 700.0f;
	cameraPosition._z = cameraLookAt._z + cosf(cameraRotation._y) * 700.0f;

	//cameraRotation._y = cameraRotation._y / 360;


	//	Vector3 cameraLookAt = Vector3::ZERO;

	_camera->setPosition(cameraPosition);
	_camera->setRotation(cameraRotation);
	//	_camera->setLookAt(cameraLookAt);
	_camera->update();

	Matrix matView = _camera->getMatView();
	glMultMatrixf((GLfloat*)&matView);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


}

//======================================================================================
//! 描画
//======================================================================================
void SceneTitle::render()
{
	RenderState::setDelthTest(true);

	_mapObjectManager->render();
	_itemManager->render();

	glLoadIdentity();
	Render::getInstance()->begin2D(SCREEN_WIDTH, SCREEN_HEIGHT);

	RenderState::setBlendMode(true);

	SceneTitle::menuRender();


	int a = _blackoutTexture->getColor()._a;
	Render::getInstance()->drawRect2D(Vector3::ZERO, Vector2(SCREEN_WIDTH, SCREEN_HEIGHT), Color(0, 0, 0, a));

	RenderState::setBlendMode(false);
	RenderState::setDelthTest(false);
}

//======================================================================================
//! 解放
//======================================================================================
void SceneTitle::cleanup()
{
	SAFE_DELETE_CLEANUP(_itemManager);
	SAFE_DELETE_CLEANUP(_titleBgm);
	SAFE_DELETE_CLEANUP(_mapObjectManager);
	SAFE_DELETE_CLEANUP(_camera);
	SceneBase::cleanup();
}

void SceneTitle::menuRender()
{
	//-------------------------------------------------------------
	// 2D描画
	//-------------------------------------------------------------
	//Vector2 position = Vector2((x0+x1)/2, (y0+y1)/2);
	//	drawParticle2D(position, _menuSelection[0], _menuSelection[0]->getColor());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// GameStart
	SceneBase::menuRender(MENU_SELECTION::GAME_START, Color(255, 255, 255, 255));
	// QuitGame
	SceneBase::menuRender(MENU_SELECTION::QUIT_GAME, Color(255, 255, 255, 255));
}
