#include "CEffects.h"
#include "Global.h"

//描画用ｲﾝｽﾀﾝｽ最大数.
const int g_RenderSpriteMax = 2000;
//ｴﾌｪｸﾄ管理用ｲﾝｽﾀﾝｽ最大数.
const int g_EffectInstanceMax = 1000;

//ｺﾝｽﾄﾗｸﾀ.
clsEffects::clsEffects()
	: m_pManager(nullptr)
	, m_pRender(nullptr)
	, m_pEffect(nullptr)
	, m_Handle(0)
{
}

//ﾃﾞｽﾄﾗｸﾀ.
clsEffects::~clsEffects()
{

}


//初期化関数.
HRESULT clsEffects::Init()
{
	//COMの初期化.
	CoInitializeEx(NULL, NULL);

	return S_OK;
}

//構築関数.
HRESULT clsEffects::Create(
	ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	//描画用ｲﾝｽﾀﾝｽの生成.
	m_pRender =
		::EffekseerRendererDX11::Renderer::Create(
		pDevice11, pContext11, g_RenderSpriteMax);

	//ｴﾌｪｸﾄ管理用ｲﾝｽﾀﾝｽの生成.
	m_pManager = ::Effekseer::Manager::Create(g_EffectInstanceMax);

	//描画用ｲﾝｽﾀﾝｽから描画機能を設定.
	m_pManager->SetSpriteRenderer(m_pRender->CreateSpriteRenderer());
	m_pManager->SetRibbonRenderer(m_pRender->CreateRibbonRenderer());
	m_pManager->SetRingRenderer(m_pRender->CreateRingRenderer());
	m_pManager->SetModelRenderer(m_pRender->CreateModelRenderer());

	//描画用ｲﾝｽﾀﾝｽからﾃｸｽﾁｬの読込機能を設定.
	//独自拡張可能、現在はﾌｧｲﾙから読み込んでいる.
	m_pManager->SetTextureLoader(m_pRender->CreateTextureLoader());
	m_pManager->SetModelLoader(m_pRender->CreateModelLoader());
	return S_OK;
}


//破棄関数.
HRESULT clsEffects::Destroy()
{
	//ｴﾌｪｸﾄﾃﾞｰﾀの解放.

	//先にｴﾌｪｸﾄ管理用ｲﾝｽﾀﾝｽを破棄.
	m_pManager->Destroy();
	//次に描画用ｲﾝｽﾀﾝｽを破棄.
	m_pRender->Destroy();
	//COMの終了処理.
	CoUninitialize();

	return S_OK;
}

//ﾃﾞｰﾀ読込関数.
HRESULT clsEffects::LoadData(const char* cFilepass)
{
	const char* const end = cFilepass + sizeof(cFilepass);

	char16_t c16s[32];
	mbstate_t mbstate = { 0 };
	int mbi = 0;
	int c16i = 0;

	for (;; ) {
		const int result = (int)mbrtoc16(&c16s[c16i], &cFilepass[mbi], end - &cFilepass[mbi], &mbstate);
		if (result > 0) {
			c16i++;
			mbi += result;
		}
		else if (result == 0) {
			// ヌル文字に到達
			break;
		}
		else if (result == -1 || result == -2) {
			c16s[c16i] = u'/';
			c16i++;
			mbi += 1;
//			fputs("無効なバイト列です。\n", stderr);
		}
		else if (result == -3) {
			// char16_t の文字を構成する残りの部分を得た。
			// マルチバイト文字側のバイトは消費していない。
			c16i++;
		}
	}

	//ｴﾌｪｸﾄの読み込み.
	m_pEffect = Effekseer::Effect::Create(m_pManager, c16s);
	if (m_pEffect == nullptr) {
		_ASSERT_EXPR(false, L"ファイル読み込み失敗");
		return E_FAIL;
	}

	return S_OK;
}

//ﾃﾞｰﾀ解放関数.
HRESULT clsEffects::ReleaseData()
{
	//ｴﾌｪｸﾄの解放.
	ES_SAFE_RELEASE(m_pEffect);

	return S_OK;
}

//描画.
void clsEffects::Render(
	const D3DXMATRIX& mView,
	const D3DXMATRIX& mProj,
	const D3DXVECTOR3& vCamPos)
{
	//ﾋﾞｭｰ行列を設定.
	SetViewMatrix(mView);

	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列を設定.
	SetProjectionMatrix(mProj);

	//ｴﾌｪｸﾄの更新処理.
	m_pManager->Update();

	//------------------------------------------------
	//	Effekseerﾚﾝﾀﾞﾘﾝｸﾞ.
	//------------------------------------------------
	//ｴﾌｪｸﾄの描画開始処理を行う.
	m_pRender->BeginRendering();

	//ｴﾌｪｸﾄの描画を行う.
	m_pManager->Draw();

	//ｴﾌｪｸﾄの描画終了を行う.
	m_pRender->EndRendering();
}

//ﾋﾞｭｰ行列を設定.
void clsEffects::SetViewMatrix(const D3DXMATRIX& mView)
{
	Effekseer::Matrix44 tmpEsCamMat;
	tmpEsCamMat = ConvertToMatrixEfk(mView);

	//ｶﾒﾗ行列を設定.
	m_pRender->SetCameraMatrix(tmpEsCamMat);
}
//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列を設定.
void clsEffects::SetProjectionMatrix(const D3DXMATRIX& mProj)
{
	Effekseer::Matrix44 tmpEsPrjMat;
	tmpEsPrjMat = ConvertToMatrixEfk(mProj);

	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列を設定.
	m_pRender->SetProjectionMatrix(tmpEsPrjMat);
}

//---------------------------------------------------------------------
//	DirectX Vector3 → Effekseer Vector3 に変換する.
//---------------------------------------------------------------------
Effekseer::Vector3D clsEffects::ConvertToVector3Efk(const D3DXVECTOR3& SrcVec3Dx)
{
	return Effekseer::Vector3D(SrcVec3Dx.x, SrcVec3Dx.y, SrcVec3Dx.z);
}
//---------------------------------------------------------------------
//	Effekseer Vector3 → DirectX Vector3 に変換する.
//---------------------------------------------------------------------
D3DXVECTOR3 clsEffects::ConvertToVector3Dx(const Effekseer::Vector3D& SrcVec3Efk)
{
	return D3DXVECTOR3(SrcVec3Efk.X, SrcVec3Efk.Y, SrcVec3Efk.Z);
}
//---------------------------------------------------------------------
//	行列変換関数.
//---------------------------------------------------------------------
Effekseer::Matrix44 clsEffects::ConvertToMatrixEfk(const D3DXMATRIX& SrcMatDx)
{
	Effekseer::Matrix44 OutMatEfk;
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			OutMatEfk.Values[i][j] = SrcMatDx.m[i][j];
		}
	}
	return OutMatEfk;
}
//---------------------------------------------------------------------
//	行列変換関数.
//---------------------------------------------------------------------
D3DXMATRIX clsEffects::ConvertToMatrixDx(const Effekseer::Matrix44& SrcMatEfk)
{
	D3DXMATRIX OutMatDx;
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			OutMatDx.m[i][j] = SrcMatEfk.Values[i][j];
		}
	}
	return OutMatDx;
}