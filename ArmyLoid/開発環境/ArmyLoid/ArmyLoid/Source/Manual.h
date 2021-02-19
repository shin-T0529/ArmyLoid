#ifndef MANUAL_H
#define MANUAL_H

#include <string>
#include <vector>

#include "Base.h"
#include "C_Object.h"

#include "DebugText.h"
// �^�C�g���V�[��.
class CSceneManual : public CSceneBase
{
public:
	CSceneManual(const char* SceneName);

	virtual ~CSceneManual();

	void Load(clsResource *SetUP) {}

	void Load(cls2DTexResource *SetUP);

	void Load(clsSkinResource *SetUP, clsDInput *Controller
		, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd);

	void Load(clsSoundResource* SetUP);

	void Control();

	void Action(SceneIndex &Scene, clsDInput* Controll);

	void Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss);

	void UpdateDebugMode(ID3D11DeviceContext* pContext);

	void SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw);

protected:
	//�����l.
	D3DXVECTOR3		Pos, Pos1, Pos2;
	D3DXVECTOR3		CameraPos = D3DXVECTOR3(0.0f, 2.0f, 2.0f);

	//�����؂�ւ��@.
	bool ManualMode; //false:��������@true:���[������.
	bool MoveNow;	 //��ʐ؂�ւ�蒆�͋t�����ɂ����Ȃ��悤�ɂ���.
	int APPLY;		 //�ύX�p�����x.

	//���������E�������֘A�A.
	int FlashCnt;
	bool Ready[PlayerMAX];	//�J�n�t���O.
	
	//��ʐ؂�ւ��B.
	int StartSoundCnt, MinusApply, WaitCnt;
protected:
	//�@.//���������ʐ؂�ւ������֐�.
	void ManualModeChange(bool Flag);
	//�A.//�����֐�.
	void AreYouReady(cls2DTex* noTex, cls2DTex* Tex, bool Flag);
	//�A.//OK��_�ł�����.
	void UIFlash(cls2DTex* enTex, int &Count, bool Flag);
	//�B.//�V�[���ύX�Ɩ�.
	void ChangeScene(SceneIndex &Scene);
	//�ύX�܂Ƃ�.
	void TexChange(clsDInput* Control);
private:
	//���{.
	clsObject*					M_1PRobo;
	clsObject*					M_2PRobo;

	//�e�N�X�`��.
	cls2DTex*					PlayManual;
	cls2DTex*					PlayRule;
	cls2DTex*					PlayerPrepa[PlayerMAX];
	cls2DTex*					PlayerOK[PlayerMAX];
	cls2DTex*					PlayerName1;
	cls2DTex*					PlayerName2;

	cls2DTex*					WhiteScreen;

	clsDebugText*				ManualDebug;

	clsDirectSound*				BGM;
	//SE.
	clsDirectSound*				Ready1P;
	clsDirectSound*				Ready2P;
	clsDirectSound*				Start;
private:	//�s�ϕ���.
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;
	ID3D11Texture2D*			m_pBackBuffer_DSTex;
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;

};
#endif