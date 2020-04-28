//===========================================================================
//!
//!	@file	stdafx.h
//!	@brief	�v���R���p�C���w�b�_�[
//!
//!	�W���̃V�X�e���C���N���[�h�t�@�C���̃C���N���[�h�t�@�C���A����
//!	�Q�Ɖ񐔂������A�����܂�ύX����Ȃ��v���W�F�N�g��p��
//!	�C���N���[�h�t�@�C�����L�q���܂��B
//!
//===========================================================================
#pragma once

//===========================================================================
// �V�X�e���p�w�b�_�[
//===========================================================================
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
//--------------------------------------------------------------
// Windows �w�b�_�[ �t�@�C��:
//--------------------------------------------------------------
#include <SDKDDKVer.h>
#include <windows.h>

//--------------------------------------------------------------
// C �����^�C�� �w�b�_�[ �t�@�C��
//--------------------------------------------------------------
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <math.h>		// �Z�p���Z���C�u����
#include <float.h>


#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>



//--------------------------------------------------------------
//	OpenGL�֘A
//--------------------------------------------------------------
#include <GL/gl.h>			// OpenGL���p�ɕK�v
#include <GL/glu.h>			// OpenGL���[�e�B���e�B�[���C�u���� glu*()
#include <GL/glut.h>			// OpenGL���p�ɕK�v

#pragma comment(lib, "opengl32.lib")	// OpenGL�p���C�u�����������N
#pragma comment(lib, "glu32.lib")		// glu�֐��p���C�u�����������N

//===========================================================================
// �v���O�����ɕK�v�Ȓǉ��w�b�_�[
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
