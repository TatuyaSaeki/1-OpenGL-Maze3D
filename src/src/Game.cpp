//===========================================================================
//!
//!	@file	Game.cpp
//!	@brief	ゲームメインループ
//!
//===========================================================================
#include "stdafx.h"



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
GameManager::GameManager(void)
: _isQuitGame(false)
, _isOption(false)
, _pTexture(nullptr)
, _menu(nullptr)
{
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
GameManager::~GameManager(void)
{
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool GameManager::init(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);	// 画面クリアカラーの設定
	glClearDepth(1.0f);						// Ｚバッファの初期化値
	glEnable(GL_DEPTH_TEST);				// Ｚバッファを有効にする

	
	
	
	if(!scene().init()) return false;

	_pTexture = new Texture();
	_pTexture->load("data/Texture/Blackout/ダウンロード.png");
	_pTexture->setColor(Color(0,0,0,255));
	
	// 選択判定位置
	// resume
	_menuSelectionPosition[0][0] = Vector2(40.0f, 530.0f);
	_menuSelectionPosition[0][1] = Vector2(220.0f, 610.0f);

	// backTitle
	_menuSelectionPosition[1][0] = Vector2(40.0f, 810.0f);
	_menuSelectionPosition[1][1] = Vector2(280.0f, 880.0f);


	// テクスチャ位置
	// resume
	_menuRenderPosition[0][0] = Vector2(40.0f, 530.0f);
	_menuRenderPosition[0][1] = Vector2(50 + 290, 610.0f);

	// BackTitle
	_menuRenderPosition[1][0] = Vector2(40.0f, 800.0f);
	_menuRenderPosition[1][1] = Vector2(50 + 290, 880.0f);



	_menuTextureTv[0][0] = Vector2(0.0f, 0.8f);
	_menuTextureTv[0][1] = Vector2(1.0f, 1.0f);

	_menuTextureTv[1][0] = Vector2(0.0f, 0.4f);
	_menuTextureTv[1][1] = Vector2(1.0f, 0.6f);



	_isBlackOut = false;

	_menuSelection0 = new Texture();
	_menuSelection0->load("data/Texture/MenuSelection/MenuSelections0.png");

	_menu = new Texture();
	_menu->load("data/Texture/Menu.png");

	return true;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void GameManager::update(void)
{
	// ゲーム終了
	if ( scene().isQuitGame()) {
		scene().cleanup();
		quitGame();
	}

	if(_isBlackOut) { 
		_menuSelection0->addAlpha(-2);
		if(_menuSelection0->getColor()._a <= 0) {
			_isOption = false;
			scene().jump(SCENE_TYPE::TITLE);
			_isBlackOut = false;
			_menuSelection0->setAlpha(255);
		}
	}


	// オプションメニュー	
	if (pushKeyState(VK_TAB, TAB) && scene().isActiveScene() != SCENE_TYPE::TITLE && scene().isActiveScene() != SCENE_TYPE::GAME_CLEAR && scene().isActiveScene() != SCENE_TYPE::GAME_OVER) {
		if (_isOption) {
			_isOption = false;
		}
		else {
			_isOption = true;
		}
	}

	if(_isOption) {
		GameManager::menuSelection( _menuSelectionPosition[0][0], _menuSelectionPosition[0][1],
									_menuSelectionPosition[1][0], _menuSelectionPosition[1][1]);
	}else {
		scene().update();
	}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void GameManager::render(void)
{
	//---- 画面クリア
	// 描画の開始のためにバックバッファとＺバッファを初期化します
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (_isOption) {

		RenderState::setDelthTest(true);
		//-------------------------------------------------------------
		// 2D描画
		//-------------------------------------------------------------
		glLoadIdentity();
		Render::getInstance()->begin2D(SCREEN_WIDTH, SCREEN_HEIGHT);

		RenderState::setBlendMode(true);

		Render::getInstance()->drawRect2D(Vector3::ZERO, Vector2(SCREEN_WIDTH, SCREEN_HEIGHT), Color(0, 0, 0, 255));

		menuRender(*_menu, Color(255,255,255,255), Vector2(1100,200), Vector2(1900,800));


		menuRender(*_menuSelection0, _menuSelection0->getColor(), _menuRenderPosition[0][0], _menuRenderPosition[0][1], _menuTextureTv[0][0], _menuTextureTv[0][1]);
	    menuRender(*_menuSelection0, _menuSelection0->getColor(), _menuRenderPosition[1][0], _menuRenderPosition[1][1], _menuTextureTv[1][0], _menuTextureTv[1][1]);

		// title
		//menuRender(*_menuSelection0, _menuSelection0->getColor(), _menuRenderPosition[2][0], _menuRenderPosition[2][1], Vector2(0, 0.4), Vector2(1.0, 0.6));
		//menuRender(*_menuSelection[1], _menuSelection[1]->getColor(), _menuRenderPosition[2][0], _menuRenderPosition[2][1]);

		RenderState::setBlendMode(false);
		RenderState::setDelthTest(false);
		return;
	}
	else {
		scene().render();
	}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void GameManager::cleanup(void)
{
	scene().cleanup();
	SAFE_DELETE(_pTexture);
	SAFE_DELETE(_menuSelection0);
	SAFE_DELETE(_menu);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void GameManager::quitGame(void)
{
	_isQuitGame = true;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool GameManager::isQuitGame(void) const
{
	return _isQuitGame;
}

//bool pushKeyState(int key, int arrayNum)
//{
//	
//	static int keyPushCount = 0;
//
//
//
//	if (GetKeyState(key) & 0x8000) {
//		keyPushCount++;
//	}
//	else {
//		keyPushCount = 0;
//	}
//
//	if (keyPushCount != 1) {
//		return false;
//	}
//
//	return true;
//}

//===========================================================================
//	END OF FILE
//===========================================================================

void GameManager::menuSelection(
	const Vector2& position0,
	const Vector2& position1,

	const Vector2& position2,
	const Vector2& position3
)
{
	POINT	cursor;
	GetCursorPos(&cursor);

	if (cursor.x > position0._x && cursor.x < position1._x) {
		if (cursor.y > position0._y && cursor.y < position1._y) {
			if (GetKeyState(VK_LBUTTON) & 0x8000) {
				_isOption = false;
			}
		}
	}

	if (cursor.x > position2._x&& cursor.x < position3._x) {
		if (cursor.y > position2._y && cursor.y < position3._y) {
			if (GetKeyState(VK_LBUTTON) & 0x8000) {
				//scene().jump(SCENE_TYPE::TITLE);
				_isBlackOut = true;;
			}
		}
	}
}

void GameManager::menuRender(
	const Texture& texture,
	const Color& color,
	const Vector2& position0,
	const Vector2& position1,
	const Vector2& tu0,
	const Vector2& tu1)
{
	RenderState::setDelthTest(true);
	glLoadIdentity();
	Render::getInstance()->begin2D(SCREEN_WIDTH, SCREEN_HEIGHT);

	RenderState::setBlendMode(true);

	Render::getInstance()->drawScreenTextureUV(texture, color, position0, position1, tu0, tu1);

	glLoadIdentity();
	//drawRect(Color(255, 0, 0,255), position0, position1);

	RenderState::setBlendMode(false);
	RenderState::setDelthTest(false);

}