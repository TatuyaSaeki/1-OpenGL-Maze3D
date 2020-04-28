#include "stdafx.h"

KeyInformation::KeyInformation()
	: _pPlayer(nullptr)
	, _pMapObjectManager(nullptr)
	, _keyInformationTex(nullptr)
{
	_keyInformationTex = new Texture();
	_keyInformationTex->load("data/Texture/Description/ダウンロード.png");
	_keyInformationTex2 = new Texture();
	_keyInformationTex2->load("data/Texture/Description/Move&LookAt.png");
	_keyInformation = KEY_INFO::NONE;
	moveLookAt = false;
}

KeyInformation::~KeyInformation()
{
	
}

void KeyInformation::update()
{
	if(_keyInformation != KEY_INFO::MOVE_LOOKAT) { 
		_keyInformation = KEY_INFO::NONE;
	}
	
	auto& playerHitObject = _pPlayer->getHitObject();

	for(auto& hitObject : playerHitObject) {
		switch (hitObject)
		{
		case ObjectType::WardlWeapon:
			_keyInformation = KEY_INFO::PICK_UP_WEAPON;
			_keyInformationTex->setTextureUV(Vector2(0.0f, 0.0f), Vector2(1.0f, 0.25f));
			break;
		case ObjectType::KeyItem:
			_keyInformation = KEY_INFO::START_UP_GIMMICK;
			_keyInformationTex->setTextureUV(Vector2(0.0f, 0.25f), Vector2(1.0f, 0.50f));
			break;
		default:

			break;
		}
	}

	if(_pPlayer->getWeapon() && !_pPlayer->firstAttack() && _keyInformation == KEY_INFO::NONE) {
		_keyInformation = KEY_INFO::ATTACK;
		_keyInformationTex->setTextureUV(Vector2(0.0f, 0.70f), Vector2(1.0f, 1.0f));
	}

	if (_pPlayer->firstAttack() && !_pPlayer->firstAvoidance() && _keyInformation == KEY_INFO::NONE) {
		_keyInformation = KEY_INFO::AVOIDANCE;
		_keyInformationTex->setTextureUV(Vector2(0.0f, 0.50f), Vector2(1.0f, 0.70f));
	}

	if (!moveLookAt) {
		if (!_pPlayer->firstMove()) {
			_keyInformation = KEY_INFO::MOVE_LOOKAT;
			_keyInformationTex2->setTextureUV(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));
		}
		else {
			_keyInformationTex2->addAlpha(-3);
			if (_keyInformationTex2 <= 0) {
				_keyInformationTex2->setAlpha(0);
				_keyInformation = KEY_INFO::NONE;
				moveLookAt = true;
			}
		}
	}
	
}

void KeyInformation::render()
{
	RenderState::setBlendMode(true);

	switch (_keyInformation)
	{
	case KEY_INFO::NONE:
		break;
	case KEY_INFO::AVOIDANCE:
		Render::getInstance()->drawScreenTextureUV(
			*_keyInformationTex,
			_keyInformationTex->getColor(),
//			Vector2(1280 / 2 + 120, 510),
//			Vector2(1280 / 2 + 220, 535),
			Vector2(1280 / 2 + 120, 515),
			Vector2(1280 / 2 + 300, 550),
			_keyInformationTex->getTextureUV1(),
			_keyInformationTex->getTextureUV2()
		);
		break;
	case KEY_INFO::ATTACK:
		Render::getInstance()->drawScreenTextureUV(
			*_keyInformationTex,
			_keyInformationTex->getColor(),
//			Vector2(1280 / 2 + 120, 510),
//			Vector2(1280 / 2 + 220, 545),
			Vector2(1280 / 2 + 120, 510),
			Vector2(1280 / 2 + 300, 560),
			_keyInformationTex->getTextureUV1(),
			_keyInformationTex->getTextureUV2()
		);
		break;
	case KEY_INFO::MOVE_LOOKAT:
		Render::getInstance()->drawScreenTextureUV(
			*_keyInformationTex2,
			_keyInformationTex2->getColor(),
			Vector2(1280 / 2 + 100, 480),
			Vector2(1280 / 2 + 280, 580),
			_keyInformationTex2->getTextureUV1(),
			_keyInformationTex2->getTextureUV2()
		);
		break;
	default:
		Render::getInstance()->drawScreenTextureUV(
			*_keyInformationTex,
			_keyInformationTex->getColor(),
			Vector2(1280 / 2 + 120, 500),
			Vector2(1280 / 2 + 300, 540),
			_keyInformationTex->getTextureUV1(),
			_keyInformationTex->getTextureUV2()
		);
		break;
	}
}

void KeyInformation::cleanup()
{
	SAFE_DELETE(_keyInformationTex);
	SAFE_DELETE(_keyInformationTex2);
}

void KeyInformation::setPlaeyr(Player& player)
{
	_pPlayer = &player;
}

void KeyInformation::setMapObject(MapObjectManager& mapObj)
{
	_pMapObjectManager = &mapObj;
}