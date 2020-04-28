#pragma once

class SceneGameClear : public SceneBase
{
public:
	//! �f�t�H���g�R���X�g���N�^
	SceneGameClear() { _sceneEnd = false; }
	//! �f�X�g���N�^
	~SceneGameClear() {};

	//! ������
	bool init()		override;
	//! �X�V
	void update()	override;
	//! �`��
	void render()	override;
	//! ���
	void cleanup()	override;

	//bool sceneEnd() { return _sceneEnd ;}
	void menuSelection();
	void menuRender();

private:
	Texture * _texture;
	float texAlpha;
};