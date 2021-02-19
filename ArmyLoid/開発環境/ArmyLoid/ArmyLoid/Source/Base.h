#ifndef SCENE_BASE
#define SCENE_BASE

#define ENABLE_EFFEKSEER	//effekseer有効.

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "cCommon.h"
#include "CResource.h"
#include "TexResource.h"
#include "CSkinResource.h"
#include "EffectResouse.h"
#include "DirectSoundResource.h"
#include "2DTex.h"
//#include "DInput.h"
#include "TwinDInput.h"
#ifdef ENABLE_EFFEKSEER
#include "CEffects.h"
#endif//#ifdef ENABLE_EFFEKSEER
// 基底シーンクラス.
class CSceneBase
{

public:
	CSceneBase(const char* SceneName)
	{ strcpy_s(m_SceneName, SceneName); }
	
	virtual ~CSceneBase() {}
	//Xモデル.
	virtual void Load(clsResource *SetUP) {}
	//2Dテクスチャ.
	virtual void Load(cls2DTexResource *SetUP) {}
	//スキンメッシュ.
	virtual void Load(clsSkinResource *SetUP, clsDInput *Controller
		,ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd) {}
	//Effekseer.
	virtual void Load(clsEffectsResouce *SetUP) {}
	//サウンド.
	virtual void Load(clsSoundResource* SetUP){}
	//各シーン処理.
	virtual void Update(SceneIndex &Scene, clsDInput* Controller)
	{
		Control();
		Action(Scene, Controller);
	};
	//各シーンデバッグモード用処理.
	virtual void UpdateDebugMode(ID3D11DeviceContext* pContext)
	{
		DebugText(pContext);
	}
	virtual void WindowHandleOver(HWND hWnd){}

	virtual void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw) {};

	virtual void SetLightPos(D3DXVECTOR3 &vLight) {};

	virtual void Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss){}

public:
	//変数・定数.

protected:
	//操作・動作を入れる関数.
	virtual void Control() {}
	//当たり判定などを入れる関数　引数はシーン情報 / コントローラー情報.
	virtual	void Action(SceneIndex &Scene, clsDInput* Controller){}
	virtual void DebugText(ID3D11DeviceContext* pContext){}
	char m_SceneName[64];


};
#endif