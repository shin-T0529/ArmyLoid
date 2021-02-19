#include "DX9Mesh.h"

const char SHADER_NAME[] = "Shader\\Mesh.hlsl";
//�ݽ�׸�.
clsDx9Mesh::clsDx9Mesh()
: m_hWnd(nullptr)
, m_pD3d(nullptr)
, m_pDevice9(nullptr)
, m_pMesh(nullptr)
, m_pDevice11(nullptr)
, m_pContext11(nullptr)
, m_pVertexShader(nullptr)
, m_pVertexLayout(nullptr)
, m_pPixelShader(nullptr)
, m_pConstantBuffer0(nullptr)
, m_pConstantBuffer1(nullptr)
, m_pVertexBuffer(nullptr)
, m_ppIndexBuffer(nullptr)
, m_pSampleLinear(nullptr)
, m_pMaterials(nullptr)
, m_NumAttr(0)
, m_AttrID()
, m_TextureFlag(false)
, m_Scale(1.0f)
, m_Yaw(0.0f)
, m_Pitch(0.0f)
, m_Roll(0.0f)
, m_vPos()
, CenterX(0.0f)
, CenterY(0.0f)
, CenterZ(0.0f)
, Direct(0.0f)
, SpdSpin(0.0f)
, Distance(0.0f)
, m_Sphere()
, m_BBox()
, m_pBlendState(nullptr)
, m_ShotFlag(false)
, m_DeadFlag(false)
, m_eDir(enDirection_Stop)
, m_vRay()
, m_vAxis()
, m_pMeshForRay(nullptr)
{
}

//�޽�׸�.
clsDx9Mesh::~clsDx9Mesh()
{
	Release();

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_hWnd = nullptr;
}

HRESULT clsDx9Mesh::Init(HWND hWnd, ID3D11Device* pDevice11,ID3D11DeviceContext* pContext11, LPSTR fileName)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if (FAILED(InitDx9(m_hWnd)))
	{
		return E_FAIL;
	}
	if (FAILED(LoadXMesh(fileName)))
	{
		MessageBox(NULL, "X̧�ٓǍ����s", "error", MB_OK);
		return E_FAIL;
	}
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}

	return S_OK;
}


//Dx9������.
HRESULT clsDx9Mesh::InitDx9(HWND hWnd)
{
	m_hWnd = hWnd;

	//�uDirect3D�v��޼ު�Ă̍쐬.
	m_pD3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3d == NULL)
	{
		MessageBox(NULL, "Dx9��޼ު�č쐬���s", "error", MB_OK);
		return E_FAIL;
	}

	//�uDirect3D���޲��v��޼ު�Ă̍쐬.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//�ޯ��ޯ̧��̫�ϯ�(��̫��).
	d3dpp.BackBufferCount = 1;					//�ޯ��ޯ̧�̐�.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�ܯ�ߴ̪��(��̫��).
	d3dpp.Windowed = true;						//����޳Ӱ��.
	d3dpp.EnableAutoDepthStencil = true;		//��ݼٗL��.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//��ݼق�̫�ϯ�(16�ޯ�).

	//���޲��쐬(HALӰ��:�`��ƒ��_������GPU�ōs��).
	if (FAILED(m_pD3d->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDevice9)))
	{
		//���޲��쐬(HALӰ��:�`���GPU�A���_������CPU�ōs��).
		if (FAILED(m_pD3d->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice9)))
		{
			MessageBox(NULL,
				"HALӰ�ނ����޲��쐬�ł��܂���\nREFӰ�ނōĎ��s���܂�",
				"�x��", MB_OK);
			//���޲��쐬(REFӰ��:�`���CPU�A���_������GPU�ōs��).
			if (FAILED(m_pD3d->CreateDevice(
				D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_pDevice9)))
			{
				//���޲��쐬(REFӰ��:�`��ƒ��_������CPU�ōs��).
				if (FAILED(m_pD3d->CreateDevice(
					D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_pDevice9)))
				{
					MessageBox(NULL,
						"Direct3D���޲��̍쐬���s", "error", MB_OK);
					return E_FAIL;
				}
			}
		}
	}



	return S_OK;
}

//����֐�.
void clsDx9Mesh::Release()
{
	SAFE_RELEASE(m_pMeshForRay);

	//��ԐV������������̂��珇�ɉ��.
	for (int i = m_dwNumMaterials - 1; i >= 0; i--)
	{
		//���ɉ����Ȃ���Ή��.
		SAFE_RELEASE(m_ppIndexBuffer[i]);
	}
	delete[] m_ppIndexBuffer;
	m_ppIndexBuffer = nullptr;

	for (int i = m_dwNumMaterials - 1; i >= 0; i--)
	{
		SAFE_RELEASE(m_pMaterials[i].pTexture);
	}
	delete[] m_pMaterials;
	m_pMaterials = nullptr;

	//��޼ު�Ẳ��.
	SAFE_RELEASE(m_pDevice9);
	SAFE_RELEASE(m_pD3d);
}

//X̧�ق���ү����۰�ނ���.
HRESULT clsDx9Mesh::LoadXMesh(LPSTR fileName)
{
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;//��ر��ޯ̧.


	//���C�ƃ��b�V���̔���̂��߂ɒǉ�.
	if (FAILED(
		D3DXLoadMeshFromXA(
		fileName,			//̧�ٖ�.
		D3DXMESH_SYSTEMMEM,	//������؂ɓǍ�.
		m_pDevice9, NULL,
		&pD3DXMtrlBuffer,	//(out)��رُ��.
		NULL,
		&m_dwNumMaterials,	//(out)��رِ�.
		&m_pMeshForRay)))			//(out)ү����޼ު��.
	{
		MessageBox(NULL, "X̧�ٓǍ����s", "error", MB_OK);
		return E_FAIL;
	}

	//X̧�ق�۰��.
	if (FAILED(
		D3DXLoadMeshFromXA(
		fileName,			//̧�ٖ�.
		D3DXMESH_SYSTEMMEM	//������؂ɓǍ�.
			| D3DXMESH_32BIT,
		m_pDevice9, NULL,
		&pD3DXMtrlBuffer,	//(out)��رُ��.
		NULL,
		&m_dwNumMaterials,	//(out)��رِ�.
		&m_pMesh)))			//(out)ү����޼ު��.
	{
		MessageBox(NULL, "X̧�ٓǍ����s111", "error", MB_OK);
		return E_FAIL;
	}

	//�u���_�ޯ̧�v�u���ޯ���ޯ̧�v�Ŏg���܂킷�ϐ�.
	D3D11_BUFFER_DESC	bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//�ǂݍ��񂾏�񂩂�K�v�ȏ��𔲂��o��.
	D3DXMATERIAL* d3dxMaterials
		= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//��رِ����̗̈���l��.
	m_pMaterials = new MY_MATERIAL[m_dwNumMaterials];
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterials];

	//��رِ����J��Ԃ�.
	for (DWORD i=0; i<m_dwNumMaterials; i++)
	{
		//�C���f�b�N�X�o�b�t�@�̏�����.
		m_ppIndexBuffer[i] = nullptr;

		//��رُ��̺�߰.
		//���޴��.
		m_pMaterials[i].Ambient.x = d3dxMaterials[i].MatD3D.Ambient.r;
		m_pMaterials[i].Ambient.y = d3dxMaterials[i].MatD3D.Ambient.g;
		m_pMaterials[i].Ambient.z = d3dxMaterials[i].MatD3D.Ambient.b;
		m_pMaterials[i].Ambient.w = d3dxMaterials[i].MatD3D.Ambient.a;
		//�ި̭���.
		m_pMaterials[i].Diffuse.x = d3dxMaterials[i].MatD3D.Diffuse.r;
		m_pMaterials[i].Diffuse.y = d3dxMaterials[i].MatD3D.Diffuse.g;
		m_pMaterials[i].Diffuse.z = d3dxMaterials[i].MatD3D.Diffuse.b;
		m_pMaterials[i].Diffuse.w = d3dxMaterials[i].MatD3D.Diffuse.a;
		//��߷��.
		m_pMaterials[i].Specular.x = d3dxMaterials[i].MatD3D.Specular.r;
		m_pMaterials[i].Specular.y = d3dxMaterials[i].MatD3D.Specular.g;
		m_pMaterials[i].Specular.z = d3dxMaterials[i].MatD3D.Specular.b;
		m_pMaterials[i].Specular.w = d3dxMaterials[i].MatD3D.Specular.a;
		
		//(���̖ʂ�)ø������\���Ă��邩�H.
		if (d3dxMaterials[i].pTextureFilename != NULL
			&& lstrlen(d3dxMaterials[i].pTextureFilename)>0)
		{
			m_TextureFlag = true;	//ø����׸ނ𗧂Ă�.

			char path[64];
			int path_count = lstrlen(fileName);
			for(int k=path_count; k>=0; k--){
				if(fileName[k]=='\\'){
					for(int j=0; j<=k; j++){
						path[j]=fileName[j];
					}
					path[k+1]='\0';
					break;
				}
			}
			//�߽��ø���̧�ٖ���A��.
			strcat_s(path,sizeof(path),d3dxMaterials[i].pTextureFilename);

			//ø���̧�ٖ����߰.
			strcpy_s(m_pMaterials[i].szTextureName,
				sizeof(m_pMaterials[i].szTextureName), path);
			
			//ø����̍쐬.
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(
				m_pDevice11,
				m_pMaterials[i].szTextureName,//̧�ٖ�.
				NULL, NULL,
				&m_pMaterials[i].pTexture,	//(out)ø�����޼ު��.
				NULL)))
			{
				MessageBox(NULL, m_pMaterials[i].szTextureName,
					"ø����쐬���s", MB_OK);
				return E_FAIL;
			}
		}
	}
	//-----------------------------------------------
	//	���ޯ���ޯ̧���쐬.
	//-----------------------------------------------
	//ү���̑������𓾂�.
	//�������Ų��ޯ���ޯ̧����ׂ�����رق��Ƃ̲��ޯ���ޯ̧�𕪗��ł���.
	D3DXATTRIBUTERANGE* pAttrTable = NULL;

	//ү���̖ʂ���ђ��_�̏��ԕύX�𐧌䂵�A��̫��ݽ���œK������.
	// D3DXMESHOPT_COMPACT:�ʂ̏��Ԃ�ύX���A�g�p����ĂȂ����_�Ɩʂ��폜����.
	// D3DXMESHOPT_ATTRSORT:��̫��ݽ���グ��ׁA�ʂ̏��Ԃ�ύX���čœK�����s��.
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
		0, 0, 0, 0);
	//����ð��ق̎擾.
	m_pMesh->GetAttributeTable(NULL, &m_NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if (FAILED(m_pMesh->GetAttributeTable(pAttrTable, &m_NumAttr))){
		MessageBox(NULL, "����ð��َ擾���s", "error", MB_OK);
		return E_FAIL;
	}

	// �������Aۯ����Ȃ��Ǝ��o���Ȃ�.
	int* pIndex = NULL;
	m_pMesh->LockIndexBuffer(
		D3DLOCK_READONLY, (void**)&pIndex);
	//�������Ƃ̲��ޯ���ޯ̧���쐬.
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		m_AttrID[i] = pAttrTable[i].AttribId;
		//Dx9�̲��ޯ���ޯ̧����̏��ŁADx11�̲��ޯ���ޯ̧���쐬.
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth
			= sizeof(int) * pAttrTable[i].FaceCount * 3;//�ʐ�*3 �Œ��_��.
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		//�傫�����ޯ���ޯ̧���̵̾��(*3����).
		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart*3];

		if (FAILED(
			m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_ppIndexBuffer[i])))
		{
			MessageBox(NULL, "���ޯ���ޯ̧�쐬���s", "error", MB_OK);
			return E_FAIL;
		}
		//�ʂ̐����߰.
		m_pMaterials[m_AttrID[i]].dwNumFace = pAttrTable[i].FaceCount;
	}
	delete[] pAttrTable;	//����ð��ق̍폜.
	m_pMesh->UnlockIndexBuffer();

	//�s�v�ɂȂ�����ر��ޯ̧�����.
	SAFE_RELEASE(pD3DXMtrlBuffer);

	//-----------------------------------------------
	//	���_(�ްï��)�ޯ̧�̍쐬.
	//-----------------------------------------------
	//Dx9�̏ꍇ�Amap�ł͂Ȃ�ۯ��Œ��_�ޯ̧�����ް������o��.
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = NULL;
	MeshVertex* pVertex = NULL;
	if (SUCCEEDED(
		pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertex = (MeshVertex*)pVertices;
		//Dx9�̒��_�ޯ̧����̏��ŁADx11�̒��_�ޯ̧���쐬.
		bd.Usage = D3D11_USAGE_DEFAULT;
		//���_���i�[����̂ɕK�v���޲Đ�.
		bd.ByteWidth
			= m_pMesh->GetNumBytesPerVertex() * m_pMesh->GetNumVertices();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pVertex;

		//ø������W��ϲŽ�΍�.
		if (m_TextureFlag){
			//��ø���������Ƃ�.
			//���_�����J��Ԃ�.
			for (int i = 0; i < m_pMesh->GetNumVertices(); i++){
				if (pVertex[i].Tex.x < 0.0f){
					pVertex[i].Tex.x += 1.0f;
				}
				if (pVertex[i].Tex.y < 0.0f){
					pVertex[i].Tex.y += 1.0f;
				}
			}
		}

		if (FAILED(
			m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_pVertexBuffer)))
		{
			MessageBox(NULL, "���_�ޯ̧�쐬���s", "error", MB_OK);
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);//���_�ޯ̧���.


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
		MessageBox(NULL,"����׍쐬���s", "error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//HLSĻ�ق�Ǎ�����ނ��쐬����.
//HLSL(High Level Shading Language)
HRESULT clsDx9Mesh::InitShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;

	//HLSL�����ްï������ނ�����ނ��쐬.
	if (m_TextureFlag){
		if (FAILED(
			D3DX11CompileFromFile(
			SHADER_NAME, NULL, NULL, "VS_Main", "vs_5_0",
			0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "����ލ쐬���s(VS_Main)", "error", MB_OK);
			return E_FAIL;
		}
	}
	else{
		if (FAILED(
			D3DX11CompileFromFile(
			SHADER_NAME, NULL, NULL, "VS_NoTex", "vs_5_0",
			0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "����ލ쐬���s(VS_NoTex)", "error", MB_OK);
			return E_FAIL;
		}
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

	UINT numElements = 0;
	//���_���߯�ڲ��Ă��`.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	if (m_TextureFlag){
		D3D11_INPUT_ELEMENT_DESC tmp[]=
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",	0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,	  0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		//���_���߯�ڲ��Ă̔z��v�f�����Z�o.
		numElements = sizeof(tmp) / sizeof(tmp[0]);
		memcpy_s(layout, sizeof(layout),
			tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);
	}
	else{
		D3D11_INPUT_ELEMENT_DESC tmp[]=
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",	0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		//���_���߯�ڲ��Ă̔z��v�f�����Z�o.
		numElements = sizeof(tmp) / sizeof(tmp[0]);
		memcpy_s(layout, sizeof(layout),
			tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);
	}

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
	if (m_TextureFlag){
		if (FAILED(
			D3DX11CompileFromFile(
			SHADER_NAME, NULL, NULL, "PS_Main", "ps_5_0",
			0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "����ލ쐬���s(PS_Main)", "error", MB_OK);
			return E_FAIL;
		}
	}
	else{
		if (FAILED(
			D3DX11CompileFromFile(
			SHADER_NAME, NULL, NULL, "PS_NoTex", "ps_5_0",
			0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "����ލ쐬���s(PS_NoTex)", "error", MB_OK);
			return E_FAIL;
		}
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
	cb.ByteWidth = sizeof(MESHSHADER_CBUFFER0);//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//�������݂ű���.
	cb.MiscFlags = 0;	//���̑����׸�(���g�p)
	cb.StructureByteStride = 0;//�\���̂̃T�C�Y(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;//�g�p���@:���ڏ�������.

	//�ݽ����ޯ̧�쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(&cb,NULL,&m_pConstantBuffer0)))
	{
		MessageBox(NULL,
			"�ݽ����ޯ̧�쐬���s", "error", MB_OK);
		return E_FAIL;
	}

	//��رٗp �ݽ����ޯ̧.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�ݽ����ޯ̧���w��.
	cb.ByteWidth = sizeof(MESHSHADER_CBUFFER1);//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//�������݂ű���.
	cb.MiscFlags = 0;	//���̑����׸�(���g�p)
	cb.StructureByteStride = 0;//�\���̂̃T�C�Y(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;//�g�p���@:���ڏ�������.

	//�ݽ����ޯ̧�쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(&cb,NULL,&m_pConstantBuffer1)))
	{
		MessageBox(NULL,
			"�ݽ����ޯ̧�쐬���s", "error", MB_OK);
		return E_FAIL;
	}


	return S_OK;
}

//Dx���������ݸފ֐�(�ŏI�I�ɉ�ʂɏo���̂�Main��).
void clsDx9Mesh::Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye)
{
	D3DXMATRIX mWorld;	//ܰ��ލs��.
	D3DXMATRIX mScale, mYaw, mPitch, mRoll, mTrans;

	//ܰ��ޕϊ�(�\���ʒu���w�肷��).
	//�g��k��.
	D3DXMatrixScaling(&mScale, m_Scale, m_Scale, m_Scale);
	//��].
	D3DXMatrixRotationY(&mYaw, m_Yaw);		//Y����].
	D3DXMatrixRotationX(&mPitch, m_Pitch);	//X����].
	D3DXMatrixRotationZ(&mRoll, m_Roll);	//Z����].
	
	//���x�N�g����p��.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 0.7f);	//�ϊ��O�x�N�g��.
	D3DXVECTOR3 vecAxisX(0.7f, 0.0f, 0.0f);	//�ϊ��O�x�N�g��.

	//Z���x�N�g�����̂��̂���]��Ԃɂ��ω�����.
	D3DXVec3TransformCoord(&vecAxisZ,//�o��.
		&vecAxisZ, &mYaw);

	//10_09�����Ⴄ�Ƃ���ɓ��ꂽ�ق����ǂ�
	//m_eDir = enDirection_Forward;

	//�����ɂ���Đi�s������l��ݒ�.
	switch (m_eDir) {
	case enDirection_Forward://�O�i.
		//���������p�~�i�߂�l(0.1f).
		m_vPos += vecAxisZ * 0.1f;
		break;
	case enDirection_Backward://���.
		m_vPos -= vecAxisZ * 0.1f;
		break;
	}
	m_eDir = enDirection_Stop;//��~.

	//���s�ړ�.
	D3DXMatrixTranslation(&mTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	//�����s��(�g�k�~��]�~�ړ�).
	mWorld = mScale * mYaw * mPitch * mRoll * mTrans;

	//�g�p���鼪��ނ̾��.
	m_pContext11->VSSetShader(m_pVertexShader, NULL, 0);	//���_�����.
	m_pContext11->PSSetShader(m_pPixelShader, NULL, 0);	//�߸�ټ����.

	//����ނ̺ݽ����ޯ̧�Ɋe���ް���n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	//�ޯ̧�����ް��̏��������J�n����Map.
	if (SUCCEEDED(m_pContext11->Map(m_pConstantBuffer0, 0,
		D3D11_MAP_WRITE_DISCARD,0, &pData)))
	{
		//�ݽ����ޯ̧.
		MESHSHADER_CBUFFER0 cb;

		//ܰ��ލs���n��.
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//ܰ���,�ޭ�,��ۼު���ݍs���n��.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose(&m, &m);//�s���]�u����.
		//���s��̌v�Z���@��DirectX��GPU�ňقȂ邽��.
		cb.mWVP = m;

		//ײĕ�����n��.
		cb.vLightDir= D3DXVECTOR4(vLight.x, vLight.y, vLight.z, 0.0f);

		//��ׂ̈ʒu(���_)��n��.
		cb.vEye= D3DXVECTOR4(vEye.x, vEye.y, vEye.z, 0.0f);

		memcpy_s(
			pData.pData,	//��߰����ޯ̧.
			pData.RowPitch,	//��߰����ޯ̧����.
			(void*)(&cb),	//��߰�����ޯ̧.
			sizeof(cb));	//��߰���黲��.

		//�ޯ̧�����ް��̏��������I������Unmap.
		m_pContext11->Unmap(m_pConstantBuffer0, 0);
	}

	//���̺ݽ����ޯ̧���ǂ̼���ނŎg�����H.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);//���_�����.
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);//�߸�ټ����.

	//���_���߯�ڲ��Ă��.
	m_pContext11->IASetInputLayout(m_pVertexLayout);

	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�ޯ̧���.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//�����̐������A���ꂼ��̑����̲��ޯ���ޯ̧��`��.
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		//�g�p����ĂȂ���رّ΍�.
		if (m_pMaterials[m_AttrID[i]].dwNumFace == 0)
		{
			continue;
		}
		//���ޯ���ޯ̧���.
		m_pContext11->IASetIndexBuffer(m_ppIndexBuffer[i],
			DXGI_FORMAT_R32_UINT, 0);
		//��رق̊e�v�f�𼪰�ނɓn��.
		D3D11_MAPPED_SUBRESOURCE pData1;
		if (SUCCEEDED(m_pContext11->Map(m_pConstantBuffer1, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData1)))
		{
			MESHSHADER_CBUFFER1 cb1;
			//���޴�āA�ި̭��ށA��߷�ׂ𼪰�ނɓn��.
			cb1.vAmbient = m_pMaterials[m_AttrID[i]].Ambient;
			cb1.vDiffuse = m_pMaterials[m_AttrID[i]].Diffuse;
			cb1.vSpecular= m_pMaterials[m_AttrID[i]].Specular;

			memcpy_s(pData1.pData, pData1.RowPitch,
				(void*)&cb1, sizeof(cb1));

			m_pContext11->Unmap(m_pConstantBuffer1, 0);
		}

		//���̺ݽ����ޯ̧���ǂ̼���ނŎg�p���邩�H.
		m_pContext11->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		m_pContext11->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);

		//ø����𼪰�ނɓn��.
		if (m_pMaterials[m_AttrID[i]].pTexture)
		{
			//ø���������Ƃ�.
			m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
			m_pContext11->PSSetShaderResources(0, 1,
				&m_pMaterials[m_AttrID[i]].pTexture);
		}
		else
		{
			//ø������Ȃ��Ƃ�.
			ID3D11ShaderResourceView* Nothig[1] = { 0 };
			m_pContext11->PSSetShaderResources(0, 1, Nothig);
		}
		//����è��(��غ��)�������ݸ�.
		m_pContext11->DrawIndexed(
			m_pMaterials[m_AttrID[i]].dwNumFace * 3, 0, 0);
	}
}

//����(��̧������)�ݒ�̐؂�ւ�.
void clsDx9Mesh::SetBlend(bool bFlag)
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
		MessageBox(NULL, "�����޽ðč쐬���s", "error", MB_OK);
	}

	//�����޽ðĂ̐ݒ�.
	UINT mask = 0xffffffff;//Ͻ��l.
	m_pContext11->OMSetBlendState(
		m_pBlendState, NULL, mask);
}
