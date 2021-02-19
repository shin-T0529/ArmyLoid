#include "Manual.h"

CSceneManual::CSceneManual(const char* SceneName)
	: CSceneBase(SceneName)
	//モデル.
	, M_1PRobo(nullptr), M_2PRobo(nullptr)
	//テクスチャ.
	, PlayManual(nullptr), PlayRule(nullptr)
	, PlayerName1(nullptr)	, PlayerName2(nullptr)
	, WhiteScreen(nullptr)
	, ManualDebug(nullptr)
	//変数・フラグ.
	, ManualMode(false)	, FlashCnt(0)
	, MinusApply(0)	, WaitCnt(0)
	, APPLY(0), StartSoundCnt(0)
	//モデル初期値.
	, Pos(0.0f,0.0f,0.0f), Pos1(-3.0f,0.0f,5.0f),Pos2(3.0f,0.0f,5.0f)
	//音.
	, BGM(nullptr)
	, Start(nullptr), Ready1P(nullptr), Ready2P(nullptr)
{
	for (int i = 0; i < PlayerMAX; i++)
	{ Ready[i] = false; }
}

CSceneManual::~CSceneManual()
{
	SAFE_DELETE(Ready1P);
	SAFE_DELETE(Ready2P);
	SAFE_DELETE(Start);
	SAFE_DELETE(BGM);

	SAFE_DELETE(PlayManual);
	SAFE_DELETE(PlayRule);
	SAFE_DELETE(PlayerName1);
	SAFE_DELETE(PlayerName2);
	SAFE_DELETE(WhiteScreen);
	for (int i = 0; i < PlayerMAX; i++)
	{
		SAFE_DELETE(PlayerPrepa[i]);
		SAFE_DELETE(PlayerOK[i]);
	}
	SAFE_DELETE(ManualDebug);

	SAFE_DELETE(M_1PRobo);
	SAFE_DELETE(M_2PRobo);
}

void CSceneManual::Control()
{
	BGM->Play();
	ManualModeChange(ManualMode);
	for (int i = 0; i < PlayerMAX; i++)
	{
		AreYouReady(PlayerPrepa[i], PlayerOK[i], Ready[i]);
		UIFlash(PlayerOK[i], FlashCnt, Ready[i]);
	}

	M_1PRobo->m_vRot.y += 0.01f;
	M_2PRobo->m_vRot.y -= 0.01f;

#ifdef _DEBUG
	if (GetAsyncKeyState(VK_UP) & 0x01)	M_1PRobo->m_vPos.x += 1.0f;
	if (GetAsyncKeyState(VK_DOWN) & 0x01)	M_1PRobo->m_vPos.x -= 1.0f;

	if (GetAsyncKeyState('N') & 0x01)	M_1PRobo->m_vPos.z += 1.0f;
	if (GetAsyncKeyState('M') & 0x01)	M_1PRobo->m_vPos.z -= 1.0f;
#endif
}

void CSceneManual::Action(SceneIndex &Scene, clsDInput* Controll)
{
	//コントローラーの情報を引き渡す.
	Controll->GetJoypadState2();

	TexChange(Controll);

	//シーン転換.
	ChangeScene(Scene);
}

void CSceneManual::Load(clsSkinResource *SetUP, clsDInput *Controller
	, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, HWND hWnd)
{
	M_1PRobo = new clsObject;
	M_1PRobo->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_P1Manual));
	M_1PRobo->Init();
	M_1PRobo->SetPosition(Pos1);
	M_1PRobo->SetSkinSize(AnotherSkinSize);
	M_1PRobo->SetRot(Pos);

	M_2PRobo = new clsObject;
	M_2PRobo->AttachSkinModel(SetUP->GetStaticSkinMesh(enStaticSkinMesh_P2Manual));
	M_2PRobo->Init();
	M_2PRobo->SetPosition(Pos2);
	M_2PRobo->SetSkinSize(AnotherSkinSize);
	M_2PRobo->SetRot(Pos);
}

void CSceneManual::Load(cls2DTexResource *SetUP)
{
	PlayManual = new cls2DTex;
	PlayManual->AttachTex(SetUP->GetStaticTex(enSceneTexManual));
	PlayManual->m_vPos = SetUP->TexPos(enSceneTexManual);
	PlayManual->m_Alpha = 1.0f;

	PlayRule = new cls2DTex;
	PlayRule->AttachTex(SetUP->GetStaticTex(enSceneTexManualRule));
	PlayRule->m_vPos = SetUP->TexPos(enSceneTexManualRule);
	PlayRule->m_Alpha = 1.0f;

	PlayerName1 = new cls2DTex;
	PlayerName1->AttachTex(SetUP->GetStaticTex(enSceneUIP_1));
	PlayerName1->m_vPos = SetUP->TexPos(enSceneUIP_1);
	PlayerName1->m_Alpha = 1.0f;

	PlayerName2 = new cls2DTex;
	PlayerName2->AttachTex(SetUP->GetStaticTex(enSceneUIP_2));
	PlayerName2->m_vPos = SetUP->TexPos(enSceneUIP_2);
	PlayerName2->m_Alpha = 1.0f;

	for (int i = 0; i < PlayerMAX; i++)
	{
		PlayerPrepa[i] = new cls2DTex;
		PlayerPrepa[i]->AttachTex(SetUP->GetStaticTex(enSceneUITexReady));
		PlayerPrepa[i]->m_Alpha = 1.0f;
		PlayerPrepa[i]->m_DispFlag = true;

		PlayerOK[i] = new cls2DTex;
		PlayerOK[i]->AttachTex(SetUP->GetStaticTex(enSceneUITexOK));
		PlayerOK[i]->m_Alpha = 1.0f;
		PlayerOK[i]->m_DispFlag = false;
	}
	PlayerPrepa[0]->m_vPos = D3DXVECTOR3(400.0f, 600.0f, 0.0f);
	PlayerPrepa[1]->m_vPos = D3DXVECTOR3(900.0f, 600.0f, 0.0f);

	PlayerOK[0]->m_vPos = PlayerPrepa[0]->m_vPos;
	PlayerOK[1]->m_vPos = PlayerPrepa[1]->m_vPos;

	WhiteScreen = new cls2DTex;
	WhiteScreen->AttachTex(SetUP->GetStaticTex(enScreenWhite));
	WhiteScreen->m_vPos = SetUP->TexPos(enScreenWhite);
	WhiteScreen->m_Alpha = 1.0f;

}

void CSceneManual::Load(clsSoundResource* SetUP)
{
	Ready1P = new clsDirectSound;
	Ready1P->AttachData(SetUP->GetSound(enStaticSE_Ready));
	Ready1P->ChangeDSB(Ready1P);

	Ready2P = new clsDirectSound;
	Ready2P->AttachData(SetUP->GetSound(enStaticSE_Ready));
	Ready2P->ChangeDSB(Ready2P);

	Start = new clsDirectSound;
	Start->AttachData(SetUP->GetSound(enStaticSE_Start));
	Start->ChangeDSB(Start);

	BGM = new clsDirectSound;
	BGM->AttachData(SetUP->GetSound(enStaticBGM_Manual));
	BGM->ChangeDSB(BGM);

}

void CSceneManual::Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss)
{
	WhiteScreen->TexRender(ss, WhiteScreen->m_PatternNo);
	for (int i = 0; i < PlayerMAX; i++)
	{
		if (PlayerPrepa[i]->m_DispFlag)
		{
			PlayerPrepa[i]->TexRender(ss, PlayerPrepa[i]->m_PatternNo);
		}
		if (PlayerOK[i]->m_DispFlag)
		{
			PlayerOK[i]->TexRender(ss, PlayerOK[i]->m_PatternNo);
		}
	}

	M_1PRobo->SkinRender(mView, mProj, vLight, vEye);
	M_2PRobo->SkinRender(mView, mProj, vLight, vEye);

	PlayerName1->TexRender(ss, PlayerName1->m_PatternNo);
	PlayerName2->TexRender(ss, PlayerName2->m_PatternNo);

	PlayManual->TexRender(ss, PlayManual->m_PatternNo);
	PlayRule->TexRender(ss, PlayRule->m_PatternNo);
}

void CSceneManual::ManualModeChange(bool Flag)
{
	if (Flag == false)
	{
		PlayManual->m_vPos.x += ChangeManualSpeed * APPLY;
		PlayRule->m_vPos.x += ChangeManualSpeed * APPLY;
		if (0.0f < PlayManual->m_vPos.x)
		{
			MoveNow = false;
			PlayManual->m_vPos.x = 0.0f;
			PlayRule->m_vPos.x = WND_W;
			APPLY = 0;
		}
		APPLY++;
	}
	else//if (Flag == true)
	{
		PlayManual->m_vPos.x -= ChangeManualSpeed * APPLY;
		PlayRule->m_vPos.x -= ChangeManualSpeed * APPLY;
		if (-WND_W > PlayManual->m_vPos.x)
		{
			MoveNow = false;
			PlayManual->m_vPos.x = -WND_W;
			PlayRule->m_vPos.x = 0.0f;
			APPLY = 0;
		}
		APPLY++;
	}
}

void CSceneManual::AreYouReady(cls2DTex* noTex, cls2DTex* Tex, bool Flag)
{
	if (Flag)
	{
		noTex->m_DispFlag = false;
		Tex->m_DispFlag = true;
	}
}

void CSceneManual::UIFlash(cls2DTex* enTex, int &Count, bool Flag)
{
	if (Flag)
	{
		Count++;
		if (Count < 10)
		{
			enTex->m_DispFlag = false;
		}
		else if (11 < Count && Count < 20)
		{
			enTex->m_DispFlag = true;
		}
		else if (21 < Count)
		{
			Count = 0;
		}
	}
}

void CSceneManual::ChangeScene(SceneIndex &Scene)
{
	if (Ready[0] && Ready[1])
	{
		StartSoundCnt++;
		BGM->Stop();

		if (60 < StartSoundCnt)
		{

			Start->Play();
			WhiteScreen->m_vPos.y -= 0.5f * MinusApply;
			if (WhiteScreen->m_vPos.y <= 0.0f)
			{
				WhiteScreen->m_vPos.y = 0.0f;
				if (120 < StartSoundCnt)
				{
					Start->Stop();
					Ready1P->Stop();
					Ready2P->Stop();
					WaitCnt++;
					if (80 < WaitCnt)
					{
						Scene = SceneIndex::PLAY;
					}
				}
			}
			MinusApply += 1;
		}
	}
}

void CSceneManual::TexChange(clsDInput* Control)
{
	//説明画面移動 false:操作説明　true:ルール説明.
	if (GetAsyncKeyState(VK_LEFT) & 0x01 && !MoveNow
		|| Control->g_diJoyState2[0] & BUTTON_LEFT && !MoveNow
		|| Control->g_diJoyState2[1] & BUTTON_LEFT && !MoveNow)
	{
		ManualMode = false;
		MoveNow = true;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x01 && !MoveNow
		|| Control->g_diJoyState2[0] & BUTTON_RIGHT && !MoveNow
		|| Control->g_diJoyState2[1] & BUTTON_RIGHT && !MoveNow)
	{
		ManualMode = true;
		MoveNow = true;
	}

	if (GetAsyncKeyState(VK_RETURN) & 0x01 && !Ready[0]
	 || Control->g_diJoyState2[0] & BUTTON_B && !Ready[0])
	{
		Ready[0] = true;
		Ready1P->Play();
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x01 && !Ready[1]
	 || Control->g_diJoyState2[1] & BUTTON_B && !Ready[1])
	{
		Ready[1] = true;
		Ready2P->Play();
	}
}

void CSceneManual::SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw)
{
	//関数の引数に反映する.
	vLook = CameraPos;
	vEye = CameraPos;
	Yaw = 0.0f;
}

void CSceneManual::UpdateDebugMode(ID3D11DeviceContext* pContext)
{
#ifdef _DEBUG
	//デバッグテキストの初期化.
	ManualDebug = new clsDebugText;
	D3DXVECTOR4 vColor(1.0f, 0.0f, 0.0f, 1.0f);
	if (FAILED(ManualDebug->Init(pContext, WND_W, WND_H, 50.0f, vColor)))
	{
		return;
	}
	//デバッグテキスト.
	char strDebugText[256];
	sprintf_s(strDebugText, " %d ", MinusApply);
	//デバッグテキスト.
	ManualDebug->Render(strDebugText, 0.0f, 5.0f);
#endif // !1
}