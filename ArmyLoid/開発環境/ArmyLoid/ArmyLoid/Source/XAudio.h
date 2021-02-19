#ifndef XADUIO_H
#define XAUDIO_H

#include <windows.h>
#include <xaudio2.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "SDKwavefile.h"

class XAudioSound
{
public:
	void Init();			//‰Šú‰».
	void CleanUp();			//”jŠü(‰ğ•ú).

	void SettingWav();

	void LoadWav();
private:
	IXAudio2*					XAudio;
	IXAudio2MasteringVoice*		Mastering_Voice;
	IXAudio2SourceVoice*		Source_Voice;
	WAVEFORMATEX*				format;
	DWORD						size;

}; 

#endif