#ifndef DEBUG_TEXT_H
#define DEBUG_TEXT_H
//警告についてのｺｰﾄﾞ分析を無効にする. 4005:再定義.
#pragma warning( disable : 4005 )
#pragma warning(disable:4018)

#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//「D3DX～」の定義使用時に必要.

#pragma warning(disable:4005)

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )


//ﾃﾞﾊﾞｯｸﾞﾃｷｽﾄｸﾗｽ.
class clsDebugText
{
public:
	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ構造体.
	struct TEXT_CBUFFER
	{
		D3DXMATRIX	mWVP;
		D3DXVECTOR4	vColor;
		D3DXVECTOR4	Alpha;	//x値しか使わない.
	};
	//ﾃｷｽﾄの位置情報.
	struct TextVertex
	{
		D3DXVECTOR3	vPos;	//位置.
		D3DXVECTOR2	vTex;	//ﾃｸｽﾁｬ座標.
	};


	clsDebugText();		//ｺﾝｽﾄﾗｸﾀ.
	~clsDebugText();	//ﾃﾞｽﾄﾗｸﾀ.

	HRESULT Init(ID3D11DeviceContext* pContext,
		DWORD Width, DWORD Height,
		float Size, D3DXVECTOR4 vColor);

	//透過(ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ)設定の切り替え.
	void SetBlend(bool bFlag);

	//ﾚﾝﾀﾞﾘﾝｸﾞ関数.
	void Render(char* text, float x, float y);
	//ﾌｫﾝﾄﾚﾝﾀﾞﾘﾝｸﾞ関数.
	void RenderFont(int FontIndex, float x, float y);

private:
	ID3D11Device*	m_pDevice11;//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext* m_pContext11;//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.

	ID3D11VertexShader* m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*	m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*	m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*		m_pConstantBuffer;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	ID3D11Buffer*		m_pVertexBuffer[100];	//頂点ﾊﾞｯﾌｧ(100個分).

	ID3D11SamplerState*	m_pSampleLinear;		//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ.
	ID3D11ShaderResourceView* m_pAsciiTexture;	//ﾃｸｽﾁｬ.

	ID3D11BlendState*	m_pBlendState;	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ.

	DWORD	m_WindowWidth;	//ｳｨﾝﾄﾞｳ幅.
	DWORD	m_WindowHeight;	//ｳｨﾝﾄﾞｳ高さ.

	float	m_Kerning[100];	//ｶｰﾆﾝｸﾞ(100個分).
	float	m_Scale;		//拡縮値(25pixelを基準 25pixel=1.0).
	float	m_Alpha;		//透過値.
	D3DXVECTOR4	m_vColor;	//色.

	D3DXMATRIX m_mView;	//ﾋﾞｭｰ行列.
	D3DXMATRIX m_mProj;	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列.
};


#endif//#ifndef DEBUG_TEXT_H