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
//enum�E�\����.
//-------------------
	//�V�[������.

// �V�[���}�l�[�W��.
class CSceneManager
{
public:
	CSceneManager();

	~CSceneManager();
	//X�t�@�C�����f��.
	void MeshLoad(clsResource *SetUP);
	//2�c�e�N�X�`��.
	void TexLoad(cls2DTexResource *SetUP);
	//�X�L�����b�V�����f��.
	void SkinLoad(clsSkinResource *SetUP, clsDInput* Controller,
		ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd);
	//�G�t�F�N�g�f�[�^.
	void EffLoad(clsEffectsResouce *SetUP);
	//�T�E���h�f�[�^.
	void SoundLoad(clsSoundResource* SetUP);
	//�e�V�[������.
	void Update(SceneIndex &Scene, clsDInput* Controller);
	//�E�B���h�E����p.
	void WindowDestroy(HWND hWnd);
	//�f�o�b�O�e�L�X�g.
	void UpdateDebugMode(ID3D11DeviceContext* pContext);

	//���f���E�e�N�X�`���`��
	void Draw(D3DXMATRIX & mView, D3DXMATRIX & mProj, D3DXVECTOR3 & vLight, D3DXVECTOR3 & vEye, SPRITE_STATE & ss);

	//�J����.
	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

	void SetLightPos(D3DXVECTOR3 &vLight);
	//�V�[���̐ݒ�.
	void SetNextScene(SceneIndex index);
	void TransitionScene();
	//�V�[���ύX.
	bool CheckingScene = false;

private:

	CSceneBase*		m_pScene;

	CSceneTitle*	m_pS_Title;
	SceneIndex		m_NowScnene;
	SceneIndex		m_NextScene;


};


#endif