#ifndef GAME_H
#define GAME_H

#pragma warning(disable:4005)
#pragma warning(disable:4305)
#pragma warning(disable:4819)
#pragma warning(disable:4996)

#include "cCommon.h"
#include "Global.h"

#include "TwinDInput.h"
//#include "DInput.h"

#include "Direct3D.h"
#include "SceneManager.h"

#include "DX9Mesh.h"
#include "ED3DXSKINMESH.h"
#include "CEffects.h"
#include "DSound.h"

#include "DebugText.h"

#include "C_LoadData.h"

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )//「D3DX～」の定義使用時に必要.

//外部データ.
//座標読み込みデータ構造体.
struct ObjectPosList
{
	//floatで座標を設定しているのでfloatに合わせる.
	float m_ePosX;
	float m_ePosY;
	float m_ePosZ;
};
//座標読込.
struct Data
{
	D3DXVECTOR3 stPos;
	float m_eScalef;
	D3DXVECTOR4 PosSca;
	D3DXVECTOR2 SpriteState;
};

//構造体.
//カメラ情報.
struct CAMERA
{
	D3DXVECTOR3			vEye;					//カメラ位置.
	D3DXVECTOR3			vLook;					//カメラ注視位置.
	float				Yaw;					//Y回転軸.
	D3DXMATRIX			mRotation;				//回転行列.
};

class clsGame
{
public:
	clsGame();
	~clsGame();

	void			Loop();
	void			AppMain();
	void			Render();
	//最初のデータ読込.
	void			LoadData();
	void			CreateResource(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//ｶﾒﾗ関数.
	void			Camera();
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ関数.
	void			Proj();

	//リソース用.
	clsResource*				m_pResource;
	cls2DTexResource*			m_pResourceTex;
	clsSkinResource*			m_pResourceSkin;
	clsEffectsResouce*			m_pResourceEff;
	clsSoundResource*			m_pResourceSound;
	//シーン.
	CSceneManager*				pSceneMgr;
	//コントローラー設定.
	clsDInput*					m_pCtrler;

	//Direct3D生成.
	clsDirect3D*				m_pDirect3D;

	HWND						m_hWnd;

public:
	//シーンの情報.
	SceneIndex index = SceneIndex::TITLE;

	clsDirectSound*				m_pD_Sound;

private:
	//読込用.
	int SSi = 0;
	int SSLi = 0;

	const char* SSData = "Data\\csv\\Texture\\SpriteState\\TexSizeSS.csv";
private:
	//武装一覧の分割読込用.
	int WeponStart = enUIShoot1PMaWeponTex;
	int WeponEnd   = enUIShoot2PMiWeponTex;

	//制限時間の分割読込用.
	int TimeStart = enUITimeMi;
	int TimeEnd   = enUITimese2;

private:
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	ID3D11SamplerState*			m_pSampleLinear;		//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.
	ID3D11ShaderResourceView*	m_pTexture;				//ﾃｸｽﾁｬ.
	CAMERA						m_Camera;				//ｶﾒﾗ情報.
	Data						List;
	D3DXMATRIX					m_mView;				//ﾋﾞｭｰ(ｶﾒﾗ行列).
	D3DXMATRIX					m_mProj;				//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列.
	D3DXVECTOR3					m_vLight;				//ﾗｲﾄの方向.

	SPRITE_STATE				SpSt;

	clsDx9Mesh*					m_pClsDx9Mesh;			//Dx9ﾒｯｼｭｸﾗｽ.

	clsLoadData*				m_Load;					//外部データ取得ｸﾗｽ.

	clsEffects*					m_EffectData;
	Manager*					m_pManager;
};


#endif//#ifndef GAME_H