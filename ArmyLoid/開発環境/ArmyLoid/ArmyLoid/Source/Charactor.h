#ifndef CHARACTOR_H
#define CHARACTOR_H

#include "DX9Mesh.h"
#include "ED3DXSKINMESH.h"
#include "DebugText.h"
#include "Global.h"
#include "DInput.h"

enum Anim
{	//順は仮.
	enStartUp				= 0,			//起動.
	enWait					= 1,			//待機.

	enActionHirumi			= 2,			//攻撃ヒット時のひるみ.

	enAttackMode			= 3,			//近接攻撃.
	enShotMissile			= 4,			//ミサイル発射.
	enActionGuard			= 5,
	enActionGuardDuring		= 6,

	enHoverFlontStart		= 7,			//前進.
	enHoverFlontDuring		= 8,
	enHoverFlontEnd			= 9,

	enHoverRightStart		= 10,			//右移動.
	enHoverRightDuring		= 11,
	enHoverRightEnd			= 12,

	enHoverLeftStart		= 13,			//左移動.
	enHoverLeftDuring		= 14,
	enHoverLeftEnd			= 15,

	enHoverBackStart		= 16,			//後退.
	enHoverBackDuring		= 17,
	enHoverBackEnd			= 18,

	
	enActionDown			= 19,			//体力0の時.
	enActionDownEnd			= 20,

	enWinnerPause			= 21,			//勝利ポーズ.
	enWinnerPauseEnd		= 22,

	enFreaze				= 23,
	enResetState,	//最大値設定.
};
//キャラクラス.
class clsChara
{
public:
	struct HIT_POS
	{
		D3DXVECTOR3 vPos[4];
		HIT_POS()
			: vPos()
		{}
	};

	clsChara();
	virtual ~clsChara();

	//拡張や回転など座標の値をモデルデータに反映.
	void Update();
	void UpdateSkin();

	//----------------モデル関連-----------------.
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye);
	void SkinRender(D3DXMATRIX &mView, D3DXMATRIX &mProj, 
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye);

	//モデルデータ受け渡し(関連付け).
	void AttachModel(clsDx9Mesh* pModel);
	void AttachSkinModel(clsD3DXSKINMESH* pSkinModel);

	//モデルデータ関連付け解除.
	void DetatchModel();
	void DetatchSkinModel();

	//------------------------------------------.


	void WallJudge(clsChara* pTackle, clsChara* pWall,float WSPACE);

	void dirCheck(float *fYaw);


	//-----------------当たり判定-------------------.
	//●判定.
	bool SphereCollision(clsChara* pTarget, SSPHERE *pSSphere = NULL);
	//■判定.
	bool BBoxCollision(clsChara* pTarget, BBOX *pBBox = NULL);
	//線分衝突判定.
	bool RayCollsion(clsChara* pAttacker, clsChara* pTarget);
	bool Intersect(clsChara* pAttacker, clsChara* pTarget, float* pfDistance, D3DXVECTOR3* pIntersect);

	//---------------------------------------------.

	HRESULT	FindVerticesOnPoly(LPD3DXMESH pMeshForRay, 
		DWORD dwPolyIndex, D3DXVECTOR3* pVecVertices);


	//Init・Actionは各オブジェクトに割り当てる.
	virtual void Init() = 0;
	virtual void Action() = 0;

	virtual void Init_NotColl() {};
	virtual void Action2(){};


/*************アニメーション**************/
public:

private:
	D3DXMATRIX Target_Yaw;

public:
	double animTime;				//ｱﾆﾒｰｼｮﾝ時間.

	/********設定関数*********/
	//座標設定.
	void SetPosition(D3DXVECTOR3 &vPos){ m_vPos = vPos; }
	//大きさ設定.
	void SetScale(float Scale){ m_Scale = Scale; }
	//スキンメッシュの大きさ設定.
	void SetSkinSize(D3DXVECTOR3 SkinScale){ m_SkinScale = SkinScale; }
	//回転値設定(x:Pitch y:Yaw z:Roll).
	void SetRot(D3DXVECTOR3 &vRot){ m_vRot = vRot; }

	/********取得関数(xモデル)*********/
	clsDx9Mesh* GetStaicMesh(){ return m_pStaticMesh; }

	//個別でほしいときはGetPos().x or y or zでとれる.
	D3DXVECTOR3 GetPos(){ return m_vPos; }
	//同上.
	D3DXVECTOR3 GetRot() { return m_vRot; }
	//大きさ取得.
	float GetScale(){ return m_Scale; }
	//スキンメッシュ大きさ取得.
	D3DXVECTOR3 Getm_SkinScale() { return m_SkinScale; }

	/********取得関数(スキンメッシュモデル)*********/

	//その他共通項目.
	bool m_DeadFlag = false;
	int	 m_HP;
	int m_DamageHP;
	bool m_Stopping;

public:
	//ｽﾌｨｱ作成.
	HRESULT InitSphere(clsDx9Mesh* pMesh = NULL);

	//ﾊﾞｳﾝﾃﾞｨﾝｸﾞﾎﾞｯｸｽ作成.
	HRESULT	InitBBox(clsDx9Mesh* pMesh = NULL);

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3	m_vRot;						//回転値の設定.
	D3DXVECTOR3 m_vCRot;					//回転値比較用.
	float		m_ScaleSize;				//データ読み込み時のモデルサイズ代入用.

	float		m_Yaw;
	float		m_Pitch;					//Intersect関数で使用.
	float		m_SaveYaw[10];

	D3DXVECTOR3	vIntersect;					//交差座標(レイ判定) 影の位置にも使用.

	HIT_POS		m_tagHitPos;

	int			ActState = 0;
	int			ActState2 = 0;

protected:
	//処理用モデル宣言.
	clsDx9Mesh*			m_pStaticMesh;
	clsD3DXSKINMESH*	m_pStaticSkinMesh;

	float				m_Scale;			//大きさ.
	float				m_Roll;
	bool				bHit;				//線分が当たったか.
	D3DXVECTOR3			m_SkinScale;
	SSPHERE				m_SSphere;			//判定生成用.
	BBOX				m_BBox;				//同上.

	D3DXVECTOR3			m_vRay;				//ﾚｲの位置.
	D3DXVECTOR3			m_vAxis;			//回転軸.


};



#endif