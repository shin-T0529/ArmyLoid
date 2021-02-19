#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H


#include "Base.h"
#include "Title.h"
#include "Manual.h"
#include "Play.h"
#include "1Pwin.h"
#include "2Pwin.h"
#include "Draw.h"
#include "Credit.h"
//-------------------
//enum・構造体.
//-------------------
	//シーン内訳.

// シーンマネージャ.
class CSceneManager
{
public:
	CSceneManager();

	~CSceneManager();
	//Xファイルモデル.
	void MeshLoad(clsResource *SetUP);
	//2Ｄテクスチャ.
	void TexLoad(cls2DTexResource *SetUP);
	//スキンメッシュモデル.
	void SkinLoad(clsSkinResource *SetUP, clsDInput* Controller,
		ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd);
	//エフェクトデータ.
	void EffLoad(clsEffectsResouce *SetUP);
	//サウンドデータ.
	void SoundLoad(clsSoundResource* SetUP);
	//各シーン動作.
	void Update(SceneIndex &Scene, clsDInput* Controller);
	//ウィンドウ閉じる用.
	void WindowDestroy(HWND hWnd);
	//デバッグテキスト.
	void UpdateDebugMode(ID3D11DeviceContext* pContext);

	//モデル・テクスチャ描画
	void Draw(D3DXMATRIX & mView, D3DXMATRIX & mProj, D3DXVECTOR3 & vLight, D3DXVECTOR3 & vEye, SPRITE_STATE & ss);

	//カメラ.
	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

	void SetLightPos(D3DXVECTOR3 &vLight);
	//シーンの設定.
	void SetNextScene(SceneIndex index);
	void TransitionScene();
	//シーン変更.
	bool CheckingScene = false;

private:

	CSceneBase*		m_pScene;

	CSceneTitle*	m_pS_Title;
	SceneIndex		m_NowScnene;
	SceneIndex		m_NextScene;


};


#endif