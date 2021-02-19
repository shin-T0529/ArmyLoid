#include "1Pwin.h"

int P1LoadNo = 0;
CSceneP1win::CSceneP1win(const char* SceneName)
	: CSceneBase(SceneName)
	//スキンモデル.
	,W1_P1(nullptr),L1_P2(nullptr)
	//メッシュモデル.
	,Ground(nullptr),Sora(nullptr)
	//テクスチャ.
	, Win_1P(nullptr), BlackScreen(nullptr)
	, WhiteScreen(nullptr), RetryPoint(nullptr)
	, ExitPoint(nullptr), Cursor(nullptr), DecCursor(nullptr)
	//音.
	, WinFanfare(nullptr), Add(nullptr), SelectSound(nullptr)
	//エフェクト.
	, LoseSpark(nullptr)
	//変数.
	, Win1Debug(nullptr)
	, APPLY(0)
	, DispCnt(0), SparkCnt(0)
	, Blackmove(false)
	, Whitemove(false)
	, SEContR(0), SEContL(0)
{

}

CSceneP1win::~CSceneP1win()
{
	//エフェクト.
	SAFE_DELETE(LoseSpark);

	//サウンドデータ.
	SAFE_DELETE(WinFanfare);
	SAFE_DELETE(Add);
	SAFE_DELETE(SelectSound);

	//テクスチャ.
	SAFE_DELETE(Win1Debug);
	SAFE_DELETE(Win_1P);
	SAFE_DELETE(BlackScreen);
	SAFE_DELETE(WhiteScreen);
	SAFE_DELETE(RetryPoint);
	SAFE_DELETE(ExitPoint);
	SAFE_DELETE(Cursor);
	SAFE_DELETE(DecCursor);


}

void CSceneP1win::Control()
{
	DispCnt++;
	SparkCnt++;
	LoseSpark->SetPosition(L1_P2->m_vPos);
}

void CSceneP1win::Action(SceneIndex &Scene, clsDInput* Controll)
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


	if (!W1_P1->PlayMotion)
	{
		W1_P1->ActState = enWinnerPause;
		W1_P1->Win(W1_P1->ActState,1.0);
		L1_P2->ActState = enActionDownEnd;
		L1_P2->Win(L1_P2->ActState,1.0);

	}

	if (GetAsyncKeyState('C') & 0x01)
	{
		W1_P1->ActState = enWinnerPause;
	}
	WinFanfare->Play();
	if (250 < DispCnt)
	{
		//あとでActionへ.
		if (GetAsyncKeyState(VK_LEFT) & 0x01
			|| Controll->g_diJoyState2[0] & BUTTON_LEFT)
		{
			Cursor->m_vPos = LeftCursor;
			if (SEContL == 0)
			{
				SelectSound->Play();
				SEContL = 1;
				SEContR = 0;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x01
			|| Controll->g_diJoyState2[0] & BUTTON_RIGHT)
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
		if (270 < DispCnt)
		{
			KeyDown = true;
		}
		WinFanfare->Stop();
		UseChangeScreen(Whitemove, WhiteScreen, Blackmove, BlackScreen, Scene);
	}
	W1_P1->AnimControll(W1_P1->ActState);
	L1_P2->AnimControll(L1_P2->ActState);

}

void CSceneP1win::Load(clsResource *SetUP)
{
	LoadPos(
		"Data\\csv\\ModelPos\\1PWin\\XModelPos.csv", MeshMax);

	Ground = new clsObject;
	Ground->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Ground));
	Ground->SetPosition(LoadMPos[P1LoadNo]);
	P1LoadNo++;

	Sora = new clsObject;
	Sora->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Sora));
	Sora->SetPosition(LoadMPos[P1LoadNo]);
	Sora->SetScale(1.2f);
	P1LoadNo = 0;
}

void CSceneP1win::Load(cls2DTexResource *SetUP)
{
	Win_1P = new cls2DTex;
	Win_1P->AttachTex(SetUP->GetStaticTex(enSceneTex1PWin));
	Win_1P->m_vPos = SetUP->TexPos(enSceneTex1PWin);
	Win_1P->m_Alpha = 1.0f;

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
	//DecCursor->m_Alpha = 1.0f;
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

void CSceneP1win::Load(clsSoundResource* SetUP)
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

void CSceneP1win::Load(clsSkinResource * SetUP, clsDInput * Controller, ID3D11Device * pDevice, ID3D11DeviceContext * pContext, HWND hWnd)
{	
	LoadPos(
		"Data\\csv\\ModelPos\\1PWin\\SkinModelPos.csv", SkinMax);
	LoadRot(
		"Data\\csv\\ModelPos\\1PWin\\SkinModelRot.csv", SkinMax);

	W1_P1 = new clsPlayer;
	W1_P1->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_ResultWhite));
	W1_P1->Init();
	W1_P1->SetPosition(LoadMPos[P1LoadNo]);
	W1_P1->SetSkinSize(AnotherSkinSize);
	W1_P1->SetRot(LoadMRot[P1LoadNo]);
	P1LoadNo++;

	L1_P2 = new clsPlayer;
	L1_P2->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_ResultBlack));
	L1_P2->Init();
	L1_P2->SetPosition(LoadMPos[P1LoadNo]);
	L1_P2->SetSkinSize(AnotherSkinSize);
	L1_P2->SetRot(LoadMRot[P1LoadNo]);
	L1_P2->ActState = enActionDownEnd;
	P1LoadNo = 0;
}

void CSceneP1win::Load(clsEffectsResouce* SetUP)
{
	LoseSpark = new clsEffectsMain;
	LoseSpark->AttachEffect(SetUP->GetEffects(enEffects_Spark));
}

void CSceneP1win::Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye,SPRITE_STATE &ss)
{
	//幕.
	BlackScreen->TexRender(ss, BlackScreen->m_PatternNo);
	WhiteScreen->TexRender(ss, WhiteScreen->m_PatternNo);

	W1_P1->SkinRender(mView, mProj, vLight, vEye);
	L1_P2->SkinRender(mView, mProj, vLight, vEye);

	Ground->Render(mView, mProj, vLight, vEye);
	Sora->Render(mView, mProj, vLight, vEye);
	if (200 < DispCnt)
	{
		Win_1P->TexRender(ss, Win_1P->m_PatternNo);
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

void CSceneP1win::UseChangeScreen(bool RetryFlag, cls2DTex* WScreenTex, bool ExitFlag, cls2DTex* BScreenTex, SceneIndex &Scene)
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

void CSceneP1win::UpdateDebugMode(ID3D11DeviceContext* pContext)
{
#ifdef _DEBUG
	//デバッグテキストの初期化.
	Win1Debug = new clsDebugText;
	D3DXVECTOR4 vColor(1.0f, 0.0f, 0.0f, 1.0f);
	if (FAILED(Win1Debug->Init(pContext, WND_W, WND_H, 50.0f, vColor)))
	{
		return;
	}
#endif // !1

}

void CSceneP1win:: LoadPos(LPCSTR File, int Max)
{
	for (int i = 0; i < Max; i++)
	{
		LoadMPos[i] = m_mLoad->LoadData(File, i + 1);
	}
}

void CSceneP1win::LoadRot(LPCSTR File, int Max)
{
	for (int i = 0; i < Max; i++)
	{
		LoadMRot[i] = m_mLoad->LoadData(File, i + 1);
	}
}


void CSceneP1win::SetCameraPos(D3DXVECTOR3 & vLook, D3DXVECTOR3 & vEye, float Yaw)
{
	vLook = vEye = D3DXVECTOR3(0.5f, 1.5f, 0.0f);
	Yaw = 0.0f;
}

void CSceneP1win::SetLightPos(D3DXVECTOR3 &vLight)
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

