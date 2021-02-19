#include "2DTex.h"
#include "MyMacro.h"

const char	SHADER_NAME[] = "Shader\\Sprite2D.hlsl";

cls2DTex::cls2DTex()
: m_pVertexShader(nullptr)
, m_pVertexLayout(nullptr)
, m_pPixelShader(nullptr)
, m_pConstantBuffer(nullptr)
, m_pVertexBuffer(nullptr)
, m_pSampleLinear(nullptr)
, m_pTexture(nullptr)
{
	m_PatternNo.x = 0.0f;
	m_PatternNo.y = 0.0f;
	m_Alpha = 1.0f;
	m_DispFlag = true;
}

cls2DTex::~cls2DTex()
{
	DetatchTex();
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pTexture);

	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;

}

HRESULT cls2DTex::Init(ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const char* fileName,
	const SPRITE_STATE& ss)
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if (FAILED(InitShader()))
	{	return E_FAIL;	}

	if (FAILED(InitTex(ss.Width,ss.Height,ss.U,ss.V)))
	{	return E_FAIL;	}

	if (FAILED(CreateTexture(fileName,&m_pTexture)))
	{	return E_FAIL;	}

	return S_OK;
}

HRESULT cls2DTex::InitShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT	uCompileFlag = 0;
#ifdef _DEBUG

	uCompileFlag = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;

#endif//#ifdef _DEBUG


	//HLSL�����ްï������ނ�����ނ��쐬.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL, "VS_Main", "vs_5_0",
		uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(NULL, "����ލ쐬���s", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬��������ނ���u�ްï������ށv���쐬.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pVertexShader)))//(out)�ްï�������.
	{
		MessageBox(NULL, "�ްï������ލ쐬���s", "error", MB_OK);
		return E_FAIL;
	}

	//���_���߯�ڲ��Ă��`.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	//���_���߯�ڲ��Ă̔z��v�f�����Z�o.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_���߯�ڲ��č쐬.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
		layout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		&m_pVertexLayout)))//(out)���_���߯�ڲ���.
	{
		MessageBox(NULL, "���_���߯�ڲ��č쐬���s", "error", MB_OK);
		return E_FAIL;
	}
	//�ްï������ޗp������ނ����.
	SAFE_RELEASE(pCompiledShader);

	//HLSL�����߸�ټ���ނ�����ނ��쐬.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL, "PS_Main", "ps_5_0",
		uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(NULL, "����ލ쐬���s", "error", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬��������ނ���u�߸�ټ���ށv���쐬.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pPixelShader)))	//(out)�߸�ټ����.
	{
		MessageBox(NULL, "�߸�ټ���ލ쐬���s", "error", MB_OK);
		return E_FAIL;
	}
	//�߸�ټ���ޗp������ނ����.
	SAFE_RELEASE(pCompiledShader);

	//-----------------------------------------------
	//	�ݽ���(�萔)�ޯ̧�쐬.
	//		����ނɓ���̐��l�𑗂��ޯ̧.
	//-----------------------------------------------
	//�����ł͕ϊ��s��n���p.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�ݽ����ޯ̧���w��.
	cb.ByteWidth = sizeof(SPRITESHADER_CBUFFER);//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//�������݂ű���.
	cb.MiscFlags = 0;	//���̑����׸�(���g�p)
	cb.StructureByteStride = 0;//�\���̂̃T�C�Y(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;//�g�p���@:���ڏ�������.

	//�ݽ����ޯ̧�쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer)))//(out)�ݽ����ޯ̧.
	{
		MessageBox(NULL,
			"�ݽ����ޯ̧�쐬���s", "error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT cls2DTex::InitTex(float Width, float Height, float U, float V)
{


	//�����(�l�p�`)�̒��_���쐬.
	SpriteVertex vertices[] =
	{
		//���_���W(x,y,z)					UV���W(u,v)
		D3DXVECTOR3(0.0f,	Height,	0.0f), D3DXVECTOR2(0.0f, V),	//���_�P.
		D3DXVECTOR3(0.0f,	0.0f,	0.0f), D3DXVECTOR2(0.0f, 0.0f),	//���_�Q.
		D3DXVECTOR3(Width,	Height,	0.0f), D3DXVECTOR2(U,	 V),	//���_�R.
		D3DXVECTOR3(Width,	0.0f,	0.0f), D3DXVECTOR2(U,	 0.0f)	//���_�S.
	};
	//�ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//�ޯ̧�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;//�g�p���@(��̫��)
	bd.ByteWidth = sizeof(SpriteVertex) * uVerMax;//���_�̻���(���_��).
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���_�ޯ̧�Ƃ��Ĉ���.
	bd.CPUAccessFlags = 0;//CPU����ͱ������Ȃ�.
	bd.MiscFlags = 0; //���̑����׸�(���g�p).
	bd.StructureByteStride = 0;//�\���̻̂���(���g�p).

	//���ؿ���ް��\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	//����؂̒��_���.

	//���_�ޯ̧�̍쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(
		&bd, &InitData,
		&m_pVertexBuffer)))//(out)���_�ޯ̧.
	{
		MessageBox(NULL, "���_�ޯ̧�쐬���s", "error", MB_OK);
		return E_FAIL;
	}

	//���_�ޯ̧���.
	UINT stride = sizeof(SpriteVertex);//�ް��Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer, &stride, &offset);

	//ø����p����׍\����.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//�Ʊ̨��(���`���).
	//POINT:���������e��.
	//ׯ��ݸ�Ӱ��.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//WRAP  :�J��Ԃ�.
	//MIRROR:���]�J��Ԃ�.
	//CLAMP :�[�̖͗l�������L�΂�.
	//BORDER:�ʓr���E�F�����߂�.
	//����׍쐬.
	if (FAILED(
		m_pDevice11->CreateSamplerState(
		&SamDesc, &m_pSampleLinear)))
	{
		MessageBox(NULL,
			"����׍쐬���s", "error", MB_OK);
		return E_FAIL;
	}


	return S_OK;
}

//�e�N�X�`���쐬.
HRESULT	cls2DTex::CreateTexture(const char* fileName, ID3D11ShaderResourceView** pTexture)
{

	//ø����쐬.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,	//ؿ�����g�p�������޲��ւ��߲��.
		fileName,	//̧�ٖ�.
		NULL, NULL,pTexture,//(out)ø���.
		NULL)))
	{ 
		ERR_MSG("�e�N�X�`���쐬���s","error")
		return E_FAIL;
	}


	return S_OK;
}

//�`��(�����ݸ�).
void cls2DTex::Render(SPRITE_STATE &ss, POINTFLOAT &PatNo)
{
	//ܰ��ލs��.
	D3DXMATRIX	mWorld;

	//ܰ��ޕϊ�(�\���ʒu��ݒ肷��).
	D3DXMatrixIdentity(&mWorld);//������.
	//���s�ړ�.
	D3DXMatrixTranslation(&mWorld,m_vPos.x, m_vPos.y, m_vPos.z);

	//�g�p���鼪��ނ�ݒ�.
	m_pContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext11->PSSetShader(m_pPixelShader, NULL, 0);

	//����ނ̺ݽ����ޯ̧�Ɋe���ް���n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITESHADER_CBUFFER cb;	//�ݽ����ޯ̧.
	//�ޯ̧�����ް��̏��������J�n����map.
	if (SUCCEEDED(m_pContext11->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ܰ��ލs���n��.
		D3DXMATRIX m = mWorld;
		//�s��̌v�Z���@��DirectX��GPU�ňقȂ邽�߁A�s���]�u����.
		D3DXMatrixTranspose(&m, &m);
		cb.mW = m;

		//�r���[�|�[�g�̕��A������n��.
		cb.fViewPortWidth = WND_W;
		cb.fViewPortHeight = WND_H;

		//�A���t�@�l��n��.
		cb.fAlpha = m_Alpha;

		//UV���W.
		//�P�ϓ�����̊�����Ͻ�ԍ����|���Ĉʒu��ݒ肷��.
		//�ԍ��̓��肪�ł��Ă��Ȃ�.
		cb.vUV.x = (ss.Stride.w / ss.Base.w) * PatNo.x;
		cb.vUV.y = (ss.Stride.h / ss.Base.h) * PatNo.y;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	//���̺ݽ����ޯ̧���ǂ̼���ނŎg���̂��H.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//���_�ޯ̧���.
	UINT stride = sizeof(SpriteVertex);//�ް��̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//���_���߯�ڲ��Ă��.
	m_pContext11->IASetInputLayout(m_pVertexLayout);
	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ø����𼪰�ނɓn��.
	m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);//����׾��.
	m_pContext11->PSSetShaderResources(0, 1, &m_pTexture);//ø����𼪰�ނɓn��.

	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬
	SetBlend(true);


	//����è�ނ������ݸ�.
	m_pContext11->Draw(4, 0);

	SetBlend(false);

}

//���f�����R�t.
void cls2DTex::AttachTex(cls2DTex* pTex)
{
	if (pTex != NULL)
	{
		m_pStaticTex = pTex;
	}
}
//���f�����j��.
void cls2DTex::DetatchTex()
{
	if (m_pStaticTex != NULL)
	{
		m_pStaticTex = NULL;
	}
}

void cls2DTex::TexRender(SPRITE_STATE &ss,POINTFLOAT &PatNo)
{
	if (m_pStaticTex == NULL)
	{
		return;
	}
	//�X�V.
	if (m_pStaticTex == NULL)
	{
		return;
	}
	m_pStaticTex->m_vPos = m_vPos;
	m_pStaticTex->m_Alpha = m_Alpha;
	m_pStaticTex->m_PatternNo = PatNo;
	m_pStaticTex->m_DispFlag = m_DispFlag;
	//���b�V���t�@�C���̃����_�����O�֐����Ă�,
	m_pStaticTex->Render(ss, PatNo);

}
