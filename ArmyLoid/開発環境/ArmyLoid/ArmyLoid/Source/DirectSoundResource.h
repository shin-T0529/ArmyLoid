#ifndef DIRECTSOUNDRESOURCE_H
#define DIRECTSOUNDRESOURCE_H

#include "DSound.h"
#include "C_LoadData.h"
enum StaticSound
{
	//そのシーンのBGM→そのシーンのSE(以下ループ)で宣言する.
	enStaticBGM_Title,

	enStaticSE_Ready,			//決定音も兼ねている.
	enStaticSE_Start,

	enStaticBGM_Manual,

	enStaticBGM_Play,
	enStaticSE_Gatling,
	enStaticSE_Missile,
	enStaticSE_Caution,			//HPの通知音.
	enStaticSE_Danger,
	enStaticSE_TimeCount,		//制限時間10秒前のカウントダウン.
	enStaticSE_BattleStart,
	enStaticSE_Hit,				//被弾.
	enStaticSE_BladeHit,		//近接被弾.
	enStaticSE_BladeNo1Hit,		//かぶりが修正できないため二つ用意する.
	enStaticSE_BladeNo2Hit,
	enStaticSE_GuardHit,
	enStaticSE_WallHit,

	enStaticBGM_ResultWin,		//1P,2P併用.
	enStaticBGM_Draw,
	enStaticSE_Select,			//選択音(併用する).

	enStaticBGM_CreditBGM,

	enStaticSound_Max,
	enStaticSound_Min = 0,
};
class clsSoundResource
{
public:
	clsSoundResource();
	~clsSoundResource();
	HRESULT Init(HWND hWnd);
	HRESULT CreateSound(int enSound, char* FilePass);
	HRESULT ReleaseSound(int enSound);
	HRESULT LoadSoundDataAll();
	clsDirectSound*	GetSound(int enSound);

	bool CheckRangeStaticSound(int enSound);


private:
	HWND				m_hWnd;

	clsLoadData*		LoadData;
	clsDirectSound**	m_ppStaticSound;
	int					isound;
};

#endif // !DIRECTSOUNDRESOURCE_H
