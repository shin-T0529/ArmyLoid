#include "Play.h"

int TimeCnt;					//�������ԑ���p.
int Li;							//���[�v�p.

//�萔.
const int Caution = 50;			//���C�t.
const int Danger = 20;
const int ModelPosNo = 7;
const int GatlingMax = 10;		//�K�g�����O�̒e�ő吔.
const int MissileMax = 5;		//�~�T�C���̒e��.
const float VSRot = 15.705f;	//��]�l�̃Y���C���p.

CSceneGameMain::CSceneGameMain(const char* SceneName)
	: CSceneBase(SceneName)
	//���f��.
	, P_1P(nullptr), P_2P(nullptr)
	, Ground(nullptr), StageRing(nullptr), StageBShelter(nullptr)
	, StageRingColl(nullptr), StageBShelterColl(nullptr)
	, P_Shot(nullptr), P_2Shot(nullptr)
	//�e�N�X�`��.
	, PlayerName1(nullptr), PlayerName2(nullptr)				//�L�����A�C�R��.
	, GatlingRemaining1P(nullptr), MissileRemaining1P(nullptr)	//1P������.
	, GatlingRemaining2P(nullptr), MissileRemaining2P(nullptr)	//2P������.
	, TimeMinute(nullptr), TimeBack(nullptr)					//��������.
	, TimeSecondL(nullptr), TimeSecondR(nullptr)
	//�J�n���E�I�����̃e�N�X�`��.
	, B_Start(nullptr), S_SigunalBack(nullptr)
	, B_End(nullptr), B_Finish(nullptr), E_SigunalBack(nullptr)
	//�_���[�W�e�N�X�`������.
	, G1DamageCnt(0), M1DamageCnt(0), B1DamageCnt(0), G1DamageCheck(false), M1DamageCheck(false), B1DamageCheck(false)
	, G2DamageCnt(0), M2DamageCnt(0), B2DamageCnt(0), G2DamageCheck(false), M2DamageCheck(false), B2DamageCheck(false)
	//�f�B���C�֌W
	, DelayFlag(false), DelayCnt(0)
	//�ϐ�(��������).
	, TimeUpCheck(false)
	//�ϐ�(�V�[���ؑցE���o).
	, LightScreenCheck(false), CameraMoveCnt(0)
	//�ϐ�(�J�n�E�I�����}).
	, BattleStart(false), BattleEnd(false)
	//�ϐ�(�����ꗗ�c�e���Ǘ��p).
	, GatlingReload1P(0), MissileReload1P(0)
	, GatlingReload2P(0), MissileReload2P(0)
	//�ϐ�(��������p).
	, HPCnt1P(0), HPCnt2P(0), HitCnt(0)
	, Swing1Cnt(0), Swing2Cnt(0), WallHitCnt(0), WallHit_B(false), GatlingShotCnt(0),MissileShotCnt(0)
	//�ϐ�(�G�t�F�N�g�Đ��E���o�p).
	, ExpStartCnt(0),ExpFinishCnt(0)
	//����(BGM).
	, PlayBGM(nullptr)
	//����(SE).
	, CountDown10(nullptr), BSSound(nullptr)
	, Blade1NoHit(nullptr), Blade2NoHit(nullptr)
	//�����ϐ�.
	, SESetCnt(0)
	//�f�o�b�O�p.
	, PlayDebug(nullptr)
{
	for (Li = 0; Li < LIFE_MAX; Li++)
	{
		P_1FINE[Li] = nullptr;
		P_2FINE[Li] = nullptr;
		P_1CAUTION[Li] = nullptr;
		P_2CAUTION[Li] = nullptr;
	}

	for (Li = 0; Li < PlayerMAX; Li++)
	{//�e�N�X�`�������f���������G�t�F�N�g.
		P_HPBackGround[Li] = nullptr;
		BoostGauge[Li] = nullptr;
		BoostHeat[Li] = nullptr;
		WeponBack[Li] = nullptr;

		P_Coll[Li] = NULL;

		HPCaution[Li] = nullptr;
		HPDanger[Li] = nullptr;
		BulletHit[Li] = nullptr;
		BladeHit[Li] = nullptr;
		GuardHit[Li] = nullptr;

		ExpBurn[Li] = nullptr;
	}
	//�u�[�X�g�Q�[�W.
	for (Li = 0; Li < GaugeMax; Li++)
	{
		Boost1P[Li] = nullptr;
		Boost2P[Li] = nullptr;
		BoostHeat1P[Li] = nullptr;
		BoostHeat2P[Li] = nullptr;
	}

	for (Li = 0; Li < 4; Li++)
	{
		HitTagPos[Li] = nullptr;
		StageShelter[Li] = nullptr;
		StageShelterColl[Li] = nullptr;
	}
	//�e������.
	for (Li = 0; Li < 10; Li++)
	{
		cP_Shot[Li] = nullptr;
		cP2_Shot[Li] = nullptr;
	}
	for (Li = 0; Li < 5; Li++)
	{
		mP_Shot[Li] = nullptr;
		mP2_Shot[Li] = nullptr;
	}
	//�J����.
	EEye = { 0.0f,0.0f,0.0f };
	ELook = { 0.0f,0.0f,0.0f };
}

CSceneGameMain::~CSceneGameMain()
{
	//���f���̉��.
	SAFE_DELETE(Ground);
	SAFE_DELETE(P_Shot);
	SAFE_DELETE(P_2Shot);
	SAFE_DELETE(StageRing);
	SAFE_DELETE(StageRingColl);
	SAFE_DELETE(StageBShelter);
	SAFE_DELETE(StageBShelterColl);

	for (Li = 0; Li < PlayerMAX; Li++)
	{
		SAFE_DELETE(P_Coll[Li]);
		SAFE_DELETE(BulletHit[Li]);
		SAFE_DELETE(BladeHit[Li]);
	}

	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(StageShelter[i]);
		SAFE_DELETE(StageShelterColl[i]);
	}

	for (int i = 0; i < GatlingMax; i++)
	{
		SAFE_DELETE(cP_Shot[i]);
		SAFE_DELETE(cP2_Shot[i]);
	}
	for (int i = 0; i < MissileMax; i++)
	{
		SAFE_DELETE(mP_Shot[i]);
		SAFE_DELETE(mP2_Shot[i]);
	}

	//�f�o�b�O�p�̂���.
#ifdef _DEBUG
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(HitTagPos[i]);
	}
#endif
	//�e�N�X�`���̉��.
	for (Li = 0; Li < LIFE_MAX; Li++)
	{//���C�t.
		SAFE_DELETE(P_1FINE[Li]);
		SAFE_DELETE(P_2FINE[Li]);
		SAFE_DELETE(P_1CAUTION[Li]);
		SAFE_DELETE(P_2CAUTION[Li]);
		SAFE_DELETE(P_1HITDAMAGE[Li]);
		SAFE_DELETE(P_2HITDAMAGE[Li]);
	}

	for (Li = 0; Li < PlayerMAX; Li++)
	{
		//���C�t�̔w�i.
		SAFE_DELETE(P_HPBackGround[Li]);
		//�u�[�X�g�Q�[�W�g.
		SAFE_DELETE(BoostGauge[Li]);
		//�I�[�o�[�q�[�g��.
		SAFE_DELETE(BoostHeat[Li]);
		//�����ꗗ�w�i.
		SAFE_DELETE(WeponBack[Li]);
	}

	//�u�[�X�g�Q�[�W.
	for (Li = 0; Li < GaugeMax; Li++)
	{
		//�Q�[�W�̒��̕�.
		SAFE_DELETE(Boost1P[Li]);
		SAFE_DELETE(Boost2P[Li]);
		SAFE_DELETE(BoostHeat1P[Li]);
		SAFE_DELETE(BoostHeat2P[Li]);
	}
	for (Li = 0; Li < GatlingMax; Li++)
	{
		SAFE_DELETE(cP_Shot[Li]);
		SAFE_DELETE(cP2_Shot[Li]);
	}
	for (Li = 0; Li < MissileMax; Li++)
	{
		SAFE_DELETE(mP_Shot[Li]);
		SAFE_DELETE(mP2_Shot[Li]);
	}

	//��������.
	SAFE_DELETE(TimeMinute);
	SAFE_DELETE(TimeSecondL);
	SAFE_DELETE(TimeSecondR);
	SAFE_DELETE(TimeBack);

	//�����ꗗ.
	SAFE_DELETE(GatlingRemaining1P);
	SAFE_DELETE(GatlingRemaining2P);
	SAFE_DELETE(MissileRemaining1P);
	SAFE_DELETE(MissileRemaining2P);

	//���O.
	SAFE_DELETE(PlayerName1);
	SAFE_DELETE(PlayerName2);

	//��.
	SAFE_DELETE(LightScreen);

	//�J�n���E�I����.
	SAFE_DELETE(B_Start);
	SAFE_DELETE(S_SigunalBack);
	SAFE_DELETE(B_End);
	SAFE_DELETE(E_SigunalBack);
	SAFE_DELETE(B_Finish);

	//�X�L�����b�V��.
	SAFE_DELETE(P_1P);
	SAFE_DELETE(P_2P);

	//�T�E���h�f�[�^�̊J��.
	SAFE_DELETE(PlayBGM);
	SAFE_DELETE(CountDown10);
	SAFE_DELETE(BSSound);

	SAFE_DELETE(Blade1NoHit);
	SAFE_DELETE(Blade2NoHit);

	for (Li = 0; Li < PlayerMAX; Li++)
	{
		SAFE_DELETE(HPCaution[Li]);
		SAFE_DELETE(HPDanger[Li]);
		SAFE_DELETE(BulletHit[Li]);
		SAFE_DELETE(BladeHit[Li]);
		SAFE_DELETE(GuardHit[Li]);
	}

	for (Li = 0; Li < GatlingMax; Li++)
	{
		SAFE_DELETE(GatlingSE[Li]);
		SAFE_DELETE(WallHit[Li]);
	}
	for (Li = 0; Li < MissileMax; Li++)
	{
		SAFE_DELETE(MissileSE[Li]);
	}

	//�G�t�F�N�g.
	for (Li = 0; Li < PlayerMAX; Li++)
	{
		SAFE_DELETE(ExpBurn[Li]);
	}
	for (Li = 0; Li < 10; Li++)
	{
		SAFE_DELETE(BulletExp[Li]);
	}

	//�f�o�b�O�e�L�X�g.
	SAFE_DELETE(PlayDebug);


}

//����֘A.
void CSceneGameMain::Control()
{//�L�����N�^�[�ɕR�t����f�[�^�̍X�V.
	CharaUpDate(P_1P, P_2P, 0);
	CharaUpDate(P_2P, P_1P, 1);
	ShelterUpDate();
	PlayBGM->Play();

	if (GetAsyncKeyState(VK_F1) & 0x01)
	{
		Mountain->m_vRot.y += 0.1f;
	}
	if (P_1P->BladeAtk)
	{ WeponSound(Blade1NoHit, Swing1Cnt, 133); }
	if (P_2P->BladeAtk)
	{ WeponSound(Blade2NoHit, Swing2Cnt, 133); }
	
	//�V�[���ڍs���A�O��V�[���̃A�j���[�V�����������p���Ȃ�.
	if (!P_1P->PlayMotion)
	{
		P_1P->SetChangeAnim(enStartUp, P_1P->ActState, AnimPlaySpeed);
		P_2P->SetChangeAnim(enStartUp, P_2P->ActState2, AnimPlaySpeed);
		P_1P->PlayMotion = true;
	}

	//�o�g���J�n��̏���.
	if (BattleStart && !BattleEnd )
	{//����t���O�ŏI�������鏈���œ��������Ȃ��悤�ɂ���.
		if (!P_1P->m_Stopping && !P_2P->m_Stopping)
		{
			for (Li = 0; Li < GatlingMax; Li++)
			{//�e�̏��A�b�v�f�[�g.
				BulletUpDate(P_1P, cP_Shot[Li], P_1P->ReloadG1Flag, GatlingSE[Li], Li);
				if (P_1P->ShotFlag == true)
				{
					cP_Shot[P_1P->cShotCnt]->ShotFlag = P_1P->ShotFlag;
				}
				if (Li == GatReroad)
				{
					cP_Shot[Li]->ReloadProc(P_1P, P_1P->ReloadG1Flag, P_1P->ReloadG1Cnt);
				}
			}

			for (Li = 0; Li < MissileMax; Li++)
			{//�e�̏��A�b�v�f�[�g.
				MissileUpDate(P_1P, mP_Shot[Li], P_1P->ReloadM1Flag, MissileSE[Li], Li);
				if (P_1P->H_ShotFlag == true)
				{
					mP_Shot[P_1P->mShotCnt]->H_ShotFlag = P_1P->H_ShotFlag;
				}
				if (Li == 4)
				{
					mP_Shot[Li]->M_ReloadProc(P_1P, P_1P->ReloadM1Flag, P_1P->ReloadM1Cnt);
				}
			}

			for (Li = 0; Li < GatlingMax; Li++)
			{
				Bullet2UpDate(P_2P, cP2_Shot[Li], P_2P->ReloadG2Flag, GatlingSE[Li], Li);
				if (P_2P->ShotFlag == true)
				{
					cP2_Shot[P_2P->c2ShotCnt]->ShotFlag = P_2P->ShotFlag;
				}
				if (Li == GatReroad)
				{
					cP2_Shot[Li]->Reload2Proc(P_2P, P_2P->ReloadG2Flag, P_2P->ReloadG2Cnt);
				}
			}

			for (Li = 0; Li < MissileMax; Li++)
			{
				Missile2UpDate(P_2P, mP2_Shot[Li], P_2P->ReloadM2Flag, MissileSE[Li], Li);
				if (P_2P->H_ShotFlag == true)
				{
					mP2_Shot[P_2P->m2ShotCnt]->H_ShotFlag = P_2P->H_ShotFlag;
				}
				if (Li == 4)
				{
					mP2_Shot[Li]->M_Reload2Proc(P_2P, P_2P->ReloadM2Flag, P_2P->ReloadM2Cnt);
				}
			}

#ifdef _DEBUG
			/*****************UI����******************/
			//���C�t����.
			if (GetAsyncKeyState(VK_RETURN) & 0x0001) { P_1P->m_HP -= 1; }
			if (GetAsyncKeyState(VK_SPACE) & 0x0001) { P_2P->m_HP -= 1; }
#endif //_DEBUG.

		}
	}
	else
	{//�Օ��������Ă���.
		for (Li = 0; Li < 4; Li++)
		{
			StageShelter[Li]->m_vPos.y += 0.05f;
			if (StageShelter[Li]->m_vPos.y >= -0.5f)
			{
				StageShelter[Li]->m_vPos.y = 0.0f;
			}
		}
	}

	//����.
	if (!P_1P->m_DeadFlag && !P_2P->m_DeadFlag)
	{
		P_1P->Action();							//1P���̑���..
		P_1P->SlashStartProc(P_2P);

		P_2P->Action2();						//2P���̑���.
		P_2P->SlashStartProc2(P_1P);

	}

	//�A�j���[�V��������.
	P_1P->AnimControll(P_1P->ActState);
	P_2P->AnimControll(P_2P->ActState2);

}

//���C����������.
void CSceneGameMain::Action(SceneIndex &Scene, clsDInput *Controller)
{
	S_WhiteChange(LightScreen);
	//�o�g���J�n��ɓ���������.
	if (BattleStart && !BattleEnd)
	{	//�����ꗗ���l�ύX.
		ArmedList();
		//SE�Đ�.
		WarningHP(P_1P->m_HP, HPCaution[0], HPDanger[0], HPCnt1P);
		WarningHP(P_2P->m_HP, HPCaution[1], HPDanger[1], HPCnt2P);
		//��������.
		TimeCountDown();
		//�v���C���[�̊e����.
		PlayerAllColl();
		//���݂��Ɍ������킹��.
		PlayerRotation(P_1P, P_2P);
		//���������̏���(1230:4�����̂�).
		FlontRotation(P_1P);
		FlontRotation(P_2P);
		BoostGaugeUpDown(P_1P, GaugeMax);
		BoostGaugeUpDown(P_2P, GaugeMax);
	}
	//�����G�t�F�N�g�E�v���C���[���S����.
	ExpEff(P_1P, P_1P->Exp, ExpBurn[0]);
	ExpEff(P_2P, P_2P->Exp, ExpBurn[1]);

	//���S����.
	P_1P->DeadProc(P_1P->ActState);
	P_2P->DeadProc(P_2P->ActState2);

	//�V�[���ڍs.
	ChangeScene(Scene);
}

/**********���f���Ǎ��E�`��**********/
//X���f��.
void CSceneGameMain::Load(clsResource *SetUP)
{
	LoadPos("Data\\csv\\ModelPos\\Play\\XModelPos.csv", ModelPosNo);
	int PLoadNo = 0;

	Ground = new clsObject;
	Ground->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Ground));
	Ground->SetPosition(LoadMPos[PLoadNo]);
	Ground->SetScale(1.0f);
	PLoadNo++;

	StageRing = new clsObject;
	StageRing->AttachModel(SetUP->GetStaticMesh(enStaticMesh_StageRing));
	StageRing->SetPosition(LoadMPos[PLoadNo]);
	StageRing->SetScale(3.7f);

	StageRingColl = new clsObject;
	StageRingColl->AttachModel(SetUP->GetStaticMesh(enStaticMesh_StageRing));
	StageRingColl->SetScale(3.6f);
	StageRingColl->InitBBox();

	PLoadNo++;

	float tmplate = 0.0f;
	for (Li = 0; Li < GatlingMax; Li++)
	{
		cP_Shot[Li] = new clsPlayer;
		cP_Shot[Li]->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Shot));
		cP_Shot[Li]->SetScale(10.0f);
		cP_Shot[Li]->InitBBox();
		cP_Shot[Li]->m_Yaw = 0.0f;
		cP2_Shot[Li] = new clsPlayer;
		cP2_Shot[Li]->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Shot));
		cP2_Shot[Li]->SetScale(10.0f);
		cP2_Shot[Li]->m_Yaw = 0.0f;
		cP2_Shot[Li]->InitBBox();

		cP_Shot[Li]->m_vPos = cP2_Shot[Li]->m_vPos = D3DXVECTOR3(0.0f, -50.0f, -70.0f);
	}

	tmplate = 0.0f;
	for (Li = 0; Li < MissileMax; Li++)
	{
		mP_Shot[Li] = new clsPlayer;
		mP_Shot[Li]->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Shot));
		mP_Shot[Li]->SetScale(20.0f);
		mP_Shot[Li]->InitBBox();
		mP2_Shot[Li] = new clsPlayer;
		mP2_Shot[Li]->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Shot));
		mP2_Shot[Li]->SetScale(20.0f);
		mP2_Shot[Li]->InitBBox();

		mP_Shot[Li]->m_Yaw = mP2_Shot[Li]->m_Yaw = 0.0f;
		mP_Shot[Li]->m_vPos = mP2_Shot[Li]->m_vPos = D3DXVECTOR3(0.0f, -50.0f, -70.0f);
		tmplate += 2.0f;
	}

	for (Li = 0; Li < PlayerMAX; Li++)
	{
		P_Coll[Li] = new clsPlayer;
		P_Coll[Li]->AttachModel(SetUP->GetStaticMesh(enStaticMesh_P_Coll));
		P_Coll[Li]->m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		P_Coll[Li]->SetScale(0.25f);
		P_Coll[Li]->InitBBox();

		P_AttackColl[Li] = new clsPlayer;
		P_AttackColl[Li]->AttachModel(SetUP->GetStaticMesh(enStaticMesh_P_AttackColl));
		P_AttackColl[Li]->SetScale(0.16f);
		P_AttackColl[Li]->InitBBox();
	}

	for (Li = 0; Li < PlayerMAX; Li++)
	{
		P_GuardColl[Li] = new clsPlayer;
		P_GuardColl[Li]->AttachModel(SetUP->GetStaticMesh(enStaticMesh_P_Coll));
		P_GuardColl[Li]->SetScale(0.26f);
		P_GuardColl[Li]->InitBBox();
	}

	float YRotShelter = 11.0f;	//��]�l�����p.
	for (Li = 0; Li < ShelterMax; Li++)
	{
		StageShelter[Li] = new clsObject;
		StageShelter[Li]->AttachModel(SetUP->GetStaticMesh(enStaticMesh_StageLShelter));
		StageShelter[Li]->m_vRot  = D3DXVECTOR3(0.0f, YRotShelter, 0.0f);
		StageShelter[Li]->SetScale(2.0f);
		StageShelter[Li]->m_Yaw = StageShelter[Li]->m_vRot.y;
		StageShelter[Li]->SetPosition(LoadMPos[PLoadNo]);

		StageShelterColl[Li] = new clsObject;
		StageShelterColl[Li]->AttachModel(SetUP->GetStaticMesh(enStaticMesh_StageLShelter));
		StageShelterColl[Li]->SetRot(StageShelter[Li]->m_vRot);
		StageShelterColl[Li]->SetScale(2.2f);
		StageShelterColl[Li]->m_Yaw = StageShelter[Li]->m_vRot.y;
		StageShelterColl[Li]->InitBBox();
		YRotShelter += 11.0f;
		PLoadNo++;
	}

#ifdef _DEBUG
	for (int i = 0; i < 4; i++)
	{
		HitTagPos[i] = new clsPlayer;
		HitTagPos[i]->AttachModel(SetUP->GetStaticMesh(enStaticMesh_P_WallColl));
		HitTagPos[i]->SetScale(1.0f);
		HitTagPos[i]->InitBBox();
	}
#endif
	PLoadNo = 0;
}
//2D�e�N�X�`��.
void CSceneGameMain::Load(cls2DTexResource *SetUP)
{
	float temp = 1.0f;
	//HP�Q�[�W.
	for (Li = 0; Li < LIFE_MAX; Li++)
	{
		P_1FINE[Li] = new cls2DTex;
		P_1FINE[Li]->AttachTex(SetUP->GetStaticTex(enUITexHP1_FINE));
		P_1FINE[Li]->m_vPos = SetUP->TexPos(enUITexHP1_FINE);
		P_1FINE[Li]->m_vPos.x = P_1FINE[Li]->m_vPos.x + temp;

		P_2FINE[Li] = new cls2DTex;
		P_2FINE[Li]->AttachTex(SetUP->GetStaticTex(enUITexHP2_FINE));
		P_2FINE[Li]->m_vPos = SetUP->TexPos(enUITexHP2_FINE);//
		P_2FINE[Li]->m_vPos.x = P_2FINE[Li]->m_vPos.x + temp;

		temp += 2.8f;
	}

	temp = 1.0f;

	for (Li = 0; Li < Caution; Li++)
	{
		P_1CAUTION[Li] = new cls2DTex;
		P_1CAUTION[Li]->AttachTex(SetUP->GetStaticTex(enUITexHP_CAUTION));
		P_1CAUTION[Li]->m_vPos = P_1FINE[Li]->m_vPos;
		P_2CAUTION[Li] = new cls2DTex;
		P_2CAUTION[Li]->AttachTex(SetUP->GetStaticTex(enUITexHP_CAUTION));
		P_2CAUTION[Li]->m_vPos = P_2FINE[Li]->m_vPos;
	}

	for (Li = 0; Li < LIFE_MAX; Li++)
	{
		P_1HITDAMAGE[Li] = new cls2DTex;
		P_1HITDAMAGE[Li]->AttachTex(SetUP->GetStaticTex(enUITexHP_DAMAGE));
		P_1HITDAMAGE[Li]->m_vPos = SetUP->TexPos(enUITexHP_DAMAGE);
		P_1HITDAMAGE[Li]->m_vPos.x = P_1FINE[Li]->m_vPos.x;

		P_2HITDAMAGE[Li] = new cls2DTex;
		P_2HITDAMAGE[Li]->AttachTex(SetUP->GetStaticTex(enUITexHP_DAMAGE));
		P_2HITDAMAGE[Li]->m_vPos = P_2FINE[Li]->m_vPos;
		P_2HITDAMAGE[Li]->m_vPos.x = P_2FINE[Li]->m_vPos.x;

		temp += 2.8f;
	}

	//���C�t�̔w�i.
	for (Li = 0; Li < PlayerMAX; Li++)
	{
		P_HPBackGround[Li] = new cls2DTex;
		BoostGauge[Li] = new cls2DTex;

		if (Li == 0)
		{
			P_HPBackGround[Li]->AttachTex(SetUP->GetStaticTex(enUITexHP1_Bar));
			P_HPBackGround[Li]->m_vPos = SetUP->TexPos(enUITexHP1_Bar);
			BoostGauge[Li]->AttachTex(SetUP->GetStaticTex(enUITex1Boost));
			BoostGauge[Li]->m_vPos = SetUP->TexPos(enUITex1Boost);

		}
		else
		{
			P_HPBackGround[Li]->AttachTex(SetUP->GetStaticTex(enUITexHP2_Bar));
			P_HPBackGround[Li]->m_vPos = SetUP->TexPos(enUITexHP2_Bar);
			BoostGauge[Li]->AttachTex(SetUP->GetStaticTex(enUITex2Boost));
			BoostGauge[Li]->m_vPos = SetUP->TexPos(enUITex2Boost);

		}
	}


	PlayerName1 = new cls2DTex;
	PlayerName1->AttachTex(SetUP->GetStaticTex(enUITexNamePrate1P));
	PlayerName1->m_vPos = SetUP->TexPos(enUITexNamePrate1P);

	PlayerName2 = new cls2DTex;
	PlayerName2->AttachTex(SetUP->GetStaticTex(enUITexNamePrate2P));
	PlayerName2->m_vPos = SetUP->TexPos(enUITexNamePrate2P);

	//�L�����u�[�X�g�Q�[�W.
	for (Li = 0; Li < PlayerMAX; Li++)
	{
	}
	//���̃Q�[�W.
	temp = 1.0f;
	for (Li = 0; Li < GaugeMax; Li++)
	{	//�ʏ�Q�[�W.
		Boost1P[Li] = new cls2DTex;
		Boost1P[Li]->AttachTex(SetUP->GetStaticTex(enUITex1Gauge));
		Boost1P[Li]->m_vPos = SetUP->TexPos(enUITex1Gauge);
		Boost1P[Li]->m_vPos.x = Boost1P[Li]->m_vPos.x + temp;
		temp = temp + 2.83f;
		//�I�[�o�[�q�[�g�Q�[�W.
		BoostHeat1P[Li] = new cls2DTex;
		BoostHeat1P[Li]->AttachTex(SetUP->GetStaticTex(enUITexGaugeHeat));
		BoostHeat1P[Li]->m_vPos = Boost1P[Li]->m_vPos;
		BoostHeat1P[Li]->m_Alpha = 0.7f;
	}

	for (Li = 0; Li < GaugeMax; Li++)
	{
		//�ʏ�Q�[�W.
		Boost2P[Li] = new cls2DTex;
		Boost2P[Li]->AttachTex(SetUP->GetStaticTex(enUITex2Gauge));
		Boost2P[Li]->m_vPos = SetUP->TexPos(enUITex2Gauge);
		Boost2P[Li]->m_vPos.x = Boost2P[Li]->m_vPos.x + temp;
		temp = temp + 2.83f;
		//�I�[�o�[�q�[�g�Q�[�W.
		BoostHeat2P[Li] = new cls2DTex;
		BoostHeat2P[Li]->AttachTex(SetUP->GetStaticTex(enUITexGaugeHeat));
		BoostHeat2P[Li]->m_vPos = Boost2P[Li]->m_vPos;
		BoostHeat2P[Li]->m_Alpha = 0.7f;
	}

	//�L�����I�[�o�[�q�[�g(�Q�[�W�����w�i).
	for (Li = 0; Li < PlayerMAX; Li++)
	{
		BoostHeat[Li] = new cls2DTex;
		BoostHeat[Li]->AttachTex(SetUP->GetStaticTex(enUITexHeat));
		BoostHeat[Li]->m_vPos = BoostGauge[Li]->m_vPos;
	}

	//�����ꗗ����(�K�g�����O�E�~�T�C���̏�).
	GatlingRemaining1P = new cls2DTex;
	GatlingRemaining1P->AttachTex(SetUP->GetStaticTex(enUIShoot1PMaWeponTex));
	GatlingRemaining1P->m_vPos = SetUP->TexPos(enUIShoot1PMaWeponTex);
	GatlingRemaining1P->m_PatternNo.x = 0.0f;

	GatlingRemaining2P = new cls2DTex;
	GatlingRemaining2P->AttachTex(SetUP->GetStaticTex(enUIShoot2PMaWeponTex));
	GatlingRemaining2P->m_vPos = SetUP->TexPos(enUIShoot2PMaWeponTex);
	GatlingRemaining2P->m_PatternNo.x = 0.0f;

	MissileRemaining1P = new cls2DTex;
	MissileRemaining1P->AttachTex(SetUP->GetStaticTex(enUIShoot1PMiWeponTex));
	MissileRemaining1P->m_vPos = SetUP->TexPos(enUIShoot1PMiWeponTex);
	MissileRemaining1P->m_PatternNo.x = 5.0f;

	MissileRemaining2P = new cls2DTex;
	MissileRemaining2P->AttachTex(SetUP->GetStaticTex(enUIShoot2PMiWeponTex));
	MissileRemaining2P->m_vPos = SetUP->TexPos(enUIShoot2PMiWeponTex);
	MissileRemaining2P->m_PatternNo.x = 5.0f;

	//�e�̃����[�h�\��.
	for (int i = 0; i < BulletMax; i++)
	{
		WeponReload[i] = new cls2DTex;
		WeponReload[i]->AttachTex(SetUP->GetStaticTex(enUIWeponReloading));
		WeponReload[i]->m_vPos = SetUP->TexPos(enUIWeponReloading);
	}

	for (Li = 0; Li < PlayerMAX; Li++)
	{
		WeponBack[Li] = new cls2DTex;
		if (Li == 0)
		{
			WeponBack[Li]->AttachTex(SetUP->GetStaticTex(enUIWepon1PBack));
			WeponBack[Li]->m_vPos = SetUP->TexPos(enUIWepon1PBack);
		}
		else
		{
			WeponBack[Li]->AttachTex(SetUP->GetStaticTex(enUIWepon2PBack));
			WeponBack[Li]->m_vPos = SetUP->TexPos(enUIWepon2PBack);
		}
	}

	//��������.
	//��.
	TimeMinute = new cls2DTex;
	TimeMinute->AttachTex(SetUP->GetStaticTex(enUITimeMi));
	TimeMinute->m_vPos = SetUP->TexPos(enUITimeMi);
	TimeMinute->m_PatternNo.x = 5.0f;
	//�b.
	TimeSecondL = new cls2DTex;
	TimeSecondL->AttachTex(SetUP->GetStaticTex(enUITimeSe1));
	TimeSecondL->m_vPos = SetUP->TexPos(enUITimeSe1);
	TimeSecondL->m_PatternNo.x = 0.0f;

	TimeSecondR = new cls2DTex;
	TimeSecondR->AttachTex(SetUP->GetStaticTex(enUITimese2));
	TimeSecondR->m_vPos = SetUP->TexPos(enUITimese2);
	TimeSecondR->m_PatternNo.x = 0.0f;

	TimeBack = new cls2DTex;
	TimeBack->AttachTex(SetUP->GetStaticTex(enUITimeBackTex));
	TimeBack->m_vPos = SetUP->TexPos(enUITimeBackTex);//D3DXVECTOR3(576.0f, 5.0f, 0.0f);

	//�X�^�[�g���}.
	B_Start = new cls2DTex;
	B_Start->AttachTex(SetUP->GetStaticTex(enSceneUIStart));
	B_Start->m_vPos = SetUP->TexPos(enSceneUIStart);
	B_Start->m_Alpha = 1.0f;

	S_SigunalBack = new cls2DTex;
	S_SigunalBack->AttachTex(SetUP->GetStaticTex(enSceneUISigunalBack));
	S_SigunalBack->m_vPos = SetUP->TexPos(enSceneUISigunalBack);
	S_SigunalBack->m_Alpha = 1.0f;

	//�G���h���}.
	B_End = new cls2DTex;
	B_End->AttachTex(SetUP->GetStaticTex(enSceneUIEnd));
	B_End->m_vPos = SetUP->TexPos(enSceneUIEnd);
	B_End->m_Alpha = 1.0f;
	B_End->m_DispFlag = false;

	B_Finish = new cls2DTex;
	B_Finish->AttachTex(SetUP->GetStaticTex(enSceneUIFinish));
	B_Finish->m_vPos = SetUP->TexPos(enSceneUIFinish);
	B_Finish->m_Alpha = 1.0f;
	B_Finish->m_DispFlag = false;

	E_SigunalBack = new cls2DTex;
	E_SigunalBack->AttachTex(SetUP->GetStaticTex(enSceneUISigunalBack));
	E_SigunalBack->m_vPos = SetUP->TexPos(enSceneUISigunalBack);
	E_SigunalBack->m_Alpha = 1.0f;
	E_SigunalBack->m_DispFlag = false;

	//�Ö��E����.
	LightScreen = new cls2DTex;
	LightScreen->AttachTex(SetUP->GetStaticTex(enScreenWhite));
	LightScreen->m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	LightScreen->m_Alpha = 1.0f;
}
//�X�L�����b�V���p.
void CSceneGameMain::Load(clsSkinResource *SetUP, clsDInput *Controller
	, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd)
{
	LoadPos(
		"Data\\csv\\ModelPos\\Play\\SkinModelPos.csv", SkinMax);
	LoadRot(
		"Data\\csv\\ModelPos\\Play\\SkinModelRot.csv", SkinMax);
	
	int PLoadNo = 0;

	P_1P = new clsPlayer;
	P_1P->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_White));
	P_1P->Init();
	P_1P->SetPosition(LoadMPos[PLoadNo]);
	P_1P->SetSkinSize(SkinSize);
	P_1P->SetRot(LoadMRot[PLoadNo]);
	P_1P->Setgamepad(Controller);

	PLoadNo++;

	P_2P = new clsPlayer;
	P_2P->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_Black));
	P_2P->Init();
	P_2P->SetPosition(LoadMPos[PLoadNo]);
	P_2P->SetSkinSize(SkinSize);
	P_2P->SetRot(LoadMRot[PLoadNo]);
	P_2P->Setgamepad2(Controller);

	for (Li = 0; Li < PlayerMAX; Li++)
	{
		//���C�̏�����(����)
		m_pRayV[Li] = new clsRay;
		m_pRayV[Li]->m_Ray.vPoint[0] = D3DXVECTOR3(0.0f, -5.0f, 0.0f);
		m_pRayV[Li]->m_Ray.vPoint[1] = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		m_pRayV[Li]->Init(pDevice11, pContext11);

		//���C�̏�����(�O��)
		m_pRayFB[Li] = new clsRay;
		m_pRayFB[Li]->m_Ray.vPoint[0] = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
		m_pRayFB[Li]->m_Ray.vPoint[1] = D3DXVECTOR3(0.0f, 0.0f, 5.0f);
		m_pRayFB[Li]->Init(pDevice11, pContext11);

		//���C�̏�����(���E)
		m_pRayH[Li] = new clsRay;
		m_pRayH[Li]->m_Ray.vPoint[0] = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
		m_pRayH[Li]->m_Ray.vPoint[1] = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
		m_pRayH[Li]->Init(pDevice11, pContext11);
	}

	PLoadNo = 0;
}
//�G�t�F�N�g�p.
void CSceneGameMain::Load(clsEffectsResouce* SetUP)
{
	for (Li = 0; Li < PlayerMAX; Li++)
	{
		ExpBurn[Li] = new clsEffectsMain;
		ExpBurn[Li]->AttachEffect(SetUP->GetEffects(enEffects_Exp));
	}

	for (Li = 0; Li < 10; Li++)
	{
		BulletExp[Li] = new clsEffectsMain;
		BulletExp[Li]->AttachEffect(SetUP->GetEffects(enEffects_ExpBullet));
	}

}
//�T�E���h�p.
void CSceneGameMain::Load(clsSoundResource* SetUP)
{
	PlayBGM = new clsDirectSound;
	PlayBGM->AttachData(SetUP->GetSound(enStaticBGM_Play));
	PlayBGM->ChangeDSB(PlayBGM);

	BSSound = new clsDirectSound;
	BSSound->AttachData(SetUP->GetSound(enStaticSE_BattleStart));
	BSSound->ChangeDSB(BSSound);

	for (Li = 0; Li < PlayerMAX; Li++)
	{	//HP�̒ʒm��.
		HPCaution[Li] = new clsDirectSound;
		HPCaution[Li]->AttachData(SetUP->GetSound(enStaticSE_Caution));
		HPCaution[Li]->ChangeDSB(HPCaution[Li]);
		HPDanger[Li] = new clsDirectSound;
		HPDanger[Li]->AttachData(SetUP->GetSound(enStaticSE_Danger));
		HPDanger[Li]->ChangeDSB(HPDanger[Li]);
		//�e�����q�b�g��.
		BulletHit[Li] = new clsDirectSound;
		BulletHit[Li]->AttachData(SetUP->GetSound(enStaticSE_Hit));
		BulletHit[Li]->ChangeDSB(BulletHit[Li]);

		BladeHit[Li] = new clsDirectSound;
		BladeHit[Li]->AttachData(SetUP->GetSound(enStaticSE_BladeHit));
		BladeHit[Li]->ChangeDSB(BladeHit[Li]);

		GuardHit[Li] = new clsDirectSound;
		GuardHit[Li]->AttachData(SetUP->GetSound(enStaticSE_GuardHit));
		GuardHit[Li]->ChangeDSB(GuardHit[Li]);
	}
	Blade1NoHit = new clsDirectSound;
	Blade1NoHit->AttachData(SetUP->GetSound(enStaticSE_BladeNo1Hit));
	Blade1NoHit->ChangeDSB(Blade1NoHit);

	Blade2NoHit = new clsDirectSound;
	Blade2NoHit->AttachData(SetUP->GetSound(enStaticSE_BladeNo2Hit));
	Blade2NoHit->ChangeDSB(Blade2NoHit);

	CountDown10 = new clsDirectSound;
	CountDown10->AttachData(SetUP->GetSound(enStaticSE_TimeCount));
	CountDown10->ChangeDSB(CountDown10);

	for (Li = 0; Li < GatlingMax; Li++)
	{
		GatlingSE[Li] = new clsDirectSound;
		GatlingSE[Li]->AttachData(SetUP->GetSound(enStaticSE_Gatling));
		GatlingSE[Li]->ChangeDSB(GatlingSE[Li]);

		WallHit[Li] = new clsDirectSound;
		WallHit[Li]->AttachData(SetUP->GetSound(enStaticSE_WallHit));
		WallHit[Li]->ChangeDSB(WallHit[Li]);
	}
	for (Li = 0; Li < MissileMax; Li++)
	{
		MissileSE[Li] = new clsDirectSound;
		MissileSE[Li]->AttachData(SetUP->GetSound(enStaticSE_Missile));
		MissileSE[Li]->ChangeDSB(MissileSE[Li]);
	}

	//���̂������Ɠ���ڈȍ~��Buf�̒��g���Ⴂ��O���o��.
	//PlayBGM = SetUP->GetSound(enStaticBGM_Play);	
}
//�`��(X���f���E2D�e�N�X�`���E�X�L�����b�V�����f��).
void CSceneGameMain::Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss)
{
	//-----------���b�V�����f��------------.
	Ground->Render(mView, mProj, vLight, vEye);
	//Mountain->Render(mView, mProj, vLight, vEye);

	for (Li = 0; Li < ShelterMax; Li++)
	{
		StageShelter[Li]->Render(mView, mProj, vLight, vEye);
	}
	StageRing->Render(mView, mProj, vLight, vEye);

	for (Li = 0; Li < GatlingMax; Li++)
	{
		cP_Shot[Li]->Render(mView, mProj, vLight, vEye);
		cP2_Shot[Li]->Render(mView, mProj, vLight, vEye);
	}

	for (Li = 0; Li < MissileMax; Li++)
	{
		mP_Shot[Li]->Render(mView, mProj, vLight, vEye);
		mP2_Shot[Li]->Render(mView, mProj, vLight, vEye);
	}

	//--------�X�L�����b�V�����f��---------.
	P_1P->SkinRender(mView, mProj, vLight, vEye);
	P_2P->SkinRender(mView, mProj, vLight, vEye);


	//-------------�e�N�X�`��--------------.
	if (LightScreen->m_DispFlag == true)
	{ LightScreen->TexRender(ss, LightScreen->m_PatternNo); }

	if (B_End->m_DispFlag == true)
		B_End->TexRender(ss, B_End->m_PatternNo);
	if (B_Finish->m_DispFlag == true)
		B_Finish->TexRender(ss, B_Finish->m_PatternNo);
	if (E_SigunalBack->m_DispFlag == true)
		E_SigunalBack->TexRender(ss, E_SigunalBack->m_PatternNo);

	//��ɏ��������ɕ`��.
	if (B_Start->m_DispFlag == true)	
		B_Start->TexRender(ss, B_Start->m_PatternNo);
	if(S_SigunalBack->m_DispFlag == true)	
		S_SigunalBack->TexRender(ss, S_SigunalBack->m_PatternNo);

	PlayerName1->TexRender(ss,PlayerName1->m_PatternNo);
	PlayerName2->TexRender(ss,PlayerName2->m_PatternNo);

	//���C�t�̕`��ݒ�.
	//1P.
	if (P_1P->m_HP <= Caution)
	{//�ΐF.
		for (Li = 0; Li < P_1P->m_HP; Li++)
		{
			P_1CAUTION[Li]->TexRender(ss, P_1CAUTION[Li]->m_PatternNo);

			if (!HP_ChackAlpha[0])
			{
				P_1CAUTION[Li]->m_Alpha -= 0.01f;
				if (P_1CAUTION[Li]->m_Alpha < 0.6f)
				{
					P_1CAUTION[Li]->m_Alpha = 0.6f;
					HP_ChackAlpha[0] = true;
				}
			}
			else
			{
				P_1CAUTION[Li]->m_Alpha += 0.01f;
				if (1.0f < P_1CAUTION[Li]->m_Alpha)
				{
					P_1CAUTION[Li]->m_Alpha = 1.0f;
					HP_ChackAlpha[0] = false;
				}
			}

		}
	}
	else
	{
		for (Li = 0; Li < P_1P->m_HP; Li++)
		{
			P_1FINE[Li]->TexRender(ss, P_1FINE[Li]->m_PatternNo);
		}
	}
	for (Li = 0; Li < P_1P->m_DamageHP; Li++)
	{
		P_1HITDAMAGE[Li]->TexRender(ss, P_1HITDAMAGE[Li]->m_PatternNo);
	}

	//2P.
	if (P_2P->m_HP <= Caution)
	{//�ΐF.
		for (Li = 0; Li < P_2P->m_HP; Li++)
		{
			P_2CAUTION[Li]->TexRender(ss, P_2CAUTION[Li]->m_PatternNo);

			if (!HP_ChackAlpha[1])
			{
				P_2CAUTION[Li]->m_Alpha -= 0.01f;
				if (P_2CAUTION[Li]->m_Alpha < 0.6f)
				{
					P_2CAUTION[Li]->m_Alpha = 0.6f;
					HP_ChackAlpha[1] = true;
				}
			}
			else
			{
				P_2CAUTION[Li]->m_Alpha += 0.01f;
				if (1.0f < P_2CAUTION[Li]->m_Alpha)
				{
					P_2CAUTION[Li]->m_Alpha = 1.0f;
					HP_ChackAlpha[1] = false;
				}
			}

		}
	}
	else
	{
		for (Li = 0; Li < P_2P->m_HP; Li++)
		{
			P_2FINE[Li]->TexRender(ss, P_2FINE[Li]->m_PatternNo);
		}
	}

	for (Li = 0; Li < P_2P->m_DamageHP; Li++)
	{
		P_2HITDAMAGE[Li]->TexRender(ss, P_2HITDAMAGE[Li]->m_PatternNo);
	}

	//�u�[�X�g�Q�[�W(���̕�).
	for (Li = 0; Li < GaugeMax - P_1P->BoostCnt; Li++)
	{
		if (P_1P->BoostEmpty)
		{ BoostHeat1P[Li]->TexRender(ss,BoostHeat1P[Li]->m_PatternNo); }
		else
		{ Boost1P[Li]->TexRender(ss,Boost1P[Li]->m_PatternNo);	}
	}
	for (Li = 0; Li < GaugeMax - P_2P->BoostCnt; Li++)
	{
		if (P_2P->BoostEmpty)
		{ BoostHeat2P[Li]->TexRender(ss,BoostHeat2P[Li]->m_PatternNo);	}
		else
		{ Boost2P[Li]->TexRender(ss,Boost2P[Li]->m_PatternNo);	}
	}

	for (int i = 0; i < BulletMax; i++)
	{
		WeponReload[i]->TexRender(ss, WeponReload[i]->m_PatternNo);
	}
	//�K�g�����O�E�~�T�C���c�e��.
	GatlingRemaining1P->TexRender(ss, GatlingRemaining1P->m_PatternNo);
	GatlingRemaining2P->TexRender(ss, GatlingRemaining2P->m_PatternNo);
	MissileRemaining1P->TexRender(ss, MissileRemaining1P->m_PatternNo);
	MissileRemaining2P->TexRender(ss, MissileRemaining2P->m_PatternNo);

	//�������ԁ@���E�b.
	TimeMinute->TexRender(ss, TimeMinute->m_PatternNo);
	TimeSecondL->TexRender(ss, TimeSecondL->m_PatternNo);
	TimeSecondR->TexRender(ss, TimeSecondR->m_PatternNo);

	//UI�w�i����(��ԉ��ɕ`��).
	for (Li = 0; Li < PlayerMAX; Li++)
	{
		if (P_1P->BoostEmpty)
		{ BoostHeat[0]->TexRender(ss,BoostHeat[Li]->m_PatternNo); }

		if (P_2P->BoostEmpty)
		{ BoostHeat[1]->TexRender(ss, BoostHeat[Li]->m_PatternNo);}

		P_HPBackGround[Li]->TexRender(ss,P_HPBackGround[Li]->m_PatternNo);
		BoostGauge[Li]->TexRender(ss,BoostGauge[Li]->m_PatternNo);
		WeponBack[Li]->TexRender(ss, WeponBack[Li]->m_PatternNo);
	}
	TimeBack->TexRender(ss, TimeBack->m_PatternNo);

	//�G�t�F�N�g.
	for (Li = 0; Li < 10; Li++)
	{ BulletExp[Li]->Render(mView, mProj, vLight, vEye); }

	for (Li = 0; Li < PlayerMAX; Li++)
	{
		ExpBurn[Li]->Render(mView, mProj, vLight, vEye);
	}

	/*�ȉ�����`�F�b�N�p���f���̕`��*/
#ifdef _DEBUG
	//���f���{�̂̓����蔻�蓤��.
	for (Li = 0; Li < PlayerMAX; Li++)
	{
		P_Coll[Li]->Render(mView, mProj, vLight, vEye);
	}

	if(P_1P->Gurad)
	{
		P_GuardColl[0]->Render(mView, mProj, vLight, vEye);
	}

	if (P_2P->Gurad)
	{
		P_GuardColl[1]->Render(mView, mProj, vLight, vEye);
	}

	if (P_1P->BladeAtk && !P_2P->HitFlag)
	{
		P_AttackColl[0]->Render(mView, mProj, vLight, vEye);
	}
	if (P_2P->BladeAtk && !P_1P->HitFlag)
	{
		P_AttackColl[1]->Render(mView, mProj, vLight, vEye);
	}



	//�����蔻��(��)�̔���ӏ��\���p.
	for (int i = 0; i < 4; i++)
	{
		HitTagPos[i]->SetPosition(P_1P->m_tagHitPos.vPos[i]);
		HitTagPos[i]->Render(mView, mProj, vLight, vEye);
	}


//���C�E�f�o�b�O�e�L�X�g.
	//���C(Vertex).
	//�ʒu�����@�ɍ��킹��.
	for (Li = 0; Li < PlayerMAX; Li++)
	{
		m_pRayV[Li]->m_Ray.vPos =  P_Coll[Li]->m_vPos;
		//��]�ʒu�����@�ɍ��킹��.	 
		m_pRayV[Li]->m_Ray.Yaw =   P_Coll[Li]->m_Yaw;
		//���C(FrontBack).		   
		m_pRayFB[Li]->m_Ray.vPos = P_Coll[Li]->m_vPos;
		m_pRayFB[Li]->m_Ray.Yaw =  P_Coll[Li]->m_Yaw;
		//���C(Height).			  
		m_pRayH[Li]->m_Ray.vPos =  P_Coll[Li]->m_vPos;
		m_pRayH[Li]->m_Ray.Yaw =   P_Coll[Li]->m_Yaw;
		//���C�̕\��.
		m_pRayV[Li]->Render(mView, mProj);
		m_pRayFB[Li]->Render(mView, mProj);
		m_pRayH[Li]->Render(mView, mProj);
	}

	//�f�o�b�O�e�L�X�g(DebugText).
	char strDebugText[256];
	char strDebugTextS[256];
	char strDebugTextAnim[256];
	char strDebugTextEX[256];
	char strDebugTextFlag[256];

	sprintf_s(strDebugText, "Axis x%f y%f z%f",
		P_1P->AxsizM.x, P_1P->AxsizM.y, P_1P->AxsizM.z);
	sprintf_s(strDebugTextS, "Rot x%f y%f z%f",
		P_1P->m_vRot.x, P_1P->m_vRot.y, P_1P->m_vRot.z);
	sprintf_s(strDebugTextAnim, "Pos x%f y%f z%f",
		P_1P->m_vPos.x, P_1P->m_vPos.y, P_1P->m_vPos.z);


	//�e�t���O�m�F�p�e�L�X�g
	sprintf_s(strDebugTextEX, " AnimNo:%d %d", P_1P->ActState,P_2P->ActState2);
	sprintf_s(strDebugTextFlag, " S2Flag [0]%d [1]%d [2]%d [3]%d [4]%d [5]%d [6]%d [7]%d [8]%d [9]%d [10]%d",
		cP2_Shot[0]->ShotFlag, cP2_Shot[1]->ShotFlag, cP2_Shot[2]->ShotFlag, cP2_Shot[3]->ShotFlag, cP2_Shot[4]->ShotFlag,
		cP2_Shot[5]->ShotFlag, cP2_Shot[6]->ShotFlag, cP2_Shot[7]->ShotFlag, cP2_Shot[8]->ShotFlag, cP2_Shot[9]->ShotFlag, cP2_Shot[10]->ShotFlag);


	//�f�o�b�O�e�L�X�g.
	PlayDebug->Render(strDebugText, 5.0f, 100.0f);
	PlayDebug->Render(strDebugTextS, 5.0f, 140.0f);
	PlayDebug->Render(strDebugTextAnim, 5.0f, 180.0f);

	PlayDebug->Render(strDebugTextEX, 5.0f, 260.0f);

#endif
}
//�V�[���ڍs�E�o�g���I�������֐�.
void CSceneGameMain::ChangeScene(SceneIndex &Scene)
{
	//2P����.
	if (P_1P->m_DeadFlag && !P_2P->m_DeadFlag)
	{	//P1 ���S.
		if (130 < ExpStartCnt)
		{
			if (P_1P->Exp)
			{
				BattleEnd = true;
				B_Finish->m_DispFlag = true;
				E_SigunalBack->m_DispFlag = true;
				TexSigunal(E_SigunalBack, E_SigunalBack->m_Alpha, B_Finish, B_Finish->m_Alpha);
				if (E_SigunalBack->m_Alpha <= 0.0f && B_Finish->m_Alpha <= 0.0f)
				{
					LightScreen->m_DispFlag = true;
					LightScreen->m_Alpha += 0.01f;
					PlayBGM->Stop();
					if (1.0f < LightScreen->m_Alpha)
					{
						ExpStartCnt = 0;
						P_1P->ActState = enStartUp;
						P_1P->SetChangeAnim(enStartUp, P_1P->ActState, AnimPlaySpeed);
						P_2P->ActState2 = enStartUp;
						P_2P->SetChangeAnim(enStartUp, P_2P->ActState2, AnimPlaySpeed);
						PlayBGM->Stop();
						Scene = SceneIndex::P2WIN;
					}
				}
			}
		}
	}

	//1P����.
	else if (!P_1P->m_DeadFlag && P_2P->m_DeadFlag)
	{
		if (130 < ExpStartCnt)
		{

			if (P_2P->Exp)
			{
				BattleEnd = true;
				B_Finish->m_DispFlag = true;
				E_SigunalBack->m_DispFlag = true;
				TexSigunal(E_SigunalBack, E_SigunalBack->m_Alpha, B_Finish, B_Finish->m_Alpha);
				if (E_SigunalBack->m_Alpha <= 0.0f && B_Finish->m_Alpha <= 0.0f)
				{
					LightScreen->m_DispFlag = true;
					LightScreen->m_Alpha += 0.01f;
					if (1.0f < LightScreen->m_Alpha)
					{
						ExpStartCnt = 0;
						P_1P->ActState = enStartUp;
						P_1P->SetChangeAnim(enStartUp, P_1P->ActState, AnimPlaySpeed);
						P_2P->ActState2 = enStartUp;
						P_2P->SetChangeAnim(enStartUp, P_2P->ActState2, AnimPlaySpeed);
						PlayBGM->Stop();
						Scene = SceneIndex::P1WIN;
					}
				}
			}
		}
	}


	//��������(�^�C���A�b�v���������񂾂Ƃ�).
	else if (TimeMinute->m_PatternNo.x == 0.0f
		  && TimeSecondL->m_PatternNo.x == 0.0f
		  && TimeSecondR->m_PatternNo.x == 0.0f
		  || P_1P->m_DeadFlag && P_2P->m_DeadFlag && P_1P->Exp && P_2P->Exp)
	{
		BattleEnd = true;
		B_End->m_DispFlag = true;
		E_SigunalBack->m_DispFlag = true;
		TexSigunal(E_SigunalBack, E_SigunalBack->m_Alpha, B_End, B_End->m_Alpha);
		if (E_SigunalBack->m_Alpha <= 0.0f && B_End->m_Alpha <= 0.0f)
		{
			LightScreen->m_DispFlag = true;
			LightScreen->m_Alpha += 0.01f;
			PlayBGM->Stop();

			if (1.0f < LightScreen->m_Alpha)
			{ Scene = SceneIndex::DRAW; }
		}
	}
}

/*************�ȉ��L�����֘A�֐�****************/
//�L�����N�^�[�ɕK�v�ȏ��̓���.
void CSceneGameMain::CharaUpDate(clsPlayer* Player,clsPlayer* Enemy, int No)
{//�{�̂̓����蔻����e�v���C���[�̈ʒu�E�p�x�ɍ��킹��.
	//�K�[�h.
	P_GuardColl[No]->m_vPos = Player->m_vPos;
	P_GuardColl[No]->m_vPos.y = Player->m_vPos.y - 2.0f;
	P_GuardColl[No]->m_vRot.y = P_GuardColl[No]->m_Yaw = Player->m_vRot.y + VSRot;
	//P_Coll[No]->m_vPos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	P_Coll[No]->m_vPos = Player->m_vPos;
	P_Coll[No]->m_vPos.y = Player->m_vPos.y - 2.0f;
	P_Coll[No]->m_vRot.y = Player->m_vRot.y + VSRot;
	P_Coll[No]->m_Yaw = Player->m_Yaw;

	//P_Shadow[No]->m_vPos = Player->m_vPos;
	//P_Shadow[No]->m_vPos.y = Player->m_vPos.y - 1.0f;
	//�ߐڍU��.
	if (Player->BladeAtk && Player->SlashDelay == 0 && !Enemy->HitFlag)
	{
		P_AttackColl[No]->m_vPos = Player->m_vPos;
		P_AttackColl[No]->m_vPos.y = Player->m_vPos.y + 1.0f;
		P_AttackColl[No]->m_vRot.y = Player->m_vRot.y;
	}
	else
	{
		P_AttackColl[No]->m_vPos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
		P_AttackColl[No]->m_vRot.y = Player->m_vRot.y;
	}
}
//�Օ���.
void CSceneGameMain::ShelterUpDate()
{
	//�ǂ̔���p���f���̓���.
	StageRingColl->m_vPos = D3DXVECTOR3(15.0f, -1.0f, 20.0f);
	for (Li = 0; Li < 4; Li++)
	{
		StageShelterColl[Li]->m_vPos = StageShelter[Li]->m_vPos;
	}
}
//�v���C���[���m�����������킹��.
void CSceneGameMain::PlayerRotation(clsPlayer* P1, clsPlayer* P2)
{
	//���f���̊p�x.
	float fPlayer1P_Yaw = P1->m_vRot.y;	//1P�̊p�x.
	float fPlayer2P_Yaw = P2->m_vRot.y;	//2P�̊p�x.

	D3DXMatrixIdentity(&P1_Yaw);
	D3DXMatrixRotationY(&P1_Yaw, fPlayer1P_Yaw);

	D3DXMatrixIdentity(&P2_Yaw);
	D3DXMatrixRotationY(&P2_Yaw, fPlayer2P_Yaw);

	fPlayer1P_Yaw = atan2f(
		P2->m_vPos.x - P1->m_vPos.x, P2->m_vPos.z - P1->m_vPos.z);
	fPlayer2P_Yaw = atan2f(
		P1->m_vPos.x - P2->m_vPos.x, P1->m_vPos.z - P2->m_vPos.z);

	P1->m_vRot.y = fPlayer1P_Yaw + VSRot;
	P2->m_vRot.y = fPlayer2P_Yaw + VSRot;
}

void CSceneGameMain::FlontRotation(clsPlayer* Player)
{
	//�␳�l��������(�ق�)���m�Ȓl�Ōv�Z.
	//�v�Z�pVector = �v���C���[�̉�]�l - �␳�l(15.705f).
	Player->m_vCRot.y = Player->m_vRot.y - VSRot;

	//�㐳��.
	if (-0.705f < Player->m_vCRot.y && Player->m_vCRot.y < 0.795f)
	{
		Player->LFlont = false;
		Player->RFlont = false;
		Player->UFlont = true;
		Player->DFlont = false;
	}
	//�E.
	else if (0.796f < Player->m_vCRot.y && Player->m_vCRot.y < 2.195f)
	{
		Player->LFlont = false;
		Player->RFlont = true;
		Player->UFlont = false;
		Player->DFlont = false;
	}
	//��.
	else if ( -2.105f < Player->m_vCRot.y && Player->m_vCRot.y < -0.805f)
	{
		Player->LFlont = true;
		Player->RFlont = false;
		Player->UFlont = false;
		Player->DFlont = false;
	}
	//��.
	else
	{
		Player->LFlont = false;
		Player->RFlont = false;
		Player->UFlont = false;
		Player->DFlont = true;
	}
}

//--------------�U���E�h�䔻��-----------------------.
void CSceneGameMain::PlayerAllColl()
{
	//�v���C���[���m.
	if (P_1P->m_Stopping)
	{
		P_Coll[0]->WallJudge(P_1P, P_Coll[1], 1.2f);
	}
	else if (P_2P->m_Stopping)
	{
		P_Coll[1]->WallJudge(P_2P, P_Coll[0], 1.2f);
	}
	else if (P_1P->m_Stopping && P_2P->m_Stopping)
	{
		P_Coll[0]->WallJudge(P_1P, P_Coll[1], 1.2f);
	}


	//�Օ���.
	ShelterColl(P_1P, 0);
	ShelterColl(P_2P, 1);
	for (Li = 0; Li < GatlingMax; Li++)
	{
		ShelterB_Coll(cP_Shot[Li]  ,BulletExp[Li] ,WallHit[Li]);
		ShelterB_Coll(cP2_Shot[Li] ,BulletExp[Li] ,WallHit[Li]);
	}
	for (Li = 0; Li < MissileMax; Li++)
	{
		ShelterB_Coll(mP_Shot[Li], BulletExp[Li], WallHit[Li]);
		ShelterB_Coll(mP2_Shot[Li], BulletExp[Li], WallHit[Li]);
	}

	//�ߐڕ���.
	BladeColl(P_1P, P_2P, 0, 1, BladeDamage, P_2P->ActState2, B2DamageCheck, BoostDamageB);
	BladeColl(P_2P, P_1P, 1, 0, BladeDamage, P_1P->ActState,  B1DamageCheck, BoostDamageB);

	//�ˌ�����.
	for (Li = 0; Li < GatlingMax; Li++)
	{
		WeponColl(P_2P, 0, 1, cP_Shot[Li], GatlingDamage, P_2P->ActState2, G2DamageCheck, BoostDamageG);
		WeponColl(P_1P, 1, 0, cP2_Shot[Li], GatlingDamage ,P_1P->ActState, G1DamageCheck, BoostDamageG);
	}
	for (Li = 0; Li < MissileMax; Li++)
	{
		WeponColl(P_2P, 0, 1, mP_Shot[Li], MissileDamage, P_2P->ActState2,M2DamageCheck, BoostDamageM);
		WeponColl(P_1P, 1, 0, mP2_Shot[Li], MissileDamage, P_1P->ActState,M1DamageCheck, BoostDamageM);
	}

	//�ˌ��_���[�W����.
	DamageCheck(P_2P, G2DamageCheck, GatlingDamage, G2DamageCnt);
	DamageCheck(P_1P, G1DamageCheck, GatlingDamage, G1DamageCnt);

	DamageCheck(P_2P, M2DamageCheck, MissileDamage, M2DamageCnt);
	DamageCheck(P_1P, M1DamageCheck, MissileDamage, M1DamageCnt);

	//�ߐڃ_���[�W����.
	DamageCheck(P_2P, B2DamageCheck, BladeDamage, B2DamageCnt);
	DamageCheck(P_1P, B1DamageCheck, BladeDamage, B1DamageCnt);


	P_1P->HitProc();
	P_2P->HitProc();
}

void CSceneGameMain::ShelterColl(clsPlayer * Player, int No)
{
	//�ǂ̔���.
	for (int i = 0; i < 4; i++)
	{//������P_Coll�A����������̂�Player.
		P_Coll[No]->WallJudge(Player, StageRingColl, WRepulsion);
		P_Coll[No]->WallJudge(Player, StageShelterColl[i], WRepulsion);
	}
}

void CSceneGameMain::ShelterB_Coll(clsPlayer* Bullet, clsEffectsMain* Exp, clsDirectSound* Hit)
{
	//�ǂ̔���.
	for (int i = 0; i < 4; i++)
	{//������P_Coll�A����������̂�Player.
		if(Bullet->BBoxCollision(StageShelterColl[i])
		&& Bullet->RayCollsion(Bullet,StageShelter[i]))
		{
			Exp->SetPosition(Bullet->m_vPos);
			Bullet->m_vPos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
			//Bullet->m_eDir = enDirection_Stop;
			Exp->Start_up();
			WallHit_B = true;
		}
	}

	//�ǂɓ��������Ƃ���SE�Đ�.
	if (WallHit_B)
	{
		WallHitCnt++;
	}
	if (0 < WallHitCnt && WallHitCnt < 550 && WallHit_B)
	{
		Hit->Play();
	}
	else
	{
		WallHit_B = false;
		Hit->Stop();
		WallHitCnt = 0;
	}
}

void CSceneGameMain::WeponColl(clsPlayer* Enemy, int No, int E_No,
							   clsPlayer* Bullet,int Damage, int &EnemyAct,
								bool &DamageCheck , int BoostDamage)
{
	//�ˌ��U��.
	if (Enemy->Gurad && !Enemy->BoostEmpty)
	{
		if (Bullet->RayCollsion(Bullet,P_GuardColl[E_No])
		 && Bullet->BBoxCollision(P_GuardColl[E_No]))
		{//�K�[�h�����A�̗͂ɕύX�Ȃ�.
			Enemy->m_HP = Enemy->m_HP;
			HitPlay(Enemy, E_No, GuardHit[E_No],50);
			Bullet->m_vPos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
			Enemy->BoostCnt += BoostDamage;
			if (100 <= Enemy->BoostCnt)
			{
				Enemy->BoostEmpty = true;
				Enemy->Gurad = true;
			}
		}
	}
	else//�K�[�h���s�A��e����.
	{
		if (Bullet->BBoxCollision(P_Coll[E_No]))
		{
			Enemy->m_HP -= Damage;
			DamageCheck = true;
			Enemy->HitFlag = true;
			Enemy->HitStop = true;

			if (0 < Enemy->m_HP)
			{
				HitPlay(Enemy, E_No,BulletHit[E_No],43);
				Bullet->m_eDir = enDirection_Stop;
				if (Enemy->BladeAtk)
				{
					Bullet->m_vPos = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
				}
				else
				{
					Bullet->m_vPos = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
					Enemy->SetChangeAnim(enActionHirumi, EnemyAct,0.02);
				}
			}
			else
			{//0�ȉ��Ȃ�|��.
				Bullet->m_vPos = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);
				Enemy->m_DeadFlag = true;
				Enemy->m_Stopping = true;
			}
		}
	}
}

void CSceneGameMain::BladeColl(clsPlayer * Player, clsPlayer* Enemy,int No, int E_No, 
								int Damage,int &EnemyAct,bool &DamageCheck, int BoostDamage)
{
	//�ߐڍU��.
	if (Enemy->HitFlag == false && Player->SlashDelay == 0)
	{
		if (Enemy->Gurad && !Enemy->BoostEmpty)
		{
			if(P_AttackColl[No]->BBoxCollision(P_GuardColl[E_No]))
			{//�K�[�h�����A�̗͂ɕύX�Ȃ�.
				HitPlay(Enemy, E_No, GuardHit[E_No],60);
				Enemy->m_HP = Enemy->m_HP;
				Enemy->BoostCnt += BoostDamage;		//�Q�[�W����.
				Enemy->Gurad = false;
				Enemy->HitFlag = true;
				P_AttackColl[No]->m_vPos = D3DXVECTOR3(0.0f, -100.0f, 0.0f);
				Player->BladeAtk = false;
				Player->m_eDir = enDirection_Stop;
				if (100 <= Enemy->BoostCnt)
				{
					Enemy->BoostEmpty = true;
					Enemy->Gurad = true;
				}
			}
		}
		else//�K�[�h���s�A��e����.
		{
			if (P_AttackColl[No]->BBoxCollision(P_Coll[E_No]))
			{
				Enemy->m_HP -= Damage;
				DamageCheck = true;
				Enemy->HitFlag = true;
				Enemy->HitStop = true;
				Player->m_eDir = enDirection_Stop;

				if (0 < Enemy->m_HP)
				{
					Enemy->SetChangeAnim(enActionHirumi, EnemyAct,0.02);
					HitPlay(Enemy, E_No,BladeHit[E_No],43);
				}
				else
				{//0�ȉ��Ȃ�|��.
					Enemy->m_DeadFlag = true;
					Enemy->m_Stopping = true;
				}
				if (Enemy->HitFlag == true)
				{
					P_AttackColl[No]->m_vPos = D3DXVECTOR3(0.0f, -100.0f, 0.0f);
				}
			}
		}
	}
}

void CSceneGameMain::DamageCheck(clsPlayer* Enemy, bool &DamageFlag, int Damage, int &Cnt)
{
	if (DamageFlag == true)
	{
		Cnt++;
	}

	if (80 < Cnt)
	{
		Enemy->m_DamageHP--;
		if (Enemy->m_DamageHP < Enemy->m_HP)
		{
			Cnt = 0;
			Enemy->m_DamageHP = Enemy->m_HP;
			DamageFlag = false;
		}
	}
}

//---------------------------------------------------.

//�e�̏���.
void CSceneGameMain::BulletUpDate(clsPlayer* Player, clsPlayer* Bullet
	, bool Reload, clsDirectSound* SE, int No)
{
	//������.
	if (Player->cShotCnt == -1)
	{
		if (Player->ReloadG1Flag == false)
		{
			for (int Mi = 0; Mi < GatlingMax; Mi++)
			{
				cP_Shot[Mi]->BulletInit(GatlingMax);
				Player->cShotCnt = cP_Shot[Mi]->cShotCnt;
				Player->DirectionFlag[Mi] = false;
			}
			Reload = false;
		}
	}

	//�e�̏���.
	Bullet->cShotCnt = Player->cShotCnt;

	if (Bullet->ShotFlag == true)
	{
		//SE�Đ�.
		if (Bullet->ShotCnt < 20)
		{ WeponSound(SE, GatlingShotCnt, 19); }

		//�e�̌����𓯊�.
		if (Player->DirectionFlag[Li] == false)
		{
			Bullet->ShotPos = Player->ShotPos;
			Bullet->m_Yaw = Player->m_Yaw;

			if (Player->cDirectionCnt[Li] == 0)
			{
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;			//���f���̉�]�l�C��.
				Bullet->m_SaveYaw[Li] = Bullet->m_Yaw;
				Bullet->ShotPos_Save[Li] = Bullet->ShotPos;
				Bullet->m_vRot = Player->m_vRot;					//�e�̌����ύX
			}
			if (Player->cDirectionCnt[Li] <= 5)
			{
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;			

				Player->m_BYaw = Bullet->m_Yaw;
			}
			else if (Player->cDirectionCnt[Li] > 6)
			{
				if (Player->DirectionFlag[Li] == false)
				{
					Player->DirectionFlag[Li] = true;
					Player->m_Yaw = Bullet->m_SaveYaw[Li];
				}
			}
			if (Player->cDirectionCnt[Li] >= 100)
			{
				Player->cDirectionCnt[Li] = 0;
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;
			}
			else
			{
				Bullet->m_Yaw = Player->m_BYaw;
			}
			Player->cDirectionCnt[Li]++;
		}
		else if (Player->DirectionFlag[Li] == true)
		{
			Bullet->ShotPos = Player->ShotPos_Save[Li];
			Bullet->m_Yaw = Bullet->m_SaveYaw[Li];

			if (Player->cDirectionCnt[Li] <= 5)
			{
				Player->m_BYaw = Bullet->m_Yaw;
				Player->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			else if (Player->cDirectionCnt[Li] >= 100)
			{
				Player->cDirectionCnt[Li] = 0;
				Player->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			else
			{
				Bullet->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			Player->cDirectionCnt[Li]++;
		}
	}
	else
	{
		Player->cDirectionCnt[Li] = 0;
		GatlingShotCnt = 0;
	}

	Bullet->ReloadG1Flag = Player->ReloadG1Flag;

	Bullet->ShotProc(Player->VectleGet);				//1P���̒e�̓���.
	if (Bullet->ShotFlag == true)
	{
		Bullet->m_eDir = enDirection_Forward;			//1P���̒e��O�ɔ�΂�.		
	}
	else
	{ //1P���̒e���~�߂�
		Bullet->m_eDir = enDirection_Stop;
	}

	Player->cShotCnt = Bullet->cShotCnt;

}

void CSceneGameMain::MissileUpDate(clsPlayer* Player, clsPlayer* Bullet
	, bool Reload, clsDirectSound* SE, int No)
{
	//������.
	if (Player->mShotCnt == -1)
	{
		if (Player->ReloadM1Flag == false)
		{
			for (int Mi = 0; Mi < MissileMax; Mi++)
			{
				mP_Shot[Mi]->MissileInit(MissileMax);
				Player->mShotCnt = mP_Shot[Mi]->mShotCnt;
			}
			Reload = false;
			Player->mDirectionFlag[Li] = false;
		}

	}

	//�e�̏���.
	Bullet->mShotCnt = Player->mShotCnt;

	if (Bullet->H_ShotFlag == true)
	{
		//SE�Đ�.
		if (Bullet->M_ShotCnt < 50)
		{ WeponSound(SE, MissileShotCnt, 49); }

		//�e�̌����𓯊�
		if (Player->mDirectionFlag[Li] == false)
		{
			Bullet->H_ShotPos = Player->H_ShotPos;
			Bullet->m_Yaw = Player->m_Yaw;

			if (Player->mDirectionCnt[Li] == 0)
			{
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;			//���f���̉�]�l�C��.11_16����d�v
				Bullet->m_SaveYaw[Li] = Bullet->m_Yaw;
				Bullet->H_ShotPos_Save[Li] = Bullet->H_ShotPos;
				Bullet->m_vRot = Player->m_vRot;					//11_20�����Œe�̌����ύX
			}
			if (Player->mDirectionCnt[Li] <= 5)
			{
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;			//���f���̉�]�l�C��.

				Player->m_BYaw = Bullet->m_Yaw;
			}
			else if (Player->mDirectionCnt[Li] > 6)
			{
				if (Player->mDirectionFlag[Li] == false)
				{
					Player->mDirectionFlag[Li] = true;
					Player->m_Yaw = Bullet->m_SaveYaw[Li];
				}
			}
			if (Player->mDirectionCnt[Li] >= 100)
			{
				Player->mDirectionCnt[Li] = 0;
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;
			}
			else
			{
				Bullet->m_Yaw = Player->m_BYaw;
			}
			Player->mDirectionCnt[Li]++;
		}
		else if (Player->mDirectionFlag[Li] == true)
		{
			Bullet->H_ShotPos = Player->H_ShotPos_Save[Li];
			Bullet->m_Yaw = Bullet->m_SaveYaw[Li];

			if (Player->mDirectionCnt[Li] <= 5)
			{
				Player->m_BYaw = Bullet->m_Yaw;
				Player->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			else if (Player->mDirectionCnt[Li] >= 100)
			{
				Player->mDirectionCnt[Li] = 0;
				Player->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			else
			{
				Bullet->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			Player->mDirectionCnt[Li]++;
		}
	}
	else
	{
		Player->mDirectionCnt[Li] = 0;
		MissileShotCnt = 0;
	}

	Bullet->ReloadM1Flag = Player->ReloadM1Flag;

	Bullet->M_ShotProc(Player->VectleGet);				//1P���̒e�̓���.
	//10_07
	if (Bullet->H_ShotFlag == true)
	{
		Bullet->m_eDir = enDirection_Forward;			//1P���̒e��O�ɔ�΂�.		
	}
	else
	{ //1P���̒e���~�߂�
		Bullet->m_eDir = enDirection_Stop;
	}

	Player->mShotCnt = Bullet->mShotCnt;

}


void CSceneGameMain::Bullet2UpDate(clsPlayer* Player, clsPlayer* Bullet
	, bool Reload, clsDirectSound* SE, int No)
{
	//������.
	if (Player->c2ShotCnt == -1)
	{
		if (Player->ReloadG2Flag == false)
		{
			for (int Mi = 0; Mi < GatlingMax; Mi++)
			{
				cP2_Shot[Mi]->Bullet2Init(GatlingMax);
				Player->c2ShotCnt = cP2_Shot[Mi]->c2ShotCnt;
			}
			Reload = false;
			Bullet->DirectionFlag[Li] = false;
		}
	}

	//�e�̏���.
	Bullet->c2ShotCnt = Player->c2ShotCnt;

	if (Bullet->ShotFlag == true)
	{
		//SE�Đ�.
		if (Bullet->ShotCnt < 20)
		{ WeponSound(SE, GatlingShotCnt, 19); }
		//�e�̌����𓯊�.
		if (Player->DirectionFlag[Li] == false)
		{
			Bullet->ShotPos = Player->ShotPos;
			Bullet->m_Yaw = Player->m_Yaw;

			if (Player->cDirectionCnt[Li] == 0)
			{
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;			//���f���̉�]�l�C��.11_16����d�v
				Bullet->m_SaveYaw[Li] = Bullet->m_Yaw;
				Bullet->ShotPos_Save[Li] = Bullet->ShotPos;
				Bullet->m_vRot = Player->m_vRot;					//11_20�����Œe�̌����ύX
			}
			if (Player->cDirectionCnt[Li] <= 5)
			{
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;			//���f���̉�]�l�C��.

				Player->m_BYaw = Bullet->m_Yaw;
			}
			else if (Player->cDirectionCnt[Li] > 6)
			{
				if (Player->DirectionFlag[Li] == false)
				{
					Player->DirectionFlag[Li] = true;
					Player->m_Yaw = Bullet->m_SaveYaw[Li];
				}
			}
			if (Player->cDirectionCnt[Li] >= 100)
			{
				Player->cDirectionCnt[Li] = 0;
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;
			}
			else
			{
				Bullet->m_Yaw = Player->m_BYaw;
			}
			Player->cDirectionCnt[Li]++;
		}
		else if (Player->DirectionFlag[Li] == true)
		{
			Bullet->ShotPos = Player->ShotPos_Save[Li];
			Bullet->m_Yaw = Bullet->m_SaveYaw[Li];

			if (Player->cDirectionCnt[Li] <= 5)
			{
				Player->m_BYaw = Bullet->m_Yaw;
				Player->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			else if (Player->cDirectionCnt[Li] >= 100)
			{
				Player->cDirectionCnt[Li] = 0;
				Player->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			else
			{
				Bullet->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			Player->cDirectionCnt[Li]++;
		}
	}
	else
	{
		Player->cDirectionCnt[Li] = 0;
	}

	Bullet->ReloadG2Flag = Player->ReloadG2Flag;

	Bullet->Shot2Proc(Player->VectleGet);				//2P���̒e�̓���.
	//10_07
	if (Bullet->ShotFlag == true)
	{
		Bullet->m_eDir = enDirection_Forward;			//2P���̒e��O�ɔ�΂�.		
	}
	else
	{ //1P���̒e���~�߂�
		Bullet->m_eDir = enDirection_Stop;
	}
	Player->c2ShotCnt = Bullet->c2ShotCnt;

}

void CSceneGameMain::Missile2UpDate(clsPlayer* Player, clsPlayer* Bullet
	, bool Reload, clsDirectSound* SE, int No)
{
	//������.
	if (Player->m2ShotCnt == -1)
	{
		if (Player->ReloadM2Flag == false)
		{
			for (int Mi = 0; Mi < MissileMax; Mi++)
			{
				mP2_Shot[Mi]->Missile2Init(MissileMax);
				Player->m2ShotCnt = mP2_Shot[Mi]->m2ShotCnt;
			}
			Reload = false;
			Player->mDirectionFlag[Li] = false;
		}

	}

	//�e�̏���.
	Bullet->m2ShotCnt = Player->m2ShotCnt;

	if (Bullet->H_ShotFlag == true)
	{//�e�̌����𓯊�
		//SE�Đ�.
		if (Bullet->M_ShotCnt < 50)
		{ WeponSound(SE, MissileShotCnt, 49);}

		if (Player->mDirectionFlag[Li] == false)
		{
			Bullet->H_ShotPos = Player->H_ShotPos;
			Bullet->m_Yaw = Player->m_Yaw;

			if (Player->mDirectionCnt[Li] == 0)
			{
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;			//���f���̉�]�l�C��.11_16����d�v
				Bullet->m_SaveYaw[Li] = Bullet->m_Yaw;
				Bullet->H_ShotPos_Save[Li] = Bullet->H_ShotPos;
				Bullet->m_vRot = Player->m_vRot;					//11_20�����Œe�̌����ύX
			}
			if (Player->mDirectionCnt[Li] <= 5)
			{
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;			//���f���̉�]�l�C��.

				Player->m_BYaw = Bullet->m_Yaw;
			}
			else if (Player->mDirectionCnt[Li] > 6)
			{
				if (Player->mDirectionFlag[Li] == false)
				{
					Player->mDirectionFlag[Li] = true;
					Player->m_Yaw = Bullet->m_SaveYaw[Li];
				}
			}
			if (Player->mDirectionCnt[Li] >= 100)
			{
				Player->mDirectionCnt[Li] = 0;
				Bullet->m_Yaw = Player->m_vRot.y - VSRot;
			}
			else
			{
				Bullet->m_Yaw = Player->m_BYaw;
			}
			Player->mDirectionCnt[Li]++;
		}
		else if (Player->mDirectionFlag[Li] == true)
		{
			Bullet->H_ShotPos = Player->H_ShotPos_Save[Li];
			Bullet->m_Yaw = Bullet->m_SaveYaw[Li];

			if (Player->mDirectionCnt[Li] <= 5)
			{
				Player->m_BYaw = Bullet->m_Yaw;
				Player->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			else if (Player->mDirectionCnt[Li] >= 100)
			{
				Player->mDirectionCnt[Li] = 0;
				Player->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			else
			{
				Bullet->m_Yaw = Bullet->m_SaveYaw[Li];
			}
			Player->mDirectionCnt[Li]++;
		}
	}
	else
	{
		Player->mDirectionCnt[Li] = 0;
		MissileShotCnt = 0;
	}

	Bullet->ReloadM2Flag = Player->ReloadM2Flag;

	Bullet->M_Shot2Proc(Player->VectleGet);				//1P���̒e�̓���.
	//10_07
	if (Bullet->H_ShotFlag == true)
	{
		Bullet->m_eDir = enDirection_Forward;			//2P���̒e��O�ɔ�΂�.		
	}
	else
	{ //2P���̒e���~�߂�
		Bullet->m_eDir = enDirection_Stop;
	}

	Player->m2ShotCnt = Bullet->m2ShotCnt;

}


void CSceneGameMain::WeponSound(clsDirectSound* SE, int &Cnt, int MaxCnt)
{
	SE->Play();
	Cnt++;
	if (MaxCnt < Cnt)
	{
		SE->Stop();
		Cnt = 0;
	}
}

void CSceneGameMain::HitPlay(clsPlayer* Enemy, int E_No, clsDirectSound* HitSound, int Cnt)	//�����������̏��𓾂�.
{
	HitSound->Play();
	HitCnt++;
	if (Cnt < HitCnt)
	{
		HitSound->Stop();
		HitCnt = 0;
	}
}



/*************�ȉ��������Ȃ�****************/
//����(�J�n��).
void CSceneGameMain::S_WhiteChange(cls2DTex* ScreenTex)
{
	if (!LightScreenCheck)
		ScreenTex->m_Alpha -= 0.05f;

	if (201 < CameraMoveCnt)
	{//�J�����Ń��f�����f��.
		if (ScreenTex->m_Alpha <= 0.0f && !LightScreenCheck)
		{
			ScreenTex->m_DispFlag = false;
			ScreenTex->m_vPos.y = 0.0f;
			TexSigunal(S_SigunalBack, S_SigunalBack->m_Alpha, B_Start, B_Start->m_Alpha);
			if (S_SigunalBack->m_Alpha <= 0.0f && B_Start->m_Alpha <= 0.0f)
			{
				LightScreenCheck = true;
				BattleStart = true;
				ScreenTex->m_Alpha = 0.0f;
			}
		}

	}
}

//���}����.
void CSceneGameMain::TexSigunal(
	cls2DTex* Back, float bAlpha, cls2DTex* Sigunal, float sAlpha)
{
	//�w�i�̈ړ�����.
	if (Back->m_vPos.x < 0.0f)
	{
		Back->m_vPos.x += 20.0f;
	}
	else
	{
		Back->m_vPos.x = 0.0f;//�Œ�.
		//�����e�N�X�`���̈ړ�.
		if (Sigunal->m_vPos.x < 470.0f)
		{
			Sigunal->m_vPos.x += 10.0f;
		}
		else
		{
			Sigunal->m_vPos.x = 470.0f;
		}//�Œ�.

	}

	//�J�n���̍쐬(����).
	if (384.0f <= Sigunal->m_vPos.x)
	{
		BSSound->Play();
	}

	//�w��ʒu�Ɉړ�������A���t�@�l�������ď���.
	if (Back->m_vPos.x == 0.0f && Sigunal->m_vPos.x == 470.0f)
	{
		//BSSound->Play();
		Back->m_Alpha -= 0.01f;
		Sigunal->m_Alpha -= 0.01f;
		if (Back->m_Alpha < 0.0f && Sigunal->m_Alpha < 0.0f)
		{
			Back->m_DispFlag = false;
			Sigunal->m_DispFlag = false;
			BSSound->Stop();
		}
	}
}

/***********�ȉ��t�h*************/
//�����ꗗ(�����ɃR���g���[���[�ǉ��\��).
void CSceneGameMain::ArmedList()
{
	//1P�K�g�����O.
	for (Li = 0; Li < GatlingMax; Li++)
	{
		if (cP_Shot[Li]->ShotCnt == 1)
		{
			GatlingRemaining1P->m_PatternNo.x += 1.0f;
		}
		if (cP2_Shot[Li]->ShotCnt == 1)
		{
			GatlingRemaining2P->m_PatternNo.x += 1.0f;
		}
	}
	//1P�~�T�C��.
	for (Li = 0; Li < MissileMax; Li++)
	{
		if (mP_Shot[Li]->M_ShotCnt == 1)
		{
			MissileRemaining1P->m_PatternNo.x += 1.0f;
		}	
		//2P�~�T�C��.
		if (mP2_Shot[Li]->M_ShotCnt == 1)
		{
			MissileRemaining2P->m_PatternNo.x += 1.0f;
		}
	}

	/************************************************/

/*************************�����͕ύX�Ȃ�****************************/
	//1P�����[�h����(���ݕ\���̂�).
	if (GatlingRemaining1P->m_PatternNo.x >= 10.0f)
	{
		GatlingRemaining1P->m_PatternNo.x = 10.0f;
		WeponReload[0]->m_vPos = GatlingRemaining1P->m_vPos;
		GatlingReload1P++;
		if(150 < GatlingReload1P)
		{//�����[�h�e�N�X�`�����ړ�.
			WeponReload[0]->m_vPos.x = GatlingRemaining1P->m_vPos.x - 2000.0f;
			GatlingReload1P = 0;
			GatlingRemaining1P->m_PatternNo.x = 0.0f;
		}
	}
	if (MissileRemaining1P->m_PatternNo.x >= 10.0f)
	{
		MissileRemaining1P->m_PatternNo.x = 10.0f;
		WeponReload[1]->m_vPos = MissileRemaining1P->m_vPos;
		MissileReload1P++;
		if (150 < MissileReload1P)
		{
			WeponReload[1]->m_vPos.x = MissileRemaining1P->m_vPos.x - 2000.0f;
			MissileReload1P = 0;
			MissileRemaining1P->m_PatternNo.x = 5.0f;
		}
	}

	//2P�����[�h����(����).
	//�ˌ��@.
	if (GatlingRemaining2P->m_PatternNo.x >= 10.0f)
	{
		GatlingRemaining2P->m_PatternNo.x = 10.0f;
		WeponReload[2]->m_vPos = GatlingRemaining2P->m_vPos;
		GatlingReload2P++;
		if ( 150 < GatlingReload2P)
		{
			WeponReload[2]->m_vPos.x = GatlingRemaining2P->m_vPos.x - 2000.0f;
			GatlingReload2P = 0;
			GatlingRemaining2P->m_PatternNo.x = 0.0f;
		}
	}
	//�ˌ��A.
	if (MissileRemaining2P->m_PatternNo.x >= 10.0f)
	{
		MissileRemaining2P->m_PatternNo.x = 10.0f;
		WeponReload[3]->m_vPos = MissileRemaining2P->m_vPos;
		MissileReload2P++;
		if (150 < MissileReload2P)
		{
			WeponReload[3]->m_vPos.x = MissileRemaining2P->m_vPos.x - 2000.0f;
			MissileReload2P = 0;
			MissileRemaining2P->m_PatternNo.x = 5.0f;
		}
	}
/******************************************************************/

}
//�Q�[�W�̑���.
void CSceneGameMain::BoostGaugeUpDown(clsPlayer* Player, int GaugeMax)
{//Action����ɑΉ�.
	if (Player->BoostFlag && Player->XMoveFlag && !Player->BoostEmpty
	 || Player->BoostFlag && Player->ZMoveFlag && !Player->BoostEmpty)
	{
		Player->BoostUseSpeCnt++;
		if (5 < Player->BoostUseSpeCnt)
		{
			Player->BoostCnt++;
			Player->BoostUseSpeCnt = 0;
		}
		if (GaugeMax < Player->BoostCnt)
		{
			Player->BoostCnt = GaugeMax;
			Player->BoostEmpty = true;
		}

	}
	else
	{	//�K�[�h���͔�e���Ă��u�[�X�g���񕜂����Ȃ�.
		if (!Player->BoostEmpty && !Player->Gurad)
		{
			Player->BoostCnt -= 3;
			if (Player->BoostCnt < 0)
			{
				Player->BoostCnt = 0;
				Player->BoostUseSpeCnt = 0;
			}
		}
	}

	if (Player->BoostEmpty)
	{
		Player->BoostCnt--;
		Player->Gurad = false;
		if (Player->BoostCnt < 0)
		{
			Player->BoostCnt = 0;
			Player->BoostUseSpeCnt = 0;
			Player->BoostEmpty = false;
			Player->BoostFlag = false;
		}
	}
}
//�������ԓ�����.
void CSceneGameMain::TimeCountDown()					
{//�ǂ��������ł��Ȃ����Ƃ��O��.
	if (P_1P->m_DeadFlag == false && P_2P->m_DeadFlag == false)
	{
		TimeCnt++;
		if (60 < TimeCnt)
		{//�b���炵.
			TimeSecondR->m_PatternNo.x -= 1.0f;
			TimeCnt = 0;
		}
		//�b�ꌅ�ڂ�0�ɂȂ����Ƃ�.
		if (TimeSecondR->m_PatternNo.x < 0.0f)
		{
			TimeSecondR->m_PatternNo.x = 9.0f;
			TimeSecondL->m_PatternNo.x -= 1.0f;
			//�񌅖ڂ�0�ɂȂ����Ƃ��������炷.
			if (TimeSecondL->m_PatternNo.x < 0.0f)
			{
				TimeSecondL->m_PatternNo.x = 5.0f;	//�񌅖ږ߂�.
				TimeMinute->m_PatternNo.x -= 1.0f;	//�����炵.
			}
		}
		//�J�E���g�_�E����.
		if (TimeMinute->m_PatternNo.x <= 0.0f 
		 && TimeSecondL->m_PatternNo.x < 1.0f
		 && 0.0f <= TimeSecondR->m_PatternNo.x)
		{
			CountDown10->Play();
		}
		//�S�Ă�0�ɂȂ����Ƃ�.
		if (TimeMinute->m_PatternNo.x < 0.0f)
		{
			TimeCnt = 0;
			TimeMinute->m_PatternNo.x = 0.0f;
			TimeSecondL->m_PatternNo.x = 0.0f;
			TimeSecondR->m_PatternNo.x = 0.0f;
			CountDown10->Stop();
			TimeUpCheck = true;
		}
	}
}
//�����G�t�F�N�g����.
void CSceneGameMain::ExpEff(clsChara* Player, bool &ExpFlg, clsEffectsMain* Effect)
{
	if (Player->m_DeadFlag == true && !ExpFlg)
	{//�G�t�F�N�g�̓�d�Đ��h�~.
		ExpStartCnt++;
		if (130 < ExpStartCnt)
		{
			if (0 < ExpFinishCnt && ExpFinishCnt < 10)
			{//��x����.
				Effect->SetPosition(Player->m_vPos);
				Effect->Start_up();
			}
			ExpFinishCnt++;
			if (40 < ExpFinishCnt)
			{
				//Player->m_vPos.y = -20.0f;
			}
			if (50 < ExpFinishCnt)
			{
				ExpFlg = true;
			}
		}
	}
}
//HPSE�Đ��p.
void CSceneGameMain::WarningHP(int HP, clsDirectSound* Sound1, clsDirectSound* Sound2, int &Cnt)
{
	if (HP <= 50 && 33 < HP && Cnt < 1)
	{
		Cnt++;
		Sound1->Play();
	}
	if (HP <= 20 && Cnt < 2)
	{
		Cnt++;
		Sound2->Play();
	}

}

/**************(�ق�)�s�ϊ֐�****************/
void CSceneGameMain::LoadPos(LPCSTR File, int Max)
{
	for (int i = 0; i < Max; i++)
	{
		LoadMPos[i] = m_mLoad->LoadData(File, i + 1);
	}
}

void CSceneGameMain::LoadRot(LPCSTR File, int Max)
{
	for (int i = 0; i < Max; i++)
	{
		LoadMRot[i] = m_mLoad->LoadData(File, i + 1);
	}
}

//�J����.
void CSceneGameMain::SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw)
{
	CameraMoveCnt++;
	//1P�Ɋ��.
	if (CameraMoveCnt < 90)
	{
		P_1P->m_Stopping = P_2P->m_Stopping = true;
		EEye = D3DXVECTOR3(3.0f, 3.0f, 21.0f);
		ELook = D3DXVECTOR3(7.0f, 7.0f, 19.0f);
	}
	//2P�Ɋ��.
	if (91 < CameraMoveCnt && CameraMoveCnt < 181)
	{
		EEye = D3DXVECTOR3(86.0f, -33.0f, 28.0f);
		ELook = D3DXVECTOR3(21.0f, 9.0f, 14.0f);
	}
	//�J�n���鎋�_�ɂ���.
	if (182 < CameraMoveCnt)
	{
		if (440 < CameraMoveCnt)
		{
			P_1P->m_Stopping = P_2P->m_Stopping = false;
		}
		if (!P_1P->m_DeadFlag && !P_2P->m_DeadFlag)
		{
			EEye.x  = (P_1P->m_vPos.x + P_2P->m_vPos.x) / 2;
			EEye.z  = (P_1P->m_vPos.z + P_2P->m_vPos.z) / 2;
			ELook.x = (P_1P->m_vPos.x + P_2P->m_vPos.x) / 2;
			ELook.z = (P_1P->m_vPos.z + P_2P->m_vPos.z) / 2;
			EEye.y = 0.0f;
			if (SetUpCameraLength(P_1P, P_2P))
			{
				ELook.y -= 0.8f;
				if (ELook.y < 40.0f)
				{
					ELook.y = 40.0f;
				}
			}
			else
			{
				ELook.y += 0.8f;
				if (ELook.y > 90.0f)
				{
					ELook.y = 90.0f;
				}
			}
		}
	}

	//�֐��̈����ɔ��f����.
	vLook = ELook;
	vEye = EEye;
	Yaw = 0.0f;
}
bool CSceneGameMain::SetUpCameraLength(clsPlayer* P_1, clsPlayer* P_2)
{
	D3DXVECTOR3 vecPlayer1, vecPlayer2;
	//������.
	vecPlayer1 = P_1->m_vPos;
	vecPlayer2 = P_2->m_vPos;

	//���v���C���[���W�̒��S����̋��������߂�.
	D3DXVECTOR3 vLength = (P_1->m_vPos - P_2->m_vPos);
	//�����ɕϊ�.
	float Length = D3DXVec3Length(&vLength);

	if (Length <= 20.0f)
	{
		return true;	//�J�������񂹂�.
	}
	return false;		//�J�����𗣂�.
}

void CSceneGameMain::SetLightPos(D3DXVECTOR3 &vLight)
{
	//(46.1 26.4 -2.1)
	vLight = D3DXVECTOR3(0.3f, 1.2f, -3.0f);
	if (GetAsyncKeyState(VK_F2) & 0x01)
	{
		vLight.x += 0.1f;
	}
	if (GetAsyncKeyState(VK_F3) & 0x01)
	{
		vLight.x -= 0.1f;
	}

	if (GetAsyncKeyState(VK_F4) & 0x01)
	{
		vLight.y += 0.1f;
	}
	if (GetAsyncKeyState(VK_F6) & 0x01)
	{
		vLight.y -= 0.1f;
	}

	if (GetAsyncKeyState(VK_F7) & 0x01)
	{
		vLight.z += 0.1f;
	}
	if (GetAsyncKeyState(VK_F8) & 0x01)
	{
		vLight.z -= 0.1f;
	}
	if (GetAsyncKeyState(VK_F9) & 0x01)
	{
		vLight = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

}

//�f�o�b�O�e�L�X�g������.
void CSceneGameMain::UpdateDebugMode(ID3D11DeviceContext* pContext)
{
#ifdef _DEBUG
	//�f�o�b�O�e�L�X�g�̏�����.
	PlayDebug = new clsDebugText;
	D3DXVECTOR4 vColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (FAILED(PlayDebug->Init(pContext, WND_W, WND_H, 50.0f, vColor)))
	{ return; }
#endif //_DEBUG
}