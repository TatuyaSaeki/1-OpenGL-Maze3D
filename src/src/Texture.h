//===========================================================================
//!
//!	@file	Texture.h
//!	@brief	テクスチャ
//!
//===========================================================================
#pragma once

struct Vector2;
//===========================================================================
//! テクスチャ
//===========================================================================
class Texture
{
public:
	//! コンストラクタ
	Texture();

	//! 読み込み
	bool		load(char fileName[]);

	//! OpenGLのテクスチャIDを取得
	GLuint		getTextureID() const;

	//! 幅を取得
	int			getWidth() const;

	//! 高さを取得
	int			getHeight() const;

	//! テクスチャを設定
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

	//! TGAファイルの読み込み
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

	int		_width;		//!< 幅
	int		_height;	//!< 高さ

	Vector2 _uv1;
	Vector2 _uv2;

	Color   _color;

	GLuint	_id;		//!< テクスチャID
};

//===========================================================================
//	END OF FILE
//===========================================================================
