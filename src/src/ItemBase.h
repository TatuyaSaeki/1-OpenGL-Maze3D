#pragma once
#include "ObjectBase.h"


class ItemBase : public ObjectBase
{
public:
	ItemBase();
	virtual ~ItemBase();

	virtual	bool init();
	virtual void update();
	virtual void render();
	virtual void cleanup();

	void hitObjectType(const ObjectType& hitObject)
	{
		_hitObjectType = hitObject;
	}

protected:
	Texture* _texture;
	ObjectType _hitObjectType;
};