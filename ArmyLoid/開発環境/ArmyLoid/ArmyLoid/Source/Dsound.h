#ifndef DIRECT_SOUND_H
#define DIRECT_SOUND_H

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>

#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )


class clsDirectSound
{
public:
	clsDirectSound();
	~clsDirectSound();
	HRESULT Init(HWND hWnd,char* filepass);
	void Play();
	void Stop();
	void Delete();

	void AttachData(clsDirectSound* pSound);
	void DetatchData();

	void ChangeDSB(clsDirectSound* pSound)
	{//基礎情報のバッファ情報のみ入れ替え.
		pSound->pDS8 = m_pStaticSound->pDS8;
		pSound->pDSBuffer = m_pStaticSound->pDSBuffer;
	}

protected:
	IDirectSound8 *pDS8;
	IDirectSoundBuffer8 *pDSBuffer;

private:

	clsDirectSound*	m_pStaticSound;

	bool openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx,
				  char** ppData, DWORD &dataSize);
};







#endif