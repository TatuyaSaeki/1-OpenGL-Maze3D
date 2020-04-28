//-----------------------------------------------------
//	DirectShowによる音声ファイル再生サンプル
//	2010.01.05 TMO
//-----------------------------------------------------

#pragma once

#include <dshow.h>

class Sound
{
	IGraphBuilder*	_pGraphBuilder;
	IMediaControl*	_pMediaControl;		//ファイルの読み込み、再生停止などを行う。
	IMediaPosition*	_pMediaPosition;	//再生位置を指定するのに使う。

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

	//ファイルの読み込み
	void loadFile(const BSTR& file);

	//再生 or 一時停止からの再開
	void play();

	//ループ再生
	void playRoop();
	void chkRoop();

	//再生中の場合でも先頭に戻して再生し直す
	void playFromStart();

	//一時停止
	void pause();

	//停止
	void stop();

	void volumeUp(int upVolume);
	void volumeDown(int downVolume);

	//リソース解放
	void cleanup();
};