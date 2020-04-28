#pragma once

class SceneBossBattleResult : public SceneBase
{
public:
	//! �f�t�H���g�R���X�g���N�^
	SceneBossBattleResult() { _sceneEnd = false; }
	//! �f�X�g���N�^
	~SceneBossBattleResult() {};

	//! ������
	bool init();
	//! �X�V
	void update();
	//! �`��
	void render();
	//! ���
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