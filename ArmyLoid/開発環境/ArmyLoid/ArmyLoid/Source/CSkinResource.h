#ifndef C_SKINRESOURCE_H
#define C_SKINRESOURCE_H

#include "ED3DXSKINMESH.h"

enum enStaticSkinMesh
{
	enStaticSkinMesh_P1Manual,
	enStaticSkinMesh_P2Manual,

	enStaticSkinMesh_White,		//���@.
	enStaticSkinMesh_Black,
	enStaticSkinMesh_ResultWhite,		//���@.
	enStaticSkinMesh_ResultBlack,		//���@.

	enTitle_White,
	enTitle_Black,
	enStaticSkinMesh_Max,			//�����ōő�l
	enStaticSkinMesh_Min = 0,		//�����l�̂��� 0 �Œ�

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
		//�ǂݍ���X�t�@�C�����̃��X�g.
		//enStaticMesh�Ő錾�������ɌĂ�.
		char fileList[enStaticSkinMesh_Max][64] =
		{
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_White.X",
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_Black.X",

			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_White.X",
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_Black.X",

			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_White.X",		//���U���g�\���p.
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_Black.X",		//����.
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_White.X",		//�^�C�g���\���p.
			"Data\\Model\\Player\\Robot\\tm_f22_robo_anim_Black.X",		//����.

		};
		//���ү���׽�ɕK�v�ȏ���n��.
		CD3DXSKINMESH_INIT si;
		si.hWnd = hWnd;
		si.pDevice = pDevice;
		si.pDeviceContext = pContext;

		//���Ԃ�X�t�@�C����Ǎ�.
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

	//�͈͓����m�F����֐�.
	bool CheckRangeMainStaticMesh(int enMesh);

};



#endif