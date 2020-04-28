#pragma once

class MapObjectBase : public ObjectBase
{
public:
	MapObjectBase();
	virtual ~MapObjectBase();

	// ! ������
	virtual bool init();
	// ! �X�V
	virtual void update();
	// ! �`��
	virtual	void render();
	// ! ���
	virtual	void cleanup();

private:
	Texture*     _texture;
};