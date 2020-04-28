#pragma once

class LastKeyItem : public KeyItem
{
public:
	LastKeyItem();
	~LastKeyItem();

	bool init() override;
	void update() override;
	void render() override;
	void cleanup() override;

	void setAllActive(bool allActive)
	{
		_keyItemAllActive = allActive;
	}

	bool isHit()
	{
		return _isHit;
	}
	bool isStartUp()
	{
		return _isStartUp;
	}

	void setStartUp()
	{
		_isStartUp = true;
	}


private:
	bool _keyItemAllActive;
	bool _isHit;
	bool _isStartUp;
	Sound* _activeSound;
	int _count;
	bool _flag;
};