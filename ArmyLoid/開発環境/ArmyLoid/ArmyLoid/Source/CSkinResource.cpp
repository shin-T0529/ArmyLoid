#include "CSkinResource.h"

clsSkinResource::clsSkinResource()
	: m_hWnd(nullptr)
	, m_pDevice11(nullptr)
	, m_pContext11(nullptr)
	, m_ppStaticSkinMesh(nullptr)
{

}


clsSkinResource::~clsSkinResource()
{
	for (int i = enStaticSkinMesh_Max - 1; i >= 0; i--)
	{
		ReleaseStaticSkinMesh(i);
	}

	if (m_ppStaticSkinMesh != nullptr)
	{
		delete[] m_ppStaticSkinMesh;
		m_ppStaticSkinMesh = nullptr;
	}
}

HRESULT clsSkinResource::Init(HWND hWnd, ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	//�X�^�e�B�b�N���b�V���̃|�C�C���^�̃|�C���^�̗̈�m��.
	//�|�C���^�^�̔z��̈�m��.
	m_ppStaticSkinMesh = new clsD3DXSKINMESH*[enStaticSkinMesh_Max];
	for (int i = 0; i < enStaticSkinMesh_Max; i++)
	{
		//������:NULL.
		m_ppStaticSkinMesh[i] = nullptr;
	}
	return S_OK;
}

HRESULT clsSkinResource::CreateStaticSkinMesh(
	int enMesh, LPSTR fileName, CD3DXSKINMESH_INIT si)
{
	//�͈͓����ǂ����`�F�b�N����.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return E_FAIL;
	}
	//���f�������łɑ��݂��Ă��邩.
	if (m_ppStaticSkinMesh[enMesh] != nullptr)
	{
		return E_FAIL;
	}

	//���f���ǂݍ���.
	m_ppStaticSkinMesh[enMesh] = new clsD3DXSKINMESH;
	m_ppStaticSkinMesh[enMesh]->Init(&si);
	m_ppStaticSkinMesh[enMesh]->CreateFromX(fileName);
	return S_OK;
}

HRESULT clsSkinResource::ReleaseStaticSkinMesh(int enMesh)
{
	//�͈͓����`�F�b�N����.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return E_FAIL;
	}
	//���f�������݂��Ȃ��Ȃ���.
	if (m_ppStaticSkinMesh[enMesh] == nullptr)
	{
		return S_OK;
	}

	//�̈�̉��.
	delete m_ppStaticSkinMesh[enMesh];	//�폜.

	m_ppStaticSkinMesh[enMesh] = nullptr;	//������x�g�����Ƃ��l����nullptr.

	return S_OK;
}

bool clsSkinResource::CheckRangeMainStaticMesh(int enMesh)
{
	//�͈͓����`�F�b�N.
	if (enStaticSkinMesh_Min <= enMesh && enMesh < enStaticSkinMesh_Max)
	{
		return true;
	}
	return false;
}

clsD3DXSKINMESH* clsSkinResource::GetStaticSkinMesh(int enMesh)
{
	//�͈͓����`�F�b�N����.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return NULL;
	}
	//���f�������݂��Ȃ��ꍇ.
	if (m_ppStaticSkinMesh[enMesh] == NULL)
	{
		return NULL;
	}

	return m_ppStaticSkinMesh[enMesh];

}
