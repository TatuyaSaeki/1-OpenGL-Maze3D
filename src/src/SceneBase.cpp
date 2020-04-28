#include "stdafx.h"

SceneBase::SceneBase()
:_sceneEnd(false)
,_blackoutTexture(nullptr)
,_isBlackOut(false)
,_alpha(0)
{
#if 1
	for (int i = 0; i < 4; i++) {
		char imagename[64];
		sprintf_s(imagename, "data/Texture/Objective/objective%02d.png", i + 1);

		_objectives[i] = new Texture();
		_objectives[i]->load(imagename);
		_objectives[i]->setColor(Color(255, 255, 255, 255));
	}



	// 選択判定位置
	// start
	_menuSelectionPosition[0][0] = Vector2(30.0f , 530.0f);
	_menuSelectionPosition[0][1] = Vector2(280.0f, 610.0f);

	// continue
	_menuSelectionPosition[1][0] = Vector2(30.0f,  530.0f);
	_menuSelectionPosition[1][1] = Vector2(280.0f, 610.0f);

	// backTitle
	_menuSelectionPosition[2][0] = Vector2(30.0f,  660.0f);
	_menuSelectionPosition[2][1] = Vector2(280.0f, 740.0f);

	// exit
	_menuSelectionPosition[3][0] = Vector2(40.0f,  810.0f);
	_menuSelectionPosition[3][1] = Vector2(150.0f, 880.0f);


	// テクスチャ位置
	// start
	_menuRenderPosition[0][0] = Vector2(40.0f, 530.0f);
	_menuRenderPosition[0][1] = Vector2(15 + 290, 610.0f);

	// continue
	_menuRenderPosition[1][0] = Vector2(40.0f,    530.0f);
	_menuRenderPosition[1][1] = Vector2(50 + 290, 610.0f);

	// BackTitle
	_menuRenderPosition[2][0] = Vector2(40.0f,    660.0f);
	_menuRenderPosition[2][1] = Vector2(15 + 290, 740.0f);

	// exit
	_menuRenderPosition[3][0] = Vector2(40.0f,    800.0f);
	_menuRenderPosition[3][1] = Vector2(50 + 290, 880.0f);


	_menuTextureTv[0][0] = Vector2(0.0f, 0.0f);
	_menuTextureTv[0][1] = Vector2(1.0f, 0.2f);

	_menuTextureTv[1][0] = Vector2(0.0f, 0.6f);
	_menuTextureTv[1][1] = Vector2(1.0f, 0.8f);

	_menuTextureTv[2][0] = Vector2(0.0f, 0.4f);
	_menuTextureTv[2][1] = Vector2(1.0f, 0.6f);

	_menuTextureTv[3][0] = Vector2(0.0f, 0.2f);
	_menuTextureTv[3][1] = Vector2(1.0f, 0.4f);

#endif

	_menuSelection0 = new Texture();
	_menuSelection0->load("data/Texture/MenuSelection/MenuSelections0.png");

	_tabMenu = new Texture();
	_tabMenu->load("data/Texture/MenuTab.png");
}

bool SceneBase::init()
{

	return true;
}

void SceneBase::render()
{
	auto instance = Render::getInstance();

	instance->drawScreenTexture(*_tabMenu, _tabMenu->getColor(), Vector2(10, 660.0f), Vector2(130, 750));

	switch (_objectiveNum) {
	case OBJECTIVE_NUMBER::WEAPON_PICKUP:
		instance->drawScreenTexture(*_objectives[0], _objectives[0]->getColor(), Vector2(950.0f, 10.0f), Vector2(1100.0f, 50.0f));
		break;
	case OBJECTIVE_NUMBER::FOUR_OF_GIMMICKS:
		instance->drawScreenTexture(*_objectives[1], _objectives[1]->getColor(), Vector2(800.0f, 5.0f), Vector2(1150.0f, 45.0f));
		break;
	case OBJECTIVE_NUMBER::CENTER_GIMMICK:
		instance->drawScreenTexture(*_objectives[2], _objectives[2]->getColor(), Vector2(800.0f, 10.0f), Vector2(1200.0f, 45.0f));
		break;
	case OBJECTIVE_NUMBER::DEFEAT_BOSS:
		instance->drawScreenTexture(*_objectives[3], _objectives[3]->getColor(),  Vector2(945.0f, 10.0f), Vector2(1100.0f, 50.0f));
		break;

	default:
		break;
	}
}


void SceneBase::cleanup()
{
	for (int i = 0; i < 4; i++) {
		SAFE_DELETE(_objectives[i]);
	}

	SAFE_DELETE(_blackoutTexture);
	SAFE_DELETE(_menuSelection0);
	SAFE_DELETE(_tabMenu);
}

void SceneBase::menuSelection(
	const Vector2& position0,
	const Vector2& position1,
	const MENU_SELECTION& menu)
{
	POINT	cursor;
	GetCursorPos(&cursor);

	if (cursor.x > position0._x && cursor.x < position1._x) {
		if (cursor.y > position0._y && cursor.y < position1._y) {

			if (GetKeyState(VK_LBUTTON) & 0x8000) {
				switch (menu)
				{
				case MENU_SELECTION::GAME_START:
					_isBlackOut = true;
					break;
				case MENU_SELECTION::BACK_TO_TITLE:
					_isBlackOut = true;
					break;
				case MENU_SELECTION::CONTINUE:
					scene().jump(SCENE_TYPE::GAME);
					break;
				case MENU_SELECTION::QUIT_GAME:
					scene().quitGame();
					break;
				default:
					break;
				}
			}
		}
	}
}

	
void SceneBase::menuRender(const MENU_SELECTION& menu, const Color& color)
{
	//-------------------------------------------------------------
	// 2D描画
	//-------------------------------------------------------------
	RenderState::setDelthTest(true);
	glLoadIdentity();
	auto instance = Render::getInstance();
	instance->begin2D(SCREEN_WIDTH, SCREEN_HEIGHT);

	RenderState::setBlendMode(true);

	switch (menu)
	{
	case MENU_SELECTION::GAME_START:
		instance->drawScreenTextureUV(
			*_menuSelection0,
			color,
			_menuRenderPosition[0][0],
			_menuRenderPosition[0][1],
			_menuTextureTv[0][0],
			_menuTextureTv[0][1]
		);

		//	drawRect(Color(255,0,0), _menuSelectionPosition[0][0], _menuSelectionPosition[0][1]);
		break;
	case MENU_SELECTION::QUIT_GAME:
		instance->drawScreenTextureUV(
			*_menuSelection0,
			color,
			_menuRenderPosition[3][0],
			_menuRenderPosition[3][1],
			_menuTextureTv[3][0],
			_menuTextureTv[3][1]
		);
		//drawRect(Color(255, 0, 0), _menuSelectionPosition[3][0], _menuSelectionPosition[3][1]);
		break;
	case MENU_SELECTION::BACK_TO_TITLE:
		instance->drawScreenTextureUV(
			*_menuSelection0,
			color,
			_menuRenderPosition[2][0],
			_menuRenderPosition[2][1],
			_menuTextureTv[2][0],
			_menuTextureTv[2][1]
		);
		//	drawRect(Color(255, 0, 0), _menuSelectionPosition[2][0], _menuSelectionPosition[2][1]);
		break;
	case MENU_SELECTION::CONTINUE:
		instance->drawScreenTextureUV(
			*_menuSelection0,
			color,
			_menuRenderPosition[1][0],
			_menuRenderPosition[1][1],
			_menuTextureTv[1][0],
			_menuTextureTv[1][1]
		);
		//drawRect(Color(255, 0, 0), _menuSelectionPosition[1][0], _menuSelectionPosition[1][1]);
		break;
	default:
		break;
	}

	RenderState::setBlendMode(false);
	RenderState::setDelthTest(false);
}

bool SceneBase::blackoutInit(char* const fileName, int alpha)
{
	if (_blackoutTexture == nullptr) {
		_blackoutTexture = new Texture();
		if (_blackoutTexture == nullptr)	   return false;
		if (!_blackoutTexture->load(fileName)) return false;
		_isBlackOut = false;
		_blackoutTexture->setAlpha(alpha);
	}

	return true;
}

void SceneBase::setBlackOutAlpha(int alpha)
{
	_alpha = alpha;
}