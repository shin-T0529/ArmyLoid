#ifndef CRAY_H
#define CRAY_H

#include "CCommon.h"	//共通クラス

//レイクラス
class clsRay : public clsCommon
{
public:
	//-----------------------------------------------
	//	構造体.
	//-----------------------------------------------
	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Simple.hlsl).
	//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
	struct RAYSHADER_CBUFFER
	{
		D3DXMATRIX	mWVP;	//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,射影の合成変換行列.
		D3DXVECTOR4	vColor;	//ｶﾗｰ(RGBAの型に合わせる).
	};
	//頂点構造体
	struct RAY_VERTEX
	{
		D3DXVECTOR3 vPos;	//位置
	};
	//レイ構造体
	struct RAY
	{
		D3DXVECTOR3 vPoint[2];	//始点終点
		D3DXVECTOR3 vDirection;	//方向
		D3DXVECTOR3 vPos;		//位置
		FLOAT		Yaw;		//Y軸回転値
		RAY()
		{ ZeroMemory(this, sizeof(RAY)); }
	};

	clsRay();	//コンストラクタ
	~clsRay();	//デストラクタ

	//初期化
	HRESULT Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	//シェーダー初期化
	HRESULT InitShader();
	//レイ初期化
	HRESULT InitModel();
	//描画
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj);
	//解放関数
	HRESULT Release();
	//レイ構造体
	RAY					m_Ray;

private:
	//↓ﾓﾃﾞﾙ種類ごとに用意.
	ID3D11VertexShader* m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*	m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*	m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*		m_pConstantBuffer;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*		m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.




};


#endif //#ifndef CRAY_H