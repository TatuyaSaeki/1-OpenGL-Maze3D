#include "stdafx.h"

//======================================================================================
//! デフォルトコンストラクタ
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
//! デストラクタ
//======================================================================================
MapObjectManager::~MapObjectManager()
{

}

//======================================================================================
//! 初期化
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

	_texture[TextureID::Wall]->load("data/Texture/Blackout/ダウンロード.png");
	_texture[TextureID::KeyItem1]->load("data/Texture/Map/KeyItemImage1.png");
	_texture[TextureID::KeyItem2]->load("data/Texture/Map/KeyItemImage2.png");
	_texture[TextureID::Ceiling]->load("data/Texture/Blackout/ダウンロード.png");


	return true;
}

//======================================================================================
//! 更新
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
//! 描画
//======================================================================================
void MapObjectManager::render()
{
	glLoadIdentity();


	glEnable(GL_ALPHA_TEST);
	// 半透明合成ON
	glEnable(GL_BLEND);
	// 通常ブレンド
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
//! 解放
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
//! コリジョンデーターの読み込み
//---------------------------------------------------------------------------
bool MapObjectManager::load(Model* model)
{
	if(strcmp(model->getFileName().c_str(), "data/Model/map/gameMap2.mqo") != 0) { 

		for (u32 o = 0; o < model->getObjectCount(); ++o) {
			auto	object = model->getObject(o);	// 転送元
			std::vector<Triangle*>*	dest = nullptr;				// 転送先
																// 文字列が一致しているかの検知
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

			// 該当しないオブジェクトはスキップ
			if (dest == nullptr) continue;


			//---------------------------------------------------------------------------
			// 三角形の読み込み
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
			auto	object = model->getObject(o);	// 転送元
			std::vector<Triangle*>*	dest = nullptr;				// 転送先

			if (strcmp(object->_name.c_str(), "MazeWall") == 0) {
				dest = &_mazeWall2;
			}

			// 該当しないオブジェクトはスキップ
			if (dest == nullptr) continue;


			//---------------------------------------------------------------------------
			// 三角形の読み込み
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
		MessageBox(nullptr, "cart.mqo ファイルの読み込みに失敗しました.", "エラー", MB_OK);
		return false;
	}

	_mapModels.push_back(model);
	load(model);

	return true;
}

void MapObjectManager::mazeWallRender()
{
	//-------------------------------------------------------------
	// テクスチャの描画
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
	// テクスチャの描画
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
	// テクスチャの描画
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
	// テクスチャの描画
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
	// テクスチャの描画
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
	// テクスチャの描画
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
	// テクスチャの描画
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
	// テクスチャの描画
	//-------------------------------------------------------------
	glDisable(GL_BLEND);

	Texture::set(_texture[TextureID::MazeWall]);
	glEnable(GL_ALPHA_TEST);
	// 半透明合成ON
	glEnable(GL_BLEND);
	// 通常ブレンド
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
	// テクスチャの描画
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
	// テクスチャの描画
	//-------------------------------------------------------------
	Texture::set(_texture[TextureID::Ceiling]);


	
	for (auto& ceiling : _ceiling) {
		glBegin(GL_TRIANGLE_FAN);

		Render::getInstance()->drawParticle(ceiling->_position);
	}
	Texture::set(nullptr);

}
