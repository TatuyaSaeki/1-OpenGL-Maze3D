#pragma once

class MapObjectBase : public ObjectBase
{
public:
	MapObjectBase();
	virtual ~MapObjectBase();

	// ! ‰Šú‰»
	virtual bool init();
	// ! XV
	virtual void update();
	// ! •`‰æ
	virtual	void render();
	// ! ‰ğ•ú
	virtual	void cleanup();

private:
	Texture*     _texture;
};