#include "DebugText.h"
#include "MyMacro.h"

const char	SHADER_NAME[] = "Shader\\DebugText.hlsl";
//定数.
const float DIMENSION = 12.0f;
const float WDIMENSION = 10.0f;
const float TEX_DIMENSION = 128.0f;

//ｺﾝｽﾄﾗｸﾀ.
clsDebugText::clsDebugText()
: m_pDevice11(nullptr)
, m_pContext11(nullptr)
, m_pVertexShader(nullptr)
, m_pVertexLayout(nullptr)
, m_pPixelShader(nullptr)
, m_pConstantBuffer(nullptr)
, m_pSampleLinear(nullptr)
, m_pAsciiTexture(nullptr)
, m_pBlendState(nullptr)
, m_WindowWidth(0)
, m_WindowHeight(0)
, m_Scale(1.0f)
, m_Alpha(0.0f)
, m_vColor()
, m_mView()
, m_mProj()
{
}

//ﾃﾞｽﾄﾗｸﾀ.
clsDebugText::~clsDebugText()
{

	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);

	for (int i = 100 - 1; i >= 0; i--) {
		SAFE_RELEASE(m_pVertexBuffer[i]);
	}

	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pAsciiTexture);
	SAFE_RELEASE(m_pBlendState);

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

HRESULT clsDebugText::Init(
	ID3D11DeviceContext* pContext,
	DWORD Width, DWORD Height,
	float Size, D3DXVECTOR4 vColor)
{
	m_Alpha = vColor.w;
	m_vColor = vColor;
	m_Scale = Size / 25.0f;

	//95文字分繰り返し.
	for (int i = 0; i < 95; i++){
		m_Kerning[i] = 10.0f;
	}
	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄをｺﾋﾟｰ.
	m_pContext11 = pContext;
	m_pContext11->GetDevice(&m_pDevice11);

	//windowｻｲｽﾞ.
	m_WindowWidth = Width;
	m_WindowHeight = Height;

	//ﾌｫﾝﾄ毎にｸｱｯﾄﾞ(矩形)作成.
	float left = 0, top = 0, right = 0, bottom = 0;
	int cnt = 0;
	//2重ﾙｰﾌﾟで1文字ずつ指定する.
	for (int col = 0; col < 10; col++){
		for (int row = 0; row < 10; row++){
			left = (float)col * WDIMENSION;
			top = (float)row * DIMENSION;
			right = left + m_Kerning[cnt];
			bottom = top + DIMENSION;

			left /= TEX_DIMENSION;
			top /= TEX_DIMENSION;
			right /= TEX_DIMENSION;
			bottom /= TEX_DIMENSION;

			TextVertex vertices[] =
			{
				D3DXVECTOR3(0.0f, DIMENSION, 0.0f), D3DXVECTOR2(left, bottom),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(left, top),
				D3DXVECTOR3(WDIMENSION, DIMENSION, 0.0f), D3DXVECTOR2(right, bottom),
				D3DXVECTOR3(WDIMENSION, 0.0f, 0.0f), D3DXVECTOR2(right, top)
			};
			CD3D11_BUFFER_DESC	bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(TextVertex)* 4;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;
			if (FAILED(m_pDevice11->CreateBuffer(
				&bd, &InitData, &m_pVertexBuffer[cnt])))
			{
				MessageBox(nullptr, "頂点ﾊﾞｯﾌｧ作成失敗", "error", MB_OK);
				return E_FAIL;
			}
			cnt++;
		}
	}

	//ﾃｸｽﾁｬ用ｻﾝﾌﾟﾗ作成.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	//ﾃｸｽﾁｬﾌｨﾙﾀと貼り付け方の指定.
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear)))
	{
		MessageBox(nullptr, "ｻﾝﾌﾟﾗ作成失敗", "error", MB_OK);
		return E_FAIL;
	}

	//ﾌｫﾝﾄのﾃｸｽﾁｬ作成.
	if (FAILED(
		D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11, "Data\\DebugText\\ascii.png",
		nullptr, nullptr, &m_pAsciiTexture, nullptr)))
	{
		MessageBox(nullptr, "ﾃﾞﾊﾞｯｸﾞﾌｫﾝﾄﾃｸｽﾁｬ作成失敗", "error", MB_OK);
		return E_FAIL;
	}

	//頂点ｼｪｰﾀﾞ用にﾌﾞﾛﾌﾞ作成.
	ID3DBlob* pCompileShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME, nullptr, nullptr, "VS", "vs_5_0",
		0, 0, nullptr, &pCompileShader, &pErrors, nullptr)))
	{
		MessageBox(nullptr, "ﾌﾞﾛﾌﾞ作成失敗(vs)", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//頂点ｼｪｰﾀﾞ作成.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		nullptr, &m_pVertexShader)))
	{
		MessageBox(nullptr, "頂点ﾊﾞｯﾌｧ作成失敗", "error", MB_OK);
		return E_FAIL;
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
		layout, numElements,
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		&m_pVertexLayout)))
	{
		MessageBox(nullptr, "頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompileShader);

	//ﾋﾟｸｾﾙｼｪｰﾀﾞ用にﾌﾞﾛﾌﾞ作成.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME, nullptr, nullptr, "PS", "ps_5_0",
		0, 0, nullptr, &pCompileShader, &pErrors, nullptr)))
	{
		MessageBox(nullptr, "ﾌﾞﾛﾌﾞ作成失敗(ps)", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//ﾋﾟｸｾﾙｼｪｰﾀﾞ作成.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		nullptr, &m_pPixelShader)))
	{
		MessageBox(nullptr, "ﾋﾟｸｾﾙｼｪｰﾀﾞ作成失敗", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompileShader);

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(TEXT_CBUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(
		m_pDevice11->CreateBuffer(
		&cb, nullptr, &m_pConstantBuffer)))
	{
		MessageBox(nullptr, "ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗", "error", MB_OK);
		return E_FAIL;
	}

	//透過設定.
	SetBlend(true);

	return S_OK;
}


//透過(ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ)設定の切り替え.
void clsDebugText::SetBlend(bool bFlag)
{
	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ構造体.
	// pngﾌｧｲﾙ内にｱﾙﾌｧ情報があるので透過するようにﾌﾞﾚﾝﾄﾞｽﾃｰﾄで設定する.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));//初期化.

	blendDesc.IndependentBlendEnable = false;
	// false:RenderTarget[0]のﾒﾝﾊﾞｰのみ使用する.
	// true :RenderTarget[0～7]が使用できる
	//       (ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄ毎に独立したﾌﾞﾚﾝﾄﾞ処理).
	blendDesc.AlphaToCoverageEnable = false;
	// true :ｱﾙﾌｧﾄｩｶﾊﾞﾚｯｼﾞを使用する.
	blendDesc.RenderTarget[0].BlendEnable = bFlag;
	// true :ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを使用する.

	//元素材に対する設定.
	blendDesc.RenderTarget[0].SrcBlend
		= D3D11_BLEND_SRC_ALPHA;//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ指定.

	//重ねる素材に対する設定.
	blendDesc.RenderTarget[0].DestBlend
		= D3D11_BLEND_INV_SRC_ALPHA;//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞの反転を指定.

	//ﾌﾞﾚﾝﾄﾞｵﾌﾟｼｮﾝ.
	blendDesc.RenderTarget[0].BlendOp
		= D3D11_BLEND_OP_ADD;//ADD:加算合成.

	//元素材のｱﾙﾌｧに対する設定.
	blendDesc.RenderTarget[0].SrcBlendAlpha
		= D3D11_BLEND_ONE;//そのまま使用.

	//重ねる素材のｱﾙﾌｧに対する設定.
	blendDesc.RenderTarget[0].DestBlendAlpha
		= D3D11_BLEND_ZERO;//何もしない.

	//ｱﾙﾌｧのﾌﾞﾚﾝﾄﾞｵﾌﾟｼｮﾝ.
	blendDesc.RenderTarget[0].BlendOpAlpha
		= D3D11_BLEND_OP_ADD;//ADD:加算合成.

	//ﾋﾟｸｾﾙ毎の書き込みﾏｽｸ.
	blendDesc.RenderTarget[0].RenderTargetWriteMask
		= D3D11_COLOR_WRITE_ENABLE_ALL;//全ての成分(RGBA)へのﾃﾞｰﾀの格納を許可する.

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成.
	if (FAILED(
		m_pDevice11->CreateBlendState(
		&blendDesc, &m_pBlendState)))
	{
		MessageBox(nullptr, "ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成失敗", "error", MB_OK);
	}

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄの設定.
	UINT mask = 0xffffffff;//ﾏｽｸ値.
	m_pContext11->OMSetBlendState(
		m_pBlendState, nullptr, mask);
}


//ﾚﾝﾀﾞﾘﾝｸﾞ関数.
void clsDebugText::Render(char* text, float x, float y)
{
	//ﾋﾞｭｰﾄﾗﾝｽﾌｫｰﾑ.
	D3DXVECTOR3 vEye(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3	vLook(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(
		&m_mView, &vEye, &vLook, &vUp);
#if 0
	//ﾌﾟﾛｼﾞｪｸｼｮﾝﾄﾗﾝｽﾌｫｰﾑ(固定).
	D3DMATRIX  mOtho = {
		2.0f / (float)(m_WindowWidth), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)(m_WindowHeight), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f };
#else
	//ﾌﾟﾛｼﾞｪｸｼｮﾝﾄﾗﾝｽﾌｫｰﾑ(固定).
	D3DXMATRIX mOtho;
	D3DXMatrixIdentity(&mOtho);//単位行列作成.
	mOtho._11 = 2.0f / (float)m_WindowWidth;
	mOtho._22 = -2.0f / (float)m_WindowHeight;
	mOtho._41 = -1.0f;
	mOtho._42 = 1.0f;
#endif
	m_mProj = mOtho;

	//ﾄﾎﾟﾛｼﾞｰ.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	m_pContext11->IASetInputLayout(
		m_pVertexLayout);

	//使用するｼｪｰﾀﾞの登録.
	m_pContext11->VSSetShader(
		m_pVertexShader, nullptr, 0);
	m_pContext11->PSSetShader(
		m_pPixelShader, nullptr, 0);

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを使うｼｪｰﾀﾞの登録.
	m_pContext11->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers(
		0, 1, &m_pSampleLinear);
	m_pContext11->PSSetShaderResources(
		0, 1, &m_pAsciiTexture);

	SetBlend(true);

	//文字数分ﾙｰﾌﾟ.
	for (int i = 0; i < strlen(text); i++)
	{
		char font = text[i];
		int index = font - 32;//ﾌｫﾝﾄｲﾝﾃﾞｯｸｽ作成.

		//ﾌｫﾝﾄﾚﾝﾀﾞﾘﾝｸﾞ.
		RenderFont(index, x, y);

		x += m_Kerning[index];
	}

	SetBlend(false);
}

//ﾌｫﾝﾄﾚﾝﾀﾞﾘﾝｸﾞ関数.
void clsDebugText::RenderFont(int FontIndex, float x, float y)
{
	//ﾜｰﾙﾄﾞ変換.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(
		&mWorld, x, y, -1.0f);

	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	TEXT_CBUFFER cb;
	if (SUCCEEDED(
		m_pContext11->Map(
		m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成変換行列を渡す.
		D3DXMATRIX m = mWorld * m_mView * m_mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		//ｶﾗｰを渡す.
		cb.vColor = m_vColor;
		//透明を渡す.
		cb.Alpha.x = m_Alpha;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}
	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof(TextVertex);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer[FontIndex],
		&stride, &offset);//ここ偶に例外

	//描画.
	UINT ColorKey = 0xffffffff;
	m_pContext11->OMSetBlendState(m_pBlendState, nullptr, ColorKey);
	m_pContext11->Draw(4, 0);
	m_pContext11->OMSetBlendState(0, nullptr, ColorKey);
}