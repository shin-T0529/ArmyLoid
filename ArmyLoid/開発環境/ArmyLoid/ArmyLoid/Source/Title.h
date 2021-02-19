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

// �^�C�g���V�[��.
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

protected://�֐�.
	void Button(cls2DTex* enTex ,int &Count);

	void LoadPos(LPCSTR File, int Max);

	void LoadRot(LPCSTR File, int Max);

protected://�ϐ�.
	int  TitleCnt;
	int  CurSorCnt;

	bool MenuSelect;
	bool SceneEff;
	int APPLY;

	int	SEContU;			//�I��������p.
	int	SEContD;			//�I��������p.
	//�^�C�g����ʂł̃E�B���h�E�j���p.
	bool DestroyWin = false;

	bool StartCheck = false;				//�A���J�n�h�~�p.
	//�Ǎ��p.
	D3DXVECTOR3 LoadMPos[10];
	D3DXVECTOR3 LoadMRot[10];
private:
	//���f��.
	clsPlayer*					T_P1;		//�^�C�g����ʕ\���p.

	clsObject*					Ground;
	clsObject*					Sora;

	//2D�e�N�X�`��.
	cls2DTex*					Title;
	cls2DTex*					TitleButton;
	cls2DTex*					TitleMenu;
	cls2DTex*					TitleCurSor; //495 485 495 582
	cls2DTex*					BlackScreen;
	cls2DTex*					TitleStart;
	//BGM�ESE.
	clsDirectSound*				TitleBGM;
	clsDirectSound*				Add;				//���艹.
	clsDirectSound*				SelectSound;

private://�s�ϕ���.
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