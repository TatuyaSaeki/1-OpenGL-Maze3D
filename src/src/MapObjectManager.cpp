#include "stdafx.h"

//======================================================================================
//! �f�t�H���g�R���X�g���N�^
//======================================================================================
MapObjectManager::MapObjectManager()
	:_flag(false)
	,_alphaCount(255.0f)
{
	for(auto& text : _texture) {
		text = nullptr;
	}

	for(int i = 0; i < _keyItemActives.size(); i++) { 
		_keyItemActives[i] = false;
	}
}

//======================================================================================
//! �f�X�g���N�^
//======================================================================================
MapObjectManager::~MapObjectManager()
{

}

//======================================================================================
//! ������
//======================================================================================
bool MapObjectManager::init()
{
	for(int i = 0; i < TextureID::Max; i++){
		_texture[i] = new Texture();
		if(_texture[i] == nullptr) return false;
	}

	_texture[TextureID::Ground]->load("data/Texture/Map/groundImage0.png");
	_texture[TextureID::MazeWall]->load("data/Texture/Map/wallImage1.png");

	_texture[TextureID::Tower]->load("data/Texture/Map/wallImage1.png");
	_texture[TextureID::Pedestal]->load("data/Texture/Map/groundImage0.png");
	_texture[TextureID::Slope]->load("data/Texture/Map/wallImage1.png");

	_texture[TextureID::Wall]->load("data/Texture/Blackout/�_�E�����[�h.png");
	_texture[TextureID::KeyItem1]->load("data/Texture/Map/KeyItemImage1.png");
	_texture[TextureID::KeyItem2]->load("data/Texture/Map/KeyItemImage2.png");
	_texture[TextureID::Ceiling]->load("data/Texture/Blackout/�_�E�����[�h.png");


	return true;
}

//======================================================================================
//! �X�V
//======================================================================================
void MapObjectManager::update()
{
	if(_flag) _alphaCount -= 2.0f;
	

	if(_alphaCount < 0.0f) { 
		auto itr = _mapModels.begin();
		while(itr != _mapModels.end()) { 
			if (strcmp((*itr)->getFileName().c_str(), "data/Model/map/gameMap2.mqo") == 0) {
				deleteObject(_mazeWall2);
				SAFE_DELETE(*itr);
				itr = _mapModels.erase(itr);
			}else {
				++itr;
			}
		}
	}
}

//======================================================================================
//! �`��
//======================================================================================
void MapObjectManager::render()
{
	glLoadIdentity();


	glEnable(GL_ALPHA_TEST);
	// ����������ON
	glEnable(GL_BLEND);
	// �ʏ�u�����h
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4ubv((GLubyte*)&Color(255, 255, 255, 255));

	

	groundRender();
	mazeWallRender();
	towerRender();
	wallRender();
	slopeRender();
	//keyItemRender();
	//lastKeyItemRender();
	ceilingRender();
	mazeWall2Render();

	glDisable(GL_BLEND);
}

void MapObjectManager::render(const std::array<bool, 4>* keyActives)
{
	glLoadIdentity();

	groundRender();
	mazeWallRender();
	towerRender();
	wallRender();
	slopeRender();
	keyItemRender(keyActives);
	lastKeyItemRender();
	ceilingRender();
	mazeWall2Render();
}

//======================================================================================
//! ���
//======================================================================================
void MapObjectManager::cleanup()
{
	deleteObject(_dumyWall);
	deleteObject(_ceiling);
	deleteObject(_ground);
	deleteObject(_tower);
	deleteObject(_mazeWall);
	deleteObject(_pedestal);
	deleteObject(_slope);
	deleteObject(_wall);
	deleteObject(_keyItem);
	deleteObject(_lastKeyItem);
	deleteObject(_dumyKeyItem);
	deleteObject(_dumyWall2);

	if(!_flag || _alphaCount > 0 && _alphaCount != 0.0f) {
		deleteObject(_mazeWall2);
	}

	for(auto& model : _mapModels) { 
		SAFE_DELETE(model);
	}

	for(int i = 0; i < TextureID::Max; i++){
		SAFE_DELETE(_texture[i]);
	}
}

//---------------------------------------------------------------------------
//! �R���W�����f�[�^�[�̓ǂݍ���
//---------------------------------------------------------------------------
bool MapObjectManager::load(Model* model)
{
	if(strcmp(model->getFileName().c_str(), "data/Model/map/gameMap2.mqo") != 0) { 

		for (u32 o = 0; o < model->getObjectCount(); ++o) {
			auto	object = model->getObject(o);	// �]����
			std::vector<Triangle*>*	dest = nullptr;				// �]����
																// �����񂪈�v���Ă��邩�̌��m
			if (strcmp(object->_name.c_str(), "Ground") == 0) {
				dest = &_ground;
			}

			if (strcmp(object->_name.c_str(), "Tower") == 0) {
				dest = &_tower;
			}

			if (strcmp(object->_name.c_str(), "MazeWall") == 0) {
				dest = &_mazeWall;
			}

			if (strcmp(object->_name.c_str(), "Pedestal") == 0) {
				dest = &_pedestal;
			}

			if (strcmp(object->_name.c_str(), "Slope") == 0) {
				dest = &_slope;
			}

			if (strcmp(object->_name.c_str(), "Wall") == 0) {
				dest = &_wall;
			}

			if (strcmp(object->_name.c_str(), "KeyItem") == 0) {
				dest = &_keyItem;
			}

			if (strcmp(object->_name.c_str(), "LastKeyItem") == 0) {
				dest = &_lastKeyItem;
			}
			if (strcmp(object->_name.c_str(), "Ceiling") == 0) {
				dest = &_ceiling;
			}
			if (strcmp(object->_name.c_str(), "DumyKeyItem") == 0) {
				dest = &_dumyKeyItem;
			}
			if (strcmp(object->_name.c_str(), "DumyWall") == 0) {
				dest = &_dumyWall;
			}
			if (strcmp(object->_name.c_str(), "DumyWall2") == 0) {
				dest = &_dumyWall2;
			}

			// �Y�����Ȃ��I�u�W�F�N�g�̓X�L�b�v
			if (dest == nullptr) continue;


			//---------------------------------------------------------------------------
			// �O�p�`�̓ǂݍ���
			//---------------------------------------------------------------------------
			for (u32 i = 0; i < object->_faces.size(); ++i) {
				auto& f = object->_faces[i];

				Vector3 p1 = object->_vertices[f._vertexIndex[0]];
				Vector3 p2 = object->_vertices[f._vertexIndex[1]];
				Vector3 p3 = object->_vertices[f._vertexIndex[2]];

				dest->push_back(new Triangle(p3, p2, p1));
			}
		}
	}

	else { 
		for (u32 o = 0; o < model->getObjectCount(); ++o) {
			auto	object = model->getObject(o);	// �]����
			std::vector<Triangle*>*	dest = nullptr;				// �]����

			if (strcmp(object->_name.c_str(), "MazeWall") == 0) {
				dest = &_mazeWall2;
			}

			// �Y�����Ȃ��I�u�W�F�N�g�̓X�L�b�v
			if (dest == nullptr) continue;


			//---------------------------------------------------------------------------
			// �O�p�`�̓ǂݍ���
			//---------------------------------------------------------------------------
			for (u32 i = 0; i < object->_faces.size(); ++i) {
				auto& f = object->_faces[i];

				Vector3 p1 = object->_vertices[f._vertexIndex[0]];
				Vector3 p2 = object->_vertices[f._vertexIndex[1]];
				Vector3 p3 = object->_vertices[f._vertexIndex[2]];

				dest->push_back(new Triangle(p1, p2, p3));
			}
		}
	}
	return true;
}

void MapObjectManager::deleteObject(std::vector<Triangle*> object)
{
	for(auto* o : object) {
		if (o != nullptr) { 
			delete o;
			o = nullptr;
		}
	}
	object.clear();
}

bool MapObjectManager::addMapModel(const char* fileName, float scale)
{
	Model* model = new Model();
	if(model == nullptr) return false;

	if(!model->load(fileName, scale))  {
		MessageBox(nullptr, "cart.mqo �t�@�C���̓ǂݍ��݂Ɏ��s���܂���.", "�G���[", MB_OK);
		return false;
	}

	_mapModels.push_back(model);
	load(model);

	return true;
}

void MapObjectManager::mazeWallRender()
{
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��
	//-------------------------------------------------------------
	Texture::set(_texture[TextureID::MazeWall]);

	for (auto& mazeWall : _mazeWall) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(mazeWall->_position);
	}

	Texture::set(nullptr);

#if 0
	Texture::set(_texture[TextureID::KeyItem2]);
	for (auto& keyItem : _dumyWall) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(keyItem->_position);
	}
	Texture::set(nullptr);
#endif
}

void MapObjectManager::groundRender()
{
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��
	//-------------------------------------------------------------
	Texture::set(_texture[TextureID::Ground]);



	for (auto& ground : _ground) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(ground->_position);
	}
	Texture::set(nullptr);

}

void MapObjectManager::towerRender()
{
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��
	//-------------------------------------------------------------
	Texture::set(_texture[TextureID::Tower]);

	for (auto& tower : _tower) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(tower->_position);
	}
	Texture::set(nullptr);

}

void MapObjectManager::slopeRender()
{
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��
	//-------------------------------------------------------------
	Texture::set(_texture[TextureID::Slope]);


	glColor4ubv((GLubyte*)&Color(255, 255, 255, 255));
	for (auto& slope : _slope) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(slope->_position);
	}
	Texture::set(nullptr);

}

void MapObjectManager::keyItemRender()
{
	glLoadIdentity();
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��
	//-------------------------------------------------------------
	Texture::set(_texture[TextureID::KeyItem1]);


	for (auto& keyItem : _dumyWall2) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(keyItem->_position);
	}

	Texture::set(nullptr);
}

void MapObjectManager::keyItemRender(const std::array<bool, 4>* keyActives)
{
	glLoadIdentity();
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��
	//-------------------------------------------------------------
#if 0
	for (int i = 0; i < keyActives->size(); i++) {
		if (keyActives[i]) {
			Texture::set(_texture[TextureID::KeyItem2]);
		}
		else {
			Texture::set(_texture[TextureID::KeyItem1]);
		}
	}
# else
	Texture::set(_texture[TextureID::KeyItem1]);
#endif


	for (auto& keyItem : _dumyWall) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(keyItem->_position);
	}
	Texture::set(nullptr);
}

void MapObjectManager::lastKeyItemRender()
{
	glLoadIdentity();
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��
	//-------------------------------------------------------------
	Texture::set(_texture[TextureID::KeyItem1]);


	for (auto& lastKeyItem : _lastKeyItem) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(lastKeyItem->_position);
	}

	Texture::set(nullptr);
}

void MapObjectManager::mazeWall2Render()
{
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��
	//-------------------------------------------------------------
	glDisable(GL_BLEND);

	Texture::set(_texture[TextureID::MazeWall]);
	glEnable(GL_ALPHA_TEST);
	// ����������ON
	glEnable(GL_BLEND);
	// �ʏ�u�����h
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4ubv((GLubyte*)&Color(255, 255, 255, _alphaCount));
	for (auto& mazeWall2 : _mazeWall2) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(mazeWall2->_position);
	}
	Texture::set(nullptr);
	glDisable(GL_BLEND);
}

void MapObjectManager::wallRender()
{
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��
	//-------------------------------------------------------------
	Texture::set(_texture[TextureID::Wall]);

	for (auto& wall : _wall) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(wall->_position);
	}
	Texture::set(nullptr);
}

void MapObjectManager::ceilingRender()
{
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��
	//-------------------------------------------------------------
	Texture::set(_texture[TextureID::Ceiling]);


	
	for (auto& ceiling : _ceiling) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(ceiling->_position);
	}
	Texture::set(nullptr);

}
