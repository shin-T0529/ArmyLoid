#include "C_Player.h"
const char	MODEL_NAME[] = "Shader\\MeshSkin.hlsl";

//定数.
int			ci;
const int	stop = 0;
const int	AtackcntFlg = 50;			//攻撃時間.

const float Acc = 0.02f;				//加速度.
const float Deceleration = 0.2f;		//摩擦係数.
const float Upper_Acclimit = 0.2f;		//最大加速度.
const float Lower_Acclimit = -0.2f;		//最小加速度.
D3DXVECTOR3 DUST = { 0.0f, -50.0f, -70.0f };
//ブースト定数.
const float Boost_Acc = 0.06f;				//加速度.
const float Boost_Upper_Acclimit = 0.9f;	//ブースト最大加速度.
const float Boost_Lower_Acclimit = -0.9f;	//ブースト最小加速度.

clsPlayer::clsPlayer()
	//ブースト変数.
	: BoostCnt(0), BoostEmpty(false), BoostUseSpeCnt(0)
	, BoostFlag(false), XMoveFlag(false), ZMoveFlag(false)
	//アニメーション初期化用.
	, PlayMotion(false)
	//移動アニメーション制御.
	, PlayerRotAnim(0), FlontCnt(0), BackCnt(0), LeftCnt(0), RightCnt(0)
	//射撃.
	, ShotFlag(false), H_ShotFlag(false)
	, ShotCnt(0)
	, ShotRot(0.0f, 0.0f, 0.0f), ShotPos(0.0f, 0.0f, 0.0f)
	, H_ShotPos(0.0f, 0.0f, 0.0f)
	//攻撃.
	, BladeAtk(false),SlashCnt(0),SlashDelay(0)
	, BladeStartCnt(0), BladeMove(false),SlashMoveCnt(0)
	, AxsizM(0.0f,0.0f,0.0f),m_SDir(enDirection_Stop)
	//ガードフラグ.
	, Gurad(false)
	//移動関連.
	, m_Acc(0.0f, 0.0f, 0.0f)
	, LFlont(false), RFlont(false), UFlont(false), DFlont(false)
	//弾関連.
	, VectleGet(false)
	, cShotCnt(-1), c2ShotCnt(-1)
	, mShotCnt(-1), m2ShotCnt(-1)
	, ReloadG1Flag(false), ReloadM1Flag(false)
	, ReloadG2Flag(false), ReloadM2Flag(false)
	, ReloadG1Cnt(0), ReloadM1Cnt(0), ReloadG2Cnt(0), ReloadM2Cnt(0)
	, m_eDir(enDirection_Stop)
	//UI同期.
	, UIReloadCnt(0), UIReload2Cnt(0)
	, UIHReloadCnt(0), UIHReload2Cnt(0)
	//弾の発射間隔制御.
	, G1DelayCnt(0), M1DelayCnt(0)
	, G2DelayCnt(0), M2DelayCnt(0)
	, m_BYaw(0)
	//アニメーション制御.
	, animTime(0.0),GuradAnimCnt(0)
	//ヒット時フラグ変数.
	, HitFlag(false),HitCnt(0)
	//ヒットストップフラグ変数.
	, HitStop(false), HStopCnt(0)
	//エフェクト制御.
	, Exp(false)
	//ベクトル.
	, vectle(D3DXVECTOR3(0.0f,0.0f,0.0f))
{
	//弾の初期化.
	for (ci = 0; ci < B_Dir; ci++)
	{ 
		cShotFlag[ci] = false;
		DirectionFlag[ci] = false;
		cDirectionCnt[ci] = false;

	}
	for (ci = 0; ci < M_Dir; ci++)
	{
		mShotFlag[ci] = false;
		mDirectionCnt[ci] = 0;
		mDirectionFlag[ci] = false;
	}
}

clsPlayer::~clsPlayer()
{
	DetatchModel();
}

void clsPlayer::Init()
{
	m_Yaw = 0.0f;
	m_Pitch = 0.0f;
	m_Roll = 0.0f;
	m_DeadFlag = false;
	ActState = 0;
	ActState2 = 0;
}
//1P処理.
void clsPlayer::Action()
{
	//コントロール入力情報更新
	m_tclsDInput->GetJoypadState2();

	//メイン処理.
	if (!m_Stopping && !m_DeadFlag)
	{
		//移動.
		if (!Gurad && !m_Stopping)
		{
			if (!HitStop && !BladeAtk)
			{
				Move(m_tclsDInput, 0, ActState);
			}
		}

		//ガード.
		if (!BoostEmpty && !BladeAtk && !BoostFlag)
		{
			if (GetAsyncKeyState('N') & 0x8000
			|| m_tclsDInput->g_diJoyState2[0] & BUTTON_RB)
			{
				Gurad = true;
				GuradAnimCnt++;
				SlashCnt = 0;
				BladeAtk = false;
				SetAnimTrans(ActState, enActionGuard, GuradAnimCnt);
				if(BoostFlag)
				{ BoostFlag = false; }
			}
			else
			{
				Gurad = false;
				GuradAnimCnt = 0;
			}
		}

		//方向設定.
		PlayerFlontSet(ActState);

		//射撃処理.
		ShotStartProc();
		//射撃ディレイ処理.
		if (!G1DelayCnt == 0)
		{ DelayProc(G1DelayCnt, 25,ShotFlag); }
		if (!M1DelayCnt == 0)
		{ DelayProc(M1DelayCnt, 60, H_ShotFlag); }

		//武装残弾数UIの同期.
		if (ReloadG1Flag)
		{ UIReloadCnt = 0; }
		if (ReloadM1Flag)
		{ UIHReloadCnt = 0; }


	}
}
//2P処理.
void clsPlayer::Action2()
{//パッド複数読込の都合上二ついる.
	//コントロール入力情報更新
	m_tclsDInput2->GetJoypadState2();

	//メイン処理.
	if (!m_Stopping && !m_DeadFlag)
	{
		//移動処理.
		if (!Gurad && !m_Stopping)
		{
			if (!HitStop && !BladeAtk)
			{
				Move(m_tclsDInput2, 1, ActState2);
			}
		}

		//ガード.
		if (!BoostEmpty && !BladeAtk && !BoostFlag)
		{
			if (GetAsyncKeyState('M') & 0x8000
			|| m_tclsDInput2->g_diJoyState2[1] & BUTTON_RB)
			{
				Gurad = true;
				GuradAnimCnt++;
				SlashCnt = 0;
				BladeAtk = false;
				SetAnimTrans(ActState2, enActionGuard, GuradAnimCnt);
				if (BoostFlag)
				{
					BoostFlag = false;
				}
			}
			else
			{
				Gurad = false;
				GuradAnimCnt = 0;
			}
		}

		//方向設定.
		PlayerFlontSet(ActState2);

		//格闘攻撃
		//SlashStartProc2();

		//射撃処理.
		ShotStartProc2();
		//リロード処理.
		if (m2ShotCnt == 4)
		{ UIHReload2Cnt++; }
		if (GatlingReload < UIHReload2Cnt)
		{ ReloadM2Flag = true; }

		//武装残弾数UIの同期.
		if (!G2DelayCnt == 0)
		{ DelayProc(G2DelayCnt, 25,ShotFlag); }
		if (!M2DelayCnt == 0)
		{ DelayProc(M2DelayCnt,60,H_ShotFlag); }
		//武装残弾数UIの同期.
		if (ReloadG2Flag)
		{ UIReload2Cnt = 0; }
		if (ReloadM2Flag)
		{ UIHReload2Cnt = 0; }

	}
}

//////////////////////移動//////////////////////
//移動.
void clsPlayer::Move(clsDInput* Pad,int PadNo,int inActState)
{
	//ブースト
	if (GetAsyncKeyState('L') & 0x8000 && !BoostEmpty && !Gurad
	|| !BoostEmpty && !Gurad && Pad->g_diJoyState2[PadNo] & BUTTON_LB)
	{ BoostFlag = true; }
	else
	{ BoostFlag = false; }

	//左に移動.
	if (Pad->g_diJoyState2[PadNo] & BUTTON_LEFT || GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_Stopping = true;
		if (BoostFlag == false)
		{ 
			m_Acc.x -= Boost_Acc;
			SlashCnt = 0;
		}
		else
		{ 
			m_Acc.x -= Acc;
			SlashCnt = 0;
		}
		SetMoveNo(AnimSetBack, AnimSetFlont, AnimSetLeft, AnimSetRight);
		LeftCnt++;

		XMoveFlag = true;
	}
	//右に移動.
	else if (Pad->g_diJoyState2[PadNo] & BUTTON_RIGHT || GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_Stopping = true;
		SlashCnt = 0;
		if (BoostFlag == false)
		{
			m_Acc.x += Boost_Acc;
		}
		else
		{
			m_Acc.x += Acc;
		}
		SetMoveNo(AnimSetFlont, AnimSetBack, AnimSetRight, AnimSetLeft);
		XMoveFlag = true;
		RightCnt++;
	}
	//X軸止まっている時.
	else if (m_Acc.x >= stop)
	{
		LeftCnt = 0;
		RightCnt = 0;
		XMoveFlag = false;
		PlayerRotAnim = 0;

		m_Acc.x -= m_Acc.x * Deceleration;
		if (m_Acc.x < stop) { m_Acc.x = stop; }
	}
	else if (m_Acc.x <= stop)
	{
		LeftCnt = 0;
		RightCnt = 0;
		PlayerRotAnim = 0;

		XMoveFlag = false;
		m_Acc.x -= m_Acc.x * Deceleration;
		if (m_Acc.x > stop) { m_Acc.x = stop; }
	}
	
	
	//前に移動.
	if (GetAsyncKeyState(VK_UP) & 0x8000 ||	Pad->g_diJoyState2[PadNo] & BUTTON_UP)
	{
		m_Stopping = true;
		SlashCnt = 0;
		if (BoostFlag == false)
		{ 
			m_Acc.z += Boost_Acc;
			SlashCnt = 0;
		}
		else
		{ 
			m_Acc.z += Acc;
			SlashCnt = 0;
		}
		SetMoveNo(AnimSetLeft, AnimSetRight, AnimSetFlont, AnimSetBack);
		FlontCnt++;
		ZMoveFlag = true;

	}
	//手前に移動.
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 || Pad->g_diJoyState2[PadNo] & BUTTON_DOWN)
	{
		m_Stopping = true;
		SlashCnt = 0;
		if (BoostFlag == false)
		{ 
			m_Acc.z -= Boost_Acc;
			SlashCnt = 0;
		}
		else
		{ 
			m_Acc.z -= Acc;
			SlashCnt = 0;
		}

		SetMoveNo(AnimSetRight, AnimSetLeft, AnimSetBack, AnimSetFlont);

		ZMoveFlag = true;
		BackCnt++;
	}
	//止まっている時(+).
	else if (m_Acc.z >= stop)
	{
		ZMoveFlag = false;
		BackCnt=0;
		FlontCnt=0;
		m_Acc.z -= m_Acc.z * Deceleration;
		if (m_Acc.z < stop) { m_Acc.z = stop; }
	}
	//止まっている時(-).
	else if (m_Acc.z <= stop)
	{
		ZMoveFlag = false;
		BackCnt = 0;
		FlontCnt = 0;
		m_Acc.z -= m_Acc.z * Deceleration;
		if (m_Acc.z > stop) { m_Acc.z = stop; }
	}
	//加速度処理.
	if (BoostFlag == false)
	{	//ブーストを使ってないとき.
		if (m_Acc.x > Upper_Acclimit) m_Acc.x = Upper_Acclimit;
		if (m_Acc.x < Lower_Acclimit) m_Acc.x = Lower_Acclimit;
		if (m_Acc.z > Upper_Acclimit) m_Acc.z = Upper_Acclimit;
		if (m_Acc.z < Lower_Acclimit) m_Acc.z = Lower_Acclimit;
	}
	else
	{	//ブーストを使ってるとき.
		if (m_Acc.x > Boost_Upper_Acclimit) m_Acc.x = Boost_Upper_Acclimit;
		if (m_Acc.x < Boost_Lower_Acclimit) m_Acc.x = Boost_Lower_Acclimit;
		if (m_Acc.z > Boost_Upper_Acclimit) m_Acc.z = Boost_Upper_Acclimit;
		if (m_Acc.z < Boost_Lower_Acclimit) m_Acc.z = Boost_Lower_Acclimit;
	}
	//加速度を足す
	m_vPos.x += m_Acc.x;
	m_vPos.z += m_Acc.z;


}
//方向取得、設定.
void clsPlayer::PlayerFlontSet(int &ActSet)
{
	if (!BladeAtk)
	{
		if (RFlont)		//右が正面の時.
		{
			SetAnimTrans(ActSet, enHoverLeftStart, FlontCnt);
			SetAnimTrans(ActSet, enHoverRightStart, BackCnt);
			SetAnimTrans(ActSet, enHoverFlontStart, RightCnt);
			SetAnimTrans(ActSet, enHoverBackStart, LeftCnt);
		}
		else if (LFlont)//左.
		{
			SetAnimTrans(ActSet, enHoverLeftStart, BackCnt);
			SetAnimTrans(ActSet, enHoverRightStart, FlontCnt);
			SetAnimTrans(ActSet, enHoverFlontStart, LeftCnt);
			SetAnimTrans(ActSet, enHoverBackStart, RightCnt);

		}
		else if (UFlont)	//前(下).
		{
			SetAnimTrans(ActSet, enHoverLeftStart, LeftCnt);
			SetAnimTrans(ActSet, enHoverRightStart, RightCnt);
			SetAnimTrans(ActSet, enHoverFlontStart, FlontCnt);
			SetAnimTrans(ActSet, enHoverBackStart, BackCnt);
		}
		else if (DFlont)	//後ろ(上).
		{
			SetAnimTrans(ActSet, enHoverLeftStart, RightCnt);
			SetAnimTrans(ActSet, enHoverRightStart, LeftCnt);
			SetAnimTrans(ActSet, enHoverFlontStart, BackCnt);
			SetAnimTrans(ActSet, enHoverBackStart, FlontCnt);
		}
	}

}
////////////////////////////////////////////////

//---------------------射撃処理-----------------------.
//初期化.
void clsPlayer::BulletInit(int Max)
{
	for (int Mi = 0; Mi < Max; Mi++)
	{
		cShotCnt = -1;
		ReloadG1Flag = false;
		ShotFlag = false;
		DirectionFlag[Mi] = false;
		m_SaveYaw[Mi] = 0.0f;
		ShotPos_Save[Mi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_vPos = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
	}
}

void clsPlayer::Bullet2Init(int Max)
{
	for (int Mi = 0; Mi < Max; Mi++)
	{
		c2ShotCnt = -1;
		ReloadG2Flag = false;
		ShotFlag = false;
		DirectionFlag[Mi] = false;
		m_SaveYaw[Mi] = 0.0f;
		ShotPos_Save[Mi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_vPos = D3DXVECTOR3(0.0f, -50.0f, -5.0f);
	}

}

void clsPlayer::MissileInit(int Max)
{
	for (int Mi = 0; Mi < Max; Mi++)
	{
		mShotCnt = -1;
		ReloadM1Flag = false;
		H_ShotFlag = false;
		mDirectionFlag[Mi] = false;
		m_SaveYaw[Mi] = 0.0f;
		H_ShotPos_Save[Mi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_vPos = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
	}
}

void clsPlayer::Missile2Init(int Max)
{
	for (int Mi = 0; Mi < Max; Mi++)
	{
		m2ShotCnt = -1;
		ReloadM2Flag = false;
		H_ShotFlag = false;
		mDirectionFlag[Mi] = false;
		m_SaveYaw[Mi] = 0.0f;
		H_ShotPos_Save[Mi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_vPos = D3DXVECTOR3(0.0f, -50.0f, -5.0f);
	}

}

//射撃開始用.
void clsPlayer::ShotStartProc()
{
	//弾を撃つ.
	if (ReloadG1Flag == false)
	{
		if (G1DelayCnt == 0 && Gurad == false)
		{
			if (BladeAtk == false)
			{
				if (GetAsyncKeyState('Z') & 0x01 && !H_ShotFlag
				 || m_tclsDInput->g_diJoyState2[0] & BUTTON_X && !H_ShotFlag)		//Dinput.
				{
					ShotFlag = true;
					ShotPos = m_vPos;
					cShotCnt++;
					G1DelayCnt++;

					//上限設定.
					if (9 <= cShotCnt)
					{
						cShotCnt = 9;
					}
				}
			}
		}
		if (cShotCnt == 9)
		{
			UIReloadCnt++;
		}
		if (GatlingReload < UIReloadCnt)
		{
			ReloadG1Flag = true;
		}

	}
	//強い弾を撃つ
	if (ReloadM1Flag == false)
	{
		if (M1DelayCnt == 0 && Gurad == false)
		{
			if (BladeAtk == false)
			{
				if (GetAsyncKeyState('X') & 0x01 && !ShotFlag
				 || m_tclsDInput->g_diJoyState2[0] & BUTTON_B && !ShotFlag)	
				{
					H_ShotFlag = true;
					//ShotPos = m_vPos;
					H_ShotPos = m_vPos;
					mShotCnt++;
					M1DelayCnt++;
					m_pStaticSkinMesh->SetAnimSpeed(AnimPlaySpeed * 2.5);
					ActState = enShotMissile;
					m_pStaticSkinMesh->ChangeAnimSet(ActState);

					//上限設定.
					if (4 <= mShotCnt)
					{
						mShotCnt = 4;
					}
				}
			}
		}
		if (mShotCnt == 4)
		{
			UIHReloadCnt++;
		}
		if (GatlingReload < UIHReloadCnt)
		{
			ReloadM1Flag = true;
		}

	}
}

void clsPlayer::ShotStartProc2()
{
	//弾を撃つ.
	if (ReloadG2Flag == false)
	{
		if (G2DelayCnt == 0 && Gurad == false)
		{
			if (GetAsyncKeyState('Y') & 0x01 && !H_ShotFlag
			 || m_tclsDInput2->g_diJoyState2[1] & BUTTON_X && !H_ShotFlag)
			{
				ShotFlag = true;
				ShotPos = m_vPos;
				c2ShotCnt++;
				G2DelayCnt++;
				if (9 <= c2ShotCnt)
				{
					c2ShotCnt = 9;
				}
			}
		}
		if (c2ShotCnt == 9)
		{
			UIReload2Cnt++;
		}

		if (48 < UIReload2Cnt)
		{
			ReloadG2Flag = true;
		}
	}
	//強い弾を撃つ
	if (ReloadM2Flag == false)
	{
		if (M2DelayCnt == 0 && Gurad == false)
		{
			if (BladeAtk == false)
			{
				if (GetAsyncKeyState('U') & 0x8000 && !ShotFlag
				 || m_tclsDInput2->g_diJoyState2[1] & BUTTON_B && !ShotFlag)
				{
					H_ShotFlag = true;
					H_ShotPos = m_vPos;
					m2ShotCnt++;
					M2DelayCnt++;
					m_pStaticSkinMesh->SetAnimSpeed(AnimPlaySpeed * 2.5);
					ActState2 = enShotMissile;
					m_pStaticSkinMesh->ChangeAnimSet(ActState2);

					//上限設定.
					if (4 <= m2ShotCnt)
					{
						m2ShotCnt = 4;
					}
				}
			}
		}
	}

}
//射撃の動作、処理.
void clsPlayer::ShotProc(bool flag)
{
	//軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);	//変換前ﾍﾞｸﾄﾙ
	if (ShotFlag == true)
	{
		//ボタンを押した直後に位置と向きを設定
		if (ShotCnt == 0)
		{
			m_vPos = ShotPos;

			vecAxisZ.x = m_Yaw;
			vecAxisZ.z = m_Roll;
		}
		//10_04
		m_eDir = enDirection_Stop;
		D3DXMatrixRotationY(&mYaw, m_Yaw);		//Y軸回転.

		D3DXVec3TransformCoord(&vecAxisZ,//(out).
			&vecAxisZ, &mYaw);

		m_eDir = enDirection_Forward;

		RotationProc(m_vPos, vecAxisZ,m_eDir);

		ShotCnt++;

	}
	else if (ReloadG1Flag == false)
	{
		ShotCnt = 0;
		DirectionCnt = 1;
		m_vPos = ShotPos;
		m_vPos.x = 10.0f;
		m_vPos.y = -5.0f;
		m_eDir = enDirection_Stop;
	}

	ShotRot = m_vRot;
}

void clsPlayer::Shot2Proc(bool flag)
{
	//軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);	//変換前ﾍﾞｸﾄﾙ
	if (ShotFlag == true)
	{
		//ボタンを押した直後に位置と向きを設定
		if (ShotCnt == 0)
		{
			m_vPos = ShotPos;

			vecAxisZ.x = m_Yaw;
			vecAxisZ.z = m_Roll;
		}
		//10_04

		D3DXMatrixRotationY(&mYaw, m_Yaw);		//Y軸回転.

		D3DXVec3TransformCoord(&vecAxisZ,//(out).
			&vecAxisZ, &mYaw);

		m_eDir = enDirection_Forward;

		RotationProc(m_vPos, vecAxisZ, m_eDir);

		ShotCnt++;

	}
	else if (ReloadG2Flag == false)
	{
		ShotCnt = 0;
		DirectionCnt = 1;
		m_vPos = ShotPos;
		m_vPos.x = 10.0f;
		m_vPos.y = -5.0f;
		m_eDir = enDirection_Stop;
	}

	ShotRot = m_vRot;
}

void clsPlayer::M_ShotProc(bool flag)
{
	//軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);	//変換前ﾍﾞｸﾄﾙ
	if (H_ShotFlag == true)
	{
		//ボタンを押した直後に位置と向きを設定
		if (M_ShotCnt == 0)
		{
			m_vPos = H_ShotPos;
			m_vPos.y = 7.5f;
			vecAxisZ.x = m_Yaw;
			vecAxisZ.z = m_Roll;
		}
		//10_04

		D3DXMatrixRotationY(&mYaw, m_Yaw);		//Y軸回転.

		D3DXVec3TransformCoord(&vecAxisZ,//(out).
			&vecAxisZ, &mYaw);

		m_eDir = enDirection_Forward;

		RotationProc(m_vPos, vecAxisZ, m_eDir);

		M_ShotCnt++;

	}
	else if (ReloadM1Flag == false)
	{
		M_ShotCnt = 0;
		DirectionCnt = 1;
		m_vPos = ShotPos;
		m_vPos.x = 10.0f;
		m_vPos.y = -5.0f;
		m_eDir = enDirection_Stop;
	}

	ShotRot = m_vRot;
}

void clsPlayer::M_Shot2Proc(bool flag)
{
	//軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);	//変換前ﾍﾞｸﾄﾙ
	if (H_ShotFlag == true)
	{
		//ボタンを押した直後に位置と向きを設定
		if (M_ShotCnt == 0)
		{
			m_vPos = H_ShotPos;

			vecAxisZ.x = m_Yaw;
			vecAxisZ.z = m_Roll;
		}
		//10_04

		D3DXMatrixRotationY(&mYaw, m_Yaw);		//Y軸回転.

		D3DXVec3TransformCoord(&vecAxisZ,//(out).
			&vecAxisZ, &mYaw);

		m_eDir = enDirection_Forward;
		//8
		RotationProc(m_vPos, vecAxisZ, m_eDir);

		M_ShotCnt++;

	}
	else if (ReloadM2Flag == false)
	{
		M_ShotCnt = 0;
		DirectionCnt = 1;
		m_vPos = ShotPos;
		m_vPos.x = 10.0f;
		m_vPos.y = -5.0f;
		m_eDir = enDirection_Stop;
	}

	ShotRot = m_vRot;

}

//方向設定.
void clsPlayer::RotationProc(D3DXVECTOR3 &vPos, D3DXVECTOR3 vectle,int Dir)
{
	//方向によって進行させる値を設定.
	switch (Dir) {
	case enDirection_Forward:	//前進.
		vPos += vectle * 1.0f;
		break;
	case enDirection_FastFlont:	//モデルの正面方向に進ませる.
		vPos -= vectle * 0.3f;
		break;
	}
	m_eDir = enDirection_Stop;//停止.

}

//リロード処理　設定.
void clsPlayer::ReloadProc(clsPlayer* Player, bool ReloadFlg, int &Shot)
{
	if (ReloadFlg == true)
	{
		Shot++;
		if (Shot >= 100)
		{
			ReloadFlg = false;
			m_vPos = DUST;
			Shot = 0;
			Player->cShotCnt = -1;
			Player->ReloadG1Flag = false;
			Player->ShotFlag = false;
			Player->G1DelayCnt = 1;
			for (int Mi = 0; Mi < 10; Mi++)
			{
				Player->DirectionFlag[Mi] = false;
				Player->cDirectionCnt[Mi] = 0;
			}
		}
	}
}

void clsPlayer::Reload2Proc(clsPlayer* Player, bool ReloadFlg, int &Shot)
{
	if (ReloadFlg == true)
	{
		Shot++;
		if (Shot >= 100)
		{
			ReloadFlg = false;
			m_vPos = ShotPos;
			m_vPos.y -= 2.5f;
			Shot = 0;
			Player->c2ShotCnt = -1;
			Player->ReloadG2Flag = false;
			Player->ShotFlag = false;
			Player->G2DelayCnt = 1;
			for (int Mi = 0; Mi < 10; Mi++)
			{
				Player->DirectionFlag[Mi] = false;
				Player->cDirectionCnt[Mi] = 0;
			}
		}
	}
}

void clsPlayer::M_ReloadProc(clsPlayer* Player, bool ReloadFlg, int &Shot)
{
	if (ReloadFlg == true)
	{
		Shot++;
		if (Shot >= 100)
		{
			ReloadFlg = false;
			m_vPos = ShotPos;
			m_vPos.y -= 2.5f;
			Shot = 0;
			Player->mShotCnt = -1;
			Player->ReloadM1Flag = false;
			Player->H_ShotFlag = false;
			Player->M1DelayCnt = 1;
			for (int Mi = 0; Mi < 10; Mi++)
			{
				Player->mDirectionFlag[Mi] = false;
				Player->mDirectionCnt[Mi] = 0;
			}
		}
	}

}

void clsPlayer::M_Reload2Proc(clsPlayer* Player, bool ReloadFlg, int &Shot)
{
	if (ReloadFlg == true)
	{
		Shot++;
		if (Shot >= 100)
		{
			ReloadFlg = false;
			m_vPos = ShotPos;
			m_vPos.y -= 2.5f;
			Shot = 0;
			Player->m2ShotCnt = -1;
			Player->ReloadM2Flag = false;
			Player->H_ShotFlag = false;
			Player->M2DelayCnt = 1;
			for (int Mi = 0; Mi < 10; Mi++)
			{
				Player->mDirectionFlag[Mi] = false;
				Player->mDirectionCnt[Mi] = false;
			}
		}
	}
}

//ディレイ間隔設定　処理.
void clsPlayer::DelayProc(int &DelayCnt, int ResetNo, bool &ShotFlag)
{
	if (DelayCnt != 0)
	{
		DelayCnt++;
		if (ResetNo <= DelayCnt)
		{
			DelayCnt = 0;
			ShotFlag = false;
		}
	}

}

void clsPlayer::BladeDelayProc(int &DelayCnt, int ResetNo)
{
	if (DelayCnt != 0)
	{
		DelayCnt++;
		if (ResetNo <= DelayCnt)
		{
			DelayCnt = 0;
			SlashMoveCnt = 0;
			BladeStartCnt = 0;
		}
	}

}

//----------------------------------------------------.

//************格闘処理*************.
//攻撃処理.
void clsPlayer::SlashProc(clsPlayer* Enemy)
{
	if (BladeAtk && SlashDelay == 0)
	{
		//SlashMoveCnt++;

		if (SlashMoveCnt < 10)
		{ BladeMove = true; }

		if (BladeMove)
		{ SlashCnt++; }
		if (50 < SlashCnt)
		{
			m_eDir = enDirection_Stop;
			m_vPos = SlashPos;
			BladeMove = false;
			SlashDelay++;
			//BladeAtk = false;
		}
	}
	else
	{
		SlashMoveCnt = 0;
		SlashCnt = 0;
	}

	if (BladeAtk && SlashMoveCnt < 40)
	{ 
		SlashMoveProc(Enemy);
		SlashPos = m_vPos;
	}
}


//攻撃開始処理.
void clsPlayer::SlashStartProc(clsPlayer* Enemy)
{
	if (!Gurad && !ShotFlag && !H_ShotFlag)
	{
		BladeStartCnt++;
		if (10 < BladeStartCnt)
		{
			if (GetAsyncKeyState('C') & 0x8000 || m_tclsDInput->g_diJoyState2[0] & BUTTON_A)		//Dinput.
			{
				if (SlashDelay == 0)
				{
					if (!BladeAtk)
					{
						BladeAtk = true;
						SlashChancelProc();	//移動動作中の物をキャンセル.
						m_eDir = enDirection_Stop;
						animTime = 0.0;
						m_pStaticSkinMesh->SetAnimSpeed(AnimPlaySpeed * 2.0);
						ActState = enAttackMode;
						m_pStaticSkinMesh->ChangeAnimSet(ActState);
					}
				}
			}
			SlashProc(Enemy);
			BladeDelayProc(SlashDelay, 40);
		}
	}
}

void clsPlayer::SlashStartProc2(clsPlayer* Enemy)
{
	if (!Gurad && !ShotFlag && !H_ShotFlag)
	{
		BladeStartCnt++;
		if (10 < BladeStartCnt)
		{
			if (GetAsyncKeyState('V') & 0x8000 || m_tclsDInput2->g_diJoyState2[1] & BUTTON_A)
			{
				if (SlashDelay == 0)
				{
					if (!BladeAtk)
					{
						BladeAtk = true;
						SlashChancelProc();	//移動動作中の物をキャンセル.
						animTime = 0.0;
						m_pStaticSkinMesh->SetAnimSpeed(AnimPlaySpeed * 2.0);
						ActState2 = enAttackMode;
						m_pStaticSkinMesh->ChangeAnimSet(ActState2);
					}
				}
			}
			SlashProc(Enemy);
			BladeDelayProc(SlashDelay, 40);
		}
	}
}
//近接攻撃時キャンセル要素処理.
void clsPlayer::SlashChancelProc()
{
	//リセットしないと攻撃し終えたあと加速していく.
	m_eDir = enDirection_Stop;		//反対方向に向かないよう一度止める.
	BoostFlag = false;
	m_Acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//攻撃時敵方向に前進させる.
void clsPlayer::SlashMoveProc(clsPlayer* Enemy)
{
	D3DXVECTOR3 vecAxisZSlash = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//対応キー押した時に位置を記録し、ある程度追尾する.
	if (!Enemy->HitFlag)
	{
		if (BladeAtk)
		{
			SlashMoveCnt++;

			//方向計算が完了するまで3f.
			if (SlashMoveCnt <= 2)
			{
				//1度だけでいいから1f分.
				if (SlashMoveCnt <= 1)
				{
					//モデル回転値を使っているためYawは直接代入.
					vecAxisZSlash.x = m_vRot.x;
					m_Yaw = m_pStaticSkinMesh->m_fYaw;
					vecAxisZSlash.z = m_vRot.z;
				}
				//回転行列の作成(Y軸回転).
				D3DXMatrixRotationY(&mYaw, m_Yaw);
				//頂点変換.
				D3DXVec3TransformCoord(&vecAxisZSlash, &vecAxisZSlash, &mYaw);
				//3f抜けたあとのために記録し続ける.
				AxsizM = vecAxisZSlash;
			}
			else
			{	//計算後は保持していた方に進む.
				vecAxisZSlash = AxsizM;
			}
			//方向決定
			m_SDir = enDirection_FastFlont;
			//進行計算.
			RotationProc(m_vPos, vecAxisZSlash, m_SDir);
		}
		else
		{
			//一応代入初期化.
			vecAxisZSlash = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
}

//ヒットした時連続で当たることを防ぐ.
void clsPlayer::HitProc()
{
	if (HitFlag)
	{
		HitCnt++;

		if (50 < HitCnt)
		{
			HitCnt = 0;
			SlashCnt = 0;
			BladeAtk = false;
			SlashMoveCnt = 0;
			HitFlag = false;
			HitStop = false;
		}
	}
}
//*********************************.

////////////システム処理////////////.
//死亡処理.
void clsPlayer::DeadProc(int &Act)
{
	if (m_DeadFlag && !Check)
	{
		Act = enActionDown;
		SetChangeAnim(enActionDown, Act, AnimPlaySpeed);
		Check = true;
	}
}
//勝利処理.
void clsPlayer::Win(int inAct, double Speed)
{
	m_pStaticSkinMesh->SetAnimSpeed(AnimPlaySpeed * Speed);
	m_pStaticSkinMesh->ChangeAnimSet(inAct);
	if (!PlayMotion)
	{
		PlayMotion = true;
	}
}
//スキンメッシュモデルアニメーション制御処理.
void clsPlayer::AnimControll(int &ActState)
{

	switch (ActState)
	{
	case enStartUp:		//起動(PLAY画面で一度しか通らない).
		m_pStaticSkinMesh->SetAnimSpeed(AnimPlaySpeed);
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) - 0.02 < animTime)
		{
			animTime = 0.0;
			ActState = enWait;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		break;

	case enWait:		//待機.
		m_pStaticSkinMesh->SetAnimSpeed(AnimPlaySpeed);
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) -0.01 < animTime)
		{
			SlashCnt = 0;
			animTime = 0.0;
			ActState = enWait;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		break;

	case enActionHirumi:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) - 0.01 < animTime)
		{
			animTime = 0.0;
			ActState = enWait;
			HitFlag = false;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		break;

	case enAttackMode:
		//武器を構えて前進は無理.
		//モデルをもう一つ作る.
		//			+
		//攻撃のアニメーション始動部分を変えないとだめ.
		if (BladeAtk)
		{
			if (m_pStaticSkinMesh->GetAnimPeriod(ActState) < animTime)
			{
				animTime = 0.0;
				SlashCnt = 0;
				ActState = enWait;
				m_pStaticSkinMesh->SetAnimSpeed(AnimPlaySpeed);
				m_pStaticSkinMesh->ChangeAnimSet(ActState);
				SlashCnt = 0;
				BladeAtk = false;
			}
			animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
			break;
		}
		else if (SceneCnt == 1)
		{
			if (m_pStaticSkinMesh->GetAnimPeriod(ActState) < animTime)
			{
				animTime = 0.0;
				ActState = enAttackMode;
				m_pStaticSkinMesh->SetAnimSpeed(AnimPlaySpeed);
				m_pStaticSkinMesh->ChangeAnimSet(ActState);
			}
			animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
			break;
		}

	case enShotMissile:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) -0.01 < animTime)
		{
			animTime = 0.0;
			SlashCnt = 0;
			ActState = enWait;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		break;

	case enActionGuard:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) * 0.3 < animTime)
		{
			animTime = 0.0;
			SlashCnt = 0;
			ActState = enActionGuardDuring;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		break;

	case enActionGuardDuring:
		if (Gurad && !BoostEmpty)
		{ 
			animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		}
		else
		{
			animTime = 0.0;
			SlashCnt = 0;
			ActState = enWait;
			Gurad = false;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		break;

	case enHoverFlontStart:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) - 0.01 < animTime)
		{
			animTime = 0.0;
			ActState = enHoverFlontDuring;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();
		break;

	case enHoverFlontDuring:
		if (PlayerRotAnim == 1)
		{
			animTime += m_pStaticSkinMesh->GetAnimSpeed();
		}
		else
		{
			animTime = 0.0;
			ActState = enHoverFlontEnd;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		break;

	case enHoverFlontEnd:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) - 0.02 < animTime)
		{
			animTime = 0.0;
			ActState = enWait;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();
		break;

	case enHoverLeftStart:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) - 0.01 < animTime)
		{
			animTime = 0.0;
			ActState = enHoverLeftDuring;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();
		break;

	case enHoverLeftDuring:
		if (PlayerRotAnim == 3)
		{
			animTime += m_pStaticSkinMesh->GetAnimSpeed();
		}
		else
		{
			animTime = 0.0;
			ActState = enHoverLeftEnd;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		break;

	case enHoverLeftEnd:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) - 0.02 < animTime)
		{
			animTime = 0.0;
			ActState = enWait;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();
		break;

	case enHoverRightStart:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) - 0.01 < animTime)
		{
			animTime = 0.0;
			ActState = enHoverRightDuring;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();
		break;

	case enHoverRightDuring:
		if (PlayerRotAnim == 4)
		{
			animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		}
		else
		{
			animTime = 0.0;
			ActState = enHoverRightEnd;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		break;

	case enHoverRightEnd:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) -0.02 < animTime)
		{
			animTime = 0.0;
			ActState = enWait;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();
		break;

	case enHoverBackStart:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) - 0.01 < animTime)
		{
			animTime = 0.0;
			ActState = enHoverBackDuring;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();
		break;

	case enHoverBackDuring:
		if (PlayerRotAnim == 2)
		{
			animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		}
		else
		{
			animTime = 0.0;
			ActState = enHoverBackEnd;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		break;

	case enHoverBackEnd:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) - 0.02 < animTime)
		{
			animTime = 0.0;
			ActState = enWait;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		break;

	case enActionDown:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) -0.01 < animTime)
		{
			animTime = 0.0;
			ActState = enActionDownEnd;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		break;

	case enActionDownEnd:
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) < animTime)
		{
			animTime = 0.0;
			ActState = enActionDownEnd;
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		break;

	case enWinnerPause:	//近接攻撃.
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) -0.01 < animTime)
		{
			animTime = 0.0;
			m_pStaticSkinMesh->SetAnimSpeed(AnimPlaySpeed);
			ActState = enWinnerPauseEnd;		//固定ポーズできるまでこれ.

			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		break;

	case enWinnerPauseEnd:	//近接攻撃.
		if (m_pStaticSkinMesh->GetAnimPeriod(ActState) < animTime)
		{
			animTime = 0.0;
			SlashCnt = 0;
			ActState = enWinnerPauseEnd;		//固定ポーズできるまでこれ.
			m_pStaticSkinMesh->ChangeAnimSet(ActState);
		}
		animTime += m_pStaticSkinMesh->GetAnimSpeed();//ｱﾆﾒｰｼｮﾝ時間を加算.
		break;
	case enFreaze:
		m_pStaticSkinMesh->SetAnimSpeed(0.0);

		break;
	default:
		break;
	}

}
//移動アニメーションセット.
void clsPlayer::SetMoveNo(int RNo1, int LNo2, int UNo3, int DNo4)
{
	if (RFlont)		//右が正面の時.
	{
		PlayerRotAnim = RNo1;
	}
	else if (LFlont)//左.
	{
		PlayerRotAnim = LNo2;
	}
	else if (UFlont)	//前(下).
	{
		PlayerRotAnim = UNo3;
	}
	else if (DFlont)	//後ろ(上).
	{
		PlayerRotAnim = DNo4;
	}

}

/********************不変関数*******************/

//カメラ
void clsPlayer::Camera(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw)
{
	vLook =  D3DXVECTOR3(10.0f,  50.0f, 0.0f);
	vEye  =  D3DXVECTOR3(10.0f, -50.0f, 0.0f);
	Yaw = m_Yaw;
}

//アニメーションセット、変更.
void clsPlayer::SetAnimTrans(int &SetActState, int SetAnim, int &ProcCnt)
{
	if (1 < ProcCnt && ProcCnt < 3)
	{
		animTime = 0.0;
		m_pStaticSkinMesh->SetAnimSpeed(0.01);
		SetActState = SetAnim;
		m_pStaticSkinMesh->ChangeAnimSet(SetActState);
	}
}

void clsPlayer::SetChangeAnim(int AnimNo,int &inActState, double Speed)
{
	//モーション再生.
	m_pStaticSkinMesh->SetAnimSpeed(Speed);
	inActState = AnimNo;
	animTime = 0.0;
	m_pStaticSkinMesh->ChangeAnimSet(inActState);
}

