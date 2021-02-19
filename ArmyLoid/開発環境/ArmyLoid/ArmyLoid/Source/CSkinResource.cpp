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

	//スタティックメッシュのポイインタのポインタの領域確保.
	//ポインタ型の配列領域確保.
	m_ppStaticSkinMesh = new clsD3DXSKINMESH*[enStaticSkinMesh_Max];
	for (int i = 0; i < enStaticSkinMesh_Max; i++)
	{
		//初期化:NULL.
		m_ppStaticSkinMesh[i] = nullptr;
	}
	return S_OK;
}

HRESULT clsSkinResource::CreateStaticSkinMesh(
	int enMesh, LPSTR fileName, CD3DXSKINMESH_INIT si)
{
	//範囲内かどうかチェックする.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return E_FAIL;
	}
	//モデルがすでに存在しているか.
	if (m_ppStaticSkinMesh[enMesh] != nullptr)
	{
		return E_FAIL;
	}

	//モデル読み込み.
	m_ppStaticSkinMesh[enMesh] = new clsD3DXSKINMESH;
	m_ppStaticSkinMesh[enMesh]->Init(&si);
	m_ppStaticSkinMesh[enMesh]->CreateFromX(fileName);
	return S_OK;
}

HRESULT clsSkinResource::ReleaseStaticSkinMesh(int enMesh)
{
	//範囲内かチェックする.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return E_FAIL;
	}
	//モデルが存在しないないか.
	if (m_ppStaticSkinMesh[enMesh] == nullptr)
	{
		return S_OK;
	}

	//領域の解放.
	delete m_ppStaticSkinMesh[enMesh];	//削除.

	m_ppStaticSkinMesh[enMesh] = nullptr;	//もう一度使うことを考えてnullptr.

	return S_OK;
}

bool clsSkinResource::CheckRangeMainStaticMesh(int enMesh)
{
	//範囲内かチェック.
	if (enStaticSkinMesh_Min <= enMesh && enMesh < enStaticSkinMesh_Max)
	{
		return true;
	}
	return false;
}

clsD3DXSKINMESH* clsSkinResource::GetStaticSkinMesh(int enMesh)
{
	//範囲内かチェックする.
	if (CheckRangeMainStaticMesh(enMesh) == false)
	{
		return NULL;
	}
	//モデルが存在しない場合.
	if (m_ppStaticSkinMesh[enMesh] == NULL)
	{
		return NULL;
	}

	return m_ppStaticSkinMesh[enMesh];

}
