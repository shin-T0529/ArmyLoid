#ifndef CLEAR_H
#define CLEAR_H
#include <string>
#include <vector>

//#include "cCommon.h"
#include "Base.h"
#include "C_Player.h"
#include "C_Object.h"

// タイトルシーン.
class CSceneP2win : public CSceneBase
{
public:
	CSceneP2win(const char* SceneName);
	virtual ~CSceneP2win();

	void Control();

	void Action(SceneIndex &Scene, clsDInput* Controll);

	void Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss);


	void Load(clsResource *SetUP);

	void Load(cls2DTexResource *SetUP);

	void Load(clsSoundResource* SetUP);

	void Load(clsEffectsResouce* SetUP);

	void Load(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd) {}

	void Load(clsSkinResource *SetUP, clsDInput *Controller,
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);

	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

	void UpdateDebugMode(ID3D11DeviceContext* pContext);

	void SetLightPos(D3DXVECTOR3 &vLight);


private:
	void UseChangeScreen(bool RetryFlag, cls2DTex* WScreenTex,
		bool ExitFlag, cls2DTex* BScreenTex, SceneIndex &Scene);

	void LoadPos(LPCSTR File, int Max)
	{
		for (int i = 0; i < Max; i++)
		{
			LoadMPos[i] = m_mLoad->LoadData(File, i + 1);
		}
	}

	void LoadRot(LPCSTR File, int Max)
	{
		for (int i = 0; i < Max; i++)
		{
			LoadMRot[i] = m_mLoad->LoadData(File, i + 1);
		}
	}

private:
	//モデル.
	clsPlayer*					W2_P2;
	clsPlayer*					W2_L1;

	clsObject*					Ground;
	clsObject*					Sora;

	cls2DTex*					Win_2P;
	cls2DTex*					Cursor;				//選択カーソル.
	cls2DTex*					DecCursor;			//決定後のカーソル.
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

	bool						KeyDown;
	bool						Blackmove;			//クレジットへ.
	bool						Whitemove;			//リトライしPlayへ.

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
	clsDebugText*				Win2PDebug;
	clsLoadData*				m_mLoad;
};
#endif