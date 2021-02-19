#ifndef TEXRESOURCE_H
#define TEXRESOURCE_H

#include <vector>
#include "2DTex.h"
#include "C_LoadData.h"
enum enStatic2DTex
{//いずれリスト化し外部データにすること.
	//*************シーン・シーンUI*************//
	enScreenDark,			//暗幕.
	enScreenWhite,			//明幕.

	enSceneTexTitile,		//タイトル
	enSceneUITexMenu,		//ゲーム終了ボタン.
	enSceneUITexCurSor,		//メニュー入る前のボタン.
	enSceneUITexStart,		//連続開始防止用.

	enSceneTexManual,		//操作説明.
	enSceneTexManualRule,	//ルール説明.
	enSceneUITexReady,		//操作説明・確認了承ボタン.
	enSceneUITexOK,			//同上.
	enSceneUIP_1,
	enSceneUIP_2,

	enSceneTex1PWin,		//1P勝ち.
	enSceneTex2PWin,		//2P勝ち.
	enSceneTexDraw,			//引き分け.
	enSceneUIRetry,			//リトライ.
	enSceneUIExit,			//終了.
	enSceneUISelectCursor,	//選択アイコン.
	enSceneUIDecCursor,		//チェックマーク.

	enSceneTexCredit,		//クレジット画面.
	enSceneTexCreditBack,	//クレジット背景.

	//*************メインUI*************//

	//HP.
	enUITexHP1_FINE,
	enUITexHP_CAUTION,		//仕様変更に伴い画像も変更.
	enUITexHP_DAMAGE,
	enUITexHP1_Bar,

	enUITexHP2_FINE,
	enUITexHP2_Bar,

	//顔.
	enUITexNamePrate1P,
	enUITexNamePrate2P,

	//ブーストゲージ.
	enUITex1Boost,				//ブーストゲージ中身.
	enUITex2Boost,				//ブーストゲージ中身.
	enUITex1Gauge,				//ゲージ枠.
	enUITex2Gauge,				//ゲージ枠.

	enUITexGaugeHeat,			//オーバーヒート時.
	enUITexHeat,				//オーバーヒート時ゲージ中身.

	//武装一覧.
	enUIShoot1PMaWeponTex,		//武装一覧ガトリング1P.
	enUIShoot1PMiWeponTex,		//武装一覧ミサイル1P.
	enUIShoot2PMaWeponTex,		//武装一覧ガトリング2P.
	enUIShoot2PMiWeponTex,		//武装一覧ミサイル2P.

	enUIWepon1PBack,
	enUIWepon2PBack,

	enUIWeponReloading,			//リロード表示.

	//制限時間.
	enUITimeMi,					//制限時間  分
	enUITimeSe1,				//制限時間　秒10.
	enUITimese2,				//制限時間　秒1.
	enUITimeBackTex,
	//開始合図テクスチャ.
	enSceneUIStart,
	enSceneUIEnd,
	enSceneUIFinish,
	enSceneUISigunalBack,
	enStaticTex_Max,			//末尾だから自動で最大値.
	enStaticTex_Min = 0,		//初期値のため 0 固定.
};

//座標データ保持.
struct DataList
{
	D3DXVECTOR3 Pos;
};

class cls2DTexResource
{
public:
	cls2DTexResource();
	~cls2DTexResource();

	HRESULT Init(HWND hWnd,ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	//テクスチャ作成.
	HRESULT CreateStaticTex(int enTex, const char* fileName,SPRITE_STATE &ss);
	//テクスチャデータの開放.
	HRESULT ReleaseStaticTex(int enTex);
	//テクスチャデータ取得.
	cls2DTex* GetStaticTex(int enTex);
	//テクスチャデータ読込.
	HRESULT LoadStaticTexAll(SPRITE_STATE &ss);
	//テクスチャの座標データ.
	D3DXVECTOR3	TexPos(int enTex);

public:
	//データ保持用.
	D3DXVECTOR3		m_tPos;
private:

	int itex = 0;			//テクスチャ番号(画像No).
	int Filei = 0;			//ファイルパス読番号.
	int Linei = 0;			//ファイルパス行番号.

	int texno = 0;
	int Posi = 0;			//ポジション読番号.
	int PosLi = 0;			//ポジション読番号.

	bool CheckRangeMainStaticTex(int enTex);

 /*読み込むときの各シーンテクスチャ分けの定数.
 (どこのテクスチャで読込をやめ別のところで開始するか)*/
	int TitleNo = enSceneTexTitile;
	int ManualNo = enSceneTexManual;
	int ResultNo = enSceneTex1PWin;
	int CreditNo = enSceneTexCredit;
	int MainNo = enUITexHP1_FINE;

	//本体ファイルパス.
	const char*		S_TexPass = "Data\\csv\\Texture\\FilePass\\SceneTexPass.csv";
	const char*		S_TexPos = "Data\\csv\\Texture\\TexPosData.csv";
private:	//不変部分.
	HWND					m_hWnd;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;
	clsLoadData*			m_Load;

	cls2DTex**				m_ppStaticTex;

	DataList				TexData;
	SceneIndex*				Index;
};

#endif // !TEXRESOURCE_H