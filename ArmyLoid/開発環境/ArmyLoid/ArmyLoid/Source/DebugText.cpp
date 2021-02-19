#include "DebugText.h"
#include "MyMacro.h"

const char	SHADER_NAME[] = "Shader\\DebugText.hlsl";
//�萔.
const float DIMENSION = 12.0f;
const float WDIMENSION = 10.0f;
const float TEX_DIMENSION = 128.0f;

//�ݽ�׸�.
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

//�޽�׸�.
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

	//95�������J��Ԃ�.
	for (int i = 0; i < 95; i++){
		m_Kerning[i] = 10.0f;
	}
	//���޲���÷�Ă��߰.
	m_pContext11 = pContext;
	m_pContext11->GetDevice(&m_pDevice11);

	//window����.
	m_WindowWidth = Width;
	m_WindowHeight = Height;

	//̫�Ė��ɸ����(��`)�쐬.
	float left = 0, top = 0, right = 0, bottom = 0;
	int cnt = 0;
	//2�dٰ�߂�1�������w�肷��.
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
				MessageBox(nullptr, "���_�ޯ̧�쐬���s", "error", MB_OK);
				return E_FAIL;
			}
			cnt++;
		}
	}

	//ø����p����׍쐬.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	//ø���̨���Ɠ\��t�����̎w��.
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear)))
	{
		MessageBox(nullptr, "����׍쐬���s", "error", MB_OK);
		return E_FAIL;
	}

	//̫�Ă�ø����쐬.
	if (FAILED(
		D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11, "Data\\DebugText\\ascii.png",
		nullptr, nullptr, &m_pAsciiTexture, nullptr)))
	{
		MessageBox(nullptr, "���ޯ��̫��ø����쐬���s", "error", MB_OK);
		return E_FAIL;
	}

	//���_����ޗp������ލ쐬.
	ID3DBlob* pCompileShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME, nullptr, nullptr, "VS", "vs_5_0",
		0, 0, nullptr, &pCompileShader, &pErrors, nullptr)))
	{
		MessageBox(nullptr, "����ލ쐬���s(vs)", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//���_����ލ쐬.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		nullptr, &m_pVertexShader)))
	{
		MessageBox(nullptr, "���_�ޯ̧�쐬���s", "error", MB_OK);
		return E_FAIL;
	}

	//���_���߯�ڲ��Ă��`.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_���߯�ڲ��Ă��쐬.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
		layout, numElements,
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		&m_pVertexLayout)))
	{
		MessageBox(nullptr, "���_���߯�ڲ��č쐬���s", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompileShader);

	//�߸�ټ���ޗp������ލ쐬.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME, nullptr, nullptr, "PS", "ps_5_0",
		0, 0, nullptr, &pCompileShader, &pErrors, nullptr)))
	{
		MessageBox(nullptr, "����ލ쐬���s(ps)", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//�߸�ټ���ލ쐬.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		nullptr, &m_pPixelShader)))
	{
		MessageBox(nullptr, "�߸�ټ���ލ쐬���s", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompileShader);

	//�ݽ����ޯ̧�쐬.
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
		MessageBox(nullptr, "�ݽ����ޯ̧�쐬���s", "error", MB_OK);
		return E_FAIL;
	}

	//���ߐݒ�.
	SetBlend(true);

	return S_OK;
}


//����(��̧������)�ݒ�̐؂�ւ�.
void clsDebugText::SetBlend(bool bFlag)
{
	//��̧�����ޗp�����޽ðč\����.
	// pnģ�ٓ��ɱ�̧��񂪂���̂œ��߂���悤�������޽ðĂŐݒ肷��.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));//������.

	blendDesc.IndependentBlendEnable = false;
	// false:RenderTarget[0]�����ް�̂ݎg�p����.
	// true :RenderTarget[0�`7]���g�p�ł���
	//       (���ް���ޯĖ��ɓƗ����������ޏ���).
	blendDesc.AlphaToCoverageEnable = false;
	// true :��̧ĩ���گ�ނ��g�p����.
	blendDesc.RenderTarget[0].BlendEnable = bFlag;
	// true :��̧�����ނ��g�p����.

	//���f�ނɑ΂���ݒ�.
	blendDesc.RenderTarget[0].SrcBlend
		= D3D11_BLEND_SRC_ALPHA;//��̧�����ގw��.

	//�d�˂�f�ނɑ΂���ݒ�.
	blendDesc.RenderTarget[0].DestBlend
		= D3D11_BLEND_INV_SRC_ALPHA;//��̧�����ނ̔��]���w��.

	//�����޵�߼��.
	blendDesc.RenderTarget[0].BlendOp
		= D3D11_BLEND_OP_ADD;//ADD:���Z����.

	//���f�ނ̱�̧�ɑ΂���ݒ�.
	blendDesc.RenderTarget[0].SrcBlendAlpha
		= D3D11_BLEND_ONE;//���̂܂܎g�p.

	//�d�˂�f�ނ̱�̧�ɑ΂���ݒ�.
	blendDesc.RenderTarget[0].DestBlendAlpha
		= D3D11_BLEND_ZERO;//�������Ȃ�.

	//��̧�������޵�߼��.
	blendDesc.RenderTarget[0].BlendOpAlpha
		= D3D11_BLEND_OP_ADD;//ADD:���Z����.

	//�߸�ٖ��̏�������Ͻ�.
	blendDesc.RenderTarget[0].RenderTargetWriteMask
		= D3D11_COLOR_WRITE_ENABLE_ALL;//�S�Ă̐���(RGBA)�ւ��ް��̊i�[��������.

	//�����޽ðč쐬.
	if (FAILED(
		m_pDevice11->CreateBlendState(
		&blendDesc, &m_pBlendState)))
	{
		MessageBox(nullptr, "�����޽ðč쐬���s", "error", MB_OK);
	}

	//�����޽ðĂ̐ݒ�.
	UINT mask = 0xffffffff;//Ͻ��l.
	m_pContext11->OMSetBlendState(
		m_pBlendState, nullptr, mask);
}


//�����ݸފ֐�.
void clsDebugText::Render(char* text, float x, float y)
{
	//�ޭ���ݽ̫��.
	D3DXVECTOR3 vEye(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3	vLook(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(
		&m_mView, &vEye, &vLook, &vUp);
#if 0
	//��ۼު������ݽ̫��(�Œ�).
	D3DMATRIX  mOtho = {
		2.0f / (float)(m_WindowWidth), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)(m_WindowHeight), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f };
#else
	//��ۼު������ݽ̫��(�Œ�).
	D3DXMATRIX mOtho;
	D3DXMatrixIdentity(&mOtho);//�P�ʍs��쐬.
	mOtho._11 = 2.0f / (float)m_WindowWidth;
	mOtho._22 = -2.0f / (float)m_WindowHeight;
	mOtho._41 = -1.0f;
	mOtho._42 = 1.0f;
#endif
	m_mProj = mOtho;

	//���ۼް.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//���_���߯�ڲ��Ă��.
	m_pContext11->IASetInputLayout(
		m_pVertexLayout);

	//�g�p���鼪��ނ̓o�^.
	m_pContext11->VSSetShader(
		m_pVertexShader, nullptr, 0);
	m_pContext11->PSSetShader(
		m_pPixelShader, nullptr, 0);

	//���̺ݽ����ޯ̧���g������ނ̓o�^.
	m_pContext11->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);

	//ø����𼪰�ނɓn��.
	m_pContext11->PSSetSamplers(
		0, 1, &m_pSampleLinear);
	m_pContext11->PSSetShaderResources(
		0, 1, &m_pAsciiTexture);

	SetBlend(true);

	//��������ٰ��.
	for (int i = 0; i < strlen(text); i++)
	{
		char font = text[i];
		int index = font - 32;//̫�Ĳ��ޯ���쐬.

		//̫�������ݸ�.
		RenderFont(index, x, y);

		x += m_Kerning[index];
	}

	SetBlend(false);
}

//̫�������ݸފ֐�.
void clsDebugText::RenderFont(int FontIndex, float x, float y)
{
	//ܰ��ޕϊ�.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(
		&mWorld, x, y, -1.0f);

	//����ނ̺ݽ����ޯ̧�Ɋe���ް���n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	TEXT_CBUFFER cb;
	if (SUCCEEDED(
		m_pContext11->Map(
		m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ܰ���,�ޭ�,��ۼު���݂̍����ϊ��s���n��.
		D3DXMATRIX m = mWorld * m_mView * m_mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		//�װ��n��.
		cb.vColor = m_vColor;
		//������n��.
		cb.Alpha.x = m_Alpha;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}
	//���_�ޯ̧���.
	UINT stride = sizeof(TextVertex);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer[FontIndex],
		&stride, &offset);//������ɗ�O

	//�`��.
	UINT ColorKey = 0xffffffff;
	m_pContext11->OMSetBlendState(m_pBlendState, nullptr, ColorKey);
	m_pContext11->Draw(4, 0);
	m_pContext11->OMSetBlendState(0, nullptr, ColorKey);
}