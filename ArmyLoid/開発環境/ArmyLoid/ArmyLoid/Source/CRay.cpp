#include "CRay.h"
#include "MyMacro.h"

//ｼｪｰﾀﾞﾌｧｲﾙ名(ﾃﾞｨﾚｸﾄﾘも含む).
const char SHADER_NAME[] = "Shader\\Ray.hlsl";

//コンストラクタ
clsRay::clsRay()
	: m_pVertexShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pPixelShader(nullptr)
	, m_pConstantBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
{
}
//デストラクタ
clsRay::~clsRay()
{
	Release();
}


//初期化
HRESULT clsRay::Init(
	ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	if (FAILED(InitModel()))
	{

	}

	return S_OK;
}

//HLSLﾌｧｲﾙを読込ｼｪｰﾀﾞを作成する.
//HLSL(High Level Shading Language)
HRESULT clsRay::InitShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからﾊﾞｰﾃｯｸｽｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL, "VS_Main", "vs_5_0",
		uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(NULL, "ﾌﾞﾛﾌﾞ作成失敗", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pVertexShader)))//(out)ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ.
	{
		MessageBox(NULL, "ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ作成失敗", "error", MB_OK);
		return E_FAIL;
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
		layout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		&m_pVertexLayout)))//(out)頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ.
	{
		MessageBox(NULL, "頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗", "error", MB_OK);
		return E_FAIL;
	}
	//ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ用のﾌﾞﾛﾌﾞを解放.
	SAFE_RELEASE(pCompiledShader);

	//HLSLからﾋﾟｸｾﾙｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL, "PS_Main", "ps_5_0",
		uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(NULL, "ﾌﾞﾛﾌﾞ作成失敗", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾋﾟｸｾﾙｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pPixelShader)))	//(out)ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	{
		MessageBox(NULL, "ﾋﾟｸｾﾙｼｪｰﾀﾞ作成失敗", "error", MB_OK);
		return E_FAIL;
	}
	//ﾋﾟｸｾﾙｼｪｰﾀﾞ用のﾌﾞﾛﾌﾞを解放.
	SAFE_RELEASE(pCompiledShader);

	//-----------------------------------------------
	//	ｺﾝｽﾀﾝﾄ(定数)ﾊﾞｯﾌｧ作成.
	//		ｼｪｰﾀﾞに特定の数値を送るﾊﾞｯﾌｧ.
	//-----------------------------------------------
	//ここでは変換行列渡し用.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを指定.
	cb.ByteWidth = sizeof(RAYSHADER_CBUFFER);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//書き込みでｱｸｾｽ.
	cb.MiscFlags = 0;	//その他のﾌﾗｸﾞ(未使用)
	cb.StructureByteStride = 0;//構造体のサイズ(未使用)
	cb.Usage = D3D11_USAGE_DYNAMIC;//使用方法:直接書き込み.

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer)))//(out)ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	{
		MessageBox(NULL,
			"ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗", "error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//レイ初期化
HRESULT clsRay::InitModel()
{
	//レイのバーテックスバッファ作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(RAY_VERTEX) * 2;	//2点分
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_Ray.vPoint;	//レイの座標をセット
	if (FAILED(m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer)))
	{
		ERR_MSG("頂点バッファ作成失敗", "エラー");
		return E_FAIL;
	}
	return S_OK;
}

//描画(ﾚﾝﾀﾞﾘﾝｸﾞ).
void clsRay::Render(
	D3DXMATRIX &mView, D3DXMATRIX &mProj)
{
	//ﾜｰﾙﾄﾞ行列.
	D3DXMATRIX	mWorld;
	//回転行列
	D3DXMATRIX mRot, mTran;

	//ﾜｰﾙﾄﾞ変換(表示位置を設定する).
	D3DXMatrixIdentity(&mWorld);//初期化.

	//回転行列
	D3DXMatrixRotationY(&mRot, m_Ray.Yaw);

	//平行移動.
	D3DXMatrixTranslation(&mTran,
		m_Ray.vPos.x, m_Ray.vPos.y, m_Ray.vPos.z);

	//ワールド行列作成
	mWorld = mRot * mTran;

	//使用するｼｪｰﾀﾞを設定.
	m_pContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext11->PSSetShader(m_pPixelShader, NULL, 0);

	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	RAYSHADER_CBUFFER cb;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き換え開始時にmap.
	if (SUCCEEDED(
		m_pContext11->Map(m_pConstantBuffer,
		0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝ行列を渡す.
		D3DXMATRIX m = mWorld * mView * mProj;
		//行列の計算方法がDirectXとGPUで異なるため、行列を転置する.
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		//ｶﾗｰ.
		cb.vColor = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うのか？.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof(RAY_VERTEX);//ﾃﾞｰﾀの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	m_pContext11->IASetInputLayout(m_pVertexLayout);
	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pContext11->Draw(2, 0);
}



//解放関数
HRESULT clsRay :: Release()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);

	return S_OK;
};
