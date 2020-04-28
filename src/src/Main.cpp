//===========================================================================
//!
//!	@file	Main.cpp
//!	@brief	アプリケーション開始
//!
//===========================================================================
#include "stdafx.h"

#if _DEBUG
#	define _CRTDBG_MAP_ALLOC  
#	include <stdlib.h>  
#	include <crtdbg.h>  
#endif

//---------------------------------------------------------------------------
//!	ウィンドウプロシージャ
//!	@param	[in]	hWnd	対象のウィンドウハンドル
//!	@param	[in]	message	ウィンドウメッセージ
//!	@param	[in]	wparam	パラメーター引数1
//!	@param	[in]	lparam	パラメーター引数2
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch( message ) {
	//-------------------------------------------------------------
	case WM_PAINT:		// 再描画リクエスト
	//-------------------------------------------------------------
		{
			PAINTSTRUCT	ps;
			HDC			hDC = BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
		}
		return 0;
	//-------------------------------------------------------------
	case WM_DESTROY:	// ウィンドウ破棄
	//-------------------------------------------------------------
		PostQuitMessage(0);
		return 0;
	//-------------------------------------------------------------
	default:			// その他
	//-------------------------------------------------------------
		break;
	}
	//---- デフォルトのウィンドウプロシージャ
	return DefWindowProc(hWnd, message, wparam, lparam);
}

//---------------------------------------------------------------------------
//	メインループ開始
//---------------------------------------------------------------------------
int mainLoop(const char* titleName, const char* className, s32 screenW, s32 screenH)
{
	//-------------------------------------------------------------
	// ウィンドウクラス登録
	//-------------------------------------------------------------
	WNDCLASSEX			wcex;
	wcex.cbSize        = sizeof(WNDCLASSEX);			// 自分自身の構造体サイズ
	wcex.style         = CS_HREDRAW | CS_VREDRAW |		// 水平・垂直リサイズ時に再描画
					     CS_DBLCLKS |					// ダブルクリック有効化
				         CS_OWNDC;						// DC保持
	wcex.lpfnWndProc   = WndProc;						// ウィンドウプロシージャー
	wcex.cbClsExtra    = 0;								// (常に0)
	wcex.cbWndExtra    = 0;								// (常に0)
	wcex.hInstance     = GetModuleHandle(nullptr);		// アプリケーションインスタンスハンドル
	wcex.hIcon         = nullptr;						// アイコン画像
	wcex.hCursor       = LoadCursor(NULL, IDC_CROSS);	// カーソル（十字）
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// 背景色（ブラシ）
	wcex.lpszMenuName  = nullptr;						// メニューリソース名
	wcex.lpszClassName = className;						// ウィンドウクラス名
	wcex.hIconSm       = nullptr;						// 小さいアイコン画像
	RegisterClassEx(&wcex);

	//-------------------------------------------------------------
	// ウィンドウの位置を画面中央にセンタリング
	//-------------------------------------------------------------
#if 1
	//---- ウィンドウ表示
	SIZE	windowSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	u32		style = WS_VISIBLE | WS_POPUP;
	//u32 style = WS_VISIBLE;
	//u32		style = WS_OVERLAPPEDWINDOW;
	u32		styleEx = 0;
#else
	//---- フルスクリーン表示
	SIZE	windowSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	u32		style = WS_POPUP;
	u32		styleEx = 0;
#endif

	
	//---- ウィンドウの大きさをウィンドウスタイルによって補正
	RECT windowRect = { 0, 0, windowSize.cx, windowSize.cy };
	AdjustWindowRectEx(&windowRect, style, false, styleEx);

	//---- ウィンドウ表示位置をセンタリング
	s32		w = windowRect.right - windowRect.left;
	s32		h = windowRect.bottom - windowRect.top;
	s32		x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	s32		y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

	int a = w - SCREEN_WIDTH;
	int b = h - SCREEN_HEIGHT;

	SCREEN_WIDTH = w + a;
	SCREEN_HEIGHT = h + a;

	//-------------------------------------------------------------
	// ウィンドウ作成
	//-------------------------------------------------------------
	HWND hWnd = CreateWindowEx(
		styleEx,					// 拡張ウィンドウスタイル
		className,					// ウィンドウクラス名
		titleName,					// タイトル名 (任意)
		style,						// ウィンドウスタイル
		x, y,						// X,Y座標
		SCREEN_WIDTH, SCREEN_HEIGHT,						// 幅, 高さ
		nullptr,					// 親ウィンドウのハンドル
		nullptr,					// メニューのハンドル
		GetModuleHandle(nullptr),	// アプリケーションインスタンスハンドル
		nullptr);					// WM_CREATEへの引数(任意)
									//---- 作成失敗した場合は終了
	if (hWnd == nullptr) {
		return 0;
	}

	

	//---- 初期値はウィンドウ表示OFF状態のため、ここでON
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//=============================================================
	// [OpenGL] 初期化
	//=============================================================
	if (OpenGL_setup(hWnd) == false) {
		return 0;
	}

	// 多分使っていない
	// フォント描画に使っている
	// 自力で書くべし
	int argc = 0;
	glutInit(&argc, NULL);

	// マウスカーソルをクリップして画面外では見えないようにする
	// 不要であれば消す
	RECT clipRect = { x + 10 , y + 2 , w + x - 10 , h + y - 10};
	ClipCursor(&clipRect);


	// ゲーム本体
	// ゲーム全体の進行管理を行っているオブジェクト
	GameManager game;

	//---- 【ゲーム】初期化
	MSG	msg;
	if( game.init() ) {
		//-------------------------------------------------------------
		// メインメッセージループ:
		//-------------------------------------------------------------
		while (!game.isQuitGame()) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
				if (GetMessage(&msg, nullptr, 0, 0) == 0) {
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				//---- 【ゲーム】更新処理
				game.update();
				game.render();

				//=============================================================
				// [OpenGL]	画面更新
				//=============================================================
				OpenGL_swapBuffer();
			}
		}
	}

	//---- 【ゲーム】解放
	game.cleanup();

	//=============================================================
	// [OpenGL]	解放
	//=============================================================
	OpenGL_cleanup();

	return (int)msg.wParam;
}

//---------------------------------------------------------------------------
//	アプリケーション開始関数
//---------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	const char*	titleName = "OpenGL 3D";	// タイトルバーのテキスト
	const char*	className = "OpenGL";		// メインウィンドウクラス名
	int result = mainLoop(
		titleName,
		className,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	);

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return result;
}

//===========================================================================
// END OF FILE
//===========================================================================
