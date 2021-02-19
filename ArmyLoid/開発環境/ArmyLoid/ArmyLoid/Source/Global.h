#ifndef GLOBAL_H	//ｲﾝｸﾙｰﾄﾞｶﾞｰﾄﾞ.
#define GLOBAL_H

//警告についてのｺｰﾄﾞ分析を無効にする. 4005:再定義.
#pragma warning(disable:4005)
//#pragma warning(disable:3206)

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3D10.h>

//ﾗｲﾌﾞﾗﾘ.
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )


//定数(どのソースにも使う可能性のあるもののみ).
#define WND_W	1280	//ｳｨﾝﾄﾞｳ幅.
#define WND_H	720		//ｳｨﾝﾄﾞｳ高さ.
#define WND_W_X	1300	//ｳｨﾝﾄﾞｳ幅.
const int  ZERO	= 0;
const float ChangeSpeed = 0.5f;
const float ChangeManualSpeed = 0.3f;
const int PlayerMAX = 2;		//プレイヤー人数.
const int BulletMax = 4;
//武装ダメージ.
const int BoostDamageG = 5;
const int BoostDamageM = 15;
const int BoostDamageB = 25;

const int GatlingReload = 50;
const double AnimPlaySpeed = 0.01;

//読込用.
const int	MeshMax = 2;		//メッシュモデルの数.
const int	SkinMax = 2;		//メッシュモデルの数.

const float	WRepulsion = 1.4f;	//壁の判定の反発距離.

//読み込み時のサイズ設定.
const D3DXVECTOR3 SkinSize = D3DXVECTOR3(0.06f, 0.06f, 0.06f);
const D3DXVECTOR3 AnotherSkinSize = D3DXVECTOR3(0.04f, 0.04f, 0.04f);

//リザルト画面共通で使う座標.
const D3DXVECTOR3 LeftCursor(200.0f, 568.0f, 0.0f);
const D3DXVECTOR3 RightCursor(810.0f, 568.0f, 0.0f);

//std::を省く.
using namespace std;

enum class SceneIndex
{
	Nothing = -1,
	TITLE = 0,
	MANUAL = 1,
	PLAY = 2,
	P1WIN = 3,
	P2WIN = 4,
	DRAW = 5,
	CREDIT = 6,

	Start = TITLE,
	End = CREDIT,
};


#endif//#ifndef GLOBAL_H
