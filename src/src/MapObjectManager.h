#pragma once
#include <vector>

#include <array>
class MapObjectManager
{
public:
	//! コンストラクタ
	MapObjectManager();
	//! デストラクタ
	~MapObjectManager();

	//! 初期化
	bool init();
	//! 更新
	void update();
	//! 描画
	void render();
	void render(const std::array<bool, 4>* keyActives);

	//! 解放
	void cleanup();

	bool load(Model* model);

	void deleteObject(std::vector<Triangle*> object);

	std::vector<Triangle*>& getGround()		{ return _ground;}
	std::vector<Triangle*>& getTower()		{ return _tower; }
	std::vector<Triangle*>& getMazeWall()	{ return _mazeWall; }
	std::vector<Triangle*>& getPedestal()	{ return _pedestal; }
	std::vector<Triangle*>& getSlope()		{ return _slope; }
	std::vector<Triangle*>& getDumyKeyItem() { return _dumyKeyItem; }
	std::vector<Triangle*>& getDumyWall() { return _dumyWall; }
	std::vector<Triangle*>& getDumyWall2() { return _dumyWall2; }

	std::vector<Triangle*>& getMazeWall2() { return _mazeWall2; }

	std::vector<Model*> getMapModel() const { return _mapModels; }
	bool MapObjectManager::addMapModel(const char* fileName, float scale);

	void setFlag(bool flag) { _flag = flag;}

private:
	enum  TextureID : int
	{
		Ground = 0,
		MazeWall,
		Tower,
		Pedestal,
		Slope,
		Wall,
		KeyItem1,
		KeyItem2,
		LastKeyItem,
		Ceiling,
	
		Max
	};

	std::vector<Triangle*>	_ground;
	std::vector<Triangle*>	_wall;
	std::vector<Triangle*>	_tower;
	std::vector<Triangle*>	_mazeWall;
	std::vector<Triangle*>	_pedestal;
	std::vector<Triangle*>	_slope;
	std::vector<Triangle*>	_keyItem;
	std::vector<Triangle*>	_lastKeyItem;
	std::vector<Triangle*>	_ceiling;
	std::vector<Triangle*>  _dumyKeyItem;
	std::vector<Triangle*>  _dumyWall;
	std::vector<Triangle*>  _dumyWall2;

	std::vector<Triangle*>	_mazeWall2;

	std::vector<Model*> _mapModels;
	//std::map<std::string, Texture*> _images;
	Texture* _texture[TextureID::Max];

	bool  _flag;
	float _alphaCount;

	void groundRender();
	void wallRender();
	void towerRender();
	void mazeWallRender();
	void slopeRender();
	void keyItemRender();
	void keyItemRender(const std::array<bool, 4>* keyActives);
	void lastKeyItemRender();
	void ceilingRender();
	
	void mazeWall2Render();

	std::array<bool, 4> _keyItemActives;
};
