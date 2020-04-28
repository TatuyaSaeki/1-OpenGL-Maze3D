#include "stdafx.h"

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
Model::Model()
	: _displayListID((GLuint)-1)
	, _color(Color(255,255,255,255))
{

}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
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
//! �ǂݍ���
//---------------------------------------------------------------------------
bool Model::load(const char* fileName, f32 scale)
{
	//----------------------------------------------------------
	// �t�@�C�����J��
	//----------------------------------------------------------
	_fileName = fileName;
	FILE*	fp = fopen(fileName, "rt");
	if (fp == nullptr) {
		return false;
	}
	
	//----------------------------------------------------------
	// ��s���t�@�C���I�[�܂ŉ�͂��Ă���
	//----------------------------------------------------------
	enum class State {
		Root,
		Object,
		Vertex,
		Face,
		Material,
	};

	State	state = State::Root;	// ��Ԃ̏����l



	Object*	object = nullptr;

	char	str[1024];
	while (fgets(str, sizeof(str), fp)) {

		if(strncmp(str, "Object ""blade"" {",16) != 0) {
			int a = 0;
		}

		switch (state) {
		case State::Root:		//---- ���[�g��͒�
			if (strncmp(str, "Object ", 7) == 0) {

				object = new Object();

				// �I�u�W�F�N�g�������o�� Object "name"
				char*	p = strchr(str, '\"');
				p += 1;	// ��؂�L�����X�L�b�v

				char*	name = p;	// ���O�擪

				p = strchr(p, '\"');
				*p = 0;				// �����I�[

				object->_name = name;

				state = State::Object;
				break;
			}

			if (strncmp(str, "Material", 8) == 0) {
				state = State::Material;
				break;
			}

			break;
		case State::Object:		//---- �I�u�W�F�N�g��͒�
			if (strncmp(str, "}", 1) == 0) {

				// �œK��
				object->optimize();

				// �I�u�W�F�N�g���X�g�ɂȂ�
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
		case State::Vertex:		//---- ���_��͒�
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

		case State::Face:		//---- �t�F�C�X��͒�
			if (strstr(str, "}") != nullptr) {
				state = State::Object;
				break;
			}

			// �O�p�`���l�p�`�����`�F�b�N
			s32	vertexCount;		// 3 or 4
			sscanf(str, " %d", &vertexCount);

			if (vertexCount <= 2) {	// �s���Ȓl�̏ꍇ�̓X�L�b�v
				break;
			}

			Face	f;

			// �}�e���A���ԍ����擾
			f._material = -1;

			{
				char*	p = strstr(str, "M(");
				if (p != nullptr) {
					sscanf(p, "M(%d)", &f._material);
				}
			}

			if (vertexCount == 3) {
				// �O�p�`�̏ꍇ
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

				// �O�p�`�̓o�^
				object->_faces.push_back(f);
			}
			else {
				// �l�p�`�̏ꍇ
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

			// tex("")�\�������o
			char*	p = strstr(str, "tex(");
			if (p != nullptr) {
				// tex(" ��5�������i�߂�
				p += 5;

				// �e�N�X�`�����𒊏o
				char*	textureName = p;	// �擪��ۑ�
				while (*p != '\"') {
					p++;
				}

				*p = 0;

				// �e�N�X�`����ǂݍ���

				// MQO�t�@�C���̃p�X�w��ӏ��̃p�X���̂ݒ��o
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
					MessageBox(nullptr, textureFullPath, "�e�N�X�`���ǂݍ��݂Ɏ��s���܂���.", MB_OK);
					delete texture;
					texture = nullptr;
				}
			}

			// �}�e���A����o�^
			Material	m;
			m._texture = texture;

			_materials.push_back(m);
		}
		break;
		}

		//		MessageBox(nullptr, str, "�ǂݍ���", MB_OK);

	}

	//----------------------------------------------------------
	// �t�@�C�������
	//----------------------------------------------------------
	fclose(fp);

	return true;
}

void Model::wireFrame()
{
	// �A���t�@�e�X�g�L����
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 1.0f);	// ��r�����@A > 0.5�̂Ƃ��ɕ`�悷��
	glColor3ub(255, 255, 255);

	glBegin(GL_LINES);

	// �S�ẴI�u�W�F�N�g���i
	for (auto& o : _objects) {
		auto&	v = o->_vertices;	// ���_�z��  

									// �S�Ẵt�F�C�X
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

	// �A���t�@�e�X�g������
	glDisable(GL_ALPHA_TEST);

}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void Model::render()
{
	// ������
#if 0
	//-------------------------------------------------------------
	// 2��ڈȍ~�̓f�B�X�v���C���X�g�����s���ďI���
	//-------------------------------------------------------------
	if (_displayListID != (GLuint)-1) {

		glCallList(_displayListID);
		return;
	}

	// �f�B�X�v���C���C�X�g�𐶐����L�^�J�n
	_displayListID = glGenLists(1);

	glNewList(_displayListID, GL_COMPILE_AND_EXECUTE);
#endif

	//-------------------------------------------------------------
	// �_�Q�̕`��
	//-------------------------------------------------------------
#if 0
	glPointSize(3.0f);
	glColor3ub(255, 255, 255);

	glBegin(GL_POINTS);

	// �S�ẴI�u�W�F�N�g���i
	for (auto& o : _objects) {

		// �I�u�W�F�N�g���̂��ׂĂ̒��_
		for (auto& v : o->_vertices) {
			glVertex3fv((GLfloat*)&v);
		}
	}

	glEnd();

	glPointSize(1.0f);
#endif
	//-------------------------------------------------------------
	// ���C���[�t���[���̕`��
	//-------------------------------------------------------------

#if 0
	// �A���t�@�e�X�g�L����
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 1.0f);	// ��r�����@A > 0.5�̂Ƃ��ɕ`�悷��
	glColor3ub(255, 255, 255);

	glBegin(GL_LINES);

	// �S�ẴI�u�W�F�N�g���i
	for (auto& o : _objects) {
		auto&	v = o->_vertices;	// ���_�z��  

									// �S�Ẵt�F�C�X
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

	// �A���t�@�e�X�g������
	glDisable(GL_ALPHA_TEST);

#endif

	//glColor4ub(128, 128, 128, a);

	//-------------------------------------------------------------
	// �O�p�`�̕`��
	//-------------------------------------------------------------
	s32	material = -1;	// ���݂̃}�e���A���ԍ�
	Texture::set(nullptr);
	//Color c(128, 128, 128, a);
	glColor4ubv((GLubyte*)&_color);
	// �|���S���I�t�Z�b�g�@�\�Ńf�v�X�����炷
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);

	// �A���t�@�e�X�g�L����
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);	// ��r�����@A > 0.5�̂Ƃ��ɕ`�悷��

									// ����������ON
	glEnable(GL_BLEND);
	// �ʏ�u�����h
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	glBegin(GL_TRIANGLES);
	// �S�ẴI�u�W�F�N�g���i
	for (auto& o : _objects) {

		auto&	v = o->_vertices;	// ���_�z��  

									// �S�Ẵt�F�C�X
		for (auto& face : o->_faces) {

			// �}�e���A���̕ύX
			if (material != face._material) {

				glEnd();				//--- ��������`��R�}���h�����

										// �e�N�X�`���̐ݒ�
				if (face._material != -1) {
					// �e�N�X�`������
					Texture::set(_materials[face._material]._texture);
				}
				else {
					// �e�N�X�`���Ȃ�
					Texture::set(nullptr);
				}

				glBegin(GL_TRIANGLES);	//--- �ēx�J���Ȃ���

										// ���݂̔ԍ����X�V
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
	// �A���t�@�e�X�g������
	glDisable(GL_ALPHA_TEST);

	// �|���S���I�t�Z�b�g������
	glDisable(GL_POLYGON_OFFSET_FILL);

	// �e�N�X�`�������Ƃɖ߂�
	Texture::set(nullptr);

	// �f�B�X�v���C���X�g�L�^�I��
	glEndList();
}



//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void Model::renderDisplayList()
{
	// ������
	//-------------------------------------------------------------
	// 2��ڈȍ~�̓f�B�X�v���C���X�g�����s���ďI���
	//-------------------------------------------------------------
	if (_displayListID != (GLuint)-1) {

		glCallList(_displayListID);
		return;
	}

	// �f�B�X�v���C���C�X�g�𐶐����L�^�J�n
	_displayListID = glGenLists(1);

	glNewList(_displayListID, GL_COMPILE_AND_EXECUTE);

	//-------------------------------------------------------------
	// �_�Q�̕`��
	//-------------------------------------------------------------
#if 0
	glPointSize(3.0f);
	glColor3ub(255, 255, 255);

	glBegin(GL_POINTS);

	// �S�ẴI�u�W�F�N�g���i
	for (auto& o : _objects) {

		// �I�u�W�F�N�g���̂��ׂĂ̒��_
		for (auto& v : o->_vertices) {
			glVertex3fv((GLfloat*)&v);
		}
	}

	glEnd();

	glPointSize(1.0f);
#endif
	//-------------------------------------------------------------
	// ���C���[�t���[���̕`��
	//-------------------------------------------------------------
#if 0
	glBegin(GL_LINES);

	// �S�ẴI�u�W�F�N�g���i
	for (auto& o : _objects) {
		auto&	v = o->_vertices;	// ���_�z��  

									// �S�Ẵt�F�C�X
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
	// �O�p�`�̕`��
	//-------------------------------------------------------------
	s32	material = -1;	// ���݂̃}�e���A���ԍ�
	Texture::set(nullptr);

	// �|���S���I�t�Z�b�g�@�\�Ńf�v�X�����炷
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);

	// �A���t�@�e�X�g�L����
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);	// ��r�����@A > 0.5�̂Ƃ��ɕ`�悷��

	glBegin(GL_TRIANGLES);
	// �S�ẴI�u�W�F�N�g���i
	for (auto& o : _objects) {

		auto&	v = o->_vertices;	// ���_�z��  

									// �S�Ẵt�F�C�X
		for (auto& face : o->_faces) {

			// �}�e���A���̕ύX
			if (material != face._material) {

				glEnd();				//--- ��������`��R�}���h�����

										// �e�N�X�`���̐ݒ�
				if (face._material != -1) {
					// �e�N�X�`������
					Texture::set(_materials[face._material]._texture);
				}
				else {
					// �e�N�X�`���Ȃ�
					Texture::set(nullptr);
				}

				glBegin(GL_TRIANGLES);	//--- �ēx�J���Ȃ���

										// ���݂̔ԍ����X�V
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
	// �A���t�@�e�X�g������
	glDisable(GL_ALPHA_TEST);

	// �|���S���I�t�Z�b�g������
	glDisable(GL_POLYGON_OFFSET_FILL);

	// �e�N�X�`�������Ƃɖ߂�
	Texture::set(nullptr);

	// �f�B�X�v���C���X�g�L�^�I��
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
//	�œK��
//---------------------------------------------------------------------------
void Model::Object::optimize()
{
	std::qsort(_faces.data(), _faces.size(), sizeof(Model::Face), &sortFunc);
}