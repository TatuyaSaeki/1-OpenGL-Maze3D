#pragma once

class SceneGameClear : public SceneBase
{
public:
	//! デフォルトコンストラクタ
	SceneGameClear() { _sceneEnd = false; }
	//! デストラクタ
	~SceneGameClear() {};

	//! 初期化
	bool init()		override;
	//! 更新
	void update()	override;
	//! 描画
	void render()	override;
	//! 解放
	void cleanup()	override;

	//bool sceneEnd() { return _sceneEnd ;}
	void menuSelection();
	void menuRender();

private:
	Texture * _texture;
	float texAlpha;
};