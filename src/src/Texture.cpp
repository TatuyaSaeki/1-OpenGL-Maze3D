//===========================================================================
//!
//!	@file	Texture.cpp
//!	@brief	テクスチャ
//!
//===========================================================================
#include "stdafx.h"
#include "Camera.h"
#include <locale.h>
#include <vector>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")        // リンク時に必要なライブラリ

#include <memory>

#pragma pack(push, 1)				// コンパイラーに変数の詰め込みを指示。パディング生成を抑制
struct HeaderTGA {
	u8		_id;
	u8		_colorMap;			//!< カラーマップ有無   0:なし 1:あり

	u8		_type;				//!< 画像形式
								//!<  0:イメージなし
								//!<  1:インデックスカラー(256色)
								//!<  2:フルカラー
								//!<  3:白黒
								//!<  9:(RLE圧縮)インデックスカラー(256色)
								//!< 10:(RLE圧縮)フルカラー
								//!< 11:(RLE圧縮)白黒
	u16		_colorMapIndex;
	u16		_colorMapLength;
	u8		_colorMapSize;

	u16		_x;
	u16		_y;
	u16		_width;				//!< 画像の幅
	u16		_height;			//!< 画像の高さ

	u8		_bpp;				//!< 色深度 (Bit per pixel) 8=256色 16:65536色 24:フルカラー 32:フルカラー+α

	u8		_attribute;			//!< 属性
								//!< bit0-3:属性
								//!< bit4  :格納方向 0:左から右  1:右から左
								//!< bit5  :格納方向 0:下から上  1:上から下
								//!< bit6,7:インターリーブ（使用不可）
};
#pragma pack(pop)

#if 0
struct Color
{
	Color() {}

	Color(u32 r, u32 g, u32 b, u32 a) : _r(r), _g(g), _b(b), _a(a) {}

	u8	_r;
	u8	_g;
	u8	_b;
	u8	_a;
};
#endif

//===========================================================================
//! 画像クラス
//===========================================================================
class Image
{
public:
	//! コンストラクタ
	Image();

	//! 初期化
	bool	setup(s32 w, s32 h);

	//! ピクセル参照
	Color&	pixel(s32 x, s32 y);

	//! 画像をUV座標で読み取り(バイリニアフィルタ)
	Color	fetch(f32 u, f32 v);

private:
	std::vector<Color>	_image;
	s32					_width;
	s32					_height;
};

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Image::Image()
	: _width(0)
	, _height(0)
{
}

//---------------------------------------------------------------------------
//! ピクセル参照
//---------------------------------------------------------------------------
Color& Image::pixel(s32 x, s32 y)
{
	return _image[y * _width + x];
}

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool Image::setup(s32 w, s32 h)
{
	_image.resize(w * h);

	_width = w;
	_height = h;
	return true;
}

//---------------------------------------------------------------------------
//! 画像をUV座標で読み取り(バイリニアフィルタ)
//---------------------------------------------------------------------------
Color Image::fetch(f32 u, f32 v)
{
	f32 fx = u * (f32)(_width - 1);
	f32 fy = v * (f32)(_height - 1);

	s32 x = (s32)fx;
	s32 y = (s32)fy;

	f32	s = fx - (f32)x;
	f32	t = fy - (f32)y;

	f32	r1, g1, b1, a1;
	f32	r2, g2, b2, a2;
	r1 = (f32)pixel(x, y)._r * (1.0f - s);
	g1 = (f32)pixel(x, y)._g * (1.0f - s);
	b1 = (f32)pixel(x, y)._b * (1.0f - s);
	a1 = (f32)pixel(x, y)._a * (1.0f - s);

	r1 += (f32)pixel(x + 1, y)._r * s;
	g1 += (f32)pixel(x + 1, y)._g * s;
	b1 += (f32)pixel(x + 1, y)._b * s;
	a1 += (f32)pixel(x + 1, y)._a * s;

	r2 = (f32)pixel(x, y + 1)._r * (1.0f - s);
	g2 = (f32)pixel(x, y + 1)._g * (1.0f - s);
	b2 = (f32)pixel(x, y + 1)._b * (1.0f - s);
	a2 = (f32)pixel(x, y + 1)._a * (1.0f - s);

	r2 += (f32)pixel(x + 1, y + 1)._r * s;
	g2 += (f32)pixel(x + 1, y + 1)._g * s;
	b2 += (f32)pixel(x + 1, y + 1)._b * s;
	a2 += (f32)pixel(x + 1, y + 1)._a * s;

	f32	fr = r1 * (1.0f - t) + r2 * t;
	f32	fg = g1 * (1.0f - t) + g2 * t;
	f32	fb = b1 * (1.0f - t) + b2 * t;
	f32	fa = a1 * (1.0f - t) + a2 * t;
	Color	color;
	color._r = max(0, min((s32)fr, 255));
	color._g = max(0, min((s32)fg, 255));
	color._b = max(0, min((s32)fb, 255));
	color._a = max(0, min((s32)fa, 255));

	return color;
}

// x より大きい最小の２のべき乗数を計算
s32	nextPow2(const u32 x)
{
	u32	count = 1;
	for (s32 valule = x; valule>0; valule >>= 1, count <<= 1);
	return  2 * x == count ? x : count;
}

class TGAStream
{
public:
	//! コンストラクタ
	TGAStream() {}

	//! デストラクタ
	virtual ~TGAStream() {}

	//! 1ピクセル読み込み
	virtual	Color	read() = 0;
};


//===========================================================================
//! 非圧縮データストリーム
//===========================================================================
class TGAStreamRAW : public TGAStream
{
public:
	TGAStreamRAW(FILE* fp) : _fp(fp) {}

	//! 1ピクセル読み込み
	virtual	Color	read()
	{
		u8	b = fgetc(_fp);
		u8	g = fgetc(_fp);
		u8	r = fgetc(_fp);
		u8	a = fgetc(_fp);

		return Color(r, g, b, a);
	}
public:

	FILE * _fp;
};

//===========================================================================
//! RLE圧縮データストリーム
//===========================================================================
class TGAStreamRLE : public TGAStream
{
public:
	enum STATE {
		STATE_UNKNOWN,
		STATE_COMPRESSED,
		STATE_UNCOMPRESSED
	};

	TGAStreamRLE(FILE* fp)
		: _count(0)
		, _state(STATE_UNKNOWN)
		, _color(0, 0, 0, 0)
		, _fp(fp)
	{}

	//! 1ピクセル読み込み
	virtual	Color	read()
	{
		// 繰り返しフラグ
		if (_count == 0) {
			u8	flagCount = fgetc(_fp);

			// 最上位ビットがセット(1のとき)の場合→「連続するデータの数」
			// 最上位ビットがセット(0のとき)の場合→「連続しないデータの数」
			if (flagCount & 0x80) {
				_state = STATE_COMPRESSED;

				u8	b = fgetc(_fp);
				u8	g = fgetc(_fp);
				u8	r = fgetc(_fp);
				u8	a = fgetc(_fp);

				_color = Color(r, g, b, a);
			}
			else {
				_state = STATE_UNCOMPRESSED;
			}
			_count = (flagCount & 127) + 1;
		}

		if (_state == STATE_UNCOMPRESSED) {
			u8	b = fgetc(_fp);
			u8	g = fgetc(_fp);
			u8	r = fgetc(_fp);
			u8	a = fgetc(_fp);

			_color = Color(r, g, b, a);
		}

		_count--;

		return _color;
	}
public:
	s32		_count;
	STATE	_state;
	Color	_color;

	FILE*	_fp;
};


//---------------------------------------------------------------------------
//! TGAファイルの読み込み
//---------------------------------------------------------------------------
bool Texture::loadTGA(char fileName[])
{
	HeaderTGA	header;

	FILE*	fp;
	if (fopen_s(&fp, fileName, "rb") != 0) {
		return false;
	}

	//-------------------------------------------------------------
	// ヘッダーの読み込み
	//-------------------------------------------------------------
	fread(&header, sizeof(header), 1, fp);

	if (header._bpp != 32) {
		MessageBox(nullptr, fileName, "32bitカラー形式ではありません.", MB_OK);
		return false;
	}

	s32	width = header._width;
	s32	height = header._height;

	//---- 画像クラスにイメージを読み込み
	// ここでは2の乗数になっている必要はない
	Image	image;
	image.setup(width, height);

	//-------------------------------------------------------------
	// 圧縮モード判定
	//-------------------------------------------------------------
	std::unique_ptr<TGAStream>	pStream;

	if (header._type & (1 << 3)) {
		// RLE圧縮
		pStream = std::unique_ptr<TGAStream>(new TGAStreamRLE(fp));
	}
	else {
		// 非圧縮
		pStream = std::unique_ptr<TGAStream>(new TGAStreamRAW(fp));
	}

	//-------------------------------------------------------------
	// イメージの取り出し
	//-------------------------------------------------------------
	if (header._attribute & (1 << 5)) {
		// 通常
		for (s32 y = 0; y<height; y++) {
			for (s32 x = 0; x<width; x++) {
				image.pixel(x, y) = pStream->read();
			}
		}
	}
	else {
		// 上下反転
		for (s32 y = height - 1; y >= 0; y--) {
			for (s32 x = 0; x<width; x++) {
				image.pixel(x, y) = pStream->read();
			}
		}
	}
	// ファイルアクセス終了
	fclose(fp);

	//---- OpenGLで画像の転送
	// OpenGLでは2の乗数のサイズではないテクスチャをサポートしていないため
	// リサイズを行う
	s32	alignedW = nextPow2(width);
	s32	alignedH = nextPow2(height);

	std::vector<Color>	alignedImage;
	alignedImage.resize(alignedW * alignedH);

	for (s32 y = 0; y<alignedH; y++) {
		for (s32 x = 0; x<alignedW; x++) {
			f32	u = (f32)x / (f32)alignedW;
			f32	v = (f32)y / (f32)alignedH;

			alignedImage[y * alignedW + x] = image.fetch(u, v);
		}
	}

	// 画像転送
	glTexImage2D(GL_TEXTURE_2D,		// テクスチャタイプ
		0,					// ミップマップ段数 (0で無効)
		GL_RGBA,			// 透明度あり
		alignedW,			// 幅
		alignedH,			// 高さ
		0,					// テクスチャボーダーON/OFF
		GL_RGBA,			// テクスチャのピクセル形式
		GL_UNSIGNED_BYTE,	// ピクセル1要素のサイズ
		&alignedImage[0]);			// 画像の場所

	return true;
}

bool Texture::loadFromFile(const char fileName[])
{
	// 文字コードをワイド文字列に変換
	// 【注意】本来はこの箇所は文字列バッファ長の考慮の他に文字列終端コードを処理するよりセキュアな対応が好ましいです。
	wchar_t     path[MAX_PATH];
	size_t      pathLength = 0;

	setlocale(LC_ALL, "jpn");
	if (mbstowcs_s(&pathLength,            // [out]    変換された文字数
		&path[0],               // [out]    変換されたワイド文字列を格納するバッファのアドレス(変換先)
		MAX_PATH,               // [in]     出力側のバッファのサイズ(単位:文字数)
		fileName,               // [in]     マルチバイト文字列のアドレス(変換元)
		_TRUNCATE) != 0) {    // [in]     出力先に格納するワイド文字の最大数
		return false;
	}

	//  GDI+オブジェクト（画像展開に必要）
	Gdiplus::GdiplusStartupInput    gdiplusStartupInput;
	ULONG_PTR                       gdiplusToken;

	//---- GDI+の初期設定
	if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) != Gdiplus::Ok) {
		return false;
	}

	//-------------------------------------------------------------
	// 画像の読み込み
	//-------------------------------------------------------------
	bool    result = false;
	int     width = 0;         // 画像の幅
	int     height = 0;         //  〃  高さ

	unsigned char*	pImage = nullptr;


	//--- 画像ファイルを開く
	//  【対応画像形式】  BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	Gdiplus::Bitmap*    pBitmap = Gdiplus::Bitmap::FromFile(path);
	if (pBitmap && pBitmap->GetLastStatus() == Gdiplus::Ok) {
		//---- 画像サイズ分の領域確保
		width = pBitmap->GetWidth();
		height = pBitmap->GetHeight();

		// ■ロードする画像のメモリの解像度を変更/設定（この位置に任意に記述して下さい）

		// サイズを保存しておく
		_width = width;
		_height = height;

		// 画像用にメモリ確保
		pImage = new unsigned char[width * height * 4];
		if (pImage) {
			result = true;
		}
	}

	//---- 画像イメージの読み込み
	if (result == true) {
		for (int y = 0; y<height; y++) {
			for (int x = 0; x<width; x++) {
				Gdiplus::Color  srcColor;
				pBitmap->GetPixel(x, y, &srcColor);

				unsigned char r = srcColor.GetR();
				unsigned char g = srcColor.GetG();
				unsigned char b = srcColor.GetB();
				unsigned char a = srcColor.GetA();

				// ■ピクセルへの描画（この位置に任意に記述して下さい）

				pImage[(y * width + x) * 4 + 0] = r;
				pImage[(y * width + x) * 4 + 1] = g;
				pImage[(y * width + x) * 4 + 2] = b;
				pImage[(y * width + x) * 4 + 3] = a;

			}
		}
	}

	//---- OpenGLで画像の転送
	glTexImage2D(GL_TEXTURE_2D,		// テクスチャタイプ
		0,					// ミップマップ段数 (0で無効)
		GL_RGBA,			// 透明度あり
		width,				// 幅
		height,			// 高さ
		0,					// テクスチャボーダーON/OFF
		GL_RGBA,			// テクスチャのピクセル形式
		GL_UNSIGNED_BYTE,	// ピクセル1要素のサイズ
		pImage);			// 画像の場所



	delete pImage;

	delete pBitmap;

	//---- GDI+の解放
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return result;
}


//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Texture::Texture()
	: _width(0)
	, _height(0)
	, _id(-1)
	, _uv1(Vector2(0.0f, 0.0f))
	, _uv2(Vector2(1.0f, 1.0f))
{
	_color = Color(255,255,255,255);
}

//---------------------------------------------------------------------------
//! 読み込み
//!	@param	[in]	fileName	画像ファイル名
//---------------------------------------------------------------------------
bool Texture::load(char fileName[])
{
	//-------------------------------------------------------------
	// (1) テクスチャIDを作成
	//-------------------------------------------------------------
	glGenTextures(1, &_id);

	//-------------------------------------------------------------
	// (2) IDをGPUに設定
	//-------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _id);

	//-------------------------------------------------------------
	// (3) テクスチャーのフィルター設定
	//-------------------------------------------------------------
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//-------------------------------------------------------------
	// (4) 画像イメージの転送
	//-------------------------------------------------------------
#if 0
	int	w = 16;
	int	h = 16;

	unsigned char	image[16 * 16][4];

	unsigned char*	pImage = &image[0][0];


	for (int y = 0; y<h; y++) {
		for (int x = 0; x<w; x++) {
			image[y * w + x][0] = rand();	// R
			image[y * w + x][1] = rand();	// G
			image[y * w + x][2] = rand();	// B
			image[y * w + x][3] = rand();	// A
		}
	}
#endif

	if (strstr(fileName, ".tga") || strstr(fileName, ".TGA")) {
		return loadTGA(fileName);
	}
	return loadFromFile(fileName);
}

//---------------------------------------------------------------------------
//! OpenGLのテクスチャIDを取得
//---------------------------------------------------------------------------
GLuint Texture::getTextureID() const
{
	return _id;
}

//---------------------------------------------------------------------------
//! 幅を取得
//---------------------------------------------------------------------------
int Texture::getWidth() const
{
	return _width;
}

//---------------------------------------------------------------------------
//! 高さを取得
//---------------------------------------------------------------------------
int Texture::getHeight() const
{
	return _height;
}



//---------------------------------------------------------------------------
//!	テクスチャの設定
//!	@param	[in]	pTexture	テクスチャのポインタ(nullptr指定でOFF)
//---------------------------------------------------------------------------
void Texture::set(const Texture* pTexture)
{
	static const Texture* pTextureLast = nullptr;

	if (pTexture == pTextureLast)	return;



	if (pTexture) {
		//---- テクスチャがある場合はテクスチャマッピングを有効にして設定
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pTexture->getTextureID());
	}
	else {
		//---- テクスチャがない場合はテクスチャマッピングを無効化
		glDisable(GL_TEXTURE_2D);
	}

	// 現在のテクスチャを覚えておく
	pTextureLast = pTexture;
}

#undef max
#undef min

//===========================================================================
//	END OF FILE
//===========================================================================
