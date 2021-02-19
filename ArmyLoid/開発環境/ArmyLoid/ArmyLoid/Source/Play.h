#ifndef PLAY_H
#define PLAY_H

#include "Base.h"
#include "DX9Mesh.h"
#include "ED3DXSKINMESH.h"

#include "Charactor.h"
#include "C_Player.h"
#include "C_Object.h"
#include "CRay.h"

//UI.
const int LIFE_MAX = 100;		//�̗̓������ő吔.
const int GaugeMax = 100;		//�Q�[�W�ő吔.
const int TimeSecond = 60;		//�������ԃJ�E���g�_�E���p.

//�����Η�.
const int BladeDamage = 10;
const int MissileDamage = 4;
const int GatlingDamage = 2;
const int GatReroad = 9;

//���f����.
const int ShelterMax = 4;
// �Q�[�����C���V�[��.
class CSceneGameMain: public CSceneBase
{
public:
	CSceneGameMain(const char* SceneName);

	virtual ~CSceneGameMain();

	void Load(clsResource *setUP);
	void Load(cls2DTexResource *SetUP);
	void Load(clsSkinResource *SetUP, clsDInput *Controller
	,ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd);
	void Load(clsEffectsResouce* SetUP);
	void Load(clsSoundResource* SetUP);
	void Control();
	void Action(SceneIndex &Scene, clsDInput *Controller);
	void Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss);
	//�J����.
	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

	void UpdateDebugMode(ID3D11DeviceContext* pContext);

	void SetLightPos(D3DXVECTOR3 &vLight);

private:
	void ChangeScene(SceneIndex &Scene);		//�V�[���ڍs����.

	/****�e�N�X�`��****/
	void ArmedList();		//�����ꗗ.
	void BoostGaugeUpDown(clsPlayer* Player,int GaugeMax);

	void TimeCountDown();						//�������ԓ�����.

	void S_WhiteChange(cls2DTex* ScreenTex);	//���].

	//�J�n�E�I�����}�p.
	void TexSigunal(cls2DTex* Back,float bAlpha,cls2DTex* Sigunal, float sAlpha);
	
	/*********�G�t�F�N�g********/
	//�L�����N�^�[���E.
	void ExpEff(clsChara* Player,bool &ExpFlg, clsEffectsMain* Effect);

	/****************�e�퓖���蔻��***************/
	void ShelterUpDate();
	//�����蔻�菈��(�܂Ƃ�).
	void PlayerAllColl();
	//�Օ���.
	void ShelterColl(clsPlayer * Player, int No);
	void ShelterB_Coll(clsPlayer* Bullet,clsEffectsMain* Exp,clsDirectSound* Hit);
	//�ˌ�����(�g���܂킵�\).
	void WeponColl(clsPlayer* Enemy, int No, int E_No
		, clsPlayer* Bullet,int Damage, int &EnemyAct, bool &DamageCheck
		, int BoostDamage);
	//�ߐڕ���.
	void BladeColl(clsPlayer * Player, clsPlayer* Enemy,int No, int E_No, 
					int Damage,int &EnemyAct, bool &DamageCheck,int BoostDamage);
	/*************************�L�������*****************************/
	//�L�����N�^�[�ɕR�t����f�[�^�̍X�V.
	void CharaUpDate(clsPlayer* Player, clsPlayer* Enemy,int No);
	//�L�����N�^�[�̌���(��������).
	void PlayerRotation(clsPlayer* P1, clsPlayer* P2);
	//�ړ��A�j���[�V�������������֐�.
	void FlontRotation(clsPlayer* Player);


	//----------�e����---------.
	//�e�̏�Ԑ���(����������L����,�e�̍ő吔,�G�₯�ǃ����[�h��������).
	void BulletUpDate(clsPlayer* Player,clsPlayer* Bullet
		,bool Reload,clsDirectSound* SE,int No);
	void Bullet2UpDate(clsPlayer* Player, clsPlayer* Bullet
		, bool Reload, clsDirectSound* SE, int No);
	void MissileUpDate(clsPlayer* Player, clsPlayer* Bullet
		, bool Reload, clsDirectSound* SE, int No);
	void Missile2UpDate(clsPlayer* Player, clsPlayer* Bullet
		, bool Reload, clsDirectSound* SE, int No);

	//�e���ʂ�SE����.
	void WeponSound(clsDirectSound* SE, int &Cnt, int MaxCnt);

	//-----------------���֘A---------------.
	void WarningHP(int HP, clsDirectSound* Sound1,
					clsDirectSound* Sound2, int &Cnt);

	//�J�����̃Y�[���C���E�A�E�g���f.
	bool SetUpCameraLength(clsPlayer* P_1, clsPlayer* P_2);

	void LoadPos(LPCSTR File, int Max);
	void LoadRot(LPCSTR File, int Max);

	void HitPlay(clsPlayer* Enemy, int E_No, clsDirectSound* HitSound, int Cnt);	//�����������̏��𓾂�.

	void DamageCheck(clsPlayer* Enemy, bool &DamageFlag, int Damage, int &Cnt);

/*************************************************************/
/***********************�ϐ��E�t���O**************************/
/*************************************************************/
private:
	
	//�����[�h�Ǘ��ϐ�(������Ɛ�����).
	int			GatlingReload1P, MissileReload1P;
	int			GatlingReload2P, MissileReload2P;

	//�f�B���C�p�t���O�E�J�E���g.
	bool		DelayFlag;
	int			DelayCnt;

	//�������ԕϐ�.
	bool		TimeUpCheck;				//�������Ԃ��m�F���A�V�[�����΂�.

	//�V�[���ؑցE���o�ϐ�.
	bool		LightScreenCheck;			//�����̓�\�����h�~�p.

	//�o�g���J�n�E�I���E����t���O.
	bool		BattleStart;				//true:�J�n.
	bool		BattleEnd;					//true:�I��.

	//�G�t�F�N�g�Đ��E���o�p.
	int			CameraMoveCnt;
	int			ExpFinishCnt;
	int			ExpStartCnt;

	//�e�N�X�`������p.
	//�ԃ_������.
	int			G1DamageCnt, M1DamageCnt,B1DamageCnt;
	int			G2DamageCnt, M2DamageCnt,B2DamageCnt;
	bool		G1DamageCheck, G2DamageCheck;
	bool		M1DamageCheck, M2DamageCheck;
	bool		B1DamageCheck, B2DamageCheck;
	//HP�̓_��.
	bool		HP_ChackAlpha[2];			

	//������p.
	int			HPCnt1P, HPCnt2P;
	int			HitCnt;
	int			Swing1Cnt, Swing2Cnt;
	int			WallHitCnt;
	bool		WallHit_B;
	int			GatlingShotCnt,MissileShotCnt;

	//�J����.
	D3DXVECTOR3 EEye;						//�����ʒu.
	D3DXVECTOR3 ELook;						//�J�����ʒu.

	//�z��SE����p.
	int			SESetCnt;

	//�Ǎ��p.
	D3DXVECTOR3 LoadMPos[10];
	D3DXVECTOR3 LoadMRot[10];




/*****************************************************************/

//----------------------------------------------//
//------------���f���E�e�N�X�`�����------------//
//----------------------------------------------//
private:
//------------���b�V�����f�����(�t�@�C���p�X�O���f�[�^�ɕύX�ς�)------------------.

	clsPlayer*					P_Shot;						//�e(��).
	clsPlayer*					P_2Shot;					//��ɂ���.
	clsPlayer*					P_HShot;					
	clsPlayer*					P_2HShot;
	clsPlayer*					P_Coll[PlayerMAX];			//�����蔻��A�`�悵�Ȃ�.
	clsPlayer*					P_AttackColl[PlayerMAX];	//����.
	clsPlayer*					HitTagPos[4];				//�ǔ���\��(����1P��).
	clsPlayer*					P_GuardColl[PlayerMAX];		//�K�[�h����.

	clsPlayer*					cP_Shot[10];				//��e.
	clsPlayer*					cP2_Shot[10];				

	clsPlayer*					mP_Shot[10];				//���e.
	clsPlayer*					mP2_Shot[10];

	//�X�e�[�W�I�u�W�F�N�g.
	clsObject*					Ground;						//�n��.
	clsObject*					Mountain;
	clsObject*					StageRing;					//�ړ��\�͈�.
	clsObject*					StageRingColl;				//��̓����蔻�� �`�悵�Ȃ�.
	clsObject*					StageShelter[4];			//�Օ���.
	clsObject*					StageShelterColl[4];
	clsObject*					StageBShelter;				//�����ɔz�u����Օ���.
	clsObject*					StageBShelterColl;

	//------------�X�L�����f�����--------------------.
	clsPlayer*					P_1P;						//1P�ˌ����[�h.
	clsPlayer*					P_2P;						//2P�ˌ����[�h.

//---------------�e�N�X�`�����-------------------.
	//HP�̕\��(��Ԑ��ڍ�)
	cls2DTex*					P_1FINE[LIFE_MAX];
	cls2DTex*					P_1CAUTION[LIFE_MAX];
	cls2DTex*					P_1HITDAMAGE[LIFE_MAX];
	cls2DTex*					P_2FINE[LIFE_MAX];
	cls2DTex*					P_2CAUTION[LIFE_MAX];
	cls2DTex*					P_2HITDAMAGE[LIFE_MAX];
	cls2DTex*					P_HPBackGround[PlayerMAX];	//HP�w�i.

	//�u�[�X�g�Q�[�W(�ׂ����̂𑽐��`��).
	cls2DTex*					Boost1P[GaugeMax];			//���̕���.
	cls2DTex*					Boost2P[GaugeMax];
	cls2DTex*					BoostHeat1P[GaugeMax];		//�I�[�o�[�q�[�g���̐ԐF.
	cls2DTex*					BoostHeat2P[GaugeMax];
	cls2DTex*					BoostHeat[PlayerMAX];		//�I�[�o�[�q�[�g�\��.
	cls2DTex*					BoostGauge[PlayerMAX];		//�u�[�X�g�Q�[�W�w�i.

	//UI���ǂ̃L����������.
	cls2DTex*					PlayerName1;
	cls2DTex*					PlayerName2;
	
	//�����ꗗ(�c�e��).
	cls2DTex*					GatlingRemaining1P;			//�ˌ��@�e���\��.
	cls2DTex*					GatlingRemaining2P;			
	cls2DTex*					MissileRemaining1P;			//�ˌ��A�e���\��.
	cls2DTex*					MissileRemaining2P;			
	cls2DTex*					WeponBack[PlayerMAX];		//�����ꗗ�w�i.
	cls2DTex*					WeponReload[BulletMax];		//�����[�h�\��.

	//��������(��莞�Ԃ��牺���Ă���/���ƕb��2���`��).
	cls2DTex*					TimeMinute;					//��.
	cls2DTex*					TimeSecondL;				//10�̈�.
	cls2DTex*					TimeSecondR;				//1�̈�.
	cls2DTex*					TimeBack;					//�w�i.
	//��ʐ؂�ւ��p�e�N�X�`��.
	cls2DTex*					B_Start;					//�J�n���}.
	cls2DTex*					S_SigunalBack;				//�J�n�w�i.
	cls2DTex*					B_End;						//�I�����}.
	cls2DTex*					B_Finish;					//���s�������ꍇ.
	cls2DTex*					E_SigunalBack;				//�I���w�i.

	cls2DTex*					LightScreen;				//����.

//---------------�G�t�F�N�g���-------------------.
	clsEffectsMain*				ExpBurn[PlayerMAX];			//�v���C���[�̔��U.

	clsEffectsMain*				BulletExp[10];				//�e�̔���.

//---------------�T�E���h(BGM)���-------------------.
	clsDirectSound*				PlayBGM;

//---------------�T�E���h(SE)���-------------------.
	//�e�핐��SE.
	clsDirectSound*				GatlingSE[10];				//�e���ˎ��̉�.
	clsDirectSound*				MissileSE[5];

	//��Ԑ��ڒʒm��.
	clsDirectSound*				HPCaution[PlayerMAX];		
	clsDirectSound*				HPDanger[PlayerMAX];
	//�o�g���J�n���̉�.
	clsDirectSound*				BSSound;					
	//�^�C���A�b�v10�b�O.
	clsDirectSound*				CountDown10;				
	//��e��.
	clsDirectSound*				BulletHit[PlayerMAX];
	clsDirectSound*				BladeHit[PlayerMAX];
	clsDirectSound*				Blade1NoHit;
	clsDirectSound*				Blade2NoHit;

	clsDirectSound*				GuardHit[PlayerMAX];
	//�ǃq�b�g��.
	clsDirectSound*				WallHit[10];
private://�s�ϕ���.
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;
	ID3D11Texture2D*			m_pBackBuffer_DSTex;
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;

	clsDebugText*				PlayDebug;					//�f�o�b�O�e�L�X�g.
	clsLoadData*				m_mLoad;

	//Y����]�s��̍쐬
	D3DXMATRIX					P1_Yaw;		
	D3DXMATRIX					P2_Yaw;		
	D3DXMATRIX					Shelter_Yaw;
	//���C�̐���.
	clsRay*						m_pRayV[PlayerMAX];
	clsRay*						m_pRayFB[PlayerMAX];
	clsRay*						m_pRayH[PlayerMAX];
};
#endif