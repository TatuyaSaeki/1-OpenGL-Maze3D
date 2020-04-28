


#pragma once

#include "Texture.h"

//===========================================================================
//!	モデル
//===========================================================================
class Model
{
public:
	// 面情報
	struct Face
	{
		s32		_vertexIndex[3];		//!< 頂点番号
		s32		_material;				//!< マテリアル番号(-1の場合はマテリアルなし)

		f32		_u[3];					//!< テクスチャ座標U
		f32		_v[3];					//!< テクスチャ座標V
	};

	// パーツオブジェクト
	struct Object
	{
		std::string				_name;		//!< 名前
		std::vector<Vector3>	_vertices;	//!< 頂点
		std::vector<Face>		_faces;		//!< フェイス

											//! 最適化
		void	optimize();
	};

	// マテリアル
	struct Material
	{
		Texture*				_texture;
	};


	void setAlpha(int a)
	{
		_color._a = CHECK_ALPHA(a);
	}

	void addAlpha(int a) 
	{
		_color._a = CHECK_ALPHA(_color._a + a);
	}

	Color getColor()
	{
		return _color;
	}

	void setColor(u8 r, u8 g, u8 b, u8 a)
	{
		Color color(r,g,b,a);
		_color = color;
	}

	void setColor(const Color& color)
	{
		_color = color;
	}

	//! コンストラクタ
	Model();

	//! デストラクタ
	~Model();

	//! 読み込み
	bool	load(const char* fileName, f32 scale = 1.0f);

	//! 描画
	void	render();
	void	renderDisplayList();


	//! Objectの個数を取得
	size_t	getObjectCount() const { return _objects.size(); }

	//! Objectの取得
	Object*	getObject(u32 index) const { return _objects[index]; }

	std::string& getFileName() { 
		return _fileName; 
	}

	void wireFrame();
public:


private:
	std::vector<Object*>		_objects;
	std::vector<Material>		_materials;

	GLuint						_displayListID;
	
	Color	_color;

	std::string _fileName;
};

