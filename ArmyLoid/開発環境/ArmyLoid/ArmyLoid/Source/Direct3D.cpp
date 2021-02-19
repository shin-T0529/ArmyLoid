#include "Direct3D.h"

clsDirect3D::clsDirect3D()
	: m_pSwapChain(nullptr)
	, m_pBackBuffer_TexRTV(nullptr)
	, m_pBackBuffer_DSTex(nullptr)
	, m_pBackBuffer_DSTexDSV(nullptr)

{

}

clsDirect3D::~clsDirect3D()
{
	SAFE_DELETE(m_pSwapChain);
	SAFE_DELETE(m_pBackBuffer_TexRTV);
	SAFE_DELETE(m_pBackBuffer_DSTex);
	SAFE_DELETE(m_pBackBuffer_DSTexDSV);

}

void clsDirect3D::Create3D(HWND hWnd)
{
	//-----------------------------------------------
//	ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝ関係.
//-----------------------------------------------

//ｽﾜｯﾌﾟﾁｪｰﾝ構造体.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;//ﾊﾞｯｸﾊﾞｯﾌｧの数.
	sd.BufferDesc.Width = WND_W;//ﾊﾞｯｸﾊﾞｯﾌｧの幅.
	sd.BufferDesc.Height = WND_H;//ﾊﾞｯｸﾊﾞｯﾌｧの高さ.
	//ﾌｫｰﾏｯﾄ(32ﾋﾞｯﾄｶﾗｰ)
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//ﾘﾌﾚｯｼｭﾚｰﾄ(分母) ※FPS:60
	sd.BufferDesc.RefreshRate.Numerator = 60;
	//ﾘﾌﾚｯｼｭﾚｰﾄ(分子)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	//使い方(表示先)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
	sd.SampleDesc.Count = 1;//ﾏﾙﾁｻﾝﾌﾟﾙ数.
	sd.SampleDesc.Quality = 0;//ﾏﾙﾁｻﾝﾌﾟﾙのｸｵﾘﾃｨ
	sd.Windowed = TRUE;//ｳｨﾝﾄﾞｳﾓｰﾄﾞ(ﾌﾙｽｸﾘｰﾝ時はFALSE)

	//作成を試みる機能ﾚﾍﾞﾙの優先を指定.
	//	(GPUがｻﾎﾟｰﾄする機能ｾｯﾄの定義)
	//	D3D_FEATURE_LEVEL列挙型の配列.
	//	D3D_FEATURE_LEVEL_10_1：Direct3D 10.1のGPUﾚﾍﾞﾙ.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;//配列の要素数.

	//ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝの作成.
	//ﾊｰﾄﾞｳｪｱ(GPU)ﾃﾞﾊﾞｲｽで作成.
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL,	//ﾋﾞﾃﾞｵｱﾀﾞﾌﾟﾀへのﾎﾟｲﾝﾀ.
		D3D_DRIVER_TYPE_HARDWARE,//作成するﾃﾞﾊﾞｲｽの種類.
		NULL,	//ｿﾌﾄｳｪｱ ﾗｽﾀﾗｲｻﾞｰを実装するDLLのﾊﾝﾄﾞﾙ.
		0,	//有効にするﾗﾝﾀｲﾑﾚｲﾔｰ.
		&pFeatureLevels,//作成を試みる機能ﾚﾍﾞﾙの順序を指定する配列へのﾎﾟｲﾝﾀ.
		1,	//↑の要素数.
		D3D11_SDK_VERSION,//SKDのﾊﾞｰｼﾞｮﾝ.
		&sd,	//ｽﾜｯﾌﾟﾁｪｰﾝの初期化ﾊﾟﾗﾒｰﾀのﾎﾟｲﾝﾀ.
		&m_pSwapChain,//(out)ﾚﾝﾀﾞﾘﾝｸﾞに使用するｽﾜｯﾌﾟﾁｪｰﾝ.
		&m_pDevice,	//(out)作成されたﾃﾞﾊﾞｲｽ.
		pFeatureLevel,//機能ﾚﾍﾞﾙの配列にある最初の要素を表すﾎﾟｲﾝﾀ.
		&m_pContext)))//(out)ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	{
		//WARPﾃﾞﾊﾞｲｽの作成.(Windows Advanced Rasterization Platform)
		// D3D_FEATURE_LEVEL_9_1～D3D_FEATURE_LEVEL_10_1
		if (FAILED(
			D3D11CreateDeviceAndSwapChain(
				NULL,
				D3D_DRIVER_TYPE_WARP,//作成するﾃﾞﾊﾞｲｽの種類.
				NULL, 0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
				&sd, &m_pSwapChain, &m_pDevice, pFeatureLevel, &m_pContext)))
		{
			//ﾘﾌｧﾚﾝｽﾃﾞﾊﾞｲｽの作成.
			// DirectX SKDがｲﾝｽﾄｰﾙされてないと使えない.
			if (FAILED(
				D3D11CreateDeviceAndSwapChain(
					NULL,
					D3D_DRIVER_TYPE_REFERENCE,//作成するﾃﾞﾊﾞｲｽの種類.
					NULL, 0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
					&sd, &m_pSwapChain, &m_pDevice, pFeatureLevel, &m_pContext)))
			{
				MessageBox(NULL,
					"ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝの作成に失敗",
					"error(main.cpp)", MB_OK);
				//return E_FAIL;
			}
		}
	}

	//各種ﾃｸｽﾁｬと、それに付帯する各種ﾋﾞｭｰを作成.

	//-----------------------------------------------
	//	ﾊﾞｯｸﾊﾞｯﾌｧ準備：ｶﾗｰﾊﾞｯﾌｧ設定.
	//-----------------------------------------------

	//ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬを取得(既にあるので作成ではない)
	ID3D11Texture2D* pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0,
		__uuidof(ID3D11Texture2D),//__uuidof：式に関連付けされたGUIDを取得.
		//Texture2Dの唯一の物として扱う.
		(LPVOID*)&pBackBuffer_Tex);//(out)ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬ.

	//そのﾃｸｽﾁｬに対しﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ(RTV)を作成.
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex,NULL,
		&m_pBackBuffer_TexRTV);//(out)RTV.
	//ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬを開放.
	SAFE_RELEASE(pBackBuffer_Tex);

	//-----------------------------------------------
	//	ﾊﾞｯｸﾊﾞｯﾌｧ準備:ﾃﾞﾌﾟｽ(深度)ｽﾃﾝｼﾙ関係.
	//-----------------------------------------------
	//ﾃﾞﾌﾟｽ(深さor深度)ｽﾃﾝｼﾙﾋﾞｭｰ用のﾃｸｽﾁｬを作成.
	D3D11_TEXTURE2D_DESC descDepth;

	descDepth.Width = WND_W;//幅.
	descDepth.Height = WND_H;//高さ.
	descDepth.MipLevels = 1;//ﾐｯﾌﾟﾏｯﾌﾟﾚﾍﾞﾙ:1.
	descDepth.ArraySize = 1;//配列数:1.
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;//32ﾋﾞｯﾄﾌｫｰﾏｯﾄ.
	descDepth.SampleDesc.Count = 1;//ﾏﾙﾁｻﾝﾌﾟﾙの数.
	descDepth.SampleDesc.Quality = 0;//ﾏﾙﾁｻﾝﾌﾟﾙのｸｵﾘﾃｨ.
	descDepth.Usage = D3D11_USAGE_DEFAULT;//使用方法:ﾃﾞﾌｫﾙﾄ.
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;//深度(ｽﾃﾝｼﾙとして使用).
	descDepth.CPUAccessFlags = 0;//CPUからはｱｸｾｽしない.
	descDepth.MiscFlags = 0;//その他:設定なし.

	if (FAILED(
		m_pDevice->CreateTexture2D(
			&descDepth,NULL,
			&m_pBackBuffer_DSTex)))//(out)ﾃﾞﾌﾟｽｽﾃﾝｼﾙ用ﾃｸｽﾁｬ.
	{
		MessageBox(NULL,"ﾃﾞﾌﾟｽｽﾃﾝｼﾙ用ﾃｸｽﾁｬ作成失敗", "error", MB_OK);
		//return E_FAIL;
	}
	//そのﾃｸｽﾁｬに対しﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ(DSV)を作成.
	if (FAILED(
		m_pDevice->CreateDepthStencilView(
			m_pBackBuffer_DSTex,NULL,
			&m_pBackBuffer_DSTexDSV)))//(out)ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ(DSV).
	{
		MessageBox(NULL,"ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ作成失敗", "error", MB_OK);
		//return E_FAIL;
	}

	//ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰとﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰをﾊﾟｲﾌﾟﾗｲﾝにｾｯﾄ.
	m_pContext->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV);

	//ﾋﾞｭｰﾎﾟｰﾄの設定.
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)WND_W;	//幅.
	vp.Height = (FLOAT)WND_H;	//高さ.
	vp.TopLeftX = 0.0f;	//左上位置x.
	vp.TopLeftY = 0.0f;	//左上位置y.
	vp.MinDepth = 0.0f;	//最小深度(手前).
	vp.MaxDepth = 1.0f;	//最大深度(奥).
	m_pContext->RSSetViewports(1, &vp);

	//ﾗｽﾀﾗｲｽﾞ(面の塗り潰し方)設定.
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode = D3D11_FILL_SOLID;//塗り潰し(ｿﾘｯﾄﾞ).
	//D3D11_FILL_WIREFRAME:ﾜｲﾔｰﾌﾚｰﾑ.
	rdc.CullMode = D3D11_CULL_NONE;	//D3D11_CULL_NONE:ｶﾘﾝｸﾞを切る
	//(正面背面を描画する)
	//D3D11_CULL_FRONT:正面を描画しない.
	//D3D11_CULL_BACK :背面を描画しない.
	rdc.FrontCounterClockwise = FALSE;//ﾎﾟﾘｺﾞﾝの裏表を決定するﾌﾗｸﾞ.
	//TRUE:左回りなら前向き.右回りなら後ろ向き.
	//FALSE:逆になる.
	rdc.DepthClipEnable = TRUE;	//距離についてのｸﾘｯﾋﾟﾝｸﾞ有効.

	//ﾗｽﾀﾗｲｻﾞ作成.
	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);


}

void clsDirect3D::SetBlend(bool Flag)
{
	//アルファブレンド用ブレンドステート構造体
//pngファイル内にアルファ情報があるので透過するようにブレンドステートで設定する
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));//初期化

	blendDesc.IndependentBlendEnable = false;
	//false:RenderTarget[0]のメンバーのみ使用できる
	//true :RenderTarget[1]~[7]が使用できる
	//(レンダーターゲット毎に独立したブレンド処理)
	blendDesc.AlphaToCoverageEnable = false;
	//true  : アルファトゥカバレッジを使用する
	blendDesc.RenderTarget[0].BlendEnable = Flag;
	//true  : アルファブレンドを使用する

	//元素材に対する設定
	blendDesc.RenderTarget[0].SrcBlend
		= D3D11_BLEND_SRC_ALPHA;	//アルファブレンド指定

	//重ねる素材に対する設定
	blendDesc.RenderTarget[0].DestBlend
		= D3D11_BLEND_INV_SRC_ALPHA;	//アルファブレンド指定

	//ブレンドオプション
	blendDesc.RenderTarget[0].BlendOp
		= D3D11_BLEND_OP_ADD;	//ADD:加算合成

	//元素材のアルファに対する設定
	blendDesc.RenderTarget[0].SrcBlendAlpha
		= D3D11_BLEND_ONE;	//そのまま使用

	//重ねる素材のアルファに対する設定
	blendDesc.RenderTarget[0].DestBlendAlpha
		= D3D11_BLEND_ZERO;	//何もしない

	//アルファのブレンドオプション
	blendDesc.RenderTarget[0].BlendOpAlpha
		= D3D11_BLEND_OP_ADD;	//ADD:加算合成

	//ピクセルごとの書き込みマスク
	blendDesc.RenderTarget[0].RenderTargetWriteMask
		= D3D11_COLOR_WRITE_ENABLE_ALL;	//すべての成分(RGBA)へのデータの格納を許可


	//ブレンドステートの作成
	if (FAILED(m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState)))
	{
		MessageBox(NULL, "ブレンドステート作成失敗", "error", MB_OK);
	}

	//ブレンドステートの設定
	UINT mask = 0xffffffff;			//マスク値
	m_pContext->OMSetBlendState(m_pBlendState, NULL, mask);

}

void clsDirect3D::SetBuffer()
{
	const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//ｶﾗｰﾊﾞｯｸﾊﾞｯﾌｧ.
	GetDeviceContext()->ClearRenderTargetView(GetTexRTV(), ClearColor);
	//ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾊﾞｯｸﾊﾞｯﾌｧ.
	GetDeviceContext()->ClearDepthStencilView(GetDSTexDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ(色を消さない).
	SetBlend(false);

}

void clsDirect3D::RenderPresent()
{
	//ﾚﾝﾀﾞﾘﾝｸﾞされたｲﾒｰｼﾞを表示.
	m_pSwapChain->Present(0, 0);

}