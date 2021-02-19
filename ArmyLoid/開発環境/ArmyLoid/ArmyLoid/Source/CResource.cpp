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

	//スタティックメッシュのポイインタのポインタの領域確保.
	//ポインタ型の配列領域確保.
	m_ppStaticMesh = new clsDx9Mesh*[enStaticMesh_Max];
	for (int i = 0; i < enStaticMesh_Max; i++)
	{
		//初期化:NULL.
		m_ppStaticMesh[i] = nullptr;
	}
	return S_OK;
}

HRESULT clsResource::CreateStaticMesh(int enMesh, LPSTR fileName)
{
	//範囲内かどうかチェックする.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return E_FAIL;
	}
	//モデルがすでに存在しているか.
	if (m_ppStaticMesh[enMesh] != nullptr)
	{
		return E_FAIL;
	}

	//モデル読み込み.
	m_ppStaticMesh[enMesh] = new clsDx9Mesh;
	m_ppStaticMesh[enMesh]->Init(m_hWnd, m_pDevice11, m_pContext11, fileName);

	return S_OK;
}

HRESULT clsResource::ReleaseStaticMesh(int enMesh)
{
	//範囲内かチェックする.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return E_FAIL;
	}
	//モデルが存在しないないか.
	if (m_ppStaticMesh[enMesh] == nullptr)
	{
		return S_OK;
	}

	//領域の解放.
	delete m_ppStaticMesh[enMesh];	//削除.

	m_ppStaticMesh[enMesh] = nullptr;	//もう一度使うことを考えてnullptr.

	return S_OK;
}

bool clsResource::CheckRangeMainStaticMesh(int enMesh)
{
	//範囲内かチェック.
	if (enStaticMesh_Min <= enMesh && enMesh < enStaticMesh_Max)
	{
		return true;
	}
	return false;
}

clsDx9Mesh*	 clsResource::GetStaticMesh(int enMesh)
{
	//範囲内かチェックする.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return NULL;
	}
	//モデルが存在しない場合.
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
	//順番にXファイルを読込.
	CreateStaticMesh(imodel, temp);
	imodel++;
	delete temp;
	return S_OK;
}
