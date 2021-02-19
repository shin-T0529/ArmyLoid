#include "XAudio.h"

void XAudioSound::Init()
{
	//XAudio2の初期化.
	HRESULT hr;
	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		throw "CoInitializeEx";
	}

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
	if (FAILED(hr = XAudio2Create(&XAudio, flags)))
		throw "XAudio2Create";

	//マスタリングの生成.
	if (FAILED(hr = XAudio->CreateMasteringVoice(&Mastering_Voice)))
	{
		throw "CreateMasteringVoice";
	}
}

void XAudioSound::CleanUp()
{
	if (Mastering_Voice != 0)
	{
		Mastering_Voice->DestroyVoice();
		Mastering_Voice = 0;
	}
	if (XAudio != 0)
	{
		XAudio->Release();
		XAudio = 0;
	}
	CoUninitialize();
}

void XAudioSound::SettingWav()
{
	Init();

	HRESULT hr;
	format = { 0 };
	//Waveフォーマットの設定.
	format->wFormatTag = WAVE_FORMAT_PCM;
	format->nChannels = 1;                   //チャンネル数.
	format->wBitsPerSample = 16;             //１サンプルあたりのビット数.
	format->nSamplesPerSec = 44100;          //サンプリング周波数.
	format->nBlockAlign =
		format->wBitsPerSample / 8 * format->nChannels;
	format->nAvgBytesPerSec =
		format->nSamplesPerSec * format->nBlockAlign;
}

void XAudioSound::LoadWav()
{
	CWaveFile wav;

	//マルチバイト文字列(char*)をワイド文字列(WCHAR*)に変換.
	//とりあえず現状名前を代入.
	char* 	wstrC = "Data\Sound\SE\WAVE\nouto.wav";		//変換前文字列.
	//変換文字列格納バッファ.
	WCHAR	wstrW[50];

	size_t wLen = 0;
	errno_t err = 0;

	//ロケール指定.
	setlocale(LC_ALL, "japanese");
	//wcharに変換.
	err = mbstowcs_s(&wLen, wstrW, 20, wstrC, _TRUNCATE);


	//LPWSTRはWcharだからファイル名として受け付けていない.
	if (FAILED(wav.Open(wstrW, format, size)))
		throw "Open";

	//WAVファイルのWAVEFORMATEXを使ってSourceVoiceを作成.
	if (FAILED(XAudio->CreateSourceVoice(&Source_Voice, wav.GetFormat())))
		throw "CreateSourceVoice";

	//WAVファイルのサイズ分のバッファを作成し、データを書き込む.
	size = wav.GetSize();
	std::vector< BYTE > data(size);
	if (FAILED(wav.Read(&data[0], size, &size)))
		throw "Read";


	//SourceVoiceにデータを送信.
	XAUDIO2_BUFFER submit = { 0 };
	submit.AudioBytes = size;               //バッファのバイト数.
	submit.pAudioData = &data[0];         //バッファの先頭アドレス.
	submit.Flags = XAUDIO2_END_OF_STREAM;
	Source_Voice->SubmitSourceBuffer(&submit);
}
