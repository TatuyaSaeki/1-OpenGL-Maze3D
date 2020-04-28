//-----------------------------------------------------
//	DirectShow�ɂ�鉹���t�@�C���Đ��T���v��
//	2010.01.05 TMO
//-----------------------------------------------------

#pragma once

#include <dshow.h>

class Sound
{
	IGraphBuilder*	_pGraphBuilder;
	IMediaControl*	_pMediaControl;		//�t�@�C���̓ǂݍ��݁A�Đ���~�Ȃǂ��s���B
	IMediaPosition*	_pMediaPosition;	//�Đ��ʒu���w�肷��̂Ɏg���B

	bool _isPlay;
	bool _isRoopMode;

public:
	Sound()
	{
		_pGraphBuilder = nullptr;
		_pMediaControl = nullptr;
		_pMediaPosition = nullptr;
		
		_isPlay = false;
		_isRoopMode = false;
	}
	~Sound() {  }

	//�t�@�C���̓ǂݍ���
	void loadFile(const BSTR& file);

	//�Đ� or �ꎞ��~����̍ĊJ
	void play();

	//���[�v�Đ�
	void playRoop();
	void chkRoop();

	//�Đ����̏ꍇ�ł��擪�ɖ߂��čĐ�������
	void playFromStart();

	//�ꎞ��~
	void pause();

	//��~
	void stop();

	void volumeUp(int upVolume);
	void volumeDown(int downVolume);

	//���\�[�X���
	void cleanup();
};