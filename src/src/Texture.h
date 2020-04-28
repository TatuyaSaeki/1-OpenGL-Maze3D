//===========================================================================
//!
//!	@file	Texture.h
//!	@brief	�e�N�X�`��
//!
//===========================================================================
#pragma once

struct Vector2;
//===========================================================================
//! �e�N�X�`��
//===========================================================================
class Texture
{
public:
	//! �R���X�g���N�^
	Texture();

	//! �ǂݍ���
	bool		load(char fileName[]);

	//! OpenGL�̃e�N�X�`��ID���擾
	GLuint		getTextureID() const;

	//! �����擾
	int			getWidth() const;

	//! �������擾
	int			getHeight() const;

	//! �e�N�X�`����ݒ�
	static void set(const Texture* pTexture);

	void setAlpha(unsigned int a)
	{
		_color._a = a & 0xFF;
	}

	void addAlpha(int a)
	{
		_color._a = std::min( 255, std::max(0, _color._a + a) );
		//_color._a = std::clamp( _color._a + a, 0, 255 );
	}

	Color getColor()
	{
		return _color;
	}

	void setColor(const Color& color)
	{
		_color = color;
	}

	//! TGA�t�@�C���̓ǂݍ���
	bool		loadTGA(char fileName[]);

	void setTextureUV(const Vector2& uv1, const Vector2& uv2)
	{
		_uv1 = uv1;
		_uv2 = uv2;
	}

	Vector2 getTextureUV1() const
	{
		return _uv1;
	}

	Vector2 getTextureUV2() const
	{
		return _uv2;
	}


private:
	bool		loadFromFile(const char fileName[]);


private:

	int		_width;		//!< ��
	int		_height;	//!< ����

	Vector2 _uv1;
	Vector2 _uv2;

	Color   _color;

	GLuint	_id;		//!< �e�N�X�`��ID
};

//===========================================================================
//	END OF FILE
//===========================================================================
