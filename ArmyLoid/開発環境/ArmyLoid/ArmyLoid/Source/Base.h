#ifndef SCENE_BASE
#define SCENE_BASE

#define ENABLE_EFFEKSEER	//effekseer�L��.

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
// ���V�[���N���X.
class CSceneBase
{

public:
	CSceneBase(const char* SceneName)
	{ strcpy_s(m_SceneName, SceneName); }
	
	virtual ~CSceneBase() {}
	//X���f��.
	virtual void Load(clsResource *SetUP) {}
	//2D�e�N�X�`��.
	virtual void Load(cls2DTexResource *SetUP) {}
	//�X�L�����b�V��.
	virtual void Load(clsSkinResource *SetUP, clsDInput *Controller
		,ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd) {}
	//Effekseer.
	virtual void Load(clsEffectsResouce *SetUP) {}
	//�T�E���h.
	virtual void Load(clsSoundResource* SetUP){}
	//�e�V�[������.
	virtual void Update(SceneIndex &Scene, clsDInput* Controller)
	{
		Control();
		Action(Scene, Controller);
	};
	//�e�V�[���f�o�b�O���[�h�p����.
	virtual void UpdateDebugMode(ID3D11DeviceContext* pContext)
	{
		DebugText(pContext);
	}
	virtual void WindowHandleOver(HWND hWnd){}

	virtual void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw) {};

	virtual void SetLightPos(D3DXVECTOR3 &vLight) {};

	virtual void Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss){}

public:
	//�ϐ��E�萔.

protected:
	//����E���������֐�.
	virtual void Control() {}
	//�����蔻��Ȃǂ�����֐��@�����̓V�[����� / �R���g���[���[���.
	virtual	void Action(SceneIndex &Scene, clsDInput* Controller){}
	virtual void DebugText(ID3D11DeviceContext* pContext){}
	char m_SceneName[64];


};
#endif