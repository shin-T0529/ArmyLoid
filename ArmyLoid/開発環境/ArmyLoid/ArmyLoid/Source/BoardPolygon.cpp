#include "BoardPolygon.h"
#include "MyMacro.h"

const char	SHADER_NAME[] = "Shader\\Mesh.hlsl";

clsBoardPolygon::clsBoardPolygon()
: m_vPos()
, m_AnimCount(0)
, m_DispFlag(false)
, m_pVertexShader(nullptr)
, m_pVertexLayout(nullptr)
, m_pPixelShader(nullptr)
, m_pConstantBuffer(nullptr)
, m_pVertexBuffer(nullptr)
, m_pSampleLinear(nullptr)
, m_pTexture(nullptr)
, m_pBlendState(nullptr)
{
}

clsBoardPolygon::~clsBoardPolygon()
{
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pTexture);

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

//初期化.
HRESULT clsBoardPolygon::Init(ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, LPSTR fileName)
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	//ｼｪｰﾀﾞ作成.
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//板ﾎﾟﾘｺﾞﾝ作成.
	if (FAILED(InitModel(fileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//HLSLﾌｧｲﾙを読込ｼｪｰﾀﾞを作成する.
//HLSL(High Level Shading Language)
HRESULT clsBoardPolygon::InitShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;

	//HLSLからﾊﾞｰﾃｯｸｽｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL, "VS_Ita", "vs_5_0",
		0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,D3D11_INPUT_PER_VERTEX_DATA, 0	},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,		0,12,D3D11_INPUT_PER_VERTEX_DATA, 0}
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
		SHADER_NAME, NULL, NULL, "PS_Ita", "ps_5_0",
		0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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
	cb.ByteWidth = sizeof(SPRITESHADER_CBUFFER);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
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


//ﾓﾃﾞﾙ作成.
HRESULT clsBoardPolygon::InitModel(LPSTR fileName)
{
	float itaW = 1.0f;
	float w, h;
	w = h = (1.0f / 8.0f);

	//板ﾎﾟﾘ(四角形)の頂点を作成.
	SpriteVertex vertices[]=
	{
		//頂点座標(x,y,z)				UV座標(u,v).
		D3DXVECTOR3(-itaW,-itaW, 0.0f), D3DXVECTOR2( 0.0f,    h),//頂点１.
		D3DXVECTOR3(-itaW, itaW, 0.0f), D3DXVECTOR2( 0.0f, 0.0f),//頂点２.
		D3DXVECTOR3( itaW,-itaW, 0.0f), D3DXVECTOR2(    w,    h),//頂点３.
		D3DXVECTOR3( itaW, itaW, 0.0f), D3DXVECTOR2(    w, 0.0f) //頂点４.
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//ﾊﾞｯﾌｧ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;//使用方法(ﾃﾞﾌｫﾙﾄ)
	bd.ByteWidth = sizeof(SpriteVertex) * uVerMax;//頂点のｻｲｽﾞ(頂点数).
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//頂点ﾊﾞｯﾌｧとして扱う.
	bd.CPUAccessFlags = 0;//CPUからはｱｸｾｽしない.
	bd.MiscFlags = 0; //その他のﾌﾗｸﾞ(未使用).
	bd.StructureByteStride = 0;//構造体のｻｲｽﾞ(未使用).

	//ｻﾌﾞﾘｿｰｽﾃﾞｰﾀ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	//板ﾎﾟﾘの頂点をｾｯﾄ.

	//頂点ﾊﾞｯﾌｧの作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
		&bd, &InitData,
		&m_pVertexBuffer)))//(out)頂点ﾊﾞｯﾌｧ.
	{
		MessageBox(NULL, "頂点ﾊﾞｯﾌｧ作成失敗", "error", MB_OK);
		return E_FAIL;
	}

	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof(SpriteVertex);//ﾃﾞｰﾀ間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer, &stride, &offset);

	//ﾃｸｽﾁｬ用ｻﾝﾌﾟﾗ構造体.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//ﾘﾆｱﾌｨﾙﾀ(線形補間).
											//POINT:高速だが粗い.
	//ﾗｯﾋﾟﾝｸﾞﾓｰﾄﾞ.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
						//WRAP  :繰り返し.
						//MIRROR:反転繰り返し.
						//CLAMP :端の模様を引き伸ばす.
						//BORDER:別途境界色を決める.
	//ｻﾝﾌﾟﾗ作成.
	if (FAILED(
		m_pDevice11->CreateSamplerState(
		&SamDesc, &m_pSampleLinear)))
	{
		MessageBox(NULL,"ｻﾝﾌﾟﾗ作成失敗", "error", MB_OK);
		return E_FAIL;
	}

	//ﾃｸｽﾁｬ作成.
	if (FAILED(
		D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,	//ﾘｿｰｽを使用するﾃﾞﾊﾞｲｽへのﾎﾟｲﾝﾀ.
		fileName,	//ﾌｧｲﾙ名.
		NULL, NULL,
		&m_pTexture,//(out)ﾃｸｽﾁｬ.
		NULL)))
	{
		MessageBox(NULL, "ﾃｸｽﾁｬ作成失敗", "error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//描画(ﾚﾝﾀﾞﾘﾝｸﾞ).
void clsBoardPolygon::Render(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vEye)
{
	//ﾜｰﾙﾄﾞ行列.
	D3DXMATRIX	mWorld;

	//ﾜｰﾙﾄﾞ変換(表示位置を設定する).
	D3DXMatrixIdentity(&mWorld);//初期化.
	//平行移動.
	D3DXMatrixTranslation(&mWorld,
		m_vPos.x, m_vPos.y, m_vPos.z);

	//使用するｼｪｰﾀﾞを設定.
	m_pContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext11->PSSetShader(m_pPixelShader, NULL, 0);

	//ﾋﾞﾙﾎﾞｰﾄﾞ用.
	D3DXMATRIX	CancelRotation = mView;	//ﾋﾞｭｰ(ｶﾒﾗ)行列.
	CancelRotation._41
		= CancelRotation._42 = CancelRotation._43 = 0;//xyzを0にする.
	//CancelRotationの逆行列を求める.
	D3DXMatrixInverse(&CancelRotation, NULL, &CancelRotation);
	mWorld = CancelRotation * mWorld;

	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITESHADER_CBUFFER cb;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
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
		cb.vColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

		//ﾃｸｽﾁｬ座標.
		static D3DXVECTOR4 texUV(0.0f, 0.0f, 0.0f, 0.0f);

		cb.vUV = texUV;


		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うのか？.
	m_pContext11->VSSetConstantBuffers(2, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(2, 1, &m_pConstantBuffer);

	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof(SpriteVertex);//ﾃﾞｰﾀの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	m_pContext11->IASetInputLayout(m_pVertexLayout);
	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);//ｻﾝﾌﾟﾗｾｯﾄ.
	m_pContext11->PSSetShaderResources(0, 1, &m_pTexture);//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.

	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成.
	SetBlend(true);

	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pContext11->Draw(4, 0);

	SetBlend(false);
}



