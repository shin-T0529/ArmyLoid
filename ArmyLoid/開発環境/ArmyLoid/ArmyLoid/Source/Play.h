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
const int LIFE_MAX = 100;		//体力メモリ最大数.
const int GaugeMax = 100;		//ゲージ最大数.
const int TimeSecond = 60;		//制限時間カウントダウン用.

//武装火力.
const int BladeDamage = 10;
const int MissileDamage = 4;
const int GatlingDamage = 2;
const int GatReroad = 9;

//モデル数.
const int ShelterMax = 4;
// ゲームメインシーン.
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
	//カメラ.
	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

	void UpdateDebugMode(ID3D11DeviceContext* pContext);

	void SetLightPos(D3DXVECTOR3 &vLight);

private:
	void ChangeScene(SceneIndex &Scene);		//シーン移行処理.

	/****テクスチャ****/
	void ArmedList();		//武装一覧.
	void BoostGaugeUpDown(clsPlayer* Player,int GaugeMax);

	void TimeCountDown();						//制限時間内処理.

	void S_WhiteChange(cls2DTex* ScreenTex);	//明転.

	//開始・終了合図用.
	void TexSigunal(cls2DTex* Back,float bAlpha,cls2DTex* Sigunal, float sAlpha);
	
	/*********エフェクト********/
	//キャラクター爆殺.
	void ExpEff(clsChara* Player,bool &ExpFlg, clsEffectsMain* Effect);

	/****************各種当たり判定***************/
	void ShelterUpDate();
	//当たり判定処理(まとめ).
	void PlayerAllColl();
	//遮蔽物.
	void ShelterColl(clsPlayer * Player, int No);
	void ShelterB_Coll(clsPlayer* Bullet,clsEffectsMain* Exp,clsDirectSound* Hit);
	//射撃武器(使いまわし可能).
	void WeponColl(clsPlayer* Enemy, int No, int E_No
		, clsPlayer* Bullet,int Damage, int &EnemyAct, bool &DamageCheck
		, int BoostDamage);
	//近接武器.
	void BladeColl(clsPlayer * Player, clsPlayer* Enemy,int No, int E_No, 
					int Damage,int &EnemyAct, bool &DamageCheck,int BoostDamage);
	/*************************キャラ情報*****************************/
	//キャラクターに紐付けるデータの更新.
	void CharaUpDate(clsPlayer* Player, clsPlayer* Enemy,int No);
	//キャラクターの向き(向きだけ).
	void PlayerRotation(clsPlayer* P1, clsPlayer* P2);
	//移動アニメーション方向同期関数.
	void FlontRotation(clsPlayer* Player);


	//----------弾処理---------.
	//弾の状態推移(同期させるキャラ,弾の最大数,雑やけどリロードさせるやつ).
	void BulletUpDate(clsPlayer* Player,clsPlayer* Bullet
		,bool Reload,clsDirectSound* SE,int No);
	void Bullet2UpDate(clsPlayer* Player, clsPlayer* Bullet
		, bool Reload, clsDirectSound* SE, int No);
	void MissileUpDate(clsPlayer* Player, clsPlayer* Bullet
		, bool Reload, clsDirectSound* SE, int No);
	void Missile2UpDate(clsPlayer* Player, clsPlayer* Bullet
		, bool Reload, clsDirectSound* SE, int No);

	//弾共通のSE処理.
	void WeponSound(clsDirectSound* SE, int &Cnt, int MaxCnt);

	//-----------------音関連---------------.
	void WarningHP(int HP, clsDirectSound* Sound1,
					clsDirectSound* Sound2, int &Cnt);

	//カメラのズームイン・アウト判断.
	bool SetUpCameraLength(clsPlayer* P_1, clsPlayer* P_2);

	void LoadPos(LPCSTR File, int Max);
	void LoadRot(LPCSTR File, int Max);

	void HitPlay(clsPlayer* Enemy, int E_No, clsDirectSound* HitSound, int Cnt);	//当たった側の情報を得る.

	void DamageCheck(clsPlayer* Enemy, bool &DamageFlag, int Damage, int &Cnt);

/*************************************************************/
/***********************変数・フラグ**************************/
/*************************************************************/
private:
	
	//リロード管理変数(ちょっと数多い).
	int			GatlingReload1P, MissileReload1P;
	int			GatlingReload2P, MissileReload2P;

	//ディレイ用フラグ・カウント.
	bool		DelayFlag;
	int			DelayCnt;

	//制限時間変数.
	bool		TimeUpCheck;				//制限時間を確認し、シーンを飛ばす.

	//シーン切替・演出変数.
	bool		LightScreenCheck;			//明幕の二十処理防止用.

	//バトル開始・終了・判定フラグ.
	bool		BattleStart;				//true:開始.
	bool		BattleEnd;					//true:終了.

	//エフェクト再生・演出用.
	int			CameraMoveCnt;
	int			ExpFinishCnt;
	int			ExpStartCnt;

	//テクスチャ制御用.
	//赤ダメ制御.
	int			G1DamageCnt, M1DamageCnt,B1DamageCnt;
	int			G2DamageCnt, M2DamageCnt,B2DamageCnt;
	bool		G1DamageCheck, G2DamageCheck;
	bool		M1DamageCheck, M2DamageCheck;
	bool		B1DamageCheck, B2DamageCheck;
	//HPの点滅.
	bool		HP_ChackAlpha[2];			

	//音制御用.
	int			HPCnt1P, HPCnt2P;
	int			HitCnt;
	int			Swing1Cnt, Swing2Cnt;
	int			WallHitCnt;
	bool		WallHit_B;
	int			GatlingShotCnt,MissileShotCnt;

	//カメラ.
	D3DXVECTOR3 EEye;						//注視位置.
	D3DXVECTOR3 ELook;						//カメラ位置.

	//配列SE制御用.
	int			SESetCnt;

	//読込用.
	D3DXVECTOR3 LoadMPos[10];
	D3DXVECTOR3 LoadMRot[10];




/*****************************************************************/

//----------------------------------------------//
//------------モデル・テクスチャ情報------------//
//----------------------------------------------//
private:
//------------メッシュモデル情報(ファイルパス外部データに変更済み)------------------.

	clsPlayer*					P_Shot;						//弾(仮).
	clsPlayer*					P_2Shot;					//一つにして.
	clsPlayer*					P_HShot;					
	clsPlayer*					P_2HShot;
	clsPlayer*					P_Coll[PlayerMAX];			//当たり判定、描画しない.
	clsPlayer*					P_AttackColl[PlayerMAX];	//同上.
	clsPlayer*					HitTagPos[4];				//壁判定表示(今は1P分).
	clsPlayer*					P_GuardColl[PlayerMAX];		//ガード判定.

	clsPlayer*					cP_Shot[10];				//弱弾.
	clsPlayer*					cP2_Shot[10];				

	clsPlayer*					mP_Shot[10];				//強弾.
	clsPlayer*					mP2_Shot[10];

	//ステージオブジェクト.
	clsObject*					Ground;						//地面.
	clsObject*					Mountain;
	clsObject*					StageRing;					//移動可能範囲.
	clsObject*					StageRingColl;				//上の当たり判定 描画しない.
	clsObject*					StageShelter[4];			//遮蔽物.
	clsObject*					StageShelterColl[4];
	clsObject*					StageBShelter;				//中央に配置する遮蔽物.
	clsObject*					StageBShelterColl;

	//------------スキンモデル情報--------------------.
	clsPlayer*					P_1P;						//1P射撃モード.
	clsPlayer*					P_2P;						//2P射撃モード.

//---------------テクスチャ情報-------------------.
	//HPの表示(状態推移込)
	cls2DTex*					P_1FINE[LIFE_MAX];
	cls2DTex*					P_1CAUTION[LIFE_MAX];
	cls2DTex*					P_1HITDAMAGE[LIFE_MAX];
	cls2DTex*					P_2FINE[LIFE_MAX];
	cls2DTex*					P_2CAUTION[LIFE_MAX];
	cls2DTex*					P_2HITDAMAGE[LIFE_MAX];
	cls2DTex*					P_HPBackGround[PlayerMAX];	//HP背景.

	//ブーストゲージ(細かいのを多数描画).
	cls2DTex*					Boost1P[GaugeMax];			//中の部分.
	cls2DTex*					Boost2P[GaugeMax];
	cls2DTex*					BoostHeat1P[GaugeMax];		//オーバーヒート時の赤色.
	cls2DTex*					BoostHeat2P[GaugeMax];
	cls2DTex*					BoostHeat[PlayerMAX];		//オーバーヒート表示.
	cls2DTex*					BoostGauge[PlayerMAX];		//ブーストゲージ背景.

	//UIがどのキャラを示す.
	cls2DTex*					PlayerName1;
	cls2DTex*					PlayerName2;
	
	//武装一覧(残弾数).
	cls2DTex*					GatlingRemaining1P;			//射撃①弾数表示.
	cls2DTex*					GatlingRemaining2P;			
	cls2DTex*					MissileRemaining1P;			//射撃②弾数表示.
	cls2DTex*					MissileRemaining2P;			
	cls2DTex*					WeponBack[PlayerMAX];		//武装一覧背景.
	cls2DTex*					WeponReload[BulletMax];		//リロード表示.

	//制限時間(一定時間から下げていく/分と秒で2つ分描画).
	cls2DTex*					TimeMinute;					//分.
	cls2DTex*					TimeSecondL;				//10の位.
	cls2DTex*					TimeSecondR;				//1の位.
	cls2DTex*					TimeBack;					//背景.
	//場面切り替え用テクスチャ.
	cls2DTex*					B_Start;					//開始合図.
	cls2DTex*					S_SigunalBack;				//開始背景.
	cls2DTex*					B_End;						//終了合図.
	cls2DTex*					B_Finish;					//勝敗がついた場合.
	cls2DTex*					E_SigunalBack;				//終了背景.

	cls2DTex*					LightScreen;				//白幕.

//---------------エフェクト情報-------------------.
	clsEffectsMain*				ExpBurn[PlayerMAX];			//プレイヤーの爆散.

	clsEffectsMain*				BulletExp[10];				//弾の爆発.

//---------------サウンド(BGM)情報-------------------.
	clsDirectSound*				PlayBGM;

//---------------サウンド(SE)情報-------------------.
	//各種武装SE.
	clsDirectSound*				GatlingSE[10];				//弾発射時の音.
	clsDirectSound*				MissileSE[5];

	//状態推移通知音.
	clsDirectSound*				HPCaution[PlayerMAX];		
	clsDirectSound*				HPDanger[PlayerMAX];
	//バトル開始時の音.
	clsDirectSound*				BSSound;					
	//タイムアップ10秒前.
	clsDirectSound*				CountDown10;				
	//被弾時.
	clsDirectSound*				BulletHit[PlayerMAX];
	clsDirectSound*				BladeHit[PlayerMAX];
	clsDirectSound*				Blade1NoHit;
	clsDirectSound*				Blade2NoHit;

	clsDirectSound*				GuardHit[PlayerMAX];
	//壁ヒット時.
	clsDirectSound*				WallHit[10];
private://不変部分.
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;
	ID3D11Texture2D*			m_pBackBuffer_DSTex;
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;

	clsDebugText*				PlayDebug;					//デバッグテキスト.
	clsLoadData*				m_mLoad;

	//Y軸回転行列の作成
	D3DXMATRIX					P1_Yaw;		
	D3DXMATRIX					P2_Yaw;		
	D3DXMATRIX					Shelter_Yaw;
	//レイの生成.
	clsRay*						m_pRayV[PlayerMAX];
	clsRay*						m_pRayFB[PlayerMAX];
	clsRay*						m_pRayH[PlayerMAX];
};
#endif