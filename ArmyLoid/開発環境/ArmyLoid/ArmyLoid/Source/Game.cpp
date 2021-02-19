#include "Game.h"
#pragma warning(disable:4819)

clsGame::clsGame()
: m_pDevice(nullptr)
, m_pContext(nullptr)
, m_pSampleLinear(nullptr)
, m_pTexture(nullptr)
, m_Camera(), m_mView()
, m_mProj(), m_vLight()
, m_pClsDx9Mesh(nullptr)
, m_pResource(nullptr), m_pResourceTex(nullptr)
, m_pResourceSkin(nullptr),m_pResourceEff(nullptr)
, m_pResourceSound(nullptr)
, pSceneMgr(nullptr)
, m_pManager(nullptr)
, m_pCtrler(nullptr)
{
	m_Camera.vEye = D3DXVECTOR3(0.0f, 3.7f, 0.0f);
	m_Camera.vLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vLight = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
};

clsGame::~clsGame()
{
	//コントローラ情報.
	delete m_pCtrler;

	//シーン情報.
	SAFE_DELETE(pSceneMgr);

	//ﾓﾃﾞﾙの解放.
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pSampleLinear);

	//リソースの開放.
	SAFE_DELETE(m_pResource);
	SAFE_DELETE(m_pResourceTex);
	SAFE_DELETE(m_pResourceSkin);
	SAFE_DELETE(m_pResourceEff);
	SAFE_DELETE(m_pResourceSound);

	//Direct3Dｵﾌﾞｼﾞｪｸﾄの解放.
	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);

}

void clsGame::Loop()
{
	//3Dの作成.
	m_pDirect3D = new clsDirect3D;
	m_pDirect3D->Create3D(m_hWnd);
	//必要な情報の取得.
	m_pDevice = m_pDirect3D->GetDevice();
	m_pContext = m_pDirect3D->GetDeviceContext();
	//各リソースの作成.
	CreateResource(m_hWnd,m_pDevice, m_pContext);

	//シーンマネージャー生成.
	pSceneMgr = new CSceneManager;
	//初回起動のデータ読込.
	LoadData();		
	//デバッグテキスト用.
	pSceneMgr->UpdateDebugMode(m_pContext);		

	//---------------------ﾌﾚｰﾑﾚｰﾄ調整準備.---------------------//

	float Rate = 0.0f;					//ﾚｰﾄ.
	float FPS = 60.0f;					//FPS値.
	DWORD sync_old = timeGetTime();		//過去時間.
	DWORD sync_now;						//現在時間.
	//時間処理の為、最小単位を1ﾐﾘ秒に変更.
	timeBeginPeriod(1);

	//----------------------------------------------------------//

	//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		sync_now = timeGetTime();//現在時間を取得.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Rate = 1000.0f / FPS;	//理想時間を算出.
		if (sync_now - sync_old >= Rate)
		{
			sync_old = sync_now;//現在時間に置き換え.
			AppMain();
		}
	}

	//ｱﾌﾟﾘｹｰｼｮﾝの終了.
	timeEndPeriod(1);
}

//ﾒｲﾝ処理.
void clsGame::AppMain()
{
	//Updateを通ってからindexが変わっていないか確認.
	//変わっていたらフラグを立て、次のシーンへ移行する.
	if (pSceneMgr->CheckingScene == true)
	{
		pSceneMgr->SetNextScene(index);						//入れ替わった先のシーンにする.
		pSceneMgr->TransitionScene();						//今のシーンを破棄して次のシーンを生成.
		pSceneMgr->MeshLoad(m_pResource);					//モデル情報の読込.
		pSceneMgr->TexLoad(m_pResourceTex);
		pSceneMgr->SkinLoad(								//スキンメッシュモデル情報の読込.
			m_pResourceSkin, m_pCtrler,
			m_pDevice, m_pContext, m_hWnd);
		pSceneMgr->EffLoad(m_pResourceEff);					//エフェクトデータの読込.
		pSceneMgr->SoundLoad(m_pResourceSound);				//サウンドデータの読込.
#ifdef _DEBUG
		pSceneMgr->UpdateDebugMode(m_pContext);				//デバッグテキスト用.
#endif // _DEBUG
		pSceneMgr->CheckingScene = false;
	}
	//本体.
	pSceneMgr->Update(index, m_pCtrler);
	pSceneMgr->WindowDestroy(m_hWnd);
	//カメラ位置の設定.
	pSceneMgr->SetCameraPos(m_Camera.vEye, m_Camera.vLook, m_Camera.Yaw);
	pSceneMgr->SetLightPos(m_vLight);
	//モデル・テクスチャ描画.
	Render();
}

void clsGame::LoadData()
{
	//モデル情報の読込(起動時の初回読込).
	pSceneMgr->MeshLoad(m_pResource);
	//テクスチャ情報の読込(同上).
	pSceneMgr->TexLoad(m_pResourceTex);
	//スキンメッシュ情報の読込(同上).
	pSceneMgr->SkinLoad(m_pResourceSkin, m_pCtrler ,m_pDevice, m_pContext, m_hWnd);
	//エフェクト情報の読込(同上).
	pSceneMgr->EffLoad(m_pResourceEff);
	//サウンドデータの読込(同上).
	pSceneMgr->SoundLoad(m_pResourceSound);
}

void clsGame::CreateResource(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	//Xモデルリソース.
	m_pResource = new clsResource;
	m_pResource->Init(hWnd, pDevice, pContext);
	for (int i = 0; i < enStaticMesh_Max; i++)
	{ m_pResource->LoadStaticMeshAll(); }

	//2Dテクスチャリソース.
	m_pResourceTex = new cls2DTexResource;
	m_pResourceTex->Init(hWnd, pDevice, pContext);

	//スプライト情報の読込と設定.
	for (int i = 0; i < enStaticTex_Max; i++)
	{
		char* SSFilePass = new char[255];

		//ファイルパスの記録(定数指定).
		if (i == enSceneTexTitile || i == enSceneTexManual || i == enSceneTex1PWin
		||  i == enSceneTexCredit || i == enUITexHP1_FINE)
		{//読み込む前に各シーンに合わせて読む場所を変更する.
			SSi++;
			SSLi = 0;		//読込を始める行は0(1)行目から.
		}

		memcpy(SSFilePass, m_Load->LoadFilePass(SSData, 1 + SSi), sizeof(char) * 255);

		//サイズ情報の取得・設定.
		SpSt.U = SpSt.V = 1.0f;
		List.SpriteState = m_Load->LoadSpriteState(SSFilePass, 1 + SSLi);
		//分割するテクスチャの場合.
		if (WeponStart <= i && i <= WeponEnd || TimeStart <= i && i <= TimeEnd)
		{
			SpSt.Base.w = List.SpriteState.x;
			SpSt.Base.h = List.SpriteState.y;
			SpSt.Stride.w = SpSt.Base.w / 10.0f;
			SpSt.Stride.h = SpSt.Base.h;
			SpSt.Width = SpSt.Stride.w;
			SpSt.Height = SpSt.Stride.h;
			SpSt.Disp.w = SpSt.Width;
			SpSt.Disp.h = SpSt.Height;
			SpSt.U = SpSt.Stride.w / SpSt.Base.w;
			SpSt.V = SpSt.Stride.h / SpSt.Base.h;
		}
		else
		{//分割のいらないテクスチャ.
			SpSt.Width = List.SpriteState.x;
			SpSt.Height = List.SpriteState.y;
		}
		if (i == enUITexHP1_FINE)
		{
			SpSt.Width = List.SpriteState.x;
			SpSt.Height = List.SpriteState.y;
		}
		//テクスチャの生成.
		m_pResourceTex->LoadStaticTexAll(SpSt);
		m_pResourceTex->TexPos(i);				

		SSLi++;
		delete SSFilePass;
	}

	//スキンモデルリソース.
	m_pResourceSkin = new clsSkinResource;
	m_pResourceSkin->Init(hWnd, pDevice, pContext);
	m_pResourceSkin->LoadStaticSkinMeshAll(m_hWnd, m_pDevice, m_pContext);

	//Effecseer.
	m_pResourceEff = new clsEffectsResouce;
	m_pResourceEff->Init(hWnd, pDevice, pContext);
	m_pResourceEff->Create();

	//音リソース.
	m_pResourceSound = new clsSoundResource;
	m_pResourceSound->Init(hWnd);
	for (int i = 0; i < enStaticSound_Max; i++)
	{
		m_pResourceSound->LoadSoundDataAll();
	}
}

void clsGame::Render()
{
	//画面ｸﾘｱ.
	m_pDirect3D->SetBuffer();
	//カメラとプロジェクションの設定.
	Camera();
	Proj();
	pSceneMgr->Draw(m_mView, m_mProj, m_vLight, m_Camera.vEye, SpSt);

	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ(色消去).
	m_pDirect3D->SetBlend(true);
	m_pDirect3D->RenderPresent();
}

void clsGame::Camera()
{
	//Y軸回転行列.
	D3DXMatrixRotationY(&m_Camera.mRotation, m_Camera.Yaw);
	//軸ベクトルを用意する.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);
	//カメラ位置・注視位置を調整.
	m_Camera.vEye -= vecAxisZ * 6.0f;
	m_Camera.vLook += vecAxisZ * 3.0f;		//自機の前側.
	//それぞれの高さ調整.
	m_Camera.vEye.y += 3.5f;				//自機の背中.
	m_Camera.vLook.y += 0.1f;				//自機の前側.
	//ﾋﾞｭｰ(ｶﾒﾗ)変換.
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);	//上方位置.
	//(out)ﾋﾞｭｰ計算結果.
	D3DXMatrixLookAtLH(&m_mView,&m_Camera.vEye, &m_Camera.vLook, &vUpVec);
}

void clsGame::Proj()
{
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ(射影行列)変換.
	D3DXMatrixPerspectiveFovLH(&m_mProj,D3DX_PI / 4.0,(FLOAT)WND_W / (FLOAT)WND_H,0.1f,100.0f);
}
