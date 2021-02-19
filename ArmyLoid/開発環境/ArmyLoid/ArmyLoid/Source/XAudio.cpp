#include "XAudio.h"

void XAudioSound::Init()
{
	//XAudio2�̏�����.
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

	//�}�X�^�����O�̐���.
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
	//Wave�t�H�[�}�b�g�̐ݒ�.
	format->wFormatTag = WAVE_FORMAT_PCM;
	format->nChannels = 1;                   //�`�����l����.
	format->wBitsPerSample = 16;             //�P�T���v��������̃r�b�g��.
	format->nSamplesPerSec = 44100;          //�T���v�����O���g��.
	format->nBlockAlign =
		format->wBitsPerSample / 8 * format->nChannels;
	format->nAvgBytesPerSec =
		format->nSamplesPerSec * format->nBlockAlign;
}

void XAudioSound::LoadWav()
{
	CWaveFile wav;

	//�}���`�o�C�g������(char*)�����C�h������(WCHAR*)�ɕϊ�.
	//�Ƃ肠�������󖼑O����.
	char* 	wstrC = "Data\Sound\SE\WAVE\nouto.wav";		//�ϊ��O������.
	//�ϊ�������i�[�o�b�t�@.
	WCHAR	wstrW[50];

	size_t wLen = 0;
	errno_t err = 0;

	//���P�[���w��.
	setlocale(LC_ALL, "japanese");
	//wchar�ɕϊ�.
	err = mbstowcs_s(&wLen, wstrW, 20, wstrC, _TRUNCATE);


	//LPWSTR��Wchar������t�@�C�����Ƃ��Ď󂯕t���Ă��Ȃ�.
	if (FAILED(wav.Open(wstrW, format, size)))
		throw "Open";

	//WAV�t�@�C����WAVEFORMATEX���g����SourceVoice���쐬.
	if (FAILED(XAudio->CreateSourceVoice(&Source_Voice, wav.GetFormat())))
		throw "CreateSourceVoice";

	//WAV�t�@�C���̃T�C�Y���̃o�b�t�@���쐬���A�f�[�^����������.
	size = wav.GetSize();
	std::vector< BYTE > data(size);
	if (FAILED(wav.Read(&data[0], size, &size)))
		throw "Read";


	//SourceVoice�Ƀf�[�^�𑗐M.
	XAUDIO2_BUFFER submit = { 0 };
	submit.AudioBytes = size;               //�o�b�t�@�̃o�C�g��.
	submit.pAudioData = &data[0];         //�o�b�t�@�̐擪�A�h���X.
	submit.Flags = XAUDIO2_END_OF_STREAM;
	Source_Voice->SubmitSourceBuffer(&submit);
}
