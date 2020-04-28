#pragma once

class SceneTitle : public SceneBase
{
public:
	//! �f�t�H���g�R���X�g���N�^
	SceneTitle(){ _sceneEnd = true; }
	//! �f�X�g���N�^
	~SceneTitle(){};

	//! ������
	bool init();
	//! �X�V
	void update();
	//! �`��
	void render();
	//! ���
	void cleanup();

	//bool sceneEnd() { return _sceneEnd; }

	void menuRender();
	
private:
	Camera* _camera;
	MapObjectManager*	_mapObjectManager;

	// �T�E���h
	Sound* _titleBgm;
	ItemManager*		_itemManager;
};

