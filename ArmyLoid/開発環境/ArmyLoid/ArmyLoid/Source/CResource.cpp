#include "CResource.h"

clsResource::clsResource()
: m_hWnd(nullptr)
, m_pDevice11(nullptr)
, m_pContext11(nullptr)
, m_ppStaticMesh(nullptr)
, imodel(0)
{

}


clsResource::~clsResource()
{
	for (int i = enStaticMesh_Max - 1; i >= 0; i--)
	{
		ReleaseStaticMesh(i);
	}

	if (m_ppStaticMesh != nullptr)
	{
		delete[] m_ppStaticMesh;
		m_ppStaticMesh = nullptr;
	}
}

HRESULT clsResource::Init(HWND hWnd, ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	//�X�^�e�B�b�N���b�V���̃|�C�C���^�̃|�C���^�̗̈�m��.
	//�|�C���^�^�̔z��̈�m��.
	m_ppStaticMesh = new clsDx9Mesh*[enStaticMesh_Max];
	for (int i = 0; i < enStaticMesh_Max; i++)
	{
		//������:NULL.
		m_ppStaticMesh[i] = nullptr;
	}
	return S_OK;
}

HRESULT clsResource::CreateStaticMesh(int enMesh, LPSTR fileName)
{
	//�͈͓����ǂ����`�F�b�N����.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return E_FAIL;
	}
	//���f�������łɑ��݂��Ă��邩.
	if (m_ppStaticMesh[enMesh] != nullptr)
	{
		return E_FAIL;
	}

	//���f���ǂݍ���.
	m_ppStaticMesh[enMesh] = new clsDx9Mesh;
	m_ppStaticMesh[enMesh]->Init(m_hWnd, m_pDevice11, m_pContext11, fileName);

	return S_OK;
}

HRESULT clsResource::ReleaseStaticMesh(int enMesh)
{
	//�͈͓����`�F�b�N����.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return E_FAIL;
	}
	//���f�������݂��Ȃ��Ȃ���.
	if (m_ppStaticMesh[enMesh] == nullptr)
	{
		return S_OK;
	}

	//�̈�̉��.
	delete m_ppStaticMesh[enMesh];	//�폜.

	m_ppStaticMesh[enMesh] = nullptr;	//������x�g�����Ƃ��l����nullptr.

	return S_OK;
}

bool clsResource::CheckRangeMainStaticMesh(int enMesh)
{
	//�͈͓����`�F�b�N.
	if (enStaticMesh_Min <= enMesh && enMesh < enStaticMesh_Max)
	{
		return true;
	}
	return false;
}

clsDx9Mesh*	 clsResource::GetStaticMesh(int enMesh)
{
	//�͈͓����`�F�b�N����.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return NULL;
	}
	//���f�������݂��Ȃ��ꍇ.
	if (m_ppStaticMesh[enMesh] == NULL)
	{
		return NULL;
	}

	return m_ppStaticMesh[enMesh];

}

HRESULT clsResource::LoadStaticMeshAll()
{
	char* temp = new char[255];
	memcpy(temp, LoadData->LoadFilePass("Data\\csv\\ModelFilePass.csv", 1 + imodel), sizeof(char) * 255);
	//���Ԃ�X�t�@�C����Ǎ�.
	CreateStaticMesh(imodel, temp);
	imodel++;
	delete temp;
	return S_OK;
}
