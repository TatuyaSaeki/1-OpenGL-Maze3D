//===========================================================================
//!
//!	@file	stdafx.h
//!	@brief	プリコンパイルヘッダー
//!
//!	標準のシステムインクルードファイルのインクルードファイル、又は
//!	参照回数が多く、かつあまり変更されないプロジェクト専用の
//!	インクルードファイルを記述します。
//!
//===========================================================================
#pragma once

//===========================================================================
// システム用ヘッダー
//===========================================================================
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
//--------------------------------------------------------------
// Windows ヘッダー ファイル:
//--------------------------------------------------------------
#include <SDKDDKVer.h>
#include <windows.h>

//--------------------------------------------------------------
// C ランタイム ヘッダー ファイル
//--------------------------------------------------------------
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <math.h>		// 算術演算ライブラリ
#include <float.h>


#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>



//--------------------------------------------------------------
//	OpenGL関連
//--------------------------------------------------------------
#include <GL/gl.h>			// OpenGL利用に必要
#include <GL/glu.h>			// OpenGLユーティリティーライブラリ glu*()
#include <GL/glut.h>			// OpenGL利用に必要

#pragma comment(lib, "opengl32.lib")	// OpenGL用ライブラリをリンク
#pragma comment(lib, "glu32.lib")		// glu関数用ライブラリをリンク

//===========================================================================
// プログラムに必要な追加ヘッダー
//===========================================================================
#include "Key.h"
#include "Sound.h"
#include "Typedef.h"
#include "Vector.h"
#include "Vector2.h"
#include "Matrix.h"
#include "Color.h"

#include "ObjectBase.h"
#include "CharacterBase.h"
#include "EnemyBase.h"
#include "MapObjectBase.h"
#include "WeaponBase.h"
#include "ItemBase.h"

#include "Camera.h"
#include "Shape.h"
#include "Collision.h"
#include "DrawFunction.h"

#include "Sword.h"


#include "Player.h"
#include "BossEnemy.h"
#include "NomalEnemy.h"
#include "KeyItem.h"
#include "DummyKeyItem.h"
#include "LastKeyItem.h"
#include "BossKeyItem.h"
#include "DropItem.h"

#include "EnemyManager.h"
#include "WeaponManager.h"
#include "MapObjectManager.h"
#include "ItemManager.h"
#include "CollisionManager.h"

#include "KeyInformation.h"

#include "SceneBase.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneBossBatlle.h"
#include "SceneResult.h"
#include "SceneGameClear.h"
#include "SceneBossBattleResult.h"
#include "SceneManager.h"


#include "OpenGL.h"

#include "Texture.h"
#include "Model.h"

#include "Main.h"
#include "Game.h"

//===========================================================================
//	END OF FILE
//===========================================================================
