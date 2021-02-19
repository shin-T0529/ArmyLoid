#ifndef DRAW_H
#define DRAW_H

#include <string>
#include <vector>

#include "Base.h"
#include "C_Player.h"
#include "C_Object.h"
#include "DebugText.h"


class CSceneDraw :public CSceneBase
{
public:
	CSceneDraw(const char* SceneName);
	virtual ~CSceneDraw();
	

	void Load(clsResource *SetUP);
	
	void Load(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd) {}

	void Load(cls2DTexResource *SetUP);

	void Load(clsSoundResource* SetUP);

	void Load(clsSkinResource *SetUP, clsDInput *Controller,
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);

	void Load(clsEffectsResouce* SetUP);

	void Control();

	void Action(SceneIndex &Scene, clsDInput* Controll);

	void Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss);

	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

	void SetLightPos(D3DXVECTOR3 &vLight);

private:

	void SparkTiming(int &Cnt, clsEffectsMain* SparkEff,
					 clsPlayer* Play1, clsPlayer* Play2,bool &flag);

	void UseChangeScreen(bool RetryFlag, cls2DTex* WScreenTex,
		bool ExitFlag, cls2DTex* BScreenTex,SceneIndex &Scene);
	void DebugUIPos(cls2DTex* PosSetTex);

	void LoadPos(LPCSTR File, int Max);

	void LoadRot(LPCSTR File, int Max);

private:	
	//���f��.
	clsPlayer*					D_1P;
	clsPlayer*					D_2P;

	clsObject*					Ground;
	clsObject*					Sora;

	//������I����Ă��烊�g���Cor�C�O�W�b�g��I��������.
	cls2DTex*					DrawTex;
	cls2DTex*					Cursor;				//�I���J�[�\��.
	cls2DTex*					DecCursor;			//�����̃J�[�\��.
	cls2DTex*					RetryPoint;			//���g���C����.
	cls2DTex*					ExitPoint;			//�ްяI������(�N���W�b�g�ڍs).

	cls2DTex*					BlackScreen;		//�N���W�b�g�ڍs�p�Ö�.
	cls2DTex*					WhiteScreen;		//���g���C�I�����ڍs�p����.

	//BGM�͂܂�.
	clsDirectSound*				DrawBGM;

	clsDirectSound*				Add;				//���艹.
	clsDirectSound*				SelectSound;

	//�G�t�F�N�g.
	clsEffectsMain*				LoseSpark;					//�v���C���[�̔��U.

private:
	int xi = 19;

	int							SEContR;			//�I��������p.
	int							SEContL;			//�I��������p.

	int							APPLY;
	int							DispCnt;			//�Ȃɂقڍ��킹�邱��.
	int							SparkCnt;
	int							BGMCnt;
	bool						Blackmove;			//�N���W�b�g��.
	bool						Whitemove;			//���g���C��Play��.

	bool						SparkFlag = false;
	//�Ǎ��p.
	D3DXVECTOR3 LoadMPos[10];
	D3DXVECTOR3 LoadMRot[10];

private:
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;
	ID3D11Texture2D*			m_pBackBuffer_DSTex;
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;
	clsDebugText*				DrawDebug;
	clsLoadData*				m_mLoad;
};
#endif // !DRAW_H
