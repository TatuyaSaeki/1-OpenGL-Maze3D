#pragma once

class SceneBossBattleResult : public SceneBase
{
public:
	//! デフォルトコンストラクタ
	SceneBossBattleResult() { _sceneEnd = false; }
	//! デストラクタ
	~SceneBossBattleResult() {};

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

	void menuSelection(
		const Vector2& position0,
		const Vector2& position1,
		const MENU_SELECTION& menu
	) override;
	void menuRender();

private:
	bool _isBlackOut2;
	Texture * _texture;
	float texAlpha;
};