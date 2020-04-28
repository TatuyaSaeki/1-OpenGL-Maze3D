//-----------------------------------------------------
//	DirectShow�ɂ�鉹���t�@�C���Đ��T���v��
//	2010.01.05 TMO
//-----------------------------------------------------

#include "stdafx.h"
#include "Sound.h"

#pragma comment(lib,"strmiids.lib") 

//�t�@�C���̓ǂݍ���
void Sound::loadFile(const BSTR& file)
{
	CoInitialize(NULL);
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&_pGraphBuilder);

	_pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID *)&_pMediaControl);
	_pGraphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID *)&_pMediaPosition);

	_pMediaControl->RenderFile(file);
}

//�Đ�
void Sound::play() {

	_pMediaControl->Run();
	_isPlay = true;
	_isRoopMode = false;
}

//���[�v�Đ�
void Sound::playRoop()
{
	_pMediaControl->Run();
	_isPlay = true;
	_isRoopMode = true;

	if (!_isPlay || !_isRoopMode) { return; }

	REFTIME pos, end;
	_pMediaPosition->get_CurrentPosition(&pos);
	_pMediaPosition->get_StopTime(&end);
	if (pos >= end) {

		//���̕��@�Ŗ߂��ƊJ�n�������r�؂�Ă��܂���
		_pMediaPosition->put_CurrentPosition(0);
		_pMediaControl->Run();

		//�����ł�������~�߂Ă���Đ�����
		//op();
		//ayRoop();
	}
}

void Sound::chkRoop()
{
	if (!_isPlay || !_isRoopMode) { return; }

	REFTIME pos, end;
	_pMediaPosition->get_CurrentPosition(&pos);
	_pMediaPosition->get_StopTime(&end);
	if (pos >= end) {

		//���̕��@�Ŗ߂��ƊJ�n�������r�؂�Ă��܂���
		//pMediaPosition->put_CurrentPosition(0);
		//pMediaControl->Run();

		//�����ł�������~�߂Ă���Đ�����
		stop();
		playRoop();
	}
}

//�Đ����̏ꍇ�ł��擪�ɖ߂��čĐ�������
void Sound::playFromStart()
{
	_pMediaPosition->put_CurrentPosition(0);
	_pMediaControl->Run();

	_isPlay = true;
}

//�ꎞ��~
void Sound::pause() {
	_pMediaControl->Pause();

	_isPlay = false;
}

//��~
void Sound::stop()
{
	_pMediaControl->Stop();
	//Stop()�ł��Đ��ʒu���擪�ɖ߂�Ȃ��̂ŁA�����Ŗ߂�
	_pMediaPosition->put_CurrentPosition(0);

	_isRoopMode = false;
	_isPlay = false;
}

//�{�����[���A�b�v
void Sound::volumeUp(int upVolume)
{
	IBasicAudio *pBasicAudio;
	_pGraphBuilder->QueryInterface(IID_IBasicAudio, (LPVOID *)&pBasicAudio);

	long vol = 0;
	pBasicAudio->get_Volume(&vol);
	vol += upVolume;
	if (vol > 0) {
		vol = 0;
	}
	pBasicAudio->put_Volume(vol);

	pBasicAudio->Release();
}

//�{�����[���_�E��
void Sound::volumeDown(int downVolume)
{
	IBasicAudio *pBasicAudio;
	_pGraphBuilder->QueryInterface(IID_IBasicAudio, (LPVOID *)&pBasicAudio);

	long vol = 0;
	pBasicAudio->get_Volume(&vol);
	vol -= downVolume;
	if (vol < -10000) {
		vol = -10000;
	}
	pBasicAudio->put_Volume(vol);

	pBasicAudio->Release();
}

//���\�[�X���
void Sound::cleanup()
{
	_pGraphBuilder->Release();
	_pMediaControl->Release();
	_pMediaPosition->Release();
}