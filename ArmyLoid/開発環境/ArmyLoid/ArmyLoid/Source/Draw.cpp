#include "Draw.h"

int DLoadNo = 0;
CSceneDraw::CSceneDraw(const char* SceneName)
	: CSceneBase(SceneName)
	//スキンモデル.
	, D_1P(nullptr),D_2P(nullptr)
	//メッシュモデル.
	, Ground(nullptr),Sora(nullptr)
	//2Dテクスチャ.
	, DrawTex(nullptr)
	, BlackScreen(nullptr)
	, WhiteScreen(nullptr)
	, RetryPoint(nullptr)
	, ExitPoint(nullptr)
	, Cursor(nullptr)
	, DecCursor(nullptr)
	//変数.
	, APPLY(0), DispCnt(0),SparkCnt(0)
	, Blackmove(false), Whitemove(false)
	, SEContR(0), SEContL(0), BGMCnt(0)
	//サウンド.
	, Add(nullptr), SelectSound(nullptr)
	//エフェクト.
	, LoseSpark(nullptr)

{

}

CSceneDraw::~CSceneDraw()
{
	//エフェクト.
	SAFE_DELETE(LoseSpark);

	//サウンド.
	SAFE_DELETE(Add);
	SAFE_DELETE(SelectSound);

	//テクスチャ.
	SAFE_DELETE(DecCursor);
	SAFE_DELETE(Cursor);
	SAFE_DELETE(ExitPoint);
	SAFE_DELETE(RetryPoint);
	SAFE_DELETE(WhiteScreen);
	SAFE_DELETE(BlackScreen);
	SAFE_DELETE(DrawTex);

	//メッシュモデル.
	SAFE_DELETE(Sora);
	SAFE_DELETE(Ground);
	//スキンモデル.
	SAFE_DELETE(D_2P);
	SAFE_DELETE(D_1P);

}

void CSceneDraw::Control()
{
	DispCnt++;
	SparkCnt++;
	D_1P->AnimControll(D_1P->ActState);
	D_2P->AnimControll(D_2P->ActState2);
	BGMCnt++;
	if (BGMCnt < 200)
	{
		DrawBGM->Play();
	}
	else
	{
		DrawBGM->Stop();
	}

	if (!D_1P->PlayMotion && !D_2P->PlayMotion)
	{
		D_1P->ActState = enActionDown;
		D_1P->Win(D_1P->ActState,1.0);
		D_2P->ActState2 = enActionDown;
		D_2P->Win(D_2P->ActState2,1.0);
	}

}

void CSceneDraw::Action(SceneIndex &Scene, clsDInput* Controll)
{
	//コントローラーの情報を引き渡す.
	Controll->GetJoypadState2();


	SparkTiming(SparkCnt, LoseSpark, D_1P, D_2P, SparkFlag);

	//1Pが操作.
	if (250 < DispCnt)
	{
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
			if (GetAsyncKeyState(VK_RETURN) & 0x01 || Controll->g_diJoyState2[0] & BUTTON_B)
			{
				Add->Play();
				Blackmove = false;
				Whitemove = true;
				DecCursor->m_vPos = Cursor->m_vPos;
				Cursor->m_DispFlag = false;
				DecCursor->m_DispFlag = true;

			}
		}
		else if (Cursor->m_vPos == RightCursor)
		{
			if (GetAsyncKeyState(VK_RETURN) & 0x01 || Controll->g_diJoyState2[0] & BUTTON_B)
			{
				Add->Play();
				Blackmove = true;
				Whitemove = false;
				DecCursor->m_vPos = Cursor->m_vPos;
				Cursor->m_DispFlag = false;
				DecCursor->m_DispFlag = true;

			}
		}
		UseChangeScreen(Whitemove, WhiteScreen, Blackmove, BlackScreen, Scene);
	}

	//D_1P->AnimControll(D_1P->ActState);
	//D_2P->AnimControll(D_2P->ActState2);

}

void CSceneDraw::Load(clsResource * SetUP)
{
	LoadPos(
		"Data\\csv\\ModelPos\\Draw\\XModelPos.csv", MeshMax);

	Ground = new clsObject;
	Ground->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Ground));
	Ground->SetPosition(LoadMPos[DLoadNo]);
	DLoadNo++;

	Sora = new clsObject;
	Sora->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Sora));
	Sora->SetPosition(LoadMPos[DLoadNo]);
	Sora->SetScale(1.2f);
	DLoadNo = 0;

}

void CSceneDraw::Load(cls2DTexResource *SetUP)
{
	DrawTex = new cls2DTex;
	DrawTex->AttachTex(SetUP->GetStaticTex(enSceneTexDraw));
	DrawTex->m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	DrawTex->m_Alpha = 1.0f;

	RetryPoint = new cls2DTex;
	RetryPoint->AttachTex(SetUP->GetStaticTex(enSceneUIRetry));
	RetryPoint->m_vPos = D3DXVECTOR3(260.0f,558.0f, 0.0f);
	RetryPoint->m_Alpha = 1.0f;

	ExitPoint = new cls2DTex;
	ExitPoint->AttachTex(SetUP->GetStaticTex(enSceneUIExit));
	ExitPoint->m_vPos = D3DXVECTOR3(743.0f,558.0f, 0.0f);
	ExitPoint->m_Alpha = 1.0f;

	Cursor = new cls2DTex;
	Cursor->AttachTex(SetUP->GetStaticTex(enSceneUISelectCursor));
	Cursor->m_vPos = LeftCursor;
	Cursor->m_Alpha = 1.0f;

	DecCursor = new cls2DTex;
	DecCursor->AttachTex(SetUP->GetStaticTex(enSceneUIDecCursor));
	DecCursor->m_vPos = D3DXVECTOR3(754.0f,568.0f, 0.0f);
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

void CSceneDraw::Load(clsSkinResource *SetUP, clsDInput *Controller,
	ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	LoadPos(
		"Data\\csv\\ModelPos\\Draw\\SkinModelPos.csv", SkinMax);
	LoadRot(
		"Data\\csv\\ModelPos\\Draw\\SkinModelRot.csv", SkinMax);
	D_1P = new clsPlayer;
	D_1P->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_White));
	D_1P->Init();
	D_1P->SetPosition(LoadMPos[DLoadNo]);
	D_1P->SetSkinSize(AnotherSkinSize);
	D_1P->SetRot(LoadMRot[DLoadNo]);
	DLoadNo++;

	D_2P = new clsPlayer;
	D_2P->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_Black));
	D_2P->Init();
	D_2P->SetPosition(LoadMPos[DLoadNo]);
	D_2P->SetSkinSize(AnotherSkinSize);
	D_2P->SetRot(LoadMRot[DLoadNo]);

	DLoadNo = 0;
}

void CSceneDraw::Load(clsSoundResource* SetUP)
{
	DrawBGM = new clsDirectSound;
	DrawBGM->AttachData(SetUP->GetSound(enStaticBGM_Draw));
	DrawBGM->ChangeDSB(DrawBGM);

	Add = new clsDirectSound;
	Add->AttachData(SetUP->GetSound(enStaticSE_Ready));
	Add->ChangeDSB(Add);

	SelectSound = new clsDirectSound;
	SelectSound->AttachData(SetUP->GetSound(enStaticSE_Select));
	SelectSound->ChangeDSB(SelectSound);

}

void CSceneDraw::Load(clsEffectsResouce* SetUP)
{
	LoseSpark = new clsEffectsMain;
	LoseSpark->AttachEffect(SetUP->GetEffects(enEffects_Spark));
}

void CSceneDraw::Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss)
{
	//幕.
	BlackScreen->TexRender(ss, BlackScreen->m_PatternNo);
	WhiteScreen->TexRender(ss, WhiteScreen->m_PatternNo);

	D_1P->SkinRender(mView, mProj, vLight, vEye);
	D_2P->SkinRender(mView, mProj, vLight, vEye);

	Ground->Render(mView, mProj, vLight, vEye);
	Sora->Render(mView, mProj, vLight, vEye);

	if (200 < DispCnt)
	{
		DrawTex->TexRender(ss, DrawTex->m_PatternNo);
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

void CSceneDraw::UseChangeScreen(bool RetryFlag, cls2DTex* WScreenTex, bool ExitFlag, cls2DTex* BScreenTex, SceneIndex &Scene)
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

void CSceneDraw::SparkTiming(int &Cnt, clsEffectsMain* SparkEff,
							 clsPlayer* Play1, clsPlayer* Play2, bool &flag)
{
	if (0 < Cnt && Cnt < 70)
	{
		if (flag)
		{ SparkEff->SetPosition(Play1->m_vPos); }
		else
		{ SparkEff->SetPosition(Play2->m_vPos); }

		if (0 < Cnt && Cnt < 2)
		{
			SparkEff->Start_up();
		}
	}
	if (80 < Cnt)
	{
		if (flag)
		{ flag = false; }
		else
		{ flag = true; }
		Cnt = 0;
	}

}


void CSceneDraw::DebugUIPos(cls2DTex* PosSetTex)
{
	if (GetAsyncKeyState(VK_LEFT) & 0x01)
	{
		PosSetTex->m_vPos.x -= 1.0f;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x01)
	{
		PosSetTex->m_vPos.x += 1.0f;
	}
	if (GetAsyncKeyState(VK_UP) & 0x01)
	{
		PosSetTex->m_vPos.y -= 1.0f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x01)
	{
		PosSetTex->m_vPos.y += 1.0f;
	}
}

void CSceneDraw::LoadPos(LPCSTR File, int Max)
{
	for (int i = 0; i < Max; i++)
	{
		LoadMPos[i] = m_mLoad->LoadData(File, i + 1);
	}
}

void CSceneDraw::LoadRot(LPCSTR File, int Max)
{
	for (int i = 0; i < Max; i++)
	{
		LoadMRot[i] = m_mLoad->LoadData(File, i + 1);
	}
}


void CSceneDraw::SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw)
{
	vLook = vEye = D3DXVECTOR3(0.0f, 0.0f, 0.0);
	Yaw = 0.0f;
}

void CSceneDraw::SetLightPos(D3DXVECTOR3 &vLight)
{
	//(46.1 26.4 -2.1)
	vLight = D3DXVECTOR3(0.3f, 1.2f, -3.0f);

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
