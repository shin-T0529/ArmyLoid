#include "Title.h"

int TLoadNo = 0;				//代入先ずらす用.

D3DXVECTOR3	CurSorStart = { 495.0f, 485.0f, 0.0f };
D3DXVECTOR3	CurSorEnd = { 470.0f, 582.0f, 0.0f };

CSceneTitle::CSceneTitle(const char* SceneName)
	: CSceneBase(SceneName)
	, Title(nullptr)
	, TitleButton(nullptr)
	, BlackScreen(nullptr)
	, TitleDebug(nullptr)
	, TitleCnt(0)
	, CurSorCnt(0)
	, SEContU(1), SEContD(0)
	, SceneEff(false)			//タイトルUI点滅用フラグ.
	, MenuSelect(false)
	, APPLY(0)
	, T_P1(nullptr)
{
}

CSceneTitle::~CSceneTitle()
{
	SAFE_DELETE(T_P1);

	SAFE_DELETE(TitleBGM);
	SAFE_DELETE(TitleDebug);
	SAFE_DELETE(Title);
	SAFE_DELETE(TitleButton);
	SAFE_DELETE(BlackScreen);
}

void CSceneTitle::Control()
{
	T_P1->SceneCnt = 1;
	TitleBGM->Play();
	T_P1->AnimControll(T_P1->ActState);

	T_P1->ActState = enFreaze;
	T_P1->m_vRot.y += 0.001f;
	Sora->m_vRot.y += 0.001f;
	Ground->m_vRot.y += 0.001f;
}

void CSceneTitle::Action(SceneIndex &Scene, clsDInput* Controll)
{
	//コントローラーの情報を引き渡す.
	Controll->GetJoypadState2();

	/*以下1Pが操作する*/
	if (Controll->g_diJoyState2[0] & BUTTON_M || GetAsyncKeyState(VK_RETURN)&0x01)
	{
		StartCheck = true;
		Add->Play();
	}
	if (StartCheck)
	{
		if (GetAsyncKeyState(VK_UP) & 0x8000 || Controll->g_diJoyState2[0] & BUTTON_UP)
		{
			TitleCurSor->m_vPos = CurSorStart;
			if (SEContU == ZERO)
			{
				SelectSound->Play();
				SEContD = ZERO;
				SEContU = 1;
			}
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000 || Controll->g_diJoyState2[0] & BUTTON_DOWN)
		{
			TitleCurSor->m_vPos = CurSorEnd;
			if (SEContD == ZERO)
			{
				SelectSound->Play();
				SEContU = ZERO;
				SEContD = 1;
			}
		}

		//ゲームをする場合操作説明のシーンへ.
		if (TitleCurSor->m_vPos == CurSorStart && Controll->g_diJoyState2[0] & BUTTON_B
		 || TitleCurSor->m_vPos == CurSorStart && GetAsyncKeyState(VK_SPACE) & 0x01)
		{
			Add->Play();
			TitleCurSor->m_DispFlag = false;
			SceneEff = true;
		}
		//やめる場合ウィンドウを破棄し、ゲームを終了する.
		else if (TitleCurSor->m_vPos == CurSorEnd)
		{
			if (Controll->g_diJoyState2[0] & BUTTON_B
			 || GetAsyncKeyState(VK_SPACE) & 0x01)
			{
				DestroyWin = true;
			}
		}
	}

	//移行処理.
	if (SceneEff && StartCheck)
	{
		CurSorCnt = ZERO;
		BlackScreen->m_vPos.x += ChangeSpeed * APPLY;
		if (0.0f < BlackScreen->m_vPos.x)
		{
			TitleBGM->Stop();
			BlackScreen->m_vPos.x = 0.0f;
			Scene = SceneIndex::MANUAL;
		}
		APPLY++;
	}
}

void CSceneTitle::Load(clsResource* SetUP)
{
	LoadPos("Data\\csv\\ModelPos\\Title\\XModelPos.csv", MeshMax);
	Ground = new clsObject;
	Ground->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Ground));
	Ground->SetPosition(LoadMPos[TLoadNo]);
	TLoadNo++;

	Sora = new clsObject;
	Sora->AttachModel(SetUP->GetStaticMesh(enStaticMesh_Sora));
	Sora->SetPosition(LoadMPos[TLoadNo]);
	Sora->SetScale(1.2f);

	//次のを読み込む前に初期化する.
	TLoadNo = 0;
}

void CSceneTitle::Load(clsSkinResource *SetUP, clsDInput *Controller,
	ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	LoadPos(
		"Data\\csv\\ModelPos\\Title\\SkinModelPos.csv", SkinMax-1);
	LoadRot(
		"Data\\csv\\ModelPos\\Title\\SkinModelRot.csv", SkinMax-1);

	T_P1 = new clsPlayer;
	T_P1->AttachSkinModel(SetUP->GetStaticSkinMesh(enTitle_White));
	T_P1->Init();
	T_P1->SetPosition(LoadMPos[TLoadNo]);
	T_P1->SetSkinSize(AnotherSkinSize);
	T_P1->SetRot(LoadMRot[TLoadNo]);
	T_P1->SetChangeAnim(enStartUp, T_P1->ActState, AnimPlaySpeed);
	TLoadNo = 0;
}

void CSceneTitle::Load(cls2DTexResource *SetUP)
{
	Title = new cls2DTex;
	Title->AttachTex(SetUP->GetStaticTex(enSceneTexTitile));
	Title->m_vPos = SetUP->TexPos(enSceneTexTitile);

	TitleMenu = new cls2DTex;
	TitleMenu->AttachTex(SetUP->GetStaticTex(enSceneUITexMenu));
	TitleMenu->m_vPos = SetUP->TexPos(enSceneUITexMenu);
	TitleMenu->m_DispFlag = true;

	TitleCurSor = new cls2DTex;
	TitleCurSor->AttachTex(SetUP->GetStaticTex(enSceneUITexCurSor));
	TitleCurSor->m_vPos = SetUP->TexPos(enSceneUITexCurSor);
	TitleCurSor->m_DispFlag = true;


	BlackScreen = new cls2DTex;
	BlackScreen->AttachTex(SetUP->GetStaticTex(enScreenDark));
	BlackScreen->m_vPos = SetUP->TexPos(enScreenDark);
	BlackScreen->m_Alpha = 1.0f;

	TitleStart = new cls2DTex;
	TitleStart->AttachTex(SetUP->GetStaticTex(enSceneUITexStart));
	TitleStart->m_vPos = SetUP->TexPos(enSceneUITexStart);
	TitleStart->m_DispFlag = true;

}

void CSceneTitle::Load(clsSoundResource* SetUP)
{
	TitleBGM = new clsDirectSound;
	TitleBGM->AttachData(SetUP->GetSound(enStaticBGM_Title));
	TitleBGM->ChangeDSB(TitleBGM);

	Add = new clsDirectSound;
	Add->AttachData(SetUP->GetSound(enStaticSE_Ready));
	Add->ChangeDSB(Add);

	SelectSound = new clsDirectSound;
	SelectSound->AttachData(SetUP->GetSound(enStaticSE_Select));
	SelectSound->ChangeDSB(SelectSound);

}

void CSceneTitle::Draw(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye, SPRITE_STATE &ss)
{
	BlackScreen->TexRender(ss, BlackScreen->m_PatternNo);

	T_P1->SkinRender(mView, mProj, vLight, vEye);
	Ground->Render(mView, mProj, vLight, vEye);
	Sora->Render(mView, mProj, vLight, vEye);

	if (StartCheck)
	{
		if (TitleCurSor->m_DispFlag)
		{
			TitleCurSor->TexRender(ss, TitleCurSor->m_PatternNo);
		}
		TitleMenu->TexRender(ss, TitleMenu->m_PatternNo);
	}
	else
	{
		TitleStart->TexRender(ss, TitleStart->m_PatternNo);
	}


	Title->TexRender(ss, Title->m_PatternNo);

	//デバッグテキスト.
	char strDebugText[256];
	sprintf_s(strDebugText, " %f %f %f ",
		T_P1->m_vPos.x, T_P1->m_vPos.y, T_P1->m_vPos.z);
	//デバッグテキスト.
	//TitleDebug->Render(strDebugText, 5.0f, 100.0f);

}

void CSceneTitle::Button(cls2DTex* enTex, int &Count)
{
	Count++;

	if (Count < 20)
	{
		enTex->m_DispFlag = false;
	}
    else if (21 < Count && Count < 40)
	{
		enTex->m_DispFlag = true;
	}
	else if (41 < Count)
	{
		Count = 0;
	}

}


void CSceneTitle::LoadPos(LPCSTR File, int Max)
{
	for (int i = 0; i < Max; i++)
	{
		LoadMPos[i] = m_mLoad->LoadData(File, i + 1);
	}
}

void CSceneTitle::LoadRot(LPCSTR File, int Max)
{
	for (int i = 0; i < Max; i++)
	{
		LoadMRot[i] = m_mLoad->LoadData(File, i + 1);
	}
}

void CSceneTitle::SetCameraPos(D3DXVECTOR3 &vLook, D3DXVECTOR3 &vEye, float Yaw)
{
	//見上げる形にする(VSのタイトルをイメージ).
	vLook = D3DXVECTOR3(0.0f, -4.0f, 0.0f);
	vEye = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	Yaw = 0.0f;
}

void CSceneTitle::SetLightPos(D3DXVECTOR3 &vLight)
{
	vLight = D3DXVECTOR3(0.3f, 1.2f, -3.0f);

	if (GetAsyncKeyState(VK_SPACE) & 0x01)
	{
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

void CSceneTitle::UpdateDebugMode(ID3D11DeviceContext* pContext)
{
#ifdef _DEBUG
	//デバッグテキストの初期化.
	TitleDebug = new clsDebugText;
	D3DXVECTOR4 vColor(1.0f, 0.0f, 0.0f, 1.0f);
	if (FAILED(TitleDebug->Init(pContext, WND_W, WND_H, 50.0f, vColor)))
	{
		return;
	}

#endif
}
