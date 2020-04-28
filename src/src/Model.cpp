#include "stdafx.h"

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Model::Model()
	: _displayListID((GLuint)-1)
	, _color(Color(255,255,255,255))
{

}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
Model::~Model()
{
	for (auto& o : _objects) {
		delete o;
		o = nullptr;
	}

	for (auto& m : _materials) {
		delete m._texture;
	}


}

//---------------------------------------------------------------------------
//! 読み込み
//---------------------------------------------------------------------------
bool Model::load(const char* fileName, f32 scale)
{
	//----------------------------------------------------------
	// ファイルを開く
	//----------------------------------------------------------
	_fileName = fileName;
	FILE*	fp = fopen(fileName, "rt");
	if (fp == nullptr) {
		return false;
	}
	
	//----------------------------------------------------------
	// 一行ずつファイル終端まで解析していく
	//----------------------------------------------------------
	enum class State {
		Root,
		Object,
		Vertex,
		Face,
		Material,
	};

	State	state = State::Root;	// 状態の初期値



	Object*	object = nullptr;

	char	str[1024];
	while (fgets(str, sizeof(str), fp)) {

		if(strncmp(str, "Object ""blade"" {",16) != 0) {
			int a = 0;
		}

		switch (state) {
		case State::Root:		//---- ルート解析中
			if (strncmp(str, "Object ", 7) == 0) {

				object = new Object();

				// オブジェクト名を取り出す Object "name"
				char*	p = strchr(str, '\"');
				p += 1;	// 区切り記号をスキップ

				char*	name = p;	// 名前先頭

				p = strchr(p, '\"');
				*p = 0;				// 文字終端

				object->_name = name;

				state = State::Object;
				break;
			}

			if (strncmp(str, "Material", 8) == 0) {
				state = State::Material;
				break;
			}

			break;
		case State::Object:		//---- オブジェクト解析中
			if (strncmp(str, "}", 1) == 0) {

				// 最適化
				object->optimize();

				// オブジェクトリストにつなぐ
				_objects.push_back(object);
				object = nullptr;

				state = State::Root;
				break;
			}
			if (strstr(str, "vertex ") != nullptr) {
				state = State::Vertex;
				break;
			}

			if (strstr(str, "face ") != nullptr) {
				state = State::Face;
				break;
			}


			break;
		case State::Vertex:		//---- 頂点解析中
			if (strstr(str, "}") != nullptr) {
				state = State::Object;
				break;
			}

			f32	x;
			f32	y;
			f32	z;

			sscanf(str, " %f %f %f", &x, &y, &z);

			x *= scale;
			y *= scale;
			z *= scale;

			object->_vertices.push_back(Vector3(x, y, z));

			break;

		case State::Face:		//---- フェイス解析中
			if (strstr(str, "}") != nullptr) {
				state = State::Object;
				break;
			}

			// 三角形か四角形かをチェック
			s32	vertexCount;		// 3 or 4
			sscanf(str, " %d", &vertexCount);

			if (vertexCount <= 2) {	// 不正な値の場合はスキップ
				break;
			}

			Face	f;

			// マテリアル番号を取得
			f._material = -1;

			{
				char*	p = strstr(str, "M(");
				if (p != nullptr) {
					sscanf(p, "M(%d)", &f._material);
				}
			}

			if (vertexCount == 3) {
				// 三角形の場合
				char*	p = strstr(str, "V(");

				s32	index[3];
				sscanf(p, "V(%d %d %d)", &index[0], &index[1], &index[2]);

				f32	u[3]{};
				f32	v[3]{};

				p = strstr(str, "UV(");
				if (p) {
					sscanf(p, "UV(%f %f %f %f %f %f)", &u[0], &v[0], &u[1], &v[1], &u[2], &v[2]);
				}

				f._vertexIndex[0] = index[0];
				f._vertexIndex[1] = index[1];
				f._vertexIndex[2] = index[2];

				f._u[0] = u[0];
				f._u[1] = u[1];
				f._u[2] = u[2];

				f._v[0] = v[0];
				f._v[1] = v[1];
				f._v[2] = v[2];

				// 三角形の登録
				object->_faces.push_back(f);
			}
			else {
				// 四角形の場合
				char*	p = strstr(str, "V(");

				s32	index[4];
				sscanf(p, "V(%d %d %d %d)", &index[0], &index[1], &index[2], &index[3]);


				f32	u[4]{};
				f32	v[4]{};

				p = strstr(str, "UV(");
				if (p) {
					sscanf(p, "UV(%f %f %f %f %f %f %f %f)", &u[0], &v[0], &u[1], &v[1], &u[2], &v[2], &u[3], &v[3]);
				}


				f._vertexIndex[0] = index[0];
				f._vertexIndex[1] = index[1];
				f._vertexIndex[2] = index[2];

				f._u[0] = u[0];
				f._u[1] = u[1];
				f._u[2] = u[2];
				f._v[0] = v[0];
				f._v[1] = v[1];
				f._v[2] = v[2];

				object->_faces.push_back(f);

				f._vertexIndex[0] = index[2];
				f._vertexIndex[1] = index[3];
				f._vertexIndex[2] = index[0];

				f._u[0] = u[2];
				f._u[1] = u[3];
				f._u[2] = u[0];
				f._v[0] = v[2];
				f._v[1] = v[3];
				f._v[2] = v[0];

				object->_faces.push_back(f);
			}
			break;
		case State::Material:
		{
			if (strstr(str, "}") != nullptr) {
				state = State::Root;
				break;
			}

			Texture*	texture = nullptr;

			// tex("")構文を検出
			char*	p = strstr(str, "tex(");
			if (p != nullptr) {
				// tex(" の5文字分進める
				p += 5;

				// テクスチャ名を抽出
				char*	textureName = p;	// 先頭を保存
				while (*p != '\"') {
					p++;
				}

				*p = 0;

				// テクスチャを読み込み

				// MQOファイルのパス指定箇所のパス名のみ抽出
				char	drive[MAX_PATH];
				char	path[MAX_PATH];
				char	file[MAX_PATH];
				char	ext[MAX_PATH];
				_splitpath_s(fileName, drive, path, file, ext);

				char	textureFullPath[MAX_PATH];
				strcpy(textureFullPath, drive);
				strcat(textureFullPath, path);
				strcat(textureFullPath, textureName);

				texture = new Texture();
				if (texture->load(textureFullPath) == false) {
					MessageBox(nullptr, textureFullPath, "テクスチャ読み込みに失敗しました.", MB_OK);
					delete texture;
					texture = nullptr;
				}
			}

			// マテリアルを登録
			Material	m;
			m._texture = texture;

			_materials.push_back(m);
		}
		break;
		}

		//		MessageBox(nullptr, str, "読み込み", MB_OK);

	}

	//----------------------------------------------------------
	// ファイルを閉じる
	//----------------------------------------------------------
	fclose(fp);

	return true;
}

void Model::wireFrame()
{
	// アルファテスト有効化
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 1.0f);	// 比較条件　A > 0.5のときに描画する
	glColor3ub(255, 255, 255);

	glBegin(GL_LINES);

	// 全てのオブジェクト部品
	for (auto& o : _objects) {
		auto&	v = o->_vertices;	// 頂点配列  

									// 全てのフェイス
		for (auto& face : o->_faces) {
			glVertex3fv((GLfloat*)&v[face._vertexIndex[0]]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[1]]);

			glVertex3fv((GLfloat*)&v[face._vertexIndex[1]]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[2]]);

			glVertex3fv((GLfloat*)&v[face._vertexIndex[2]]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[0]]);
		}
	}

	glEnd();

	// アルファテスト無効化
	glDisable(GL_ALPHA_TEST);

}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void Model::render()
{
	// 聞こう
#if 0
	//-------------------------------------------------------------
	// 2回目以降はディスプレイリストを実行して終わる
	//-------------------------------------------------------------
	if (_displayListID != (GLuint)-1) {

		glCallList(_displayListID);
		return;
	}

	// ディスプレイレイストを生成し記録開始
	_displayListID = glGenLists(1);

	glNewList(_displayListID, GL_COMPILE_AND_EXECUTE);
#endif

	//-------------------------------------------------------------
	// 点群の描画
	//-------------------------------------------------------------
#if 0
	glPointSize(3.0f);
	glColor3ub(255, 255, 255);

	glBegin(GL_POINTS);

	// 全てのオブジェクト部品
	for (auto& o : _objects) {

		// オブジェクト内のすべての頂点
		for (auto& v : o->_vertices) {
			glVertex3fv((GLfloat*)&v);
		}
	}

	glEnd();

	glPointSize(1.0f);
#endif
	//-------------------------------------------------------------
	// ワイヤーフレームの描画
	//-------------------------------------------------------------

#if 0
	// アルファテスト有効化
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 1.0f);	// 比較条件　A > 0.5のときに描画する
	glColor3ub(255, 255, 255);

	glBegin(GL_LINES);

	// 全てのオブジェクト部品
	for (auto& o : _objects) {
		auto&	v = o->_vertices;	// 頂点配列  

									// 全てのフェイス
		for (auto& face : o->_faces) {
			glVertex3fv((GLfloat*)&v[face._vertexIndex[0]]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[1]]);

			glVertex3fv((GLfloat*)&v[face._vertexIndex[1]]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[2]]);

			glVertex3fv((GLfloat*)&v[face._vertexIndex[2]]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[0]]);
		}
	}

	glEnd();

	// アルファテスト無効化
	glDisable(GL_ALPHA_TEST);

#endif

	//glColor4ub(128, 128, 128, a);

	//-------------------------------------------------------------
	// 三角形の描画
	//-------------------------------------------------------------
	s32	material = -1;	// 現在のマテリアル番号
	Texture::set(nullptr);
	//Color c(128, 128, 128, a);
	glColor4ubv((GLubyte*)&_color);
	// ポリゴンオフセット機能でデプスをずらす
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);

	// アルファテスト有効化
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);	// 比較条件　A > 0.5のときに描画する

									// 半透明合成ON
	glEnable(GL_BLEND);
	// 通常ブレンド
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	glBegin(GL_TRIANGLES);
	// 全てのオブジェクト部品
	for (auto& o : _objects) {

		auto&	v = o->_vertices;	// 頂点配列  

									// 全てのフェイス
		for (auto& face : o->_faces) {

			// マテリアルの変更
			if (material != face._material) {

				glEnd();				//--- いったん描画コマンドを閉じる

										// テクスチャの設定
				if (face._material != -1) {
					// テクスチャあり
					Texture::set(_materials[face._material]._texture);
				}
				else {
					// テクスチャなし
					Texture::set(nullptr);
				}

				glBegin(GL_TRIANGLES);	//--- 再度開きなおす

										// 現在の番号を更新
				material = face._material;
			}

			glTexCoord2f(face._u[0], face._v[0]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[0]]);


			glTexCoord2f(face._u[1], face._v[1]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[1]]);


			glTexCoord2f(face._u[2], face._v[2]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[2]]);
		}
	}
	glEnd();

	glDisable(GL_BLEND);
	// アルファテスト無効化
	glDisable(GL_ALPHA_TEST);

	// ポリゴンオフセット無効化
	glDisable(GL_POLYGON_OFFSET_FILL);

	// テクスチャをもとに戻す
	Texture::set(nullptr);

	// ディスプレイリスト記録終了
	glEndList();
}



//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void Model::renderDisplayList()
{
	// 聞こう
	//-------------------------------------------------------------
	// 2回目以降はディスプレイリストを実行して終わる
	//-------------------------------------------------------------
	if (_displayListID != (GLuint)-1) {

		glCallList(_displayListID);
		return;
	}

	// ディスプレイレイストを生成し記録開始
	_displayListID = glGenLists(1);

	glNewList(_displayListID, GL_COMPILE_AND_EXECUTE);

	//-------------------------------------------------------------
	// 点群の描画
	//-------------------------------------------------------------
#if 0
	glPointSize(3.0f);
	glColor3ub(255, 255, 255);

	glBegin(GL_POINTS);

	// 全てのオブジェクト部品
	for (auto& o : _objects) {

		// オブジェクト内のすべての頂点
		for (auto& v : o->_vertices) {
			glVertex3fv((GLfloat*)&v);
		}
	}

	glEnd();

	glPointSize(1.0f);
#endif
	//-------------------------------------------------------------
	// ワイヤーフレームの描画
	//-------------------------------------------------------------
#if 0
	glBegin(GL_LINES);

	// 全てのオブジェクト部品
	for (auto& o : _objects) {
		auto&	v = o->_vertices;	// 頂点配列  

									// 全てのフェイス
		for (auto& face : o->_faces) {
			glVertex3fv((GLfloat*)&v[face._vertexIndex[0]]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[1]]);

			glVertex3fv((GLfloat*)&v[face._vertexIndex[1]]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[2]]);

			glVertex3fv((GLfloat*)&v[face._vertexIndex[2]]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[0]]);
		}
	}

	glEnd();
#endif
	//-------------------------------------------------------------
	// 三角形の描画
	//-------------------------------------------------------------
	s32	material = -1;	// 現在のマテリアル番号
	Texture::set(nullptr);

	// ポリゴンオフセット機能でデプスをずらす
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);

	// アルファテスト有効化
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);	// 比較条件　A > 0.5のときに描画する

	glBegin(GL_TRIANGLES);
	// 全てのオブジェクト部品
	for (auto& o : _objects) {

		auto&	v = o->_vertices;	// 頂点配列  

									// 全てのフェイス
		for (auto& face : o->_faces) {

			// マテリアルの変更
			if (material != face._material) {

				glEnd();				//--- いったん描画コマンドを閉じる

										// テクスチャの設定
				if (face._material != -1) {
					// テクスチャあり
					Texture::set(_materials[face._material]._texture);
				}
				else {
					// テクスチャなし
					Texture::set(nullptr);
				}

				glBegin(GL_TRIANGLES);	//--- 再度開きなおす

										// 現在の番号を更新
				material = face._material;
			}

			glTexCoord2f(face._u[0], face._v[0]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[0]]);


			glTexCoord2f(face._u[1], face._v[1]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[1]]);


			glTexCoord2f(face._u[2], face._v[2]);
			glVertex3fv((GLfloat*)&v[face._vertexIndex[2]]);
		}
	}
	glEnd();
	// アルファテスト無効化
	glDisable(GL_ALPHA_TEST);

	// ポリゴンオフセット無効化
	glDisable(GL_POLYGON_OFFSET_FILL);

	// テクスチャをもとに戻す
	Texture::set(nullptr);

	// ディスプレイリスト記録終了
	glEndList();
}


int	sortFunc(const void* a, const void* b)
{
	int m1 = ((Model::Face*)a)->_material;
	int m2 = ((Model::Face*)b)->_material;

	if (m1 < m2) return -1;
	if (m1 > m2) return +1;
	return 0;
}

//---------------------------------------------------------------------------
//	最適化
//---------------------------------------------------------------------------
void Model::Object::optimize()
{
	std::qsort(_faces.data(), _faces.size(), sizeof(Model::Face), &sortFunc);
}