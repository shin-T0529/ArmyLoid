#ifndef C_RESOURCE_H
#define C_RESOURCE_H

#include "DX9Mesh.h"

#include "C_LoadData.h"
enum enStaticMesh
{
	//�O���f�[�^�Ή��ɂ������Ă̒��ӓ_.
	//�@���݂̋@�\�ł͐錾�������́����͂����t�@�C���p�X�@�őΉ����Ă���.
	//���̂��ߐ錾�������ԂƓǂݍ��ލs�����ԈႦ��ƃ��f�����ς��悤�ɂȂ��Ă���.
	//�Acsv���Ŏw�肵���t�@�C���p�X�ɂ͖�����,�����Ȃ�����.
	//�B���͕��@�͍��܂łƓ���.

	enStaticMesh_Ground,		//�n��(csv1�s��).

	enStaticMesh_Shot,			//�e.

	enStaticMesh_StageRing,		//�ړ��\�͈�(����p���f���ƕ��p).
	enStaticMesh_StageLShelter,	//�Օ���.


	enStaticMesh_P_Coll,		//�v���C���[���g�̔���p���f��.
	enStaticMesh_P_AttackColl,	//�v���C���[�̋ߐڍU���͈͔���p���f��.

	enStaticMesh_P_WallColl,	//�ړ��\�͈͂̔���m�F�p.

	enStaticMesh_P_GuradColl,	//�K�[�h�͈͂̔���p.

	enStaticMesh_P_Search,


	enStaticMesh_Back,

	enStaticMesh_Sora,

	enStaticMesh_Max,			//�����ōő�l
	enStaticMesh_Min = 0,		//�����l�̂��� 0 �Œ�

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

	//�͈͓����m�F����֐�.
	bool CheckRangeMainStaticMesh(int enMesh);

};



#endif