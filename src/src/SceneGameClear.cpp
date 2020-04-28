#include "stdafx.h"

//======================================================================================
//! ‰Šú‰»
//======================================================================================
bool SceneGameClear::init()
{
	_texture = new Texture();
	if (_texture == nullptr) return false;
	if (!_texture->load("data/Texture/GameClear.png")) return false;
	_texture->setColor(Color(255, 255, 255, 0));

	if (!SceneBase::blackoutInit("data/Texture/Blackout/ƒ_ƒEƒ“ƒ[ƒh.png", 255)) return false;

	return true;
}

//======================================================================================
//! XV
//======================================================================================
void SceneGameClear::update()
{
	if (!_isBlackOut) {
		_texture->addAlpha(2);
		if (_texture->getColor()._a >= 255) {
			_texture->setAlpha(255);
		}
	}
	else {
		_texture->addAlpha(-2);
		if (_texture->getColor()._a <= 0) {
			scene().jump(SCENE_TYPE::TITLE);
		}
	}

	SceneGameClear::menuSelection();
}

//======================================================================================
//! •`‰æ
//======================================================================================
void SceneGameClear::render()
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
	SceneGameClear::menuRender();

	RenderState::setBlendMode(false);
	RenderState::setDelthTest(false);
}

//======================================================================================
//! ‰ð•ú
//======================================================================================
void SceneGameClear::cleanup()
{
	SAFE_DELETE(_texture);
	SceneBase::cleanup();
}

void SceneGameClear::menuSelection()
{
	SceneBase::menuSelection(_menuSelectionPosition[2][0], _menuSelectionPosition[2][1], MENU_SELECTION::BACK_TO_TITLE);
}

void SceneGameClear::menuRender()
{	
	SceneBase::menuRender(MENU_SELECTION::BACK_TO_TITLE, _texture->getColor());
}