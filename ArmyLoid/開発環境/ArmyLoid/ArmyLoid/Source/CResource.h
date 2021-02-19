#ifndef C_RESOURCE_H
#define C_RESOURCE_H

#include "DX9Mesh.h"

#include "C_LoadData.h"
enum enStaticMesh
{
	//外部データ対応にあたっての注意点.
	//①現在の機構では宣言したもの＝入力したファイルパス　で対応している.
	//このため宣言した順番と読み込む行数を間違えるとモデルが変わるようになっている.
	//②csv内で指定したファイルパスには末尾に,をつけないこと.
	//③入力方法は今までと同じ.

	enStaticMesh_Ground,		//地面(csv1行目).

	enStaticMesh_Shot,			//弾.

	enStaticMesh_StageRing,		//移動可能範囲(判定用モデルと併用).
	enStaticMesh_StageLShelter,	//遮蔽物.


	enStaticMesh_P_Coll,		//プレイヤー自身の判定用モデル.
	enStaticMesh_P_AttackColl,	//プレイヤーの近接攻撃範囲判定用モデル.

	enStaticMesh_P_WallColl,	//移動可能範囲の判定確認用.

	enStaticMesh_P_GuradColl,	//ガード範囲の判定用.

	enStaticMesh_P_Search,


	enStaticMesh_Back,

	enStaticMesh_Sora,

	enStaticMesh_Max,			//自動で最大値
	enStaticMesh_Min = 0,		//初期値のため 0 固定

};

class clsResource
{
public:
	clsResource();
	~clsResource();

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11,ID3D11DeviceContext* pContext11);

	HRESULT CreateStaticMesh(int enMesh, LPSTR fileName);

	HRESULT ReleaseStaticMesh(int enMesh);

	clsDx9Mesh*	GetStaticMesh(int enMesh);

	HRESULT LoadStaticMeshAll();

private:
	int imodel;

private:
	HWND					m_hWnd;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	clsLoadData*			LoadData;
	clsDx9Mesh**			m_ppStaticMesh;

	//範囲内か確認する関数.
	bool CheckRangeMainStaticMesh(int enMesh);

};



#endif