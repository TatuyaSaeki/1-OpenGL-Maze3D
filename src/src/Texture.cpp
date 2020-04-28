//===========================================================================
//!
//!	@file	Texture.cpp
//!	@brief	�e�N�X�`��
//!
//===========================================================================
#include "stdafx.h"
#include "Camera.h"
#include <locale.h>
#include <vector>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")        // �����N���ɕK�v�ȃ��C�u����

#include <memory>

#pragma pack(push, 1)				// �R���p�C���[�ɕϐ��̋l�ߍ��݂��w���B�p�f�B���O������}��
struct HeaderTGA {
	u8		_id;
	u8		_colorMap;			//!< �J���[�}�b�v�L��   0:�Ȃ� 1:����

	u8		_type;				//!< �摜�`��
								//!<  0:�C���[�W�Ȃ�
								//!<  1:�C���f�b�N�X�J���[(256�F)
								//!<  2:�t���J���[
								//!<  3:����
								//!<  9:(RLE���k)�C���f�b�N�X�J���[(256�F)
								//!< 10:(RLE���k)�t���J���[
								//!< 11:(RLE���k)����
	u16		_colorMapIndex;
	u16		_colorMapLength;
	u8		_colorMapSize;

	u16		_x;
	u16		_y;
	u16		_width;				//!< �摜�̕�
	u16		_height;			//!< �摜�̍���

	u8		_bpp;				//!< �F�[�x (Bit per pixel) 8=256�F 16:65536�F 24:�t���J���[ 32:�t���J���[+��

	u8		_attribute;			//!< ����
								//!< bit0-3:����
								//!< bit4  :�i�[���� 0:������E  1:�E���獶
								//!< bit5  :�i�[���� 0:�������  1:�ォ�牺
								//!< bit6,7:�C���^�[���[�u�i�g�p�s�j
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
//! �摜�N���X
//===========================================================================
class Image
{
public:
	//! �R���X�g���N�^
	Image();

	//! ������
	bool	setup(s32 w, s32 h);

	//! �s�N�Z���Q��
	Color&	pixel(s32 x, s32 y);

	//! �摜��UV���W�œǂݎ��(�o�C���j�A�t�B���^)
	Color	fetch(f32 u, f32 v);

private:
	std::vector<Color>	_image;
	s32					_width;
	s32					_height;
};

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
Image::Image()
	: _width(0)
	, _height(0)
{
}

//---------------------------------------------------------------------------
//! �s�N�Z���Q��
//---------------------------------------------------------------------------
Color& Image::pixel(s32 x, s32 y)
{
	return _image[y * _width + x];
}

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool Image::setup(s32 w, s32 h)
{
	_image.resize(w * h);

	_width = w;
	_height = h;
	return true;
}

//---------------------------------------------------------------------------
//! �摜��UV���W�œǂݎ��(�o�C���j�A�t�B���^)
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

// x ���傫���ŏ��̂Q�ׂ̂��搔���v�Z
s32	nextPow2(const u32 x)
{
	u32	count = 1;
	for (s32 valule = x; valule>0; valule >>= 1, count <<= 1);
	return  2 * x == count ? x : count;
}

class TGAStream
{
public:
	//! �R���X�g���N�^
	TGAStream() {}

	//! �f�X�g���N�^
	virtual ~TGAStream() {}

	//! 1�s�N�Z���ǂݍ���
	virtual	Color	read() = 0;
};


//===========================================================================
//! �񈳏k�f�[�^�X�g���[��
//===========================================================================
class TGAStreamRAW : public TGAStream
{
public:
	TGAStreamRAW(FILE* fp) : _fp(fp) {}

	//! 1�s�N�Z���ǂݍ���
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
//! RLE���k�f�[�^�X�g���[��
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

	//! 1�s�N�Z���ǂݍ���
	virtual	Color	read()
	{
		// �J��Ԃ��t���O
		if (_count == 0) {
			u8	flagCount = fgetc(_fp);

			// �ŏ�ʃr�b�g���Z�b�g(1�̂Ƃ�)�̏ꍇ���u�A������f�[�^�̐��v
			// �ŏ�ʃr�b�g���Z�b�g(0�̂Ƃ�)�̏ꍇ���u�A�����Ȃ��f�[�^�̐��v
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
//! TGA�t�@�C���̓ǂݍ���
//---------------------------------------------------------------------------
bool Texture::loadTGA(char fileName[])
{
	HeaderTGA	header;

	FILE*	fp;
	if (fopen_s(&fp, fileName, "rb") != 0) {
		return false;
	}

	//-------------------------------------------------------------
	// �w�b�_�[�̓ǂݍ���
	//-------------------------------------------------------------
	fread(&header, sizeof(header), 1, fp);

	if (header._bpp != 32) {
		MessageBox(nullptr, fileName, "32bit�J���[�`���ł͂���܂���.", MB_OK);
		return false;
	}

	s32	width = header._width;
	s32	height = header._height;

	//---- �摜�N���X�ɃC���[�W��ǂݍ���
	// �����ł�2�̏搔�ɂȂ��Ă���K�v�͂Ȃ�
	Image	image;
	image.setup(width, height);

	//-------------------------------------------------------------
	// ���k���[�h����
	//-------------------------------------------------------------
	std::unique_ptr<TGAStream>	pStream;

	if (header._type & (1 << 3)) {
		// RLE���k
		pStream = std::unique_ptr<TGAStream>(new TGAStreamRLE(fp));
	}
	else {
		// �񈳏k
		pStream = std::unique_ptr<TGAStream>(new TGAStreamRAW(fp));
	}

	//-------------------------------------------------------------
	// �C���[�W�̎��o��
	//-------------------------------------------------------------
	if (header._attribute & (1 << 5)) {
		// �ʏ�
		for (s32 y = 0; y<height; y++) {
			for (s32 x = 0; x<width; x++) {
				image.pixel(x, y) = pStream->read();
			}
		}
	}
	else {
		// �㉺���]
		for (s32 y = height - 1; y >= 0; y--) {
			for (s32 x = 0; x<width; x++) {
				image.pixel(x, y) = pStream->read();
			}
		}
	}
	// �t�@�C���A�N�Z�X�I��
	fclose(fp);

	//---- OpenGL�ŉ摜�̓]��
	// OpenGL�ł�2�̏搔�̃T�C�Y�ł͂Ȃ��e�N�X�`�����T�|�[�g���Ă��Ȃ�����
	// ���T�C�Y���s��
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

	// �摜�]��
	glTexImage2D(GL_TEXTURE_2D,		// �e�N�X�`���^�C�v
		0,					// �~�b�v�}�b�v�i�� (0�Ŗ���)
		GL_RGBA,			// �����x����
		alignedW,			// ��
		alignedH,			// ����
		0,					// �e�N�X�`���{�[�_�[ON/OFF
		GL_RGBA,			// �e�N�X�`���̃s�N�Z���`��
		GL_UNSIGNED_BYTE,	// �s�N�Z��1�v�f�̃T�C�Y
		&alignedImage[0]);			// �摜�̏ꏊ

	return true;
}

bool Texture::loadFromFile(const char fileName[])
{
	// �����R�[�h�����C�h������ɕϊ�
	// �y���Ӂz�{���͂��̉ӏ��͕�����o�b�t�@���̍l���̑��ɕ�����I�[�R�[�h������������Z�L���A�ȑΉ����D�܂����ł��B
	wchar_t     path[MAX_PATH];
	size_t      pathLength = 0;

	setlocale(LC_ALL, "jpn");
	if (mbstowcs_s(&pathLength,            // [out]    �ϊ����ꂽ������
		&path[0],               // [out]    �ϊ����ꂽ���C�h��������i�[����o�b�t�@�̃A�h���X(�ϊ���)
		MAX_PATH,               // [in]     �o�͑��̃o�b�t�@�̃T�C�Y(�P��:������)
		fileName,               // [in]     �}���`�o�C�g������̃A�h���X(�ϊ���)
		_TRUNCATE) != 0) {    // [in]     �o�͐�Ɋi�[���郏�C�h�����̍ő吔
		return false;
	}

	//  GDI+�I�u�W�F�N�g�i�摜�W�J�ɕK�v�j
	Gdiplus::GdiplusStartupInput    gdiplusStartupInput;
	ULONG_PTR                       gdiplusToken;

	//---- GDI+�̏����ݒ�
	if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) != Gdiplus::Ok) {
		return false;
	}

	//-------------------------------------------------------------
	// �摜�̓ǂݍ���
	//-------------------------------------------------------------
	bool    result = false;
	int     width = 0;         // �摜�̕�
	int     height = 0;         //  �V  ����

	unsigned char*	pImage = nullptr;


	//--- �摜�t�@�C�����J��
	//  �y�Ή��摜�`���z  BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	Gdiplus::Bitmap*    pBitmap = Gdiplus::Bitmap::FromFile(path);
	if (pBitmap && pBitmap->GetLastStatus() == Gdiplus::Ok) {
		//---- �摜�T�C�Y���̗̈�m��
		width = pBitmap->GetWidth();
		height = pBitmap->GetHeight();

		// �����[�h����摜�̃������̉𑜓x��ύX/�ݒ�i���̈ʒu�ɔC�ӂɋL�q���ĉ������j

		// �T�C�Y��ۑ����Ă���
		_width = width;
		_height = height;

		// �摜�p�Ƀ������m��
		pImage = new unsigned char[width * height * 4];
		if (pImage) {
			result = true;
		}
	}

	//---- �摜�C���[�W�̓ǂݍ���
	if (result == true) {
		for (int y = 0; y<height; y++) {
			for (int x = 0; x<width; x++) {
				Gdiplus::Color  srcColor;
				pBitmap->GetPixel(x, y, &srcColor);

				unsigned char r = srcColor.GetR();
				unsigned char g = srcColor.GetG();
				unsigned char b = srcColor.GetB();
				unsigned char a = srcColor.GetA();

				// ���s�N�Z���ւ̕`��i���̈ʒu�ɔC�ӂɋL�q���ĉ������j

				pImage[(y * width + x) * 4 + 0] = r;
				pImage[(y * width + x) * 4 + 1] = g;
				pImage[(y * width + x) * 4 + 2] = b;
				pImage[(y * width + x) * 4 + 3] = a;

			}
		}
	}

	//---- OpenGL�ŉ摜�̓]��
	glTexImage2D(GL_TEXTURE_2D,		// �e�N�X�`���^�C�v
		0,					// �~�b�v�}�b�v�i�� (0�Ŗ���)
		GL_RGBA,			// �����x����
		width,				// ��
		height,			// ����
		0,					// �e�N�X�`���{�[�_�[ON/OFF
		GL_RGBA,			// �e�N�X�`���̃s�N�Z���`��
		GL_UNSIGNED_BYTE,	// �s�N�Z��1�v�f�̃T�C�Y
		pImage);			// �摜�̏ꏊ



	delete pImage;

	delete pBitmap;

	//---- GDI+�̉��
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return result;
}


//---------------------------------------------------------------------------
//! �R���X�g���N�^
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
//! �ǂݍ���
//!	@param	[in]	fileName	�摜�t�@�C����
//---------------------------------------------------------------------------
bool Texture::load(char fileName[])
{
	//-------------------------------------------------------------
	// (1) �e�N�X�`��ID���쐬
	//-------------------------------------------------------------
	glGenTextures(1, &_id);

	//-------------------------------------------------------------
	// (2) ID��GPU�ɐݒ�
	//-------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _id);

	//-------------------------------------------------------------
	// (3) �e�N�X�`���[�̃t�B���^�[�ݒ�
	//-------------------------------------------------------------
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//-------------------------------------------------------------
	// (4) �摜�C���[�W�̓]��
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
//! OpenGL�̃e�N�X�`��ID���擾
//---------------------------------------------------------------------------
GLuint Texture::getTextureID() const
{
	return _id;
}

//---------------------------------------------------------------------------
//! �����擾
//---------------------------------------------------------------------------
int Texture::getWidth() const
{
	return _width;
}

//---------------------------------------------------------------------------
//! �������擾
//---------------------------------------------------------------------------
int Texture::getHeight() const
{
	return _height;
}



//---------------------------------------------------------------------------
//!	�e�N�X�`���̐ݒ�
//!	@param	[in]	pTexture	�e�N�X�`���̃|�C���^(nullptr�w���OFF)
//---------------------------------------------------------------------------
void Texture::set(const Texture* pTexture)
{
	static const Texture* pTextureLast = nullptr;

	if (pTexture == pTextureLast)	return;



	if (pTexture) {
		//---- �e�N�X�`��������ꍇ�̓e�N�X�`���}�b�s���O��L���ɂ��Đݒ�
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pTexture->getTextureID());
	}
	else {
		//---- �e�N�X�`�����Ȃ��ꍇ�̓e�N�X�`���}�b�s���O�𖳌���
		glDisable(GL_TEXTURE_2D);
	}

	// ���݂̃e�N�X�`�����o���Ă���
	pTextureLast = pTexture;
}

#undef max
#undef min

//===========================================================================
//	END OF FILE
//===========================================================================
