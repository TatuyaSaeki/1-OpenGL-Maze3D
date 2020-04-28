#pragma once

class KeyItem : public ItemBase
{
public:
	KeyItem();
	virtual ~KeyItem();

	virtual bool init();
	virtual void update();
	virtual void render();
	virtual void cleanup();

	virtual void setPosition(const Vector3& position) 
	{
		_position = position; 
		_endPosition = _position;
	}

	virtual Sphere* getCollision()
	{
		return _collisionSphere;
	}

	virtual Sphere* getDumyCollision()
	{
		return _dumyCollision;
	}

	virtual void setActive(bool flag)
	{
		_isActive = flag;
	}
	virtual bool isActive() 
	{
		return _isActive;
	}

protected:
	Sphere* _dumyCollision;
	Sphere* _collisionSphere;
	bool	_isActive;			// ‹N“®‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©
	Vector3 _endPosition;
	Vector3* _endPosition2;
	bool _flag;

	Sound* _sound;

	Color _dumyItem;
	bool flag;
};