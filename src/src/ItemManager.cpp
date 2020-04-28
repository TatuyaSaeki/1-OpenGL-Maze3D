#include "stdafx.h"

ItemManager::ItemManager()
:_lastKeyItem(nullptr)
, _bossKeyItem(nullptr)
{
}

ItemManager::~ItemManager()
{
}

bool ItemManager::init()
{

	return true;
}

void ItemManager::update()
{
	int activeCount = 0;
	int count = 0;
	for (auto& keyItem : _keyItems) {
		keyItem->update();
		if(keyItem->isActive()) {
			activeCount++;
		}
		count++;
	}

	if (_lastKeyItem != nullptr) {
		if (activeCount >= _keyItems.size()) {
			_lastKeyItem->setAllActive(true);
		}
		_lastKeyItem->update();
	}

	auto itr = _dropItem.begin();
	while (itr != _dropItem.end()) {
		if (!(*itr)->isActive()) {
			(*itr)->cleanup();
			delete *itr;
			itr = _dropItem.erase(itr);
		}
		else {
			(*itr)->update();
			++itr;
		}
	}

	if (_bossKeyItem != nullptr) {
		_bossKeyItem->update();
	}
}

void ItemManager::render()
{
	for(auto& keyItem : _keyItems) { 
		keyItem->render();
	}

	if(_lastKeyItem != nullptr) { 
		_lastKeyItem->render();
	}
	

	for (auto& dropItem : _dropItem) {
		dropItem->render();
	}

	if (_bossKeyItem != nullptr) {
		_bossKeyItem->render();
	}
}

void ItemManager::cleanup()
{
	for (auto& keyItem : _keyItems) {
		SAFE_DELETE_CLEANUP(keyItem);
	}
	_keyItems.clear();

	for (auto& dummyKeyItems : _dummyKeyItems) {
		SAFE_DELETE_CLEANUP(dummyKeyItems);
	}
	_dummyKeyItems.clear();

	SAFE_DELETE_CLEANUP(_lastKeyItem);

	for (auto& dropItem : _dropItem) {
		SAFE_DELETE_CLEANUP(dropItem);
	}
	_dropItem.clear();

	SAFE_DELETE_CLEANUP(_bossKeyItem);
}

void ItemManager::addKeyItem(KeyItem* keyItem)
{
	_keyItems.push_back(keyItem);
}

void ItemManager::addDummyKeyItem(DummyKeyItem* keyItem)
{
	_dummyKeyItems.push_back(keyItem);
}

void ItemManager::addDropItem(DropItem* dropItem)
{
	_dropItem.push_back(dropItem);
}

void ItemManager::addBossKeyItem(BossKeyItem* bossKeyItem)
{
	_bossKeyItem = bossKeyItem;
}

// コピーコンストラクタ聞く
void ItemManager::addLastKeyItem(LastKeyItem* lastKeyItem)
{
	if(_lastKeyItem == nullptr) {
		_lastKeyItem = lastKeyItem;
	}
}