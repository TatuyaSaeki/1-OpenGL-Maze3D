#include "stdafx.h"

//======================================================================================
//! ‰Šú‰»
//======================================================================================
bool SceneBossBattleResult::init()
{
	_texture = new Texture();
	if (_texture == nullptr) return false;
	if (!_texture->load("data/Texture/GameOverBossBatle.png")) return false;
	_texture->setColor(Color(255, 255, 255, 0));

	if (!SceneBase::blackoutInit("data/Texture/Blackout/ƒ_ƒEƒ“ƒ[ƒh.png", 255)) return false;
	_isBlackOut2 = false;

	return true;
}

//======================================================================================
//! XV
//======================================================================================
void SceneBossBattleResult::update()
{
	if(!_isBlackOut && !_isBlackOut2){
		_texture->addAlpha(3);
		if (_texture->getColor()._a >= 255) {
			_texture->setAlpha(255);
		}
	}

	if (_isBlackOut) {
		_texture->addAlpha(-3);
		if (_texture->getColor()._a <= 0) {
			scene().jump(SCENE_TYPE::TITLE);
		}
	}

	if (_isBlackOut2) {
		_texture->addAlpha(-3);
		if (_texture->getColor()._a <= 0) {
			scene().jump(SCENE_TYPE::BOSS_BATTLE);
		}
	}

	SceneBossBattleResult::menuSelection1();
}

//======================================================================================
//! •`‰æ
//======================================================================================
void SceneBossBattleResult::render()
{
	glLoadIdentity();
	Render::getInstance()->begin2D(SCREEN_WIDTH, SCREEN_HEIGHT);

	RenderState::setBlendMode(true);
	RenderState::setDelthTest(true);

	// ”wŒi(•)
	Color color = _blackoutTexture->getColor();
	Render::getInstance()->drawRect2D(Vector3(0, 0, 0), Vector2(SCREEN_WIDTH, SCREEN_HEIGHT), Color(0, 0, 0, 255));

	RenderState::setDelthTest(false);
	// YouDied
	Render::getInstance()->drawScreenTexture(*_texture, _texture->getColor(), Vector2(250.0f, 50.0f), Vector2(1650.0f, 600.0f));

	// ‡”Ô“ä‚¾
	SceneBossBattleResult::menuRender();

	RenderState::setBlendMode(false);
	RenderState::setDelthTest(false);
}


//======================================================================================
//! ‰ð•ú
//======================================================================================
void SceneBossBattleResult::cleanup()
{
	SAFE_DELETE(_texture);
	SceneBase::cleanup();
}

void SceneBossBattleResult::menuSelection1()
{
	SceneBossBattleResult::menuSelection(_menuSelectionPosition[1][0], _menuSelectionPosition[1][1], MENU_SELECTION::CONTINUE);
	SceneBossBattleResult::menuSelection(_menuSelectionPosition[2][0], _menuSelectionPosition[2][1], MENU_SELECTION::BACK_TO_TITLE);
}

void SceneBossBattleResult::menuRender()
{
	SceneBase::menuRender(MENU_SELECTION::CONTINUE,		 _texture->getColor());
	SceneBase::menuRender(MENU_SELECTION::BACK_TO_TITLE, _texture->getColor());
}


void SceneBossBattleResult::menuSelection(
	const Vector2& position0,
	const Vector2& position1,
	const MENU_SELECTION& menu)
{
	if (_isBlackOut || _isBlackOut2) return;

	POINT	cursor;
	GetCursorPos(&cursor);

	if (cursor.x > position0._x && cursor.x < position1._x) {
		if (cursor.y > position0._y && cursor.y < position1._y) {
			if (GetKeyState(VK_LBUTTON) & 0x8000) {
				switch (menu)
				{
				case MENU_SELECTION::BACK_TO_TITLE:
					_isBlackOut = true;
					break;
				case MENU_SELECTION::CONTINUE:
					_isBlackOut2 = true;
					break;
				default:
					break;
				}
			}
		}
	}
}