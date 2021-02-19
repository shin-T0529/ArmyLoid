#ifndef UI_D_H
#define UI_D_H

#include "cCommon.h"

#define ALIGN16 _declspec(align(16))

//-----------------------------------------------
//	構造体.
//-----------------------------------------------
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Simple.hlsl).
//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
struct SPRITESHADER_CBUFFER
{
	ALIGN16 D3DMATRIX	mW;					//ワールド行列.
	ALIGN16 float		fViewPortWidth;		//ビューポート幅.
	ALIGN16 float		fViewPortHeight;	//ビューポート高さ
	ALIGN16 float		fAlpha;				//アルファ値(透過で使用する).
	ALIGN16 D3DXVECTOR2	vUV;				//UV座標.

};
//頂点の構造体.
struct SpriteVertex
{
	D3DXVECTOR3 vPos;	//位置.
	D3DXVECTOR2 vTex;	//ﾃｸｽﾁｬ座標(UV座標).
};

//幅、高さ構造体(float型).
struct WHSIZE_FLOAT
{
	float w;
	float h;
};

struct SPRITE_STATE
{
	float			Width;		//幅.
	float			Height;		//高さ.
	float			U;			//U座標.
	float			V;			//V座標.
	WHSIZE_FLOAT	Disp;		//表示幅高さ.
	WHSIZE_FLOAT	Base;		//元画像の幅高さ.
	WHSIZE_FLOAT	Stride;		//１ｺﾏ当たりの幅高さ.

};
class cls2DTex :public clsCommon
{
public:

	D3DXVECTOR3		m_vPos;	

	int				m_AnimCount;	//カウンタ
	bool			m_DispFlag;		//表示フラグ

	float			m_Alpha = 1.0f;		//ｱﾙﾌｧ値(最大値は1.0).
	POINTFLOAT		m_PatternNo;//ﾏｽ目番号.

	cls2DTex();
	~cls2DTex();

	HRESULT Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11,
				const char* fileName,const SPRITE_STATE& ss);
	HRESULT InitShader();
	HRESULT InitTex(float Width,float Height,float U,float V);
	//テクスチャ作成.
	HRESULT	CreateTexture(const char* fileName, ID3D11ShaderResourceView** pTexture);

	void Render(SPRITE_STATE &ss, POINTFLOAT &PatNo);


	void AttachTex(cls2DTex* pTex);

	void DetatchTex();

	void TexRender(SPRITE_STATE &ss,POINTFLOAT &PatNo);


private:

	ID3D11VertexShader* m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*	m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*	m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*		m_pConstantBuffer;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	ID3D11Buffer*		m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.

	ID3D11SamplerState*	m_pSampleLinear;	//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ.
	// ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.
	ID3D11ShaderResourceView* m_pTexture;	//ﾃｸｽﾁｬ.

	cls2DTex*			m_pStaticTex;
};
#endif