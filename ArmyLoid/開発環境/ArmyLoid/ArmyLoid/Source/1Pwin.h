#ifndef OVER_H
#define OVER_H

#include <string>
#include <vector>

#include "Base.h"
#include "DX9Mesh.h"
#include "ED3DXSKINMESH.h"

#include "Charactor.h"
#include "C_Player.h"
#include "C_Object.h"
#include "CRay.h"

// タイトルシーン.
class CSceneP1win : public CSceneBase
{
public:
	CSceneP1win(const char* SceneName);
	virtual ~CSceneP1win();

	void Control();

	void Action(SceneIndex &Scene, clsDInput* Controll);

	void Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss);

	void Load(clsResource *SetUP);

	void Load(cls2DTexResource *SetUP);

	void Load(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd) {}

	void Load(clsEffectsResouce* SetUP);

	void Load(clsSoundResource* SetUP);

	void Load(clsSkinResource *SetUP, clsDInput *Controller,
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);

	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

	void SetLightPos(D3DXVECTOR3 &vLight);

	void UpdateDebugMode(ID3D11DeviceContext* pContext);

private:
	void UseChangeScreen(bool RetryFlag, cls2DTex* WScreenTex,
		bool ExitFlag, cls2DTex* BScreenTex, SceneIndex &Scene);

	void LoadPos(LPCSTR File, int Max);
	void LoadRot(LPCSTR File, int Max);

private:
	//モデル.
	clsPlayer*					W1_P1;
	clsPlayer*					L1_P2;

	clsObject*					Ground;
	clsObject*					Sora;

	//テクスチャ.
	cls2DTex*					Win_1P;
	cls2DTex*					Cursor;				//選択カーソル.
	cls2DTex*					DecCursor;				//決定後のカーソル.
	cls2DTex*					RetryPoint;			//リトライ項目.
	cls2DTex*					ExitPoint;			//ｹﾞｰﾑ終了項目(クレジット移行).

	cls2DTex*					BlackScreen;		//クレジット移行用暗幕.
	cls2DTex*					WhiteScreen;		//リトライ選択時移行用明幕.

	clsDirectSound*				WinFanfare;
	clsDirectSound*				Add;				//決定音.
	clsDirectSound*				SelectSound;

	clsEffectsMain*				LoseSpark;					//プレイヤーの爆散.
private:

	int							SEContR;			//選択音制御用.
	int							SEContL;			//選択音制御用.

	int							APPLY;
	int							DispCnt;			//曲にほぼ合わせること.
	int							SparkCnt;
	bool						Blackmove;			//クレジットへ.
	bool						Whitemove;			//リトライしPlayへ..
	bool						KeyDown;

	//読込用.
	D3DXVECTOR3 LoadMPos[10];
	D3DXVECTOR3 LoadMRot[10];

private:
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;
	ID3D11Texture2D*			m_pBackBuffer_DSTex;
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;
	clsDebugText*				Win1Debug;
	clsLoadData*				m_mLoad;
};
#endif