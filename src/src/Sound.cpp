//-----------------------------------------------------
//	DirectShowによる音声ファイル再生サンプル
//	2010.01.05 TMO
//-----------------------------------------------------

#include "stdafx.h"
#include "Sound.h"

#pragma comment(lib,"strmiids.lib") 

//ファイルの読み込み
void Sound::loadFile(const BSTR& file)
{
	CoInitialize(NULL);
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&_pGraphBuilder);

	_pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID *)&_pMediaControl);
	_pGraphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID *)&_pMediaPosition);

	_pMediaControl->RenderFile(file);
}

//再生
void Sound::play() {

	_pMediaControl->Run();
	_isPlay = true;
	_isRoopMode = false;
}

//ループ再生
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

		//この方法で戻すと開始が少し途切れてしまった
		_pMediaPosition->put_CurrentPosition(0);
		_pMediaControl->Run();

		//そこでいったん止めてから再生する
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

		//この方法で戻すと開始が少し途切れてしまった
		//pMediaPosition->put_CurrentPosition(0);
		//pMediaControl->Run();

		//そこでいったん止めてから再生する
		stop();
		playRoop();
	}
}

//再生中の場合でも先頭に戻して再生し直す
void Sound::playFromStart()
{
	_pMediaPosition->put_CurrentPosition(0);
	_pMediaControl->Run();

	_isPlay = true;
}

//一時停止
void Sound::pause() {
	_pMediaControl->Pause();

	_isPlay = false;
}

//停止
void Sound::stop()
{
	_pMediaControl->Stop();
	//Stop()でも再生位置が先頭に戻らないので、自分で戻す
	_pMediaPosition->put_CurrentPosition(0);

	_isRoopMode = false;
	_isPlay = false;
}

//ボリュームアップ
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

//ボリュームダウン
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

//リソース解放
void Sound::cleanup()
{
	_pGraphBuilder->Release();
	_pMediaControl->Release();
	_pMediaPosition->Release();
}