#ifndef PLAYER_H
#define PLAYER_H

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include "Global.h"
#include "DX9Mesh.h"
#include "ED3DXSKINMESH.h"
#include "Charactor.h"
#include "TwinDInput.h"				//�Q�[���̍��㑀��֐���2����Ă���.
//#include "DInput.h"
#include "DebugText.h"

const int	B_Max = 11;
const int	B_Dir = 10;				//�e�̐�������]�l��ݒ肷��.
const int	M_Dir = 5;				

enum CharaRotaitonVectle
{
	AnimSetNum		= 0,			//�f�t�H���g.
	AnimSetFlont	= 1,
	AnimSetBack		= 2,
	AnimSetLeft		= 3,
	AnimSetRight	= 4,
	AnimSetAttack   = 5,			//�A�j���[�V�����Đ��D��x�ݒ�p.
	AnimSetRotMax,					//�����ő吔.
};

class clsPlayer : public clsChara
{
public:
	clsPlayer();
	~clsPlayer();

	void Init();

	//�e����.
	void Action();
	void Action2();
	void Camera(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float yaw);

	//////////////�L�����ړ�////////////////
	//�ړ�.
	void Move(clsDInput* Pad,int PadNo, int inActState);
	//�����擾�A�ݒ�.
	void PlayerFlontSet(int &ActSet);
	//--------------------�ˌ�---------------------------.
	//�e�̍��W�E�t���O������.
	void BulletInit(int Max);	
	void Bullet2Init(int Max);
	void MissileInit(int Max);
	void Missile2Init(int Max);

	//�e�̏���.
	void ShotProc(bool flag);
	void Shot2Proc(bool flag);
	void M_ShotProc(bool flag);
	void M_Shot2Proc(bool flag);
	//�e�̔��ˏ�������.
	void ShotStartProc();
	void ShotStartProc2();

	//�e�̕����ݒ�.
	void RotationProc(D3DXVECTOR3 &vPos, D3DXVECTOR3 vectle,int Dir);	//��΂���.
	//���������[�h����.
	void ReloadProc(clsPlayer* Player, bool ReloadFlg, int &Shot);
	void Reload2Proc(clsPlayer* Player, bool ReloadFlg, int &Shot);
	void M_ReloadProc(clsPlayer* Player, bool ReloadFlg, int &Shot);
	void M_Reload2Proc(clsPlayer* Player, bool ReloadFlg, int &Shot);
	//�f�B���C�ݒ�.
	void DelayProc(int &DelayCnt, int ResetNo,bool &ShotFlag);
	
	/**********************�Ō�*************************/
	//�U������.
	void SlashProc(clsPlayer* Enemy);

	//�U���J�n����.
	void SlashStartProc(clsPlayer* Enemy);
	void SlashStartProc2(clsPlayer* Enemy);
	//�ߐڍU�����L�����Z���v�f����.
	void SlashChancelProc();
	//�q�b�g������.
	void HitProc();
	//�U�����O�i����.
	void SlashMoveProc(clsPlayer* Enemy);

	void SlashResetProc()
	{
		Gurad = false;
		m_Stopping = false;
		BoostFlag = false;
	}
	//�ߐڏ���.
	void BladeDelayProc(int &DelayCnt, int ResetNo);
	/***************************************************/


	/*********�A�j���[�V��������**********/
	//�X�L�����b�V�����f���A�j���[�V�������䏈��.
	void AnimControll(int &ActState);
	//�ړ��A�j���[�V�����Z�b�g(��������).
	void SetMoveNo(int RNo1,int LNo2,int UNo3,int DNo4);
	//�A�j���[�V�����Z�b�g�A�ύX(�s�ϊ֐�).
	void SetAnimTrans(int &SetActState, int SetAnim, int &ProcCnt);
	void SetChangeAnim(int AnimNo, int &inActState,double Speed);
	/****************���@����֘A******************/
	//�Q�[���p�b�h������.
	void Setgamepad(clsDInput* p)
	{ if (p != NULL)	{m_tclsDInput = p;} }

	void Setgamepad2(clsDInput* p)
	{ if (p != NULL) { m_tclsDInput2 = p; } }

	void DeadProc(int &Act);

	/***************���U���g����p*****************/
	void Win(int inAct,double Speed);

	bool			PlayMotion;

public:
	int				SceneCnt = 0;
	bool			Check = false;

	//�ړ��E�u�[�X�g����֘A.
	int				BoostCnt;		//�u�[�X�g�Q�[�W�����p.
	int				BoostUseSpeCnt;	//�u�[�X�g�Q�[�W����ʒ����J�E���g.
	bool			BoostEmpty;		//�I�[�o�[�q�[�g�m�F.

	bool			BoostFlag;		//������Ԃ��ǂ���.
	bool			XMoveFlag;		//X��.
	bool			ZMoveFlag;		//Z��.

	int				PlayerRotAnim;

	int				LeftCnt, RightCnt, FlontCnt, BackCnt;

	//�����̎擾(�A�j���[�V��������p).
	bool			LFlont,RFlont,UFlont,DFlont;
	//�e�E�ˌ��̐ݒ�p.
	D3DXVECTOR3		ShotPos, H_ShotPos;		//�e�̕\���ʒu.
	D3DXVECTOR3		ShotRot;				//�e�̉�]�l.
	D3DXVECTOR3		ShotPos_Save[10];		//�e�̕\���ʒu.
	D3DXVECTOR3		H_ShotPos_Save[5];		//�e�̕\���ʒu.

	bool			ShotFlag, H_ShotFlag;	//�����׸�.
	bool			SlashFlag;				//�i���׸�.
	int				ShotCnt;				//���Ķ���.
	int				M_ShotCnt;				//���Ķ���.
	int				DirectionCnt;			//��������.

	D3DXVECTOR3		SlashPos;				//�ߐڂ̌덷�␳.

	//�e����p.
	bool			cShotFlag[B_Max];
	int				cShotCnt, c2ShotCnt;	//��e.
	bool			mShotFlag[B_Max];	
	int				mShotCnt, m2ShotCnt;	//���e.

	//�����擾�����p.
	D3DXMATRIX		mYaw, mSYaw;			//��������p.
	enDirectioin	m_eDir, m_SDir;			//�����w��.

	D3DXVECTOR3		vectle;					//�ϊ��O�޸��
	bool			VectleGet;
	
	//�e�̌��������p.
	bool			DirectionFlag[B_Dir];
	bool			mDirectionFlag[M_Dir];
	int				cDirectionCnt[B_Dir];
	int				mDirectionCnt[M_Dir];
	float			m_BYaw;

	//�����[�hUI�����p�t���O.
	bool			ReloadG1Flag, ReloadM1Flag;
	bool			ReloadG2Flag, ReloadM2Flag;
	//��e�F���e.
	int				UIReloadCnt, UIHReloadCnt;
	int				UIReload2Cnt, UIHReload2Cnt;

	int				ReloadG1Cnt, ReloadM1Cnt;
	int				ReloadG2Cnt, ReloadM2Cnt;

	int				G1DelayCnt, M1DelayCnt;
	int				G2DelayCnt, M2DelayCnt;

	//�ߐځE�K�[�h����p.
	bool			BladeAtk;				//�ߐڍU��.
	int				BladeStartCnt;
	bool			BladeMove;
	bool			Gurad;					//�K�[�h.
	int				GuradAnimCnt;
	int				SlashCnt, SlashDelay;	//�i������.
	int				SlashMoveCnt;			//�U�����O���ړ�.				
	D3DXVECTOR3		AxsizM;					//�i�s�����x�N�g���ێ��p.

	//�A�j���[�V��������p.
	double			animTime;				//��Ұ��ݎ���.

	//�q�b�g��.
	bool			HitFlag;
	int				HitCnt;
	//�q�b�g�X�g�b�v����p.
	bool			HitStop;
	int				HStopCnt;

	//�G�t�F�N�g����.
	bool			Exp;


private:
	//�R���g���[���[�錾.
	class clsDInput*  m_tclsDInput;
	class clsDInput*  m_tclsDInput2;
protected:
	//�����x�̐ݒ�.
	D3DXVECTOR3					m_Acc;
	D3DXVECTOR3					m_Old;
};

#endif