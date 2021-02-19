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
#include "TwinDInput.h"				//ゲームの作り上操作関数は2つ作っている.
//#include "DInput.h"
#include "DebugText.h"

const int	B_Max = 11;
const int	B_Dir = 10;				//弾の数だけ回転値を設定する.
const int	M_Dir = 5;				

enum CharaRotaitonVectle
{
	AnimSetNum		= 0,			//デフォルト.
	AnimSetFlont	= 1,
	AnimSetBack		= 2,
	AnimSetLeft		= 3,
	AnimSetRight	= 4,
	AnimSetAttack   = 5,			//アニメーション再生優先度設定用.
	AnimSetRotMax,					//方向最大数.
};

class clsPlayer : public clsChara
{
public:
	clsPlayer();
	~clsPlayer();

	void Init();

	//各操作.
	void Action();
	void Action2();
	void Camera(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float yaw);

	//////////////キャラ移動////////////////
	//移動.
	void Move(clsDInput* Pad,int PadNo, int inActState);
	//方向取得、設定.
	void PlayerFlontSet(int &ActSet);
	//--------------------射撃---------------------------.
	//弾の座標・フラグ初期化.
	void BulletInit(int Max);	
	void Bullet2Init(int Max);
	void MissileInit(int Max);
	void Missile2Init(int Max);

	//弾の処理.
	void ShotProc(bool flag);
	void Shot2Proc(bool flag);
	void M_ShotProc(bool flag);
	void M_Shot2Proc(bool flag);
	//弾の発射条件処理.
	void ShotStartProc();
	void ShotStartProc2();

	//弾の方向設定.
	void RotationProc(D3DXVECTOR3 &vPos, D3DXVECTOR3 vectle,int Dir);	//飛ばし方.
	//武装リロード処理.
	void ReloadProc(clsPlayer* Player, bool ReloadFlg, int &Shot);
	void Reload2Proc(clsPlayer* Player, bool ReloadFlg, int &Shot);
	void M_ReloadProc(clsPlayer* Player, bool ReloadFlg, int &Shot);
	void M_Reload2Proc(clsPlayer* Player, bool ReloadFlg, int &Shot);
	//ディレイ設定.
	void DelayProc(int &DelayCnt, int ResetNo,bool &ShotFlag);
	
	/**********************打撃*************************/
	//攻撃処理.
	void SlashProc(clsPlayer* Enemy);

	//攻撃開始処理.
	void SlashStartProc(clsPlayer* Enemy);
	void SlashStartProc2(clsPlayer* Enemy);
	//近接攻撃時キャンセル要素処理.
	void SlashChancelProc();
	//ヒット時処理.
	void HitProc();
	//攻撃時前進処理.
	void SlashMoveProc(clsPlayer* Enemy);

	void SlashResetProc()
	{
		Gurad = false;
		m_Stopping = false;
		BoostFlag = false;
	}
	//近接処理.
	void BladeDelayProc(int &DelayCnt, int ResetNo);
	/***************************************************/


	/*********アニメーション制御**********/
	//スキンメッシュモデルアニメーション制御処理.
	void AnimControll(int &ActState);
	//移動アニメーションセット(方向同期).
	void SetMoveNo(int RNo1,int LNo2,int UNo3,int DNo4);
	//アニメーションセット、変更(不変関数).
	void SetAnimTrans(int &SetActState, int SetAnim, int &ProcCnt);
	void SetChangeAnim(int AnimNo, int &inActState,double Speed);
	/****************自機制御関連******************/
	//ゲームパッド初期化.
	void Setgamepad(clsDInput* p)
	{ if (p != NULL)	{m_tclsDInput = p;} }

	void Setgamepad2(clsDInput* p)
	{ if (p != NULL) { m_tclsDInput2 = p; } }

	void DeadProc(int &Act);

	/***************リザルト制御用*****************/
	void Win(int inAct,double Speed);

	bool			PlayMotion;

public:
	int				SceneCnt = 0;
	bool			Check = false;

	//移動・ブースト制御関連.
	int				BoostCnt;		//ブーストゲージ増減用.
	int				BoostUseSpeCnt;	//ブーストゲージ消費量調整カウント.
	bool			BoostEmpty;		//オーバーヒート確認.

	bool			BoostFlag;		//加速状態かどうか.
	bool			XMoveFlag;		//X軸.
	bool			ZMoveFlag;		//Z軸.

	int				PlayerRotAnim;

	int				LeftCnt, RightCnt, FlontCnt, BackCnt;

	//向きの取得(アニメーション制御用).
	bool			LFlont,RFlont,UFlont,DFlont;
	//弾・射撃の設定用.
	D3DXVECTOR3		ShotPos, H_ShotPos;		//弾の表示位置.
	D3DXVECTOR3		ShotRot;				//弾の回転値.
	D3DXVECTOR3		ShotPos_Save[10];		//弾の表示位置.
	D3DXVECTOR3		H_ShotPos_Save[5];		//弾の表示位置.

	bool			ShotFlag, H_ShotFlag;	//ｼｮｯﾄﾌﾗｸﾞ.
	bool			SlashFlag;				//格闘ﾌﾗｸﾞ.
	int				ShotCnt;				//ｼｮｯﾄｶｳﾝﾄ.
	int				M_ShotCnt;				//ｼｮｯﾄｶｳﾝﾄ.
	int				DirectionCnt;			//向きｶｳﾝﾄ.

	D3DXVECTOR3		SlashPos;				//近接の誤差補正.

	//弾動作用.
	bool			cShotFlag[B_Max];
	int				cShotCnt, c2ShotCnt;	//弱弾.
	bool			mShotFlag[B_Max];	
	int				mShotCnt, m2ShotCnt;	//強弾.

	//方向取得処理用.
	D3DXMATRIX		mYaw, mSYaw;			//方向決定用.
	enDirectioin	m_eDir, m_SDir;			//方向指定.

	D3DXVECTOR3		vectle;					//変換前ﾍﾞｸﾄﾙ
	bool			VectleGet;
	
	//弾の向き同期用.
	bool			DirectionFlag[B_Dir];
	bool			mDirectionFlag[M_Dir];
	int				cDirectionCnt[B_Dir];
	int				mDirectionCnt[M_Dir];
	float			m_BYaw;

	//リロードUI同期用フラグ.
	bool			ReloadG1Flag, ReloadM1Flag;
	bool			ReloadG2Flag, ReloadM2Flag;
	//弱弾：強弾.
	int				UIReloadCnt, UIHReloadCnt;
	int				UIReload2Cnt, UIHReload2Cnt;

	int				ReloadG1Cnt, ReloadM1Cnt;
	int				ReloadG2Cnt, ReloadM2Cnt;

	int				G1DelayCnt, M1DelayCnt;
	int				G2DelayCnt, M2DelayCnt;

	//近接・ガード制御用.
	bool			BladeAtk;				//近接攻撃.
	int				BladeStartCnt;
	bool			BladeMove;
	bool			Gurad;					//ガード.
	int				GuradAnimCnt;
	int				SlashCnt, SlashDelay;	//格闘ｶｳﾝﾄ.
	int				SlashMoveCnt;			//攻撃時前方移動.				
	D3DXVECTOR3		AxsizM;					//進行方向ベクトル保持用.

	//アニメーション制御用.
	double			animTime;				//ｱﾆﾒｰｼｮﾝ時間.

	//ヒット時.
	bool			HitFlag;
	int				HitCnt;
	//ヒットストップ制御用.
	bool			HitStop;
	int				HStopCnt;

	//エフェクト制御.
	bool			Exp;


private:
	//コントローラー宣言.
	class clsDInput*  m_tclsDInput;
	class clsDInput*  m_tclsDInput2;
protected:
	//加速度の設定.
	D3DXVECTOR3					m_Acc;
	D3DXVECTOR3					m_Old;
};

#endif