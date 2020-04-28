


#pragma once

#include "Texture.h"

//===========================================================================
//!	���f��
//===========================================================================
class Model
{
public:
	// �ʏ��
	struct Face
	{
		s32		_vertexIndex[3];		//!< ���_�ԍ�
		s32		_material;				//!< �}�e���A���ԍ�(-1�̏ꍇ�̓}�e���A���Ȃ�)

		f32		_u[3];					//!< �e�N�X�`�����WU
		f32		_v[3];					//!< �e�N�X�`�����WV
	};

	// �p�[�c�I�u�W�F�N�g
	struct Object
	{
		std::string				_name;		//!< ���O
		std::vector<Vector3>	_vertices;	//!< ���_
		std::vector<Face>		_faces;		//!< �t�F�C�X

											//! �œK��
		void	optimize();
	};

	// �}�e���A��
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

	//! �R���X�g���N�^
	Model();

	//! �f�X�g���N�^
	~Model();

	//! �ǂݍ���
	bool	load(const char* fileName, f32 scale = 1.0f);

	//! �`��
	void	render();
	void	renderDisplayList();


	//! Object�̌����擾
	size_t	getObjectCount() const { return _objects.size(); }

	//! Object�̎擾
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

