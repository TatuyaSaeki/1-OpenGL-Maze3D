//===========================================================================
//!
//!	@file	OpenGL.cpp
//!	@brief	OpenGL����������
//!
//===========================================================================
#include "stdafx.h"

//---- �O���[�o���ϐ��i�O������J�j
static HWND		ghWnd = nullptr;		//!< �Ώۂ̃E�B���h�E�n���h��
static HDC		ghDC  = nullptr;		//!< �f�o�C�X�R���e�L�X�g
static HGLRC	ghRC  = nullptr;		//!< OpenGL���\�[�X�R���e�L�X�g

//---------------------------------------------------------------------------
//	OpenGL�̃s�N�Z���t�H�[�}�b�g��ݒ�
//!	@param	hDC		[in]	�f�B�X�v���C�f�o�C�X�R���e�L�X�g
//!	@retval	true	����I��	(����)
//!	@retval	false	�G���[�I��	(���s)
//---------------------------------------------------------------------------
static bool setupPixelFormatGL(HDC hDC)
{
	PIXELFORMATDESCRIPTOR desc = {
		sizeof(PIXELFORMATDESCRIPTOR),								// ���̍\���̂̃T�C�Y
		1,															// �\���̂̃o�[�W�����ԍ�(1)
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	// �����t���O(�_�u���o�b�t�@)
		PFD_TYPE_RGBA,												// �s�N�Z���̃J���[�f�[�^(RGBA�J���[ or �C���f�b�N�X�J���[)
		32,															// �J���[�̃r�b�g��
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,  0, 0, 0,
		32,															// �f�v�X�o�b�t�@�̃s�N�Z��������̃r�b�g��
		0,															// �X�e���V���o�b�t�@�̃s�N�Z��������̃r�b�g��
		0,
		PFD_MAIN_PLANE,												// ���C���[�^�C�v(Win32�ł�PFD_MAIN_PLANE�ł���K�v������)
		0,
		0, 0, 0
	};

	// ���݂̃R���e�L�X�g�Ƀs�N�Z���t�H�[�}�b�g��ݒ�
	u32		pixelFormat;
	if( (pixelFormat = ChoosePixelFormat(hDC, &desc) ) == 0 ) {
		MessageBox(nullptr, "�s�N�Z���t�H�[�}�b�g�̑I���Ɏ��s���܂���.", "SetupPixelFormat()", MB_OK);
		return false;
	}
	// �s�N�Z���t�H�[�}�b�g��ݒ�
    if( !SetPixelFormat(hDC, pixelFormat, &desc) ) {
		MessageBox(nullptr, "�s�N�Z���t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���.", "SetupPixelFormat()", MB_OK);
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
//!	OpenGL��������
//!	@param	[in]	hWnd	�Ώۂ̃E�B���h�E�n���h��
//!	@retval	true	����I��	(����)
//!	@retval	false	�G���[�I��	(���s)
//---------------------------------------------------------------------------
bool	OpenGL_setup(HWND hWnd)
{
	//-------------------------------------------------------------
	// �E�B���h�E�̃f�o�C�X�R���e�L�X�g���擾
	//-------------------------------------------------------------
	HDC		hDC = GetDC(hWnd);
	if( hDC == nullptr ) {		// �R���e�L�X�g�����ĂȂ��ꍇ�͎��s
		MessageBox(hWnd, "�f�o�C�X�R���e�L�X�g�擾�Ɏ��s���܂���.", "InitializeOpenGL()", MB_OK);
		return nullptr;
	}
	ghWnd = hWnd;
	ghDC  = hDC;

	//-------------------------------------------------------------
	// �s�N�Z���t�H�[�}�b�g��ݒ�
	//-------------------------------------------------------------
	if( setupPixelFormatGL(hDC) == false ) {
		return false;
	}

	//-------------------------------------------------------------
	// OpenGL���\�[�X�n���h���𐶐�
	//-------------------------------------------------------------
	HGLRC	hrc = wglCreateContext(hDC);
	if( hrc == 0 ) {			// OpenGL���\�[�X�n���h���𐶐��ł��Ȃ�������G���[
		MessageBox(hWnd, "OpenGL���\�[�X�n���h�������Ɏ��s���܂���.", "InitializeOpenGL()", MB_OK);
		return false;
	}
	ghRC = hrc;

	//---- OpenGL���\�[�X�ƃR���e�L�X�g���֘A�t��
	if( wglMakeCurrent(hDC, hrc) == false ) {
		MessageBox(hWnd, "OpenGL���\�[�X�ƃR���e�L�X�g�֘A�t���Ɏ��s���܂���.", "InitializeOpenGL()", MB_OK);
		return false;
	}

	//-------------------------------------------------------------
	// OpenGL�����ݒ�
	//-------------------------------------------------------------
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// ��ʃN���A�J���[�̐ݒ�

	glClearDepth(1.0f);						// �y�o�b�t�@�̏������l
	glEnable(GL_DEPTH_TEST);				// �y�o�b�t�@��L���ɂ���

	return true;
}

//---------------------------------------------------------------------------
//! OpenGL��ʍX�V
//---------------------------------------------------------------------------
void	OpenGL_swapBuffer()
{
	SwapBuffers(ghDC);
}

//---------------------------------------------------------------------------
//!	OpenGL�����
//!	@retval	true	����I��		(����)
//!	@retval	false	�G���[�I��	(���s)
//---------------------------------------------------------------------------
bool	OpenGL_cleanup()
{
	// ���\�[�X�ƃf�o�C�X�R���e�L�X�g�����
	wglMakeCurrent(0, 0);

	//-------------------------------------------------------------
	// �R���e�L�X�g���폜
	//-------------------------------------------------------------
	if( wglDeleteContext(ghRC) == false ) {
		MessageBox(ghWnd, "OpenGL�̉���Ɏ��s���܂���.", "cleanupOpenGL()", MB_OK);
		return false;
	}

	//---- �f�o�C�X�R���e�L�X�g���������
	ReleaseDC(ghWnd, ghDC);

	ghWnd = nullptr;
	ghDC  = nullptr;
	ghRC  = nullptr;

	return true;
}

//===========================================================================
//	END OF FILE
//===========================================================================
