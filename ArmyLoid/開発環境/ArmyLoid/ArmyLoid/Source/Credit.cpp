#include "Credit.h"

CSceneCredit::CSceneCredit(const char* SceneName)
	: CSceneBase(SceneName)
	//モデル.
	, CreP_1P(nullptr)
	//テクスチャ.
	, CreditList(nullptr), CreditBack(nullptr), BlackScreen(nullptr)
	//サウンド.
	, BGM(nullptr)
	//ローカル変数.
	, ScroolSpeed(1.0f), ChangeCnt(0), APPLY(0)
	, ChackScreen(false),SetAnim(false)
{

}

CSceneCredit::~CSceneCredit()
{
	//サウンド.
	SAFE_DELETE(BGM);

	//テクスチャ.
	SAFE_DELETE(CreditList);
	SAFE_DELETE(CreditBack);
	SAFE_DELETE(BlackScreen);

	//モデル.
	SAFE_DELETE(CreP_1P);
}

void CSceneCredit::Load(clsResource *SetUP)
{
	Back = new clsPlayer;
	Back->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Back));
	Back->m_vPos  = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//Back->SetRot(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
}

void CSceneCredit::Load(cls2DTexResource *SetUP)
{
	CreditList = new cls2DTex;
	CreditList->AttachTex(SetUP->GetStaticTex(enSceneTexCredit));
	CreditList->m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	
	CreditBack = new cls2DTex;
	CreditBack->AttachTex(SetUP->GetStaticTex(enSceneTexCreditBack));
	CreditBack->m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	BlackScreen = new cls2DTex;
	BlackScreen->AttachTex(SetUP->GetStaticTex(enScreenDark));
	BlackScreen->m_vPos = D3DXVECTOR3(-WND_W_X, 0.0f, 0.0f);
	BlackScreen->m_Alpha = 1.0f;
}

void CSceneCredit::Load(clsSoundResource * SetUP)
{
	BGM = new clsDirectSound;
	BGM->AttachData(SetUP->GetSound(enStaticBGM_CreditBGM));
	BGM->ChangeDSB(BGM);
}

void CSceneCredit::Load(clsSkinResource * SetUP, clsDInput * Controller, ID3D11Device * pDevice11, ID3D11DeviceContext * pContext11, HWND hWnd)
{
	CreP_1P = new clsPlayer;
	CreP_1P->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_White));
	CreP_1P->Init();
	CreP_1P->m_vPos = D3DXVECTOR3(0.0f,0.0f,-2.0f);
	CreP_1P->SetSkinSize(AnotherSkinSize);
	CreP_1P->m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void CSceneCredit::Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss)
{
	BlackScreen->TexRender(ss, BlackScreen->m_PatternNo);

	CreP_1P->SkinRender(mView, mProj, vLight, vEye);

	Back->Render(mView, mProj, vLight, vEye);
	CreditList->TexRender(ss, CreditList->m_PatternNo);
	//CreditBack->TexRender(ss, CreditBack->m_PatternNo);
}



void CSceneCredit::Control()
{
	BGM->Play();
	//とりあえず起動→待機流し続ける.
	CreP_1P->AnimControll(CreP_1P->ActState);
	if (!SetAnim)
	{
		CreP_1P->SetChangeAnim(enStartUp, CreP_1P->ActState, AnimPlaySpeed);
		SetAnim = true;
	}

}

void CSceneCredit::Action(SceneIndex &Scene, clsDInput* Controll)
{
	//コントローラーの情報を引き渡す.
	Controll->GetJoypadState2();

	//クレジットスクロール.
	if (CreditList->m_vPos.y > -WND_H * 4.2f)
	{
		//加速処理.
		if (GetAsyncKeyState(VK_RETURN) & 0x8000 || Controll->g_diJoyState2[0] & BUTTON_B)
		{
			ScroolSpeed = 10.0f;
			CreditList->m_vPos.y -= ScroolSpeed;
		}
		else
		{
			ScroolSpeed = 1.0f;
			CreditList->m_vPos.y -= ScroolSpeed;
		}
	}
	else
	{
		CreditList->m_vPos.y -= 0.0f;
		ChangeCnt++;
		if (80 < ChangeCnt)
		{
			BlackScreen->m_vPos.x += 1.0f * APPLY;
			if (0.0f < BlackScreen->m_vPos.x)
			{
				BGM->Stop();
				Scene = SceneIndex::TITLE;
			}
			APPLY++;
		}
	}

}

void CSceneCredit::SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw)
{
	vLook = D3DXVECTOR3(.0f, 2.0f, -2.0f);
	vEye = D3DXVECTOR3(0.0f, 2.0f, -2.0f);
	Yaw = 0.0f;
}

void CSceneCredit::SetLightPos(D3DXVECTOR3 &vLight)
{
	if (GetAsyncKeyState(VK_SPACE) & 0x01)
	{
		vLight = D3DXVECTOR3(46.1f, 26.4f, -2.1f);
	}
	if (GetAsyncKeyState(VK_F1) & 0x01)
	{
		vLight.x += 0.1f;
	}
	if (GetAsyncKeyState(VK_F2) & 0x01)
	{
		vLight.x -= 0.1f;
	}

	if (GetAsyncKeyState(VK_F3) & 0x01)
	{
		vLight.y += 0.1f;
	}
	if (GetAsyncKeyState(VK_F4) & 0x01)
	{
		vLight.y -= 0.1f;
	}

	if (GetAsyncKeyState(VK_F5) & 0x01)
	{
		vLight.z += 0.1f;
	}
	if (GetAsyncKeyState(VK_F6) & 0x01)
	{
		vLight.z -= 0.1f;
	}
}

