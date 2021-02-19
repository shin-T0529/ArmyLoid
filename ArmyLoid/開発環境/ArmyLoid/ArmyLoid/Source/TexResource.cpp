#include "TexResource.h"

cls2DTexResource::cls2DTexResource()
	: m_hWnd(nullptr)
	, m_pDevice11(nullptr)
	, m_pContext11(nullptr)
	, m_ppStaticTex(nullptr)
{

}

cls2DTexResource::~cls2DTexResource()
{
	for (int i = enStaticTex_Max - 1; i >= 0; i--)
	{
		ReleaseStaticTex(i);
	}

	if (m_ppStaticTex != nullptr)
	{
		delete[] m_ppStaticTex;
		m_ppStaticTex = nullptr;
	}

}

HRESULT cls2DTexResource::Init(HWND hWnd, ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	//�|�C���^�^�̔z��̈�m��.
	m_ppStaticTex = new cls2DTex*[enStaticTex_Max];
	for (int i = 0; i < enStaticTex_Max; i++)
	{
		//������:NULL.
		m_ppStaticTex[i] = nullptr;
	}
	return S_OK;
}

HRESULT cls2DTexResource::CreateStaticTex(int enTex, const char* fileName,SPRITE_STATE &ss)
{
	char tmp[255];
	memcpy(tmp, fileName, sizeof(char)*255);
	//�͈͓����ǂ����`�F�b�N����.
	if (CheckRangeMainStaticTex(enTex) == false)
	{
		return E_FAIL;
	}
	//�e�N�X�`�������łɑ��݂��Ă��邩.
	if (m_ppStaticTex[enTex] != nullptr)
	{
		return E_FAIL;
	}
	//���f���ǂݍ���.
	m_ppStaticTex[enTex] = new cls2DTex;
	m_ppStaticTex[enTex]->Init( m_pDevice11, m_pContext11, tmp, ss);
	return S_OK;
}

HRESULT cls2DTexResource::ReleaseStaticTex(int enTex)
{
	//�͈͓����`�F�b�N����.
	if (CheckRangeMainStaticTex(enTex) == false)
	{
		return E_FAIL;
	}
	//�e�N�X�`�������݂��Ȃ��Ȃ���.
	if (m_ppStaticTex[enTex] == nullptr)
	{
		return S_OK;
	}

	//�̈�̉��.
	delete m_ppStaticTex[enTex];	//�폜.

	m_ppStaticTex[enTex] = nullptr;	//������x�g�����Ƃ��l����nullptr.

	return S_OK;
}

cls2DTex* cls2DTexResource::GetStaticTex(int enTex)
{
	//�͈͓����`�F�b�N����.
	if (CheckRangeMainStaticTex(enTex) == false)
	{
		return NULL;
	}
	//�e�N�X�`�������݂��Ȃ��ꍇ.s
	if (m_ppStaticTex[enTex] == NULL)
	{
		return NULL;
	}

	return m_ppStaticTex[enTex];
}

bool cls2DTexResource::CheckRangeMainStaticTex(int enMesh)
{
	//�͈͓����`�F�b�N.
	if (enStaticMesh_Min <= enMesh && enMesh < enStaticTex_Max)
	{
		return true;
	}
	return false;
}

HRESULT cls2DTexResource::LoadStaticTexAll(SPRITE_STATE &ss)
{
	char* temp = new char[255];
	char* ChackingFile = new char[255];

	//�t�@�C���p�X�̋L�^(�萔�w��).
	if (itex == TitleNo  || itex == ManualNo || itex == ResultNo
	 || itex == CreditNo || itex == MainNo)
	{//�ǂݍ��ޑO�Ɋe�V�[���̈ꖇ�G�J�n�̎��ɓǂޏꏊ��ύX����.
		Filei++;
		Linei = 0;		//�Ǎ����n�߂�s��0(1)�s�ڂ���.
	}
	memcpy(ChackingFile,m_Load->LoadFilePass(S_TexPass, 1 + Filei), sizeof(char) * 255);

	//�����E�������́E�傫��.
	memcpy(temp, m_Load->LoadFilePass(ChackingFile, 1 + Linei), sizeof(char)*255);

	//�e�N�X�`���̍쐬.
	CreateStaticTex(itex, temp, ss);
	itex++;					//�e�N�X�`���̃��X�g;
	Linei++;				//���ݓǂݍ��ރ��X�g�̍s���炵.
	delete temp;
	delete ChackingFile;
	return S_OK;
}

D3DXVECTOR3 cls2DTexResource::TexPos(int enTex)
{
	TexData.Pos = m_Load->LoadData(S_TexPos, 1 + enTex);
	m_ppStaticTex[enTex]->m_vPos = TexData.Pos;
	return m_ppStaticTex[enTex]->m_vPos;
}
