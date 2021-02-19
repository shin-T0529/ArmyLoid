#ifndef BOARDPOLYGON_H
#define BOARDPOLYGON_H

#include "cCommon.h"
/*************************************************
*	ｽﾌﾟﾗｲﾄｸﾗｽ.
**/
class clsBoardPolygon :public clsCommon
{
public://public 外部からｱｸｾｽ可能.

	D3DXVECTOR3		m_vPos;	//位置.

	int				m_AnimCount;//ｶｳﾝﾀ.
	bool			m_DispFlag;	//表示ﾌﾗｸﾞ.

	clsBoardPolygon();	//ｺﾝｽﾄﾗｸﾀ.
	~clsBoardPolygon();	//ﾃﾞｽﾄﾗｸﾀ.

	//初期化.
	HRESULT Init(ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPSTR fileName);

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
	//ﾓﾃﾞﾙ作成.
	HRESULT InitModel(LPSTR fileName);

	//描画(ﾚﾝﾀﾞﾘﾝｸﾞ).
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vEye);


	//-----------------------------------------------
	//	構造体.
	//-----------------------------------------------
	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Simple.hlsl).
	//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
	struct SPRITESHADER_CBUFFER
	{
		D3DXMATRIX	mWVP;	//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,射影の合成変換行列.
		D3DXVECTOR4	vColor;	//ｶﾗｰ(RGBAの型に合わせる).
		D3DXVECTOR4	vUV;	//UV座標.
	};
	//頂点の構造体.
	struct SpriteVertex
	{
		D3DXVECTOR3 vPos;	//位置.
		D3DXVECTOR2 vTex;	//ﾃｸｽﾁｬ座標(UV座標).
	};

private:

	//↓ﾓﾃﾞﾙ種類ごとに用意.
	ID3D11VertexShader* m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*	m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*	m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*		m_pConstantBuffer;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*		m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.

	ID3D11SamplerState*	m_pSampleLinear;	//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ.
	// ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.
	ID3D11ShaderResourceView* m_pTexture;	//ﾃｸｽﾁｬ.

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ.
	ID3D11BlendState*	m_pBlendState;
};

#endif//#ifndef SPRITE_H