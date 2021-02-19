#ifndef CLEAR_H
#define CLEAR_H
#include <string>
#include <vector>

//#include "cCommon.h"
#include "Base.h"
#include "C_Player.h"
#include "C_Object.h"

// �^�C�g���V�[��.
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
	//���f��.
	clsPlayer*					W2_P2;
	clsPlayer*					W2_L1;

	clsObject*					Ground;
	clsObject*					Sora;

	cls2DTex*					Win_2P;
	cls2DTex*					Cursor;				//�I���J�[�\��.
	cls2DTex*					DecCursor;			//�����̃J�[�\��.
	cls2DTex*					RetryPoint;			//���g���C����.
	cls2DTex*					ExitPoint;			//�ްяI������(�N���W�b�g�ڍs).

	cls2DTex*					BlackScreen;		//�N���W�b�g�ڍs�p�Ö�.
	cls2DTex*					WhiteScreen;		//���g���C�I�����ڍs�p����.

	clsDirectSound*				WinFanfare;
	clsDirectSound*				Add;				//���艹.
	clsDirectSound*				SelectSound;

	clsEffectsMain*				LoseSpark;					//�v���C���[�̔��U.
private:
	int							SEContR;			//�I��������p.
	int							SEContL;			//�I��������p.

	int							APPLY;
	int							DispCnt;			//�Ȃɂقڍ��킹�邱��.
	int							SparkCnt;

	bool						KeyDown;
	bool						Blackmove;			//�N���W�b�g��.
	bool						Whitemove;			//���g���C��Play��.

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
	clsDebugText*				Win2PDebug;
	clsLoadData*				m_mLoad;
};
#endif