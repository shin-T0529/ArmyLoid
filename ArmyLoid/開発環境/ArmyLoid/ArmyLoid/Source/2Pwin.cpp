#include "2Pwin.h"

int P2LoadNo = 0;
CSceneP2win::CSceneP2win(const char* SceneName)
	: CSceneBase(SceneName)
	//スキンモデル.
	, W2_P2(nullptr),W2_L1(nullptr)
	//メッシュモデル.
	, Ground(nullptr), Sora(nullptr)
	//テクスチャ.
	, Win_2P(nullptr), BlackScreen(nullptr)
	, WhiteScreen(nullptr), RetryPoint(nullptr)
	, ExitPoint(nullptr), Cursor(nullptr)
	, DecCursor(nullptr)
	//音.
	, WinFanfare(nullptr),Add(nullptr),SelectSound(nullptr)
	//エフェクト.
	, LoseSpark(nullptr)
	//変数.
	, Blackmove(false), Whitemove(false)
	, KeyDown(false), Win2PDebug(nullptr)
	, APPLY(0)
	, DispCnt(0), SparkCnt(0)
	, SEContR(0), SEContL(0)
{

}

CSceneP2win::~CSceneP2win()
{
	//エフェクト.
	SAFE_DELETE(LoseSpark);

	//サウンドデータ.
	SAFE_DELETE(WinFanfare);
	SAFE_DELETE(Add);
	SAFE_DELETE(SelectSound);

	//テクスチャ.
	SAFE_DELETE(Win_2P);
	SAFE_DELETE(BlackScreen);
	SAFE_DELETE(WhiteScreen);
	SAFE_DELETE(RetryPoint);
	SAFE_DELETE(ExitPoint);
	SAFE_DELETE(Cursor);
	SAFE_DELETE(DecCursor);
	SAFE_DELETE(Win2PDebug);

	//メッシュモデル.
	SAFE_DELETE(Sora);
	SAFE_DELETE(Ground);
	//スキンモデル.
	SAFE_DELETE(W2_L1);
	SAFE_DELETE(W2_P2);

}

void CSceneP2win::Control()
{
	DispCnt++;
	SparkCnt++;
	LoseSpark->SetPosition(W2_L1->m_vPos);

}

void CSceneP2win::Action(SceneIndex &Scene, clsDInput* Controll)
{
	//勝ったプレイヤーのみが操作できる.
	Controll->GetJoypadState2();	//コントローラーの情報を引き渡す.

	if (0 < SparkCnt && SparkCnt < 70)
	{
		if (0 < SparkCnt && SparkCnt < 2)
		{
			LoseSpark->Start_up();
		}
	}
	if (80 < SparkCnt)
	{
		SparkCnt = 0;
	}

	if (!W2_P2->PlayMotion)
	{
		W2_P2->ActState = enWinnerPause;
		W2_P2->Win(W2_P2->ActState,1.0);
		W2_L1->ActState = enActionDownEnd;
		W2_L1->Win(W2_L1->ActState,1.0);

	}

	WinFanfare->Play();
	if (250 < DispCnt)
	{
		WinFanfare->Stop();
		//あとでActionへ.
		if (GetAsyncKeyState(VK_LEFT) & 0x01 || Controll->g_diJoyState2[0] & BUTTON_LEFT)
		{
			Cursor->m_vPos = LeftCursor;
			if (SEContL == 0)
			{
				SelectSound->Play();
				SEContL = 1;
				SEContR = 0;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x01 || Controll->g_diJoyState2[0] & BUTTON_RIGHT)
		{
			Cursor->m_vPos = RightCursor;
			if (SEContR == 0)
			{
				SelectSound->Play();
				SEContR = 1;
				SEContL = 0;
			}
		}

		if (Cursor->m_vPos == LeftCursor)
		{
			if (KeyDown)
			{
				if (GetAsyncKeyState(VK_RETURN) & 0x01
					|| Controll->g_diJoyState2[0] & BUTTON_B)
				{
					Add->Play();
					Blackmove = false;
					Whitemove = true;
					DecCursor->m_vPos = Cursor->m_vPos;
					Cursor->m_DispFlag = false;
					DecCursor->m_DispFlag = true;
				}

			}
		}
		else if (Cursor->m_vPos == RightCursor)
		{
			if (KeyDown)
			{
				if (GetAsyncKeyState(VK_RETURN) & 0x01
					|| Controll->g_diJoyState2[0] & BUTTON_B)
				{
					Add->Play();
					Blackmove = true;
					Whitemove = false;
					DecCursor->m_vPos = Cursor->m_vPos;
					Cursor->m_DispFlag = false;
					DecCursor->m_DispFlag = true;
				}

			}
		}
		if (280 < DispCnt)
		{
			KeyDown = true;
		}

		UseChangeScreen(Whitemove, WhiteScreen, Blackmove, BlackScreen, Scene);
	}

	W2_P2->AnimControll(W2_P2->ActState);
	W2_L1->AnimControll(W2_L1->ActState);

}

void CSceneP2win::Load(clsResource *SetUP)
{
	LoadPos("Data\\csv\\ModelPos\\2PWin\\XModelPos.csv", MeshMax);
	Ground = new clsObject;
	Ground->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Ground));
	Ground->SetPosition(LoadMPos[P2LoadNo]);
	P2LoadNo++;

	Sora = new clsObject;
	Sora->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Sora));
	Sora->SetPosition(LoadMPos[P2LoadNo]);
	Sora->SetScale(1.2f);

	P2LoadNo = 0;
}

void CSceneP2win::Load(cls2DTexResource *SetUP)
{
	Win_2P = new cls2DTex;
	Win_2P->AttachTex(SetUP->GetStaticTex(enSceneTex2PWin));
	Win_2P->m_vPos = SetUP->TexPos(enSceneTex2PWin);
	Win_2P->m_Alpha = 1.0f;

	RetryPoint = new cls2DTex;
	RetryPoint->AttachTex(SetUP->GetStaticTex(enSceneUIRetry));
	RetryPoint->m_vPos = SetUP->TexPos(enSceneUIRetry);
	RetryPoint->m_Alpha = 1.0f;

	ExitPoint = new cls2DTex;
	ExitPoint->AttachTex(SetUP->GetStaticTex(enSceneUIExit));
	ExitPoint->m_vPos = SetUP->TexPos(enSceneUIExit);
	ExitPoint->m_Alpha = 1.0f;

	Cursor = new cls2DTex;
	Cursor->AttachTex(SetUP->GetStaticTex(enSceneUISelectCursor));
	Cursor->m_vPos = SetUP->TexPos(enSceneUISelectCursor);
	Cursor->m_Alpha = 1.0f;

	DecCursor = new cls2DTex;
	DecCursor->AttachTex(SetUP->GetStaticTex(enSceneUIDecCursor));
	DecCursor->m_vPos = SetUP->TexPos(enSceneUIDecCursor);
	DecCursor->m_Alpha = 1.0f;
	DecCursor->m_DispFlag = false;

	BlackScreen = new cls2DTex;
	BlackScreen->AttachTex(SetUP->GetStaticTex(enScreenDark));
	BlackScreen->m_vPos = D3DXVECTOR3(-WND_W_X, 0.0f, 0.0f);
	BlackScreen->m_Alpha = 1.0f;

	WhiteScreen = new cls2DTex;
	WhiteScreen->AttachTex(SetUP->GetStaticTex(enScreenWhite));
	WhiteScreen->m_vPos = D3DXVECTOR3(-WND_W_X, 0.0f, 0.0f);
	WhiteScreen->m_Alpha = 1.0f;

}

void CSceneP2win::Load(clsSoundResource* SetUP)
{
	WinFanfare = new clsDirectSound;
	WinFanfare->AttachData(SetUP->GetSound(enStaticBGM_ResultWin));
	WinFanfare->ChangeDSB(WinFanfare);

	Add = new clsDirectSound;
	Add->AttachData(SetUP->GetSound(enStaticSE_Ready));
	Add->ChangeDSB(Add);

	SelectSound = new clsDirectSound;
	SelectSound->AttachData(SetUP->GetSound(enStaticSE_Select));
	SelectSound->ChangeDSB(SelectSound);

}

void CSceneP2win::Load(clsSkinResource * SetUP, clsDInput * Controller, ID3D11Device * pDevice, ID3D11DeviceContext * pContext, HWND hWnd)
{
	LoadPos(
		"Data\\csv\\ModelPos\\2PWin\\SkinModelPos.csv", SkinMax);
	LoadRot(
		"Data\\csv\\ModelPos\\2PWin\\SkinModelRot.csv", SkinMax);

	W2_P2 = new clsPlayer;
	W2_P2->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_Black));
	W2_P2->Init();
	W2_P2->SetPosition(LoadMPos[P2LoadNo]);
	W2_P2->SetSkinSize(AnotherSkinSize);
	W2_P2->SetRot(LoadMRot[P2LoadNo]);
	P2LoadNo++;

	W2_L1 = new clsPlayer;
	W2_L1->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_ResultWhite));
	W2_L1->Init();
	W2_L1->SetPosition(LoadMPos[P2LoadNo]);
	W2_L1->SetSkinSize(AnotherSkinSize);
	W2_L1->SetRot(LoadMRot[P2LoadNo]);
	W2_L1->ActState = enActionDownEnd;

	P2LoadNo = 0;
}

void CSceneP2win::Load(clsEffectsResouce* SetUP)
{
	LoseSpark = new clsEffectsMain;
	LoseSpark->AttachEffect(SetUP->GetEffects(enEffects_Spark));
}

void CSceneP2win::Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss)
{
	//幕.
	BlackScreen->TexRender(ss, BlackScreen->m_PatternNo);
	WhiteScreen->TexRender(ss, WhiteScreen->m_PatternNo);

	W2_P2->SkinRender(mView, mProj, vLight, vEye);
	W2_L1->SkinRender(mView, mProj, vLight, vEye);

	Ground->Render(mView, mProj, vLight, vEye);
	Sora->Render(mView, mProj, vLight, vEye);

	if (200 < DispCnt)
	{
		Win_2P->TexRender(ss, Win_2P->m_PatternNo);

	}
	if (250 < DispCnt)
	{
		//カーソル(チェックマークは上から被せる).
		if (DecCursor->m_DispFlag == true)
			DecCursor->TexRender(ss, DecCursor->m_PatternNo);
		if (Cursor->m_DispFlag == true)
			Cursor->TexRender(ss, Cursor->m_PatternNo);

		RetryPoint->TexRender(ss, RetryPoint->m_PatternNo);
		ExitPoint->TexRender(ss, ExitPoint->m_PatternNo);
	}
	LoseSpark->Render(mView, mProj, vLight, vEye);
}

void CSceneP2win::UseChangeScreen(bool RetryFlag, cls2DTex* WScreenTex, bool ExitFlag, cls2DTex* BScreenTex, SceneIndex &Scene)
{	//リトライ→する(Play) しない→(Credit).
	if (ExitFlag)
	{
		BScreenTex->m_vPos.x += 1.0f * APPLY;
		if (0.0f < BScreenTex->m_vPos.x)
		{
			BScreenTex->m_vPos.x = 0.0f;
			Scene = SceneIndex::CREDIT;
		}
		APPLY++;
	}
	if (RetryFlag)
	{
		WScreenTex->m_vPos.x += 1.0f * APPLY;
		if (0.0f < WScreenTex->m_vPos.x)
		{
			WScreenTex->m_vPos.x = 0.0f;
			Scene = SceneIndex::PLAY;
		}
		APPLY++;
	}


}

void CSceneP2win::UpdateDebugMode(ID3D11DeviceContext* pContext)
{
#ifdef _DEBUG
	//デバッグテキストの初期化.
	Win2PDebug = new clsDebugText;
	D3DXVECTOR4 vColor(1.0f, 0.0f, 0.0f, 1.0f);
	if (FAILED(Win2PDebug->Init(pContext, WND_W, WND_H, 50.0f, vColor)))
	{
		return;
	}
	//デバッグテキスト.
	char strDebugText[256];
	sprintf_s(strDebugText, " x  y  z ");
	//デバッグテキスト.
	Win2PDebug->Render(strDebugText, 5.0f, 100.0f);

#endif // !1

}

void CSceneP2win::SetCameraPos(D3DXVECTOR3 & vLook, D3DXVECTOR3 & vEye, float Yaw)
{
	vLook = vEye = D3DXVECTOR3(0.5f, 1.5f, 0.0f);
	Yaw = 0.0f;
}


void CSceneP2win::SetLightPos(D3DXVECTOR3 &vLight)
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
