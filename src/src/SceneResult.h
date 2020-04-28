#pragma once

class SceneResult : public SceneBase
{
public:
	//! �f�t�H���g�R���X�g���N�^
	SceneResult() { _sceneEnd = false; }
	//! �f�X�g���N�^
	~SceneResult() {};

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