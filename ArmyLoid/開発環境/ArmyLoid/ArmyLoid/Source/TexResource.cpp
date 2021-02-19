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

	//ポインタ型の配列領域確保.
	m_ppStaticTex = new cls2DTex*[enStaticTex_Max];
	for (int i = 0; i < enStaticTex_Max; i++)
	{
		//初期化:NULL.
		m_ppStaticTex[i] = nullptr;
	}
	return S_OK;
}

HRESULT cls2DTexResource::CreateStaticTex(int enTex, const char* fileName,SPRITE_STATE &ss)
{
	char tmp[255];
	memcpy(tmp, fileName, sizeof(char)*255);
	//範囲内かどうかチェックする.
	if (CheckRangeMainStaticTex(enTex) == false)
	{
		return E_FAIL;
	}
	//テクスチャがすでに存在しているか.
	if (m_ppStaticTex[enTex] != nullptr)
	{
		return E_FAIL;
	}
	//モデル読み込み.
	m_ppStaticTex[enTex] = new cls2DTex;
	m_ppStaticTex[enTex]->Init( m_pDevice11, m_pContext11, tmp, ss);
	return S_OK;
}

HRESULT cls2DTexResource::ReleaseStaticTex(int enTex)
{
	//範囲内かチェックする.
	if (CheckRangeMainStaticTex(enTex) == false)
	{
		return E_FAIL;
	}
	//テクスチャが存在しないないか.
	if (m_ppStaticTex[enTex] == nullptr)
	{
		return S_OK;
	}

	//領域の解放.
	delete m_ppStaticTex[enTex];	//削除.

	m_ppStaticTex[enTex] = nullptr;	//もう一度使うことを考えてnullptr.

	return S_OK;
}

cls2DTex* cls2DTexResource::GetStaticTex(int enTex)
{
	//範囲内かチェックする.
	if (CheckRangeMainStaticTex(enTex) == false)
	{
		return NULL;
	}
	//テクスチャが存在しない場合.s
	if (m_ppStaticTex[enTex] == NULL)
	{
		return NULL;
	}

	return m_ppStaticTex[enTex];
}

bool cls2DTexResource::CheckRangeMainStaticTex(int enMesh)
{
	//範囲内かチェック.
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

	//ファイルパスの記録(定数指定).
	if (itex == TitleNo  || itex == ManualNo || itex == ResultNo
	 || itex == CreditNo || itex == MainNo)
	{//読み込む前に各シーンの一枚絵開始の時に読む場所を変更する.
		Filei++;
		Linei = 0;		//読込を始める行は0(1)行目から.
	}
	memcpy(ChackingFile,m_Load->LoadFilePass(S_TexPass, 1 + Filei), sizeof(char) * 255);

	//入れ先・入れるもの・大きさ.
	memcpy(temp, m_Load->LoadFilePass(ChackingFile, 1 + Linei), sizeof(char)*255);

	//テクスチャの作成.
	CreateStaticTex(itex, temp, ss);
	itex++;					//テクスチャのリスト;
	Linei++;				//現在読み込むリストの行ずらし.
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
