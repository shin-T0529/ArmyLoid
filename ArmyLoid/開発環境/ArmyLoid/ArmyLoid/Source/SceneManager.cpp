#include "SceneManager.h"
#include <crtdbg.h>

CSceneBase* CreateScene(SceneIndex index)
{
	_ASSERT_EXPR(SceneIndex::Start <= index && index <= SceneIndex::End, L"�ݒ肳�ꂽ�V�[���ȊO��ݒ肵��");

	switch (index) 
	{
	case SceneIndex::TITLE:
		return new CSceneTitle("Title");	
	case SceneIndex::MANUAL:				
		return new CSceneManual("Play Manual");
	case SceneIndex::PLAY:					
		return new CSceneGameMain("GameMain");
	case SceneIndex::P1WIN:					
		return new CSceneP1win("P1win");	
	case SceneIndex::P2WIN:					
		return new CSceneP2win("P2win");	
	case SceneIndex::DRAW:					
		return new CSceneDraw("Draw");		
	case SceneIndex::CREDIT:				
		return new CSceneCredit("Credit");	
	default:
		return nullptr;
	}
}

CSceneManager::CSceneManager()
	: m_pScene(nullptr)
	, m_NowScnene(SceneIndex::Nothing)
	, m_NextScene(SceneIndex::TITLE)
{
	TransitionScene();
}

CSceneManager::~CSceneManager()
{
	if (m_pScene != nullptr) 
	{
		delete m_pScene;
		m_pScene = nullptr;
	}
}
//���f���Ǎ�.
void CSceneManager::MeshLoad(clsResource *SetUP)
{
	m_pScene->Load(SetUP);

}
//�e�N�X�`���Ǎ�.
void CSceneManager::TexLoad(cls2DTexResource *SetUP)
{
	m_pScene->Load(SetUP);
}
//���f���Ǎ�.
void CSceneManager::SkinLoad(clsSkinResource *SetUP, clsDInput *Controller
	,ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd)
{
	m_pScene->Load(SetUP, Controller,pDevice11, pContext11, hWnd);

}
//�G�t�F�N�g�Ǎ�.
void CSceneManager::EffLoad(clsEffectsResouce *SetUP)
{
	m_pScene->Load(SetUP);
}
//�T�E���h�f�[�^�Ǎ�.
void CSceneManager::SoundLoad(clsSoundResource* SetUP)
{
	m_pScene->Load(SetUP);
}
void CSceneManager::SetNextScene(SceneIndex index)
{
	m_NextScene = index;
}

void CSceneManager::SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw)
{
	m_pScene->SetCameraPos(vLook, vEye, Yaw);
}

void CSceneManager::SetLightPos(D3DXVECTOR3 &vLight)
{
	m_pScene->SetLightPos(vLight);
}

void CSceneManager::Update(SceneIndex &Scene, clsDInput* Controller)
{
	SceneIndex temp = Scene;
	m_pScene->Update(Scene, Controller);
	//�L�^�����V�[���ƈႦ�Ύ���Main�ŃV�[����ς���.
	if (temp != Scene)
	{
		CheckingScene = true;
	}
}

void CSceneManager::WindowDestroy(HWND hWnd)
{
	m_pScene->WindowHandleOver(hWnd);
}
void CSceneManager::UpdateDebugMode(ID3D11DeviceContext* pContext)
{
	m_pScene->UpdateDebugMode(pContext);
}
//X���f���E�X�L�����b�V�����f���E2D�e�N�X�`���̕`��.
void CSceneManager::Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye,SPRITE_STATE &ss)
{
	m_pScene->Draw(mView,mProj,vLight,vEye, ss);
}


void CSceneManager::TransitionScene()
{
	if (m_NowScnene != m_NextScene)
	{
		if (m_pScene != nullptr)
		{
			delete m_pScene;
			m_pScene = nullptr;
		}
		m_pScene = CreateScene(m_NextScene);
		m_NowScnene = m_NextScene;
	}
}
