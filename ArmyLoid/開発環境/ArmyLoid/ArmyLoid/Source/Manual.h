#ifndef MANUAL_H
#define MANUAL_H

#include <string>
#include <vector>

#include "Base.h"
#include "C_Object.h"

#include "DebugText.h"
// タイトルシーン.
class CSceneManual : public CSceneBase
{
public:
	CSceneManual(const char* SceneName);

	virtual ~CSceneManual();

	void Load(clsResource *SetUP) {}

	void Load(cls2DTexResource *SetUP);

	void Load(clsSkinResource *SetUP, clsDInput *Controller
		, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd);

	void Load(clsSoundResource* SetUP);

	void Control();

	void Action(SceneIndex &Scene, clsDInput* Controll);

	void Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss);

	void UpdateDebugMode(ID3D11DeviceContext* pContext);

	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

protected:
	//初期値.
	D3DXVECTOR3		Pos, Pos1, Pos2;
	D3DXVECTOR3		CameraPos = D3DXVECTOR3(0.0f, 2.0f, 2.0f);

	//説明切り替え①.
	bool ManualMode; //false:操作説明　true:ルール説明.
	bool MoveNow;	 //画面切り替わり中は逆方向にいかないようにする.
	int APPLY;		 //変更用加速度.

	//準備完了・未完了関連②.
	int FlashCnt;
	bool Ready[PlayerMAX];	//開始フラグ.
	
	//場面切り替え③.
	int StartSoundCnt, MinusApply, WaitCnt;
protected:
	//①.//操作説明画面切り替え処理関数.
	void ManualModeChange(bool Flag);
	//②.//準備関数.
	void AreYouReady(cls2DTex* noTex, cls2DTex* Tex, bool Flag);
	//②.//OKを点滅させる.
	void UIFlash(cls2DTex* enTex, int &Count, bool Flag);
	//③.//シーン変更と幕.
	void ChangeScene(SceneIndex &Scene);
	//変更まとめ.
	void TexChange(clsDInput* Control);
private:
	//ロボ.
	clsObject*					M_1PRobo;
	clsObject*					M_2PRobo;

	//テクスチャ.
	cls2DTex*					PlayManual;
	cls2DTex*					PlayRule;
	cls2DTex*					PlayerPrepa[PlayerMAX];
	cls2DTex*					PlayerOK[PlayerMAX];
	cls2DTex*					PlayerName1;
	cls2DTex*					PlayerName2;

	cls2DTex*					WhiteScreen;

	clsDebugText*				ManualDebug;

	clsDirectSound*				BGM;
	//SE.
	clsDirectSound*				Ready1P;
	clsDirectSound*				Ready2P;
	clsDirectSound*				Start;
private:	//不変部分.
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;
	ID3D11Texture2D*			m_pBackBuffer_DSTex;
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;

};
#endif