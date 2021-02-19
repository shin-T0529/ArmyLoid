#ifndef TITLE_H
#define TITLE_H

#include <string>
#include <vector>

#include "Base.h"
#include "C_Player.h"
#include "C_Object.h"
#include "DebugText.h"

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

// タイトルシーン.
class CSceneTitle : public CSceneBase
{
public:
	CSceneTitle(const char* SceneName);

	virtual ~CSceneTitle();

	void Load(clsResource* SetUP);

	void Load(cls2DTexResource *SetUP);

	void Load(clsSoundResource* SetUP);

	void Load(clsSkinResource *SetUP, clsDInput *Controller,
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);

	void Control();

	void Action(SceneIndex &Scene, clsDInput* Controll);

	void Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss);

	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

	void UpdateDebugMode(ID3D11DeviceContext* pContext);

	void SetLightPos(D3DXVECTOR3 &vLight);

	void WindowHandleOver(HWND hWnd)
	{
		if (DestroyWin)
		{
			DestroyWindow(hWnd);
		}
	}

protected://関数.
	void Button(cls2DTex* enTex ,int &Count);

	void LoadPos(LPCSTR File, int Max);

	void LoadRot(LPCSTR File, int Max);

protected://変数.
	int  TitleCnt;
	int  CurSorCnt;

	bool MenuSelect;
	bool SceneEff;
	int APPLY;

	int	SEContU;			//選択音制御用.
	int	SEContD;			//選択音制御用.
	//タイトル画面でのウィンドウ破棄用.
	bool DestroyWin = false;

	bool StartCheck = false;				//連続開始防止用.
	//読込用.
	D3DXVECTOR3 LoadMPos[10];
	D3DXVECTOR3 LoadMRot[10];
private:
	//モデル.
	clsPlayer*					T_P1;		//タイトル画面表示用.

	clsObject*					Ground;
	clsObject*					Sora;

	//2Dテクスチャ.
	cls2DTex*					Title;
	cls2DTex*					TitleButton;
	cls2DTex*					TitleMenu;
	cls2DTex*					TitleCurSor; //495 485 495 582
	cls2DTex*					BlackScreen;
	cls2DTex*					TitleStart;
	//BGM・SE.
	clsDirectSound*				TitleBGM;
	clsDirectSound*				Add;				//決定音.
	clsDirectSound*				SelectSound;

private://不変部分.
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;
	ID3D11Texture2D*			m_pBackBuffer_DSTex;
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;
	clsDebugText*				TitleDebug;
	clsLoadData*				m_mLoad;
};
#endif