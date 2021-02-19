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
#pragma comment( lib, "d3dx10.lib" )//�uD3DX�`�v�̒�`�g�p���ɕK�v.

//�O���f�[�^.
//���W�ǂݍ��݃f�[�^�\����.
struct ObjectPosList
{
	//float�ō��W��ݒ肵�Ă���̂�float�ɍ��킹��.
	float m_ePosX;
	float m_ePosY;
	float m_ePosZ;
};
//���W�Ǎ�.
struct Data
{
	D3DXVECTOR3 stPos;
	float m_eScalef;
	D3DXVECTOR4 PosSca;
	D3DXVECTOR2 SpriteState;
};

//�\����.
//�J�������.
struct CAMERA
{
	D3DXVECTOR3			vEye;					//�J�����ʒu.
	D3DXVECTOR3			vLook;					//�J���������ʒu.
	float				Yaw;					//Y��]��.
	D3DXMATRIX			mRotation;				//��]�s��.
};

class clsGame
{
public:
	clsGame();
	~clsGame();

	void			Loop();
	void			AppMain();
	void			Render();
	//�ŏ��̃f�[�^�Ǎ�.
	void			LoadData();
	void			CreateResource(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//��׊֐�.
	void			Camera();
	//��ۼު���݊֐�.
	void			Proj();

	//���\�[�X�p.
	clsResource*				m_pResource;
	cls2DTexResource*			m_pResourceTex;
	clsSkinResource*			m_pResourceSkin;
	clsEffectsResouce*			m_pResourceEff;
	clsSoundResource*			m_pResourceSound;
	//�V�[��.
	CSceneManager*				pSceneMgr;
	//�R���g���[���[�ݒ�.
	clsDInput*					m_pCtrler;

	//Direct3D����.
	clsDirect3D*				m_pDirect3D;

	HWND						m_hWnd;

public:
	//�V�[���̏��.
	SceneIndex index = SceneIndex::TITLE;

	clsDirectSound*				m_pD_Sound;

private:
	//�Ǎ��p.
	int SSi = 0;
	int SSLi = 0;

	const char* SSData = "Data\\csv\\Texture\\SpriteState\\TexSizeSS.csv";
private:
	//�����ꗗ�̕����Ǎ��p.
	int WeponStart = enUIShoot1PMaWeponTex;
	int WeponEnd   = enUIShoot2PMiWeponTex;

	//�������Ԃ̕����Ǎ��p.
	int TimeStart = enUITimeMi;
	int TimeEnd   = enUITimese2;

private:
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	ID3D11SamplerState*			m_pSampleLinear;		//ø����̻���� ø����Ɋe��̨����������.
	ID3D11ShaderResourceView*	m_pTexture;				//ø���.
	CAMERA						m_Camera;				//��׏��.
	Data						List;
	D3DXMATRIX					m_mView;				//�ޭ�(��׍s��).
	D3DXMATRIX					m_mProj;				//��ۼު���ݍs��.
	D3DXVECTOR3					m_vLight;				//ײĂ̕���.

	SPRITE_STATE				SpSt;

	clsDx9Mesh*					m_pClsDx9Mesh;			//Dx9ү���׽.

	clsLoadData*				m_Load;					//�O���f�[�^�擾�׽.

	clsEffects*					m_EffectData;
	Manager*					m_pManager;
};


#endif//#ifndef GAME_H