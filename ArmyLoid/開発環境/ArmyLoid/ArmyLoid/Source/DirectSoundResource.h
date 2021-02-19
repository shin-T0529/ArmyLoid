#ifndef DIRECTSOUNDRESOURCE_H
#define DIRECTSOUNDRESOURCE_H

#include "DSound.h"
#include "C_LoadData.h"
enum StaticSound
{
	//���̃V�[����BGM�����̃V�[����SE(�ȉ����[�v)�Ő錾����.
	enStaticBGM_Title,

	enStaticSE_Ready,			//���艹�����˂Ă���.
	enStaticSE_Start,

	enStaticBGM_Manual,

	enStaticBGM_Play,
	enStaticSE_Gatling,
	enStaticSE_Missile,
	enStaticSE_Caution,			//HP�̒ʒm��.
	enStaticSE_Danger,
	enStaticSE_TimeCount,		//��������10�b�O�̃J�E���g�_�E��.
	enStaticSE_BattleStart,
	enStaticSE_Hit,				//��e.
	enStaticSE_BladeHit,		//�ߐڔ�e.
	enStaticSE_BladeNo1Hit,		//���Ԃ肪�C���ł��Ȃ����ߓ�p�ӂ���.
	enStaticSE_BladeNo2Hit,
	enStaticSE_GuardHit,
	enStaticSE_WallHit,

	enStaticBGM_ResultWin,		//1P,2P���p.
	enStaticBGM_Draw,
	enStaticSE_Select,			//�I����(���p����).

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
