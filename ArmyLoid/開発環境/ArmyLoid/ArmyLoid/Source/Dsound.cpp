#include "Dsound.h"
clsDirectSound::clsDirectSound()
	: pDS8(nullptr)
	, pDSBuffer(nullptr)
{

}

clsDirectSound::~clsDirectSound()
{
	DetatchData();
}

// Waveファイルオープン関数.
bool clsDirectSound::openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize)
{
	if (filepath == 0)
		return false;

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	// Waveファイルオープン.
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
	if (!hMmio)
		return false;

	// RIFFチャンク検索.
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}

	// フォーマットチャンク検索.
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) 
	{
		mmioClose(hMmio, 0);
		return false;
	}
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&waveFormatEx, fmsize);
	if (size != fmsize)
	{
		mmioClose(hMmio, 0);
		return false;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// データチャンク検索.
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) 
	{
		mmioClose(hMmio, 0);
		return false;
	}
	*ppData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)*ppData, dataChunk.cksize);
	if (size != dataChunk.cksize) 
	{
		delete[] * ppData;
		return false;
	}
	dataSize = size;

	mmioClose(hMmio, 0);

	return true;
}

HRESULT clsDirectSound::Init(HWND hWnd, char* filepass)
{
	// Waveファイルオープン.
	WAVEFORMATEX wFmt;
	char *pWaveData = 0;
	DWORD waveSize = 0;
	TCHAR pass[255];
	ZeroMemory(&pass[0], 255);

	memcpy(pass, filepass,sizeof(char)*strlen(filepass)+1);
	if (!openWave(pass, wFmt, &pWaveData, waveSize)) 
	{
		return 0;
	}

	// サウンドデバイス作成.
	DirectSoundCreate8(NULL, &pDS8, NULL);
	pDS8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

	DSBUFFERDESC DSBufferDesc;
	DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	DSBufferDesc.dwFlags = 0;
	DSBufferDesc.dwBufferBytes = waveSize;
	DSBufferDesc.dwReserved = 0;
	DSBufferDesc.lpwfxFormat = &wFmt;
	DSBufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer *ptmpBuf = 0;
	pDS8->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
	ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pDSBuffer);
	ptmpBuf->Release();
	if (pDSBuffer == 0) 
	{
		pDS8->Release();
		return 0;
	}

	// セカンダリバッファにWaveデータ書き込み.
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if (DS_OK == pDSBuffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) 
	{
		memcpy(lpvWrite, pWaveData, dwLength);
		pDSBuffer->Unlock(lpvWrite, dwLength, NULL, 0);
	}
	delete[] pWaveData;
}

void clsDirectSound::AttachData(clsDirectSound* pSound)
{
	if (pSound != NULL)
	{//音の情報譲渡.
		m_pStaticSound = pSound;
	}
}

void clsDirectSound::DetatchData()
{
	if (m_pStaticSound != NULL)
	{
		m_pStaticSound = NULL;
	}
}

void clsDirectSound::Play()
{
	// 再生.
	pDSBuffer->Play(0, 0, 0);
}

void clsDirectSound::Stop()
{
	//再生位置の巻き戻しも同時に行う.
	pDSBuffer->Stop();
	pDSBuffer->SetCurrentPosition(0);
}

void clsDirectSound::Delete()
{
	pDSBuffer->Release();
	pDS8->Release();
}



