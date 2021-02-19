#ifndef C_SKINRESOURCE_H
#define C_SKINRESOURCE_H

#include "ED3DXSKINMESH.h"

enum enStaticSkinMesh
{
	enStaticSkinMesh_P1Manual,
	enStaticSkinMesh_P2Manual,

	enStaticSkinMesh_White,		//自機.
	enStaticSkinMesh_Black,
	enStaticSkinMesh_ResultWhite,		//自機.
	enStaticSkinMesh_ResultBlack,		//自機.

	enTitle_White,
	enTitle_Black,
	enStaticSkinMesh_Max,			//自動で最大値
	enStaticSkinMesh_Min = 0,		//初期値のため 0 固定

};

class clsSkinResource
{
public:
	clsSkinResource();
	~clsSkinResource();

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);

	HRESULT CreateStaticSkinMesh(int enMesh, LPSTR fileName, CD3DXSKINMESH_INIT si);

	HRESULT ReleaseStaticSkinMesh(int enMesh);

	clsD3DXSKINMESH* GetStaticSkinMesh(int enMesh);

	HRESULT LoadStaticSkinMeshAll(HWND hWnd, ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext)
	{
		//読み込むXファイル名のリスト.
		//enStaticMeshで宣言した順に呼ぶ.
		char fileList[enStaticSkinMesh_Max][64] =
		{
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_White.X",
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_Black.X",

			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_White.X",
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_Black.X",

			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_White.X",		//リザルト表示用.
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_Black.X",		//同上.
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_White.X",		//タイトル表示用.
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_Black.X",		//同上.

		};
		//ｽｷﾝﾒｯｼｭｸﾗｽに必要な情報を渡す.
		CD3DXSKINMESH_INIT si;
		si.hWnd = hWnd;
		si.pDevice = pDevice;
		si.pDeviceContext = pContext;

		//順番にXファイルを読込.
		for (int ix = 0; ix < enStaticSkinMesh_Max; ix++)
		{
			CreateStaticSkinMesh(ix, fileList[ix],si);
		}

		return S_OK;
	}


private:
	HWND					m_hWnd;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	clsD3DXSKINMESH**		m_ppStaticSkinMesh;

	//範囲内か確認する関数.
	bool CheckRangeMainStaticMesh(int enMesh);

};



#endif