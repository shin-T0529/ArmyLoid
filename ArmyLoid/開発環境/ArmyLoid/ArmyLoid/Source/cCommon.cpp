#include "cCommon.h"

//透過(アルファブレンド)設定の切り替え
void clsCommon::SetBlend(bool Flag)
{
	//アルファブレンド用ブレンドステート構造体
	//pngファイル内にアルファ情報があるので透過するようにブレンドステートで設定する
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));//初期化

	blendDesc.IndependentBlendEnable = false;
	//false:RenderTarget[0]のメンバーのみ使用できる
	//true :RenderTarget[1]~[7]が使用できる
	//(レンダーターゲット毎に独立したブレンド処理)
	blendDesc.AlphaToCoverageEnable = false;
	//true  : アルファトゥカバレッジを使用する
	blendDesc.RenderTarget[0].BlendEnable = Flag;
	//true  : アルファブレンドを使用する

	//元素材に対する設定
	blendDesc.RenderTarget[0].SrcBlend
		= D3D11_BLEND_SRC_ALPHA;	//アルファブレンド指定

	//重ねる素材に対する設定
	blendDesc.RenderTarget[0].DestBlend
		= D3D11_BLEND_INV_SRC_ALPHA;	//アルファブレンド指定

	//ブレンドオプション
	blendDesc.RenderTarget[0].BlendOp
		= D3D11_BLEND_OP_ADD;	//ADD:加算合成

	//元素材のアルファに対する設定
	blendDesc.RenderTarget[0].SrcBlendAlpha
		= D3D11_BLEND_ONE;	//そのまま使用

	//重ねる素材のアルファに対する設定
	blendDesc.RenderTarget[0].DestBlendAlpha
		= D3D11_BLEND_ZERO;	//何もしない

	//アルファのブレンドオプション
	blendDesc.RenderTarget[0].BlendOpAlpha
		= D3D11_BLEND_OP_ADD;	//ADD:加算合成

	//ピクセルごとの書き込みマスク
	blendDesc.RenderTarget[0].RenderTargetWriteMask
		= D3D11_COLOR_WRITE_ENABLE_ALL;	//すべての成分(RGBA)へのデータの格納を許可


	//ブレンドステートの作成
	if (FAILED(	m_pDevice11->CreateBlendState(&blendDesc, &m_pBlendState)))
	{
		MessageBox(NULL, "ブレンドステート作成失敗", "error", MB_OK);
	}

	//ブレンドステートの設定
	UINT mask = 0xffffffff;			//マスク値
	m_pContext11->OMSetBlendState(m_pBlendState, NULL, mask);
}
