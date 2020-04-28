#pragma once

class DropItem : public ItemBase
{
public:
	DropItem();
	DropItem(float radius);
	virtual ~DropItem();

	bool init()    override;
	void update()  override;
	void render()  override;
	void cleanup() override;

	Sphere* getCollision()
	{
		return _collisionSphere;
	}

	bool isActive()
	{
		return _isActive;
	}

	void setActive(const bool flag)
	{
		_isActive = flag;
	}

	void setColor(const Color& color)
	{
		_color = color;
	}

private:
	Sphere * _collisionSphere;
	bool	 _isActive;
	float _radius;
};