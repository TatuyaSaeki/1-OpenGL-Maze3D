#pragma once

class MapObjectBase : public ObjectBase
{
public:
	MapObjectBase();
	virtual ~MapObjectBase();

	// ! 初期化
	virtual bool init();
	// ! 更新
	virtual void update();
	// ! 描画
	virtual	void render();
	// ! 解放
	virtual	void cleanup();

private:
	Texture*     _texture;
};