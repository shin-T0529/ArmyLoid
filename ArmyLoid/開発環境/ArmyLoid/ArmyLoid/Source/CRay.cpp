#include "CRay.h"
#include "MyMacro.h"

//�����̧�ٖ�(�ިڸ�؂��܂�).
const char SHADER_NAME[] = "Shader\\Ray.hlsl";

//�R���X�g���N�^
clsRay::clsRay()
	: m_pVertexShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pPixelShader(nullptr)
	, m_pConstantBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
{
}
//�f�X�g���N�^
clsRay::~clsRay()
{
	Release();
}


//������
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

//HLSĻ�ق�Ǎ�����ނ��쐬����.
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
	cb.ByteWidth = sizeof(RAYSHADER_CBUFFER);//�ݽ����ޯ̧�̻���.
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

//���C������
HRESULT clsRay::InitModel()
{
	//���C�̃o�[�e�b�N�X�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(RAY_VERTEX) * 2;	//2�_��
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_Ray.vPoint;	//���C�̍��W���Z�b�g
	if (FAILED(m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer)))
	{
		ERR_MSG("���_�o�b�t�@�쐬���s", "�G���[");
		return E_FAIL;
	}
	return S_OK;
}

//�`��(�����ݸ�).
void clsRay::Render(
	D3DXMATRIX &mView, D3DXMATRIX &mProj)
{
	//ܰ��ލs��.
	D3DXMATRIX	mWorld;
	//��]�s��
	D3DXMATRIX mRot, mTran;

	//ܰ��ޕϊ�(�\���ʒu��ݒ肷��).
	D3DXMatrixIdentity(&mWorld);//������.

	//��]�s��
	D3DXMatrixRotationY(&mRot, m_Ray.Yaw);

	//���s�ړ�.
	D3DXMatrixTranslation(&mTran,
		m_Ray.vPos.x, m_Ray.vPos.y, m_Ray.vPos.z);

	//���[���h�s��쐬
	mWorld = mRot * mTran;

	//�g�p���鼪��ނ�ݒ�.
	m_pContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext11->PSSetShader(m_pPixelShader, NULL, 0);

	//����ނ̺ݽ����ޯ̧�Ɋe���ް���n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	RAYSHADER_CBUFFER cb;	//�ݽ����ޯ̧.
	//�ޯ̧�����ް��̏��������J�n����map.
	if (SUCCEEDED(
		m_pContext11->Map(m_pConstantBuffer,
		0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ܰ���,�ޭ�,��ۼު���ݍs���n��.
		D3DXMATRIX m = mWorld * mView * mProj;
		//�s��̌v�Z���@��DirectX��GPU�ňقȂ邽�߁A�s���]�u����.
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		//�װ.
		cb.vColor = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	//���̺ݽ����ޯ̧���ǂ̼���ނŎg���̂��H.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//���_�ޯ̧���.
	UINT stride = sizeof(RAY_VERTEX);//�ް��̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	//���_���߯�ڲ��Ă��.
	m_pContext11->IASetInputLayout(m_pVertexLayout);
	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//����è�ނ������ݸ�.
	m_pContext11->Draw(2, 0);
}



//����֐�
HRESULT clsRay :: Release()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);

	return S_OK;
};
