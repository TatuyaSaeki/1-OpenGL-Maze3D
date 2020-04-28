//===========================================================================
//!
//!	@file	OpenGL.cpp
//!	@brief	OpenGL初期化処理
//!
//===========================================================================
#include "stdafx.h"

//---- グローバル変数（外部非公開）
static HWND		ghWnd = nullptr;		//!< 対象のウィンドウハンドル
static HDC		ghDC  = nullptr;		//!< デバイスコンテキスト
static HGLRC	ghRC  = nullptr;		//!< OpenGLリソースコンテキスト

//---------------------------------------------------------------------------
//	OpenGLのピクセルフォーマットを設定
//!	@param	hDC		[in]	ディスプレイデバイスコンテキスト
//!	@retval	true	正常終了	(成功)
//!	@retval	false	エラー終了	(失敗)
//---------------------------------------------------------------------------
static bool setupPixelFormatGL(HDC hDC)
{
	PIXELFORMATDESCRIPTOR desc = {
		sizeof(PIXELFORMATDESCRIPTOR),								// この構造体のサイズ
		1,															// 構造体のバージョン番号(1)
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	// 特性フラグ(ダブルバッファ)
		PFD_TYPE_RGBA,												// ピクセルのカラーデータ(RGBAカラー or インデックスカラー)
		32,															// カラーのビット数
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,  0, 0, 0,
		32,															// デプスバッファのピクセルあたりのビット数
		0,															// ステンシルバッファのピクセルあたりのビット数
		0,
		PFD_MAIN_PLANE,												// レイヤータイプ(Win32ではPFD_MAIN_PLANEである必要がある)
		0,
		0, 0, 0
	};

	// 現在のコンテキストにピクセルフォーマットを設定
	u32		pixelFormat;
	if( (pixelFormat = ChoosePixelFormat(hDC, &desc) ) == 0 ) {
		MessageBox(nullptr, "ピクセルフォーマットの選択に失敗しました.", "SetupPixelFormat()", MB_OK);
		return false;
	}
	// ピクセルフォーマットを設定
    if( !SetPixelFormat(hDC, pixelFormat, &desc) ) {
		MessageBox(nullptr, "ピクセルフォーマットの設定に失敗しました.", "SetupPixelFormat()", MB_OK);
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
//!	OpenGLを初期化
//!	@param	[in]	hWnd	対象のウィンドウハンドル
//!	@retval	true	正常終了	(成功)
//!	@retval	false	エラー終了	(失敗)
//---------------------------------------------------------------------------
bool	OpenGL_setup(HWND hWnd)
{
	//-------------------------------------------------------------
	// ウィンドウのデバイスコンテキストを取得
	//-------------------------------------------------------------
	HDC		hDC = GetDC(hWnd);
	if( hDC == nullptr ) {		// コンテキストが取れてない場合は失敗
		MessageBox(hWnd, "デバイスコンテキスト取得に失敗しました.", "InitializeOpenGL()", MB_OK);
		return nullptr;
	}
	ghWnd = hWnd;
	ghDC  = hDC;

	//-------------------------------------------------------------
	// ピクセルフォーマットを設定
	//-------------------------------------------------------------
	if( setupPixelFormatGL(hDC) == false ) {
		return false;
	}

	//-------------------------------------------------------------
	// OpenGLリソースハンドラを生成
	//-------------------------------------------------------------
	HGLRC	hrc = wglCreateContext(hDC);
	if( hrc == 0 ) {			// OpenGLリソースハンドラを生成できなかったらエラー
		MessageBox(hWnd, "OpenGLリソースハンドラ生成に失敗しました.", "InitializeOpenGL()", MB_OK);
		return false;
	}
	ghRC = hrc;

	//---- OpenGLリソースとコンテキストを関連付け
	if( wglMakeCurrent(hDC, hrc) == false ) {
		MessageBox(hWnd, "OpenGLリソースとコンテキスト関連付けに失敗しました.", "InitializeOpenGL()", MB_OK);
		return false;
	}

	//-------------------------------------------------------------
	// OpenGL初期設定
	//-------------------------------------------------------------
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// 画面クリアカラーの設定

	glClearDepth(1.0f);						// Ｚバッファの初期化値
	glEnable(GL_DEPTH_TEST);				// Ｚバッファを有効にする

	return true;
}

//---------------------------------------------------------------------------
//! OpenGL画面更新
//---------------------------------------------------------------------------
void	OpenGL_swapBuffer()
{
	SwapBuffers(ghDC);
}

//---------------------------------------------------------------------------
//!	OpenGLを解放
//!	@retval	true	正常終了		(成功)
//!	@retval	false	エラー終了	(失敗)
//---------------------------------------------------------------------------
bool	OpenGL_cleanup()
{
	// リソースとデバイスコンテキストを解放
	wglMakeCurrent(0, 0);

	//-------------------------------------------------------------
	// コンテキストを削除
	//-------------------------------------------------------------
	if( wglDeleteContext(ghRC) == false ) {
		MessageBox(ghWnd, "OpenGLの解放に失敗しました.", "cleanupOpenGL()", MB_OK);
		return false;
	}

	//---- デバイスコンテキストを解放する
	ReleaseDC(ghWnd, ghDC);

	ghWnd = nullptr;
	ghDC  = nullptr;
	ghRC  = nullptr;

	return true;
}

//===========================================================================
//	END OF FILE
//===========================================================================
