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
//	���޲��ƽܯ�����݊֌W.
//-----------------------------------------------

//�ܯ�����ݍ\����.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;//�ޯ��ޯ̧�̐�.
	sd.BufferDesc.Width = WND_W;//�ޯ��ޯ̧�̕�.
	sd.BufferDesc.Height = WND_H;//�ޯ��ޯ̧�̍���.
	//̫�ϯ�(32�ޯĶװ)
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//��گ��ڰ�(����) ��FPS:60
	sd.BufferDesc.RefreshRate.Numerator = 60;
	//��گ��ڰ�(���q)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	//�g����(�\����)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;//����޳�����.
	sd.SampleDesc.Count = 1;//�������ِ�.
	sd.SampleDesc.Quality = 0;//�������ق̸��è
	sd.Windowed = TRUE;//����޳Ӱ��(�ٽ�ذݎ���FALSE)

	//�쐬�����݂�@�\���ق̗D����w��.
	//	(GPU����߰Ă���@�\��Ă̒�`)
	//	D3D_FEATURE_LEVEL�񋓌^�̔z��.
	//	D3D_FEATURE_LEVEL_10_1�FDirect3D 10.1��GPU����.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;//�z��̗v�f��.

	//���޲��ƽܯ�����݂̍쐬.
	//ʰ�޳��(GPU)���޲��ō쐬.
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL,	//���޵�������ւ��߲��.
		D3D_DRIVER_TYPE_HARDWARE,//�쐬�������޲��̎��.
		NULL,	//��ĳ�� ׽�ײ�ް����������DLL�������.
		0,	//�L���ɂ��������ڲ԰.
		&pFeatureLevels,//�쐬�����݂�@�\���ق̏������w�肷��z��ւ��߲��.
		1,	//���̗v�f��.
		D3D11_SDK_VERSION,//SKD���ް�ޮ�.
		&sd,	//�ܯ�����݂̏��������Ұ����߲��.
		&m_pSwapChain,//(out)�����ݸނɎg�p����ܯ������.
		&m_pDevice,	//(out)�쐬���ꂽ���޲�.
		pFeatureLevel,//�@�\���ق̔z��ɂ���ŏ��̗v�f��\���߲��.
		&m_pContext)))//(out)���޲���÷��.
	{
		//WARP���޲��̍쐬.(Windows Advanced Rasterization Platform)
		// D3D_FEATURE_LEVEL_9_1�`D3D_FEATURE_LEVEL_10_1
		if (FAILED(
			D3D11CreateDeviceAndSwapChain(
				NULL,
				D3D_DRIVER_TYPE_WARP,//�쐬�������޲��̎��.
				NULL, 0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
				&sd, &m_pSwapChain, &m_pDevice, pFeatureLevel, &m_pContext)))
		{
			//�̧�ݽ���޲��̍쐬.
			// DirectX SKD���ݽİق���ĂȂ��Ǝg���Ȃ�.
			if (FAILED(
				D3D11CreateDeviceAndSwapChain(
					NULL,
					D3D_DRIVER_TYPE_REFERENCE,//�쐬�������޲��̎��.
					NULL, 0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
					&sd, &m_pSwapChain, &m_pDevice, pFeatureLevel, &m_pContext)))
			{
				MessageBox(NULL,
					"���޲��ƽܯ�����݂̍쐬�Ɏ��s",
					"error(main.cpp)", MB_OK);
				//return E_FAIL;
			}
		}
	}

	//�e��ø����ƁA����ɕt�т���e���ޭ����쐬.

	//-----------------------------------------------
	//	�ޯ��ޯ̧�����F�װ�ޯ̧�ݒ�.
	//-----------------------------------------------

	//�ޯ��ޯ̧ø������擾(���ɂ���̂ō쐬�ł͂Ȃ�)
	ID3D11Texture2D* pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0,
		__uuidof(ID3D11Texture2D),//__uuidof�F���Ɋ֘A�t�����ꂽGUID���擾.
		//Texture2D�̗B��̕��Ƃ��Ĉ���.
		(LPVOID*)&pBackBuffer_Tex);//(out)�ޯ��ޯ̧ø���.

	//����ø����ɑ΂����ް���ޯ��ޭ�(RTV)���쐬.
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex,NULL,
		&m_pBackBuffer_TexRTV);//(out)RTV.
	//�ޯ��ޯ̧ø������J��.
	SAFE_RELEASE(pBackBuffer_Tex);

	//-----------------------------------------------
	//	�ޯ��ޯ̧����:���߽(�[�x)��ݼي֌W.
	//-----------------------------------------------
	//���߽(�[��or�[�x)��ݼ��ޭ��p��ø������쐬.
	D3D11_TEXTURE2D_DESC descDepth;

	descDepth.Width = WND_W;//��.
	descDepth.Height = WND_H;//����.
	descDepth.MipLevels = 1;//Я��ϯ������:1.
	descDepth.ArraySize = 1;//�z��:1.
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;//32�ޯ�̫�ϯ�.
	descDepth.SampleDesc.Count = 1;//�������ق̐�.
	descDepth.SampleDesc.Quality = 0;//�������ق̸��è.
	descDepth.Usage = D3D11_USAGE_DEFAULT;//�g�p���@:��̫��.
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;//�[�x(��ݼقƂ��Ďg�p).
	descDepth.CPUAccessFlags = 0;//CPU����ͱ������Ȃ�.
	descDepth.MiscFlags = 0;//���̑�:�ݒ�Ȃ�.

	if (FAILED(
		m_pDevice->CreateTexture2D(
			&descDepth,NULL,
			&m_pBackBuffer_DSTex)))//(out)���߽��ݼٗpø���.
	{
		MessageBox(NULL,"���߽��ݼٗpø����쐬���s", "error", MB_OK);
		//return E_FAIL;
	}
	//����ø����ɑ΂����߽��ݼ��ޭ�(DSV)���쐬.
	if (FAILED(
		m_pDevice->CreateDepthStencilView(
			m_pBackBuffer_DSTex,NULL,
			&m_pBackBuffer_DSTexDSV)))//(out)���߽��ݼ��ޭ�(DSV).
	{
		MessageBox(NULL,"���߽��ݼ��ޭ��쐬���s", "error", MB_OK);
		//return E_FAIL;
	}

	//���ް���ޯ��ޭ������߽��ݼ��ޭ����߲��ײ݂ɾ��.
	m_pContext->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV);

	//�ޭ��߰Ă̐ݒ�.
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)WND_W;	//��.
	vp.Height = (FLOAT)WND_H;	//����.
	vp.TopLeftX = 0.0f;	//����ʒux.
	vp.TopLeftY = 0.0f;	//����ʒuy.
	vp.MinDepth = 0.0f;	//�ŏ��[�x(��O).
	vp.MaxDepth = 1.0f;	//�ő�[�x(��).
	m_pContext->RSSetViewports(1, &vp);

	//׽�ײ��(�ʂ̓h��ׂ���)�ݒ�.
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode = D3D11_FILL_SOLID;//�h��ׂ�(�د��).
	//D3D11_FILL_WIREFRAME:ܲ԰�ڰ�.
	rdc.CullMode = D3D11_CULL_NONE;	//D3D11_CULL_NONE:��ݸނ�؂�
	//(���ʔw�ʂ�`�悷��)
	//D3D11_CULL_FRONT:���ʂ�`�悵�Ȃ�.
	//D3D11_CULL_BACK :�w�ʂ�`�悵�Ȃ�.
	rdc.FrontCounterClockwise = FALSE;//��غ�݂̗��\�����肷���׸�.
	//TRUE:�����Ȃ�O����.�E���Ȃ������.
	//FALSE:�t�ɂȂ�.
	rdc.DepthClipEnable = TRUE;	//�����ɂ��Ă̸د��ݸޗL��.

	//׽�ײ�ލ쐬.
	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);


}

void clsDirect3D::SetBlend(bool Flag)
{
	//�A���t�@�u�����h�p�u�����h�X�e�[�g�\����
//png�t�@�C�����ɃA���t�@��񂪂���̂œ��߂���悤�Ƀu�����h�X�e�[�g�Őݒ肷��
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));//������

	blendDesc.IndependentBlendEnable = false;
	//false:RenderTarget[0]�̃����o�[�̂ݎg�p�ł���
	//true :RenderTarget[1]~[7]���g�p�ł���
	//(�����_�[�^�[�Q�b�g���ɓƗ������u�����h����)
	blendDesc.AlphaToCoverageEnable = false;
	//true  : �A���t�@�g�D�J�o���b�W���g�p����
	blendDesc.RenderTarget[0].BlendEnable = Flag;
	//true  : �A���t�@�u�����h���g�p����

	//���f�ނɑ΂���ݒ�
	blendDesc.RenderTarget[0].SrcBlend
		= D3D11_BLEND_SRC_ALPHA;	//�A���t�@�u�����h�w��

	//�d�˂�f�ނɑ΂���ݒ�
	blendDesc.RenderTarget[0].DestBlend
		= D3D11_BLEND_INV_SRC_ALPHA;	//�A���t�@�u�����h�w��

	//�u�����h�I�v�V����
	blendDesc.RenderTarget[0].BlendOp
		= D3D11_BLEND_OP_ADD;	//ADD:���Z����

	//���f�ނ̃A���t�@�ɑ΂���ݒ�
	blendDesc.RenderTarget[0].SrcBlendAlpha
		= D3D11_BLEND_ONE;	//���̂܂܎g�p

	//�d�˂�f�ނ̃A���t�@�ɑ΂���ݒ�
	blendDesc.RenderTarget[0].DestBlendAlpha
		= D3D11_BLEND_ZERO;	//�������Ȃ�

	//�A���t�@�̃u�����h�I�v�V����
	blendDesc.RenderTarget[0].BlendOpAlpha
		= D3D11_BLEND_OP_ADD;	//ADD:���Z����

	//�s�N�Z�����Ƃ̏������݃}�X�N
	blendDesc.RenderTarget[0].RenderTargetWriteMask
		= D3D11_COLOR_WRITE_ENABLE_ALL;	//���ׂĂ̐���(RGBA)�ւ̃f�[�^�̊i�[������


	//�u�����h�X�e�[�g�̍쐬
	if (FAILED(m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState)))
	{
		MessageBox(NULL, "�u�����h�X�e�[�g�쐬���s", "error", MB_OK);
	}

	//�u�����h�X�e�[�g�̐ݒ�
	UINT mask = 0xffffffff;			//�}�X�N�l
	m_pContext->OMSetBlendState(m_pBlendState, NULL, mask);

}

void clsDirect3D::SetBuffer()
{
	const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//�װ�ޯ��ޯ̧.
	GetDeviceContext()->ClearRenderTargetView(GetTexRTV(), ClearColor);
	//���߽��ݼ��ޯ��ޯ̧.
	GetDeviceContext()->ClearDepthStencilView(GetDSTexDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//��̧������(�F�������Ȃ�).
	SetBlend(false);

}

void clsDirect3D::RenderPresent()
{
	//�����ݸނ��ꂽ�Ұ�ނ�\��.
	m_pSwapChain->Present(0, 0);

}