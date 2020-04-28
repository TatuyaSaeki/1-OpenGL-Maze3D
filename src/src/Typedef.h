//===========================================================================
//!
//!	@file	Typedef.h
//!	@brief	型宣言
//!
//===========================================================================
#pragma once

//--------------------------------------------------------------
//	変数型定義
//--------------------------------------------------------------
typedef	  signed char		 s8;		//!< 符号あり  8bit整数
typedef	unsigned char		 u8;		//!< 符号なし  8bit整数
typedef	  signed short		s16;		//!< 符号あり 16bit整数
typedef	unsigned short		u16;		//!< 符号なし 16bit整数
typedef	  signed int		s32;		//!< 符号あり 32bit整数
typedef	unsigned int		u32;		//!< 符号なし 32bit整数
typedef	  signed long long	s64;		//!< 符号あり 64bit整数
typedef	unsigned long long	u64;		//!< 符号なし 64bit整数
typedef	float				f32;		//!< 単精度浮動小数点数
typedef	double				f64;		//!< 倍精度浮動小数点数

static const f32 PI = 3.14159265f;
static	f32	DEG_TO_RAD(f32 degree)
{
	return degree * (PI / 180.0f);
}

static	f32	RAD_TO_DEG(f32 radian)
{
	return radian * 180.0f / PI;
}

template<typename T>
void SAFE_DELETE(T*& p)
{
	if (p != nullptr) {
		delete p;
		p = nullptr;
	}
}

template<typename T>
void SAFE_DELETE_CLEANUP(T*& p)
{
	if (p != nullptr) {
		p->cleanup();
		delete p;
		p = nullptr;
	}
}

template<typename T>
int CHECK_ALPHA(T alpha)
{
	if(alpha > 255) return 255;
	if(alpha < 0)   return 0;
	
	return alpha;
}

// 0.0 ~ 1.0
inline f32 frand_0_1(void)
{
	static const f32 INV_MAX = 1.0f / 32767.0f;
	return static_cast<f32>(rand()) * INV_MAX;
}

// -1.0 ~ +1.0
inline f32 frand_1_1(void)
{
	return frand_0_1() * 2.0f - 1.0f;
}
//===========================================================================
//	END OF FILE
//===========================================================================
