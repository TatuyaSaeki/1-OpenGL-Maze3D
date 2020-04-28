#pragma once

class SceneResult : public SceneBase
{
public:
	//! デフォルトコンストラクタ
	SceneResult() { _sceneEnd = false; }
	//! デストラクタ
	~SceneResult() {};

	//! 初期化
	bool init();
	//! 更新
	void update();
	//! 描画
	void render();
	//! 解放
	void cleanup();

	//bool sceneEnd() { return _sceneEnd ;}
	void menuSelection1();
	void menuRender();

	virtual void menuSelection(
		const Vector2& position0,
		const Vector2& position1,
		const MENU_SELECTION& menu
	) override;

private:
	Texture* _texture;
	float texAlpha;
	bool _isBlackOut2;
};