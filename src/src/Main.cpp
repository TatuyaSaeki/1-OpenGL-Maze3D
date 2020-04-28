//===========================================================================
//!
//!	@file	Main.cpp
//!	@brief	�A�v���P�[�V�����J�n
//!
//===========================================================================
#include "stdafx.h"

#if _DEBUG
#	define _CRTDBG_MAP_ALLOC  
#	include <stdlib.h>  
#	include <crtdbg.h>  
#endif

//---------------------------------------------------------------------------
//!	�E�B���h�E�v���V�[�W��
//!	@param	[in]	hWnd	�Ώۂ̃E�B���h�E�n���h��
//!	@param	[in]	message	�E�B���h�E���b�Z�[�W
//!	@param	[in]	wparam	�p�����[�^�[����1
//!	@param	[in]	lparam	�p�����[�^�[����2
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch( message ) {
	//-------------------------------------------------------------
	case WM_PAINT:		// �ĕ`�惊�N�G�X�g
	//-------------------------------------------------------------
		{
			PAINTSTRUCT	ps;
			HDC			hDC = BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
		}
		return 0;
	//-------------------------------------------------------------
	case WM_DESTROY:	// �E�B���h�E�j��
	//-------------------------------------------------------------
		PostQuitMessage(0);
		return 0;
	//-------------------------------------------------------------
	default:			// ���̑�
	//-------------------------------------------------------------
		break;
	}
	//---- �f�t�H���g�̃E�B���h�E�v���V�[�W��
	return DefWindowProc(hWnd, message, wparam, lparam);
}

//---------------------------------------------------------------------------
//	���C�����[�v�J�n
//---------------------------------------------------------------------------
int mainLoop(const char* titleName, const char* className, s32 screenW, s32 screenH)
{
	//-------------------------------------------------------------
	// �E�B���h�E�N���X�o�^
	//-------------------------------------------------------------
	WNDCLASSEX			wcex;
	wcex.cbSize        = sizeof(WNDCLASSEX);			// �������g�̍\���̃T�C�Y
	wcex.style         = CS_HREDRAW | CS_VREDRAW |		// �����E�������T�C�Y���ɍĕ`��
					     CS_DBLCLKS |					// �_�u���N���b�N�L����
				         CS_OWNDC;						// DC�ێ�
	wcex.lpfnWndProc   = WndProc;						// �E�B���h�E�v���V�[�W���[
	wcex.cbClsExtra    = 0;								// (���0)
	wcex.cbWndExtra    = 0;								// (���0)
	wcex.hInstance     = GetModuleHandle(nullptr);		// �A�v���P�[�V�����C���X�^���X�n���h��
	wcex.hIcon         = nullptr;						// �A�C�R���摜
	wcex.hCursor       = LoadCursor(NULL, IDC_CROSS);	// �J�[�\���i�\���j
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// �w�i�F�i�u���V�j
	wcex.lpszMenuName  = nullptr;						// ���j���[���\�[�X��
	wcex.lpszClassName = className;						// �E�B���h�E�N���X��
	wcex.hIconSm       = nullptr;						// �������A�C�R���摜
	RegisterClassEx(&wcex);

	//-------------------------------------------------------------
	// �E�B���h�E�̈ʒu����ʒ����ɃZ���^�����O
	//-------------------------------------------------------------
#if 1
	//---- �E�B���h�E�\��
	SIZE	windowSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	u32		style = WS_VISIBLE | WS_POPUP;
	//u32 style = WS_VISIBLE;
	//u32		style = WS_OVERLAPPEDWINDOW;
	u32		styleEx = 0;
#else
	//---- �t���X�N���[���\��
	SIZE	windowSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	u32		style = WS_POPUP;
	u32		styleEx = 0;
#endif

	
	//---- �E�B���h�E�̑傫�����E�B���h�E�X�^�C���ɂ���ĕ␳
	RECT windowRect = { 0, 0, windowSize.cx, windowSize.cy };
	AdjustWindowRectEx(&windowRect, style, false, styleEx);

	//---- �E�B���h�E�\���ʒu���Z���^�����O
	s32		w = windowRect.right - windowRect.left;
	s32		h = windowRect.bottom - windowRect.top;
	s32		x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	s32		y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

	int a = w - SCREEN_WIDTH;
	int b = h - SCREEN_HEIGHT;

	SCREEN_WIDTH = w + a;
	SCREEN_HEIGHT = h + a;

	//-------------------------------------------------------------
	// �E�B���h�E�쐬
	//-------------------------------------------------------------
	HWND hWnd = CreateWindowEx(
		styleEx,					// �g���E�B���h�E�X�^�C��
		className,					// �E�B���h�E�N���X��
		titleName,					// �^�C�g���� (�C��)
		style,						// �E�B���h�E�X�^�C��
		x, y,						// X,Y���W
		SCREEN_WIDTH, SCREEN_HEIGHT,						// ��, ����
		nullptr,					// �e�E�B���h�E�̃n���h��
		nullptr,					// ���j���[�̃n���h��
		GetModuleHandle(nullptr),	// �A�v���P�[�V�����C���X�^���X�n���h��
		nullptr);					// WM_CREATE�ւ̈���(�C��)
									//---- �쐬���s�����ꍇ�͏I��
	if (hWnd == nullptr) {
		return 0;
	}

	

	//---- �����l�̓E�B���h�E�\��OFF��Ԃ̂��߁A������ON
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//=============================================================
	// [OpenGL] ������
	//=============================================================
	if (OpenGL_setup(hWnd) == false) {
		return 0;
	}

	// �����g���Ă��Ȃ�
	// �t�H���g�`��Ɏg���Ă���
	// ���͂ŏ����ׂ�
	int argc = 0;
	glutInit(&argc, NULL);

	// �}�E�X�J�[�\�����N���b�v���ĉ�ʊO�ł͌����Ȃ��悤�ɂ���
	// �s�v�ł���Ώ���
	RECT clipRect = { x + 10 , y + 2 , w + x - 10 , h + y - 10};
	ClipCursor(&clipRect);


	// �Q�[���{��
	// �Q�[���S�̂̐i�s�Ǘ����s���Ă���I�u�W�F�N�g
	GameManager game;

	//---- �y�Q�[���z������
	MSG	msg;
	if( game.init() ) {
		//-------------------------------------------------------------
		// ���C�����b�Z�[�W���[�v:
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
				//---- �y�Q�[���z�X�V����
				game.update();
				game.render();

				//=============================================================
				// [OpenGL]	��ʍX�V
				//=============================================================
				OpenGL_swapBuffer();
			}
		}
	}

	//---- �y�Q�[���z���
	game.cleanup();

	//=============================================================
	// [OpenGL]	���
	//=============================================================
	OpenGL_cleanup();

	return (int)msg.wParam;
}

//---------------------------------------------------------------------------
//	�A�v���P�[�V�����J�n�֐�
//---------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	const char*	titleName = "OpenGL 3D";	// �^�C�g���o�[�̃e�L�X�g
	const char*	className = "OpenGL";		// ���C���E�B���h�E�N���X��
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
