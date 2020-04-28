#pragma once

class ItemManager
{
public:
	ItemManager();
	~ItemManager();

	bool init();

	void update();

	void render();

	void cleanup();

	void addKeyItem(KeyItem* keyItem);
	std::vector<KeyItem*>& getKeyItems()	{ return _keyItems; };

	void ItemManager::addDummyKeyItem(DummyKeyItem* keyItem);

	LastKeyItem* getLastKeyItem()			
	{
		if(_lastKeyItem != nullptr)	return _lastKeyItem;
		return nullptr;
	}

	BossKeyItem* getBossKeyItem()
	{
		if (_bossKeyItem != nullptr)	return _bossKeyItem;
		return nullptr;
	}

	void addDropItem(DropItem* dropItem);
	std::vector<DropItem*>& getDropItem()	{ return _dropItem; };

	void addLastKeyItem(LastKeyItem* lastKeyItem);
	void addBossKeyItem(BossKeyItem* bossKeyItem);

	bool getLastKeyItemIsActive()
	{
		return _lastKeyItem->isActive();
	}

	bool getLastKeyItemIsStartUp()
	{
		return _lastKeyItem->isStartUp();
	}

	bool getBossKeyItemIsActive()
	{
		return _bossKeyItem->isActive();
	}

	bool getBossKeyItemIsStartUp()
	{
		return _lastKeyItem->isStartUp();
	}

	void setlastKeyItem()
	{
		_lastKeyItem->setStartUp();
	}

private:
	std::vector<KeyItem*>		_keyItems;
	std::vector<DummyKeyItem*>	_dummyKeyItems;
	LastKeyItem*				_lastKeyItem;
	std::vector<DropItem*>		_dropItem;
	BossKeyItem*				_bossKeyItem;
};