#pragma once

class SceneTitle : public SceneBase
{
public:
	//! デフォルトコンストラクタ
	SceneTitle(){ _sceneEnd = true; }
	//! デストラクタ
	~SceneTitle(){};

	//! 初期化
	bool init();
	//! 更新
	void update();
	//! 描画
	void render();
	//! 解放
	void cleanup();

	//bool sceneEnd() { return _sceneEnd; }

	void menuRender();
	
private:
	Camera* _camera;
	MapObjectManager*	_mapObjectManager;

	// サウンド
	Sound* _titleBgm;
	ItemManager*		_itemManager;
};

