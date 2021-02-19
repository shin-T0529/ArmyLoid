#ifndef CREDIT_H
#define CREDIT_H

#include <string>
#include <vector>

#include "Base.h"
#include "C_Player.h"
//#include "cCommon.h"

#include "DebugText.h"

class CSceneCredit :public CSceneBase
{
public:
	CSceneCredit(const char* SceneName);
	virtual ~CSceneCredit();
	
	void Load(clsResource *SetUP);
	
	void Load(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd) {}

	void Load(cls2DTexResource *SetUP);

	void Load(clsSoundResource* SetUP);

	void Load(clsSkinResource *SetUP, clsDInput *Controller
		, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd);

	void Control();

	void Action(SceneIndex &Scene, clsDInput* Controll);

	void Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss);

	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

	void SetLightPos(D3DXVECTOR3 &vLight);

public:
	bool	ChackScreen;
private:
	int		ChangeCnt;
	float	ScroolSpeed;
	int		APPLY;
	bool	SetAnim;
private:

	clsPlayer*					CreP_1P;
	clsPlayer*					Back;
	//�N���W�b�g�ɂ̓��f������ҁE�����擾�T�C�g�E�v���O���}�[�̖��O.
	//URL���o�� BGM���Y��Ȃ�.
	cls2DTex*					CreditList;				//�����摜.
	cls2DTex*					CreditBack;				//�w�i.
	cls2DTex*					BlackScreen;

	clsDirectSound*				BGM;

private:	//�s��.
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;
	ID3D11Texture2D*			m_pBackBuffer_DSTex;
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;
	clsDebugText*				TitleDebug;
};
#endif // !CREDIT_H
