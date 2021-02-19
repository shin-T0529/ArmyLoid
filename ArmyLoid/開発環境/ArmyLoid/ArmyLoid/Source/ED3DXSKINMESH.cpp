/***************************************************************************************************
*	SkinMeshCode Version 1.50
*	LastUpdate	: 2017/06/30
**/
#include <stdlib.h>
#include "ED3DXSKINMESH.h"
#include <string.h>


// �V�F�[�_��(�f�B���N�g�����܂�)
const char SHADER_NAME[] = "Shader\\MeshSkin.hlsl";


// �t���[�����쐬����.
HRESULT MY_HIERARCHY::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	MYFRAME *pFrame;

	*ppNewFrame = NULL;

	pFrame = new MYFRAME;
	if (pFrame == NULL)
	{
		return E_OUTOFMEMORY;
	}
	int length = strlen(Name) + 1;
	pFrame->Name = new CHAR[length];
	if (!pFrame->Name)
	{
		return E_FAIL;
	}
	strcpy_s(pFrame->Name, sizeof(CHAR)*length, Name);

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	*ppNewFrame = pFrame;

	return S_OK;
}


// ���b�V���R���e�i�[���쐬����.
HRESULT MY_HIERARCHY::CreateMeshContainer(
	LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{
	*ppMeshContainer = NULL;
	DWORD dwBoneNum = 0;

	pMeshContainer = new MYMESHCONTAINER;
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));
	int length = strlen(Name) + 1;
	pMeshContainer->Name = new CHAR[length];
	strcpy_s(pMeshContainer->Name, sizeof(CHAR)*length, Name);

	memcpy(&pMeshContainer->MeshData, pMeshData, sizeof(pMeshContainer->MeshData));
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	int NumPoly = pMeshContainer->MeshData.pMesh->GetNumFaces();
	// ���b�V���𕡐�����.
	//	�Ȃ����X�L�����b�V�����ƁA���̊֐��𔲂��������D3DX�����Ń��b�V���|�C���^�[�����������Ȃ��Ă��܂��̂�.
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pMeshContainer->MeshData.pMesh->GetDevice(&pDevice);
	LPD3DXMESH pMesh = NULL;
	pMeshContainer->MeshData.pMesh->CloneMesh(NULL, NULL, pDevice, &pMesh);
	//SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	pMeshContainer->MeshData.pMesh = pMesh;

	// ���b�V���̃}�e���A���ݒ�.
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[pMeshContainer->MeshData.pMesh->GetNumFaces() * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		return E_FAIL;
	}
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumPoly * 3);

	if (NumMaterials > 0)
	{
		for (DWORD i = 0; i < NumMaterials; i++)
		{
			memcpy(&pMeshContainer->pMaterials[i], &pMaterials[i], sizeof(D3DXMATERIAL));
			if (pMaterials[i].pTextureFilename != NULL)
			{
				int length = 512;
				pMeshContainer->pMaterials[i].pTextureFilename = new CHAR[length];//�����[�N.
				strcpy_s(pMeshContainer->pMaterials[i].pTextureFilename,
					sizeof(CHAR) * length, pMaterials[i].pTextureFilename);
			}
		}
	}
	else
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	// ���Y���b�V�����X�L�����������Ă���ꍇ(�X�L�����b�V���ŗL�̏���).
	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
		dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];//�����[�N.

		for (DWORD i = 0; i < dwBoneNum; i++)
		{
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
		}
	}
	// ���[�J���ɐ����������b�V���R���e�i�[���Ăяo�����ɃR�s�[����.
	*ppMeshContainer = pMeshContainer;

	return S_OK;
}



// �t���[����j������.
HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	if (pFrameToFree->Name) {
		delete[] pFrameToFree->Name;
		pFrameToFree->Name = NULL;
	}
	if (pFrameToFree->pMeshContainer) {
		DestroyMeshContainer(pFrameToFree->pMeshContainer);
	}
	if (pFrameToFree->pFrameFirstChild) {
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if (pFrameToFree->pFrameSibling) {
		DestroyFrame(pFrameToFree->pFrameSibling);
	}
	delete pFrameToFree;

	return S_OK;
}


// ���b�V���R���e�i�[��j������.
HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	DWORD dwMaterial;
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);

	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);

	for (DWORD i = 0; i < pMeshContainer->dwBoneNum; i++) {
		pMeshContainer->ppBoneMatrix[i] = NULL;
	}

	if (pMeshContainer->ppTextures != NULL)
	{
		for (dwMaterial = 0; dwMaterial < pMeshContainer->NumMaterials; dwMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[dwMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	if (pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE(pMeshContainer->pBoneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}

	SAFE_DELETE(pMeshContainer);

	return S_OK;
}

/*******************************************************************************************************************************************
*
*	�ȍ~�A�p�[�T�[�N���X.
*
**/
// �{�[���s��̗̈�m��.
HRESULT D3DXPARSER::AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	MYFRAME *pFrame = NULL;
	DWORD dwBoneNum = 0;

	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		// �܂���NULL�ŏ�����.
		pMeshContainer->ppBoneMatrix[i] = NULL;

		pFrame = (MYFRAME*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		if (pFrame == NULL)
		{
			return E_FAIL;
		}
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;

	}
	return S_OK;
}



// �S�Ẵ{�[���s��̗̈���m��(�ċA�֐�).
HRESULT D3DXPARSER::AllocateAllBoneMatrices(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != NULL)
	{
		if (FAILED(AllocateBoneMatrix(pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}
	if (pFrame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

// X�t�@�C�����烁�b�V����ǂݍ���.
HRESULT D3DXPARSER::LoadMeshFromX(LPDIRECT3DDEVICE9 pDevice9, LPSTR FileName)
{
	// X�t�@�C������A�j���[�V�������b�V����ǂݍ��ݍ쐬����
	m_pHierarchy = new MY_HIERARCHY;
	if (FAILED(
		D3DXLoadMeshHierarchyFromXA(
			FileName, D3DXMESH_MANAGED, pDevice9, m_pHierarchy,
			NULL, &m_pFrameRoot, &m_pAnimController)))
	{
		MessageBoxA(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", FileName, MB_OK);
		return E_FAIL;
	}
	// �{�[�����������肠��.
	AllocateAllBoneMatrices(m_pFrameRoot);


	/*
		m_pContainer = (MYMESHCONTAINER*)GetMeshContainer( m_pFrameRoot );

		if( m_pContainer == NULL ){
			MessageBox( NULL, L"���b�V���R���e�i��������Ȃ�����", L"error", MB_OK );
			return E_FAIL;
		}
	*/


	DWORD iAnimMax = m_pAnimController->GetNumAnimationSets();
	// �A�j���[�V�����Z�b�g�𓾂�.
	for (DWORD i = 0; i < iAnimMax; i++)
	{
		m_pAnimController->GetAnimationSet(i, &m_pAnimSet[i]);
	}

	return S_OK;
}



// �t���[�����̃��b�V�����Ƀ��[���h�ϊ��s����X�V����.
VOID D3DXPARSER::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME *pFrame = (MYFRAME*)pFrameBase;

	if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}


int D3DXPARSER::GetNumVertices(MYMESHCONTAINER* pContainer)
{
	return pContainer->MeshData.pMesh->GetNumVertices();
}


int D3DXPARSER::GetNumFaces(MYMESHCONTAINER* pContainer)
{
	return pContainer->MeshData.pMesh->GetNumFaces();
}


int D3DXPARSER::GetNumMaterials(MYMESHCONTAINER* pContainer)
{
	return pContainer->NumMaterials;
}


int D3DXPARSER::GetNumUVs(MYMESHCONTAINER* pContainer)
{
	return pContainer->MeshData.pMesh->GetNumVertices();
}


// �w�肳�ꂽ�{�[�����e�����y�ڂ����_����Ԃ�.
int D3DXPARSER::GetNumBoneVertices(MYMESHCONTAINER* pContainer, int iBoneIndex)
{
	return pContainer->pSkinInfo->GetNumBoneInfluences(iBoneIndex);
}


// �w�肳�ꂽ�{�[�����e�����y�ڂ����_�̃C���f�b�N�X��Ԃ� ��2�����́A�e�����󂯂钸�_�̃C���f�b�N�X�O���[�v���̃C���f�b�N�X�i�C���f�b�N�X���Ⴂ���j
// �Ⴆ�΃{�[���ɉe���������钸�_���S���Ƃ��āA���̃{�[���ɉe����������S�̒��_�̂���2�Ԗڂ̃C���f�b�N�X��m�肽���ꍇ�́AiIndexInGroup��1���w�肷��i0�X�^�[�g�Ȃ̂Łj
DWORD D3DXPARSER::GetBoneVerticesIndices(MYMESHCONTAINER* pContainer, int iBoneIndex, int iIndexInGroup)
{
	int Num = pContainer->pSkinInfo->GetNumBoneInfluences(iBoneIndex);
	DWORD* pVerts = new DWORD[Num];
	float* pWights = new float[Num];

	DWORD dwRslt = 0;

	if (FAILED(
		pContainer->pSkinInfo->GetBoneInfluence(iBoneIndex, pVerts, pWights)))
	{
		MessageBox(NULL, "�{�[���e�����󂯂钸�_������Ȃ�", "error", MB_OK);
	}
	else {
		dwRslt = pVerts[iIndexInGroup];
	}

	delete[] pVerts;
	delete[] pWights;

	return dwRslt;
}

// �w�肳�ꂽ�{�[�����e�����y�ڂ����_�̃{�[���E�F�C�g��Ԃ� ��2�����́A�e�����󂯂钸�_�̃C���f�b�N�X�O���[�v���̃C���f�b�N�X�i�C���f�b�N�X���Ⴂ���j
// �Ⴆ�΃{�[���ɉe���������钸�_���S���Ƃ��āA���̃{�[���ɉe����������S�̒��_�̂���2�Ԗڂ̒��_�̃{�[���E�F�C�g��m�肽���ꍇ�́AiIndexInGroup��1���w�肷��i0�X�^�[�g�Ȃ̂Łj
double D3DXPARSER::GetBoneVerticesWeights(MYMESHCONTAINER* pContainer, int iBoneIndex, int iIndexInGroup)
{
	int Num = pContainer->pSkinInfo->GetNumBoneInfluences(iBoneIndex);
	DWORD* pVerts = new DWORD[Num];
	float* pWights = new float[Num];

	double dRslt = 0.0f;

	if (FAILED(
		pContainer->pSkinInfo->GetBoneInfluence(iBoneIndex, pVerts, pWights)))
	{
		MessageBox(NULL, "�{�[���e�����󂯂钸�_������Ȃ�", "error", MB_OK);
	}
	else {
		dRslt = (double)pWights[iIndexInGroup];
	}
	delete[] pVerts;
	delete[] pWights;

	return dRslt;
}


// ���W.
D3DXVECTOR3 D3DXPARSER::GetVertexCoord(MYMESHCONTAINER* pContainer, DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	D3DXVECTOR3* pCoord = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertices += Index * Stride;
		pCoord = (D3DXVECTOR3*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pCoord;
}


// �@��.
D3DXVECTOR3 D3DXPARSER::GetNormal(MYMESHCONTAINER* pContainer, DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	D3DXVECTOR3* pNormal = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertices += Index * Stride;
		pVertices += sizeof(D3DXVECTOR3);	// ���W���i�߂�.
		pNormal = (D3DXVECTOR3*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pNormal;
}


// �e�N�X�`���[���W.
D3DXVECTOR2 D3DXPARSER::GetUV(MYMESHCONTAINER* pContainer, DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	D3DXVECTOR2* pUV = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertices += Index * Stride;
		pVertices += sizeof(D3DXVECTOR3);	// ���W���i�߂�.
		pVertices += sizeof(D3DXVECTOR3);	// �@�����i�߂�.
		pUV = (D3DXVECTOR2*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pUV;
}


// �C���f�b�N�X�o�b�t�@���̎w�肵���ʒu�i�C���f�b�N�X�C���f�b�N�X�j�ɂ��钸�_�C���f�b�N�X��Ԃ�.
int D3DXPARSER::GetIndex(MYMESHCONTAINER* pContainer, DWORD Index)
{
	WORD VertIndex = 0;
	WORD* pIndex = NULL;
	pContainer->MeshData.pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pIndex);

	VertIndex = pIndex[Index];

	pContainer->MeshData.pMesh->UnlockIndexBuffer();

	return VertIndex;
}


D3DXVECTOR4 D3DXPARSER::GetAmbient(MYMESHCONTAINER* pContainer, int iIndex)
{
	D3DXCOLOR color;
	color = pContainer->pMaterials[iIndex].MatD3D.Ambient;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}


D3DXVECTOR4 D3DXPARSER::GetDiffuse(MYMESHCONTAINER* pContainer, int iIndex)
{
	D3DXCOLOR color;
	color = pContainer->pMaterials[iIndex].MatD3D.Diffuse;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}


D3DXVECTOR4 D3DXPARSER::GetSpecular(MYMESHCONTAINER* pContainer, int iIndex)
{
	D3DXCOLOR color;

	color = pContainer->pMaterials[iIndex].MatD3D.Specular;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}


CHAR* D3DXPARSER::GetTexturePath(MYMESHCONTAINER* pContainer, int index)
{
	CHAR* p = NULL;
	p = pContainer->pMaterials[index].pTextureFilename;
	return p;
}



float D3DXPARSER::GetSpecularPower(MYMESHCONTAINER* pContainer, int iIndex)
{
	float power;

	power = pContainer->pMaterials[iIndex].MatD3D.Power;
	return power;
}


// ���̃|���S�����A�ǂ̃}�e���A���ł��邩��Ԃ�.
int D3DXPARSER::GeFaceMaterialIndex(MYMESHCONTAINER* pContainer, int iFaceIndex)
{
	int MaterialIndex = 0;
	DWORD* pBuf = NULL;
	if (SUCCEEDED(
		pContainer->MeshData.pMesh->LockAttributeBuffer(D3DLOCK_READONLY, &pBuf)))
	{
		MaterialIndex = pBuf[iFaceIndex];
	}
	else
	{
		MessageBoxA(0, "�����o�b�t�@�̃��b�N���s", "", MB_OK);
	}
	return MaterialIndex;
}


// iFaceIndex�Ԗڂ̃|���S�����`������3���_�̒��ŁAiIndexInFace�Ԗ�[0,2]�̒��_�̃C���f�b�N�X��Ԃ�.
int D3DXPARSER::GetFaceVertexIndex(MYMESHCONTAINER* pContainer, int iFaceIndex, int iIndexInFace)
{
	// �C���f�b�N�X�o�b�t�@�[�𒲂ׂ�Ε�����.
	WORD VertIndex = 0;
	WORD* pIndex = NULL;
	pContainer->MeshData.pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pIndex);

	VertIndex = pIndex[iFaceIndex * 3 + iIndexInFace];

	pContainer->MeshData.pMesh->UnlockIndexBuffer();

	return VertIndex;
}


D3DXMATRIX D3DXPARSER::GetBindPose(MYMESHCONTAINER* pContainer, int iBoneIndex)
{
	return *pContainer->pSkinInfo->GetBoneOffsetMatrix(iBoneIndex);
}


// ���̊֐�����Ԃ��Ƃ�Update����Ɩ��ʂȂ̂ŁARender�֐����s����Update������̂Ƃ��Ă��̂܂ܓn��.
// �e�N���X�����̃N���X��UpdateMatrices�����s���Ȃ��ꍇ�́AUpdateMatrices���Ȃ��ƍs�񂪍ŐV�ɂȂ�Ȃ��̂ŗ���.
D3DXMATRIX D3DXPARSER::GetNewPose(MYMESHCONTAINER* pContainer, int iBoneIndex)
{
	MYMESHCONTAINER* pMyContaiber = (MYMESHCONTAINER*)pContainer;
	return *pMyContaiber->ppBoneMatrix[iBoneIndex];
}



CHAR* D3DXPARSER::GetBoneName(MYMESHCONTAINER* pContainer, int iBoneIndex)
{
	return (CHAR*)pContainer->pSkinInfo->GetBoneName(iBoneIndex);
}



int D3DXPARSER::GetNumBones(MYMESHCONTAINER* pContainer)
{
	return pContainer->pSkinInfo->GetNumBones();
}




// ���b�V���R���e�i���Ăяo��(�ċA����)
LPD3DXMESHCONTAINER D3DXPARSER::GetMeshContainer(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pCon = NULL;

	// ���b�V���R���e�i����ΕԂ�.
	if (pFrame->pMeshContainer) {
		return pFrame->pMeshContainer;
	}

	// ����������A�q�̃t���[�����`�F�b�N.
	// ���������q�t���[������H
	if (pFrame->pFrameFirstChild) {
		// ����΃`�F�b�N����.
		pCon = GetMeshContainer(pFrame->pFrameFirstChild);
	}

	// �q�̃t���[���ŉ��w�܂Ń`�F�b�N������������Ȃ�����.
	if (pCon == NULL) {
		//�Z��̃t���[�����T��.
		// ���������Z��t���[������H
		if (pFrame->pFrameSibling) {
			// �����΃`�F�b�N����.
			pCon = GetMeshContainer(pFrame->pFrameSibling);
		}
	}

	// ������Ȃ��ꍇ��NULL������.
	return pCon;
}


// �A�j���[�V�����Z�b�g�̐؂�ւ�.
void D3DXPARSER::ChangeAnimSet(int index, LPD3DXANIMATIONCONTROLLER pAC)
{
	D3DXTRACK_DESC TrackDesc;		// �A�j���[�V�����g���b�N�\����.
	ZeroMemory(&TrackDesc, sizeof(TrackDesc));

	//���ȉ�3�́A�قڌŒ��OK.
	TrackDesc.Weight = 1;		// �d��.
	TrackDesc.Speed = 1;		// ����.
	TrackDesc.Enable = 1;		// �L��.

	TrackDesc.Position = 0.0;		// �t���[���ʒu(�J�n�ʒu���w��ł���)

	LPD3DXANIMATIONCONTROLLER pTmpAC;
	if (pAC) {
		pTmpAC = pAC;
	}
	else {
		pTmpAC = m_pAnimController;
	}

	// �w��(index�j�̃A�j���[�V�����g���b�N�ɕύX.
	pTmpAC->SetTrackDesc(0, &TrackDesc);
	pTmpAC->SetTrackAnimationSet(0, m_pAnimSet[index]);
	pTmpAC->SetTrackEnable(index, true);
}

// �A�j���[�V������Ă̐؂�ւ�(�J�n�t���[���w��\��)
//�@��Q�����ɊJ�n�������t���[�����w�肷��.
//	���S��~�������ꍇ�́A�O��ŃA�j���[�V�������x��0.0f�Ɏw�肵�Ă��K�v������.
void D3DXPARSER::ChangeAnimSet_StartPos(int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC)
{
	D3DXTRACK_DESC TrackDesc;		// �A�j���[�V�����g���b�N�\����.
	ZeroMemory(&TrackDesc, sizeof(TrackDesc));

	//���ȉ�3�́A�قڌŒ��OK.
	TrackDesc.Weight = 1;		// �d��.
	TrackDesc.Speed = 1;		// ����.
	TrackDesc.Enable = 1;		// �L��.

	TrackDesc.Position = dStartFramePos;

	LPD3DXANIMATIONCONTROLLER pTmpAC;
	if (pAC) {
		pTmpAC = pAC;
	}
	else {
		pTmpAC = m_pAnimController;
	}

	// �w��(index�j�̃A�j���[�V�����g���b�N�ɕύX.
	pTmpAC->SetTrackDesc(0, &TrackDesc);
	pTmpAC->SetTrackAnimationSet(0, m_pAnimSet[index]);
	pTmpAC->SetTrackEnable(index, true);
}

// �A�j���[�V������~���Ԃ��擾.
double D3DXPARSER::GetAnimPeriod(int index)
{
	if (index < 0 || MAX_ANIM_SET <= index) {
		return 0.0;
	}
	return m_pAnimSet[index]->GetPeriod();
}

// �ő�A�j���[�V���������擾.
int D3DXPARSER::GetAnimMax(LPD3DXANIMATIONCONTROLLER pAC)
{
	if (pAC) {
		return pAC->GetNumAnimationSets();
	}
	return m_pAnimController->GetNumAnimationSets();
}

// �w�肵���{�[�����(�s��)���擾����֐�.
bool D3DXPARSER::GetMatrixFromBone(char* sBoneName, D3DXMATRIX* pOutMat)
{
	LPD3DXFRAME pFrame;
	pFrame = (MYFRAME*)D3DXFrameFind(m_pFrameRoot, sBoneName);

	if (pFrame == NULL) {
		return false;
	}

	MYFRAME* pMyFrame = (MYFRAME*)pFrame;
	// �ʒu�����擾.
	*pOutMat = pMyFrame->CombinedTransformationMatrix;

	return true;
}


// �w�肵���{�[�����(���W)���擾����֐�.
bool D3DXPARSER::GetPosFromBone(char* sBoneName, D3DXVECTOR3* pOutPos)
{
	D3DXMATRIX mBone;
	if (!GetMatrixFromBone(sBoneName, &mBone)) {
		return false;
	}
	pOutPos->x = mBone._41;
	pOutPos->y = mBone._42;
	pOutPos->z = mBone._43;

	return true;
}

// �ꊇ�������.
HRESULT D3DXPARSER::Release()
{
	// �쐬�������̂��Ō�ɍ�������̂��珇������Ă���.
	// ��{�I�ɂ� new �������̂� delete ���Ă���.

	// �A�j���[�V�����Z�b�g�̉��.
	DWORD iAnimMax = m_pAnimController->GetNumAnimationSets();
	for (DWORD i = 0; i < iAnimMax; i++)
	{
		SAFE_RELEASE(m_pAnimSet[i]);
	}

	// �A�j���[�V�����R���g���[���폜.
	SAFE_RELEASE(m_pAnimController);

	// �t���[���ƃ��b�V���R���e�i�̍폜.
	m_pHierarchy->DestroyFrame(m_pFrameRoot);

	// �����������H
	if (m_pFrameRoot) {
		m_pFrameRoot = NULL;
	}

	// Hierarchy�폜.
	if (m_pHierarchy) {
		delete m_pHierarchy;
		m_pHierarchy = NULL;
	}

	return S_OK;
}


/******************************************************************************************************************************************
*
*	�ȍ~�A�X�L�����b�V���N���X.
*
**/
// �R���X�g���N�^.
clsD3DXSKINMESH::clsD3DXSKINMESH()
	:m_pD3dxMesh(NULL)
{
	ZeroMemory(this, sizeof(clsD3DXSKINMESH));
	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//m_vScale = D3DXVECTOR3(0.04f, 0.04f, 0.04f);

	// ��悸�A���̒l.
	m_dAnimSpeed = 0.0001f;
}


// �f�X�g���N�^.
clsD3DXSKINMESH::~clsD3DXSKINMESH()
{
	// �������.
	Release();
	// �V�F�[�_��T���v���֌W.
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);

	// �R���X�^���g�o�b�t�@�֌W.
	SAFE_RELEASE(m_pConstantBufferBone);
	SAFE_RELEASE(m_pConstantBuffer1);
	SAFE_RELEASE(m_pConstantBuffer0);

	// Dx9 �f�o�C�X�֌W.
	SAFE_RELEASE(m_pDevice9);
	SAFE_RELEASE(m_pD3d9);

	// Dx11 �f�o�C�X�֌W.
	m_pDeviceContext = NULL;
	m_pDevice = NULL;
}


// ������.
HRESULT clsD3DXSKINMESH::Init(CD3DXSKINMESH_INIT* si)
{
	m_hWnd = si->hWnd;
	m_pDevice = si->pDevice;
	m_pDeviceContext = si->pDeviceContext;

	// Dx9 �̃f�o�C�X�֌W���쐬����.
	if (FAILED(CreateDeviceDx9(m_hWnd)))
	{
		return E_FAIL;
	}
	// �V�F�[�_�̍쐬.
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}

	return S_OK;
}

// Dx9�̃f�o�C�X�E�f�o�C�X�R���e�L�X�g�̍쐬.
HRESULT clsD3DXSKINMESH::CreateDeviceDx9(HWND hWnd)
{
	// D3d"9"�̃f�o�C�X�����A�S�Ă�D3DXMESH�̈����ɕK�v������.
	// Direct3D"9"�I�u�W�F�N�g�̍쐬.
	if (NULL == (m_pD3d9 = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(NULL, "Direct3D9�̍쐬�Ɏ��s���܂���", "", MB_OK);
		return E_FAIL;
	}

	// DIRECT3D"9"�f�o�C�X�I�u�W�F�N�g�̍쐬
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDevice9)))
	{
		if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice9)))
		{
			MessageBox(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);
			if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_pDevice9)))
			{
				if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_pDevice9)))
				{
					MessageBox(0, "DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}

// �V�F�[�_������.
HRESULT	clsD3DXSKINMESH::InitShader()
{
	//D3D11�֘A�̏�����
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	UINT	uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(SHADER_NAME, NULL, NULL, "VSSkin", "vs_5_0", uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		int size = pErrors->GetBufferSize();
		char* ch = (char*)pErrors->GetBufferPointer();
		MessageBox(0, "hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(
		m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(
		m_pDevice->CreateInputLayout(
			layout, numElements, pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME, NULL, NULL,
			"PSSkin", "ps_5_0",
			uCompileFlag, 0, NULL,
			&pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(
		m_pDevice->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	D3D11_BUFFER_DESC cb;
	// �R���X�^���g�o�b�t�@�[0�쐬.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_SKIN_GLOBAL0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(
		m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer0)))
	{
		return E_FAIL;
	}

	// �R���X�^���g�o�b�t�@�[1�쐬.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_SKIN_GLOBAL1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(
		m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer1)))
	{
		return E_FAIL;
	}

	// �R���X�^���g�o�b�t�@�[�{�[���p�쐬.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_GLOBAL_BONES);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(
		m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBufferBone)))
	{
		return E_FAIL;
	}

	// �e�N�X�`���[�p�T���v���[�쐬.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(
		m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear)))
	{
		return E_FAIL;
	}

	return S_OK;
}

// X�t�@�C������X�L���֘A�̏���ǂݏo���֐�.
HRESULT clsD3DXSKINMESH::ReadSkinInfo(MYMESHCONTAINER* pContainer, MY_SKINVERTEX* pvVB, SKIN_PARTS_MESH* pParts)
{
	// X�t�@�C�����璊�o���ׂ����́A
	// �u���_���Ƃ��ްݲ��ޯ���v�u���_���Ƃ̃{�[���E�F�C�g�v.
	// �u�o�C���h�s��v�u�|�[�Y�s��v��4����.

	int i, k, m, n;	// �e�탋�[�v�ϐ�.
	int iNumVertex = 0;	// ���_��.
	int iNumBone = 0;	// �{�[����.

	// ���_��.
	iNumVertex = m_pD3dxMesh->GetNumVertices(pContainer);
	// �{�[����.
	iNumBone = m_pD3dxMesh->GetNumBones(pContainer);

	// ���ꂼ��̃{�[���ɉe�����󂯂钸�_�𒲂ׂ�.
	// ��������t�ɁA���_�x�[�X�Ń{�[���C���f�b�N�X�E�d�݂𐮓ڂ���.
	for (i = 0; i < iNumBone; i++)
	{
		// ���̃{�[���ɉe�����󂯂钸�_��.
		int iNumIndex
			= m_pD3dxMesh->GetNumBoneVertices(pContainer, i);

		int*	pIndex = new int[iNumIndex];
		double*	pWeight = new double[iNumIndex];

		for (k = 0; k < iNumIndex; k++)
		{
			pIndex[k]
				= m_pD3dxMesh->GetBoneVerticesIndices(pContainer, i, k);
			pWeight[k]
				= m_pD3dxMesh->GetBoneVerticesWeights(pContainer, i, k);
		}

		// ���_������C���f�b�N�X�����ǂ��āA���_�T�C�h�Ő�������.
		for (k = 0; k < iNumIndex; k++)
		{
			// X�t�@�C����CG�\�t�g���{�[��4�{�ȓ��Ƃ͌���Ȃ�.
			// 5�{�ȏ�̏ꍇ�́A�d�݂̑傫������4�{�ɍi��.

			// �E�F�C�g�̑傫�����Ƀ\�[�g(�o�u���\�[�g)
			for (m = 4; m > 1; m--)
			{
				for (n = 1; n < m; n++)
				{
					if (pvVB[pIndex[k]].bBoneWeight[n - 1] <
						pvVB[pIndex[k]].bBoneWeight[n])
					{
						float tmp = pvVB[pIndex[k]].bBoneWeight[n - 1];
						pvVB[pIndex[k]].bBoneWeight[n - 1] = pvVB[pIndex[k]].bBoneWeight[n];
						pvVB[pIndex[k]].bBoneWeight[n] = tmp;
						int itmp = pvVB[pIndex[k]].bBoneIndex[n - 1];
						pvVB[pIndex[k]].bBoneIndex[n - 1] = pvVB[pIndex[k]].bBoneIndex[n];
						pvVB[pIndex[k]].bBoneIndex[n] = itmp;
					}
				}
			}
			// �\�[�g��́A�Ō�̗v�f�Ɉ�ԏ������E�F�C�g�������Ă�͂�.
			bool flag = false;
			for (m = 0; m < 4; m++)
			{
				if (pvVB[pIndex[k]].bBoneWeight[m] == 0)
				{
					flag = true;
					pvVB[pIndex[k]].bBoneIndex[m] = i;
					pvVB[pIndex[k]].bBoneWeight[m] = (float)pWeight[k];
					break;
				}
			}
			if (flag == false)
			{
				pvVB[pIndex[k]].bBoneIndex[3] = i;
				pvVB[pIndex[k]].bBoneWeight[3] = (float)pWeight[k];
				break;
			}
		}
		// �g���I���΍폜����.
		delete[] pIndex;
		delete[] pWeight;
	}

	// �{�[������.
	pParts->iNumBone = iNumBone;
	pParts->pBoneArray = new BONE[iNumBone];//�����[�N.
	// �|�[�Y�s��𓾂�(�����|�[�Y)
	for (i = 0; i < pParts->iNumBone; i++)
	{
		pParts->pBoneArray[i].mBindPose
			= m_pD3dxMesh->GetBindPose(pContainer, i);
	}

	return S_OK;
}

// X����X�L�����b�V�����쐬����@�@���Ӂj�f�ށiX)�̂ق��́A�O�p�|���S���ɂ��邱��.
HRESULT clsD3DXSKINMESH::CreateFromX(CHAR* szFileName)
{
	// �t�@�C�������p�X���Ǝ擾.
	strcpy_s(m_FilePath, sizeof(m_FilePath), szFileName);

	// X�t�@�C���ǂݍ���.
	m_pD3dxMesh = new D3DXPARSER;
	m_pD3dxMesh->LoadMeshFromX(m_pDevice9, szFileName);	


	// �S�Ẵ��b�V�����쐬����.
	BuildAllMesh(m_pD3dxMesh->m_pFrameRoot);

	return S_OK;
}

// Direct3D�̃C���f�b�N�X�o�b�t�@�[�쐬.
HRESULT clsD3DXSKINMESH::CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer)
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = dwSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pIndex;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, ppIndexBuffer)))
	{
		return FALSE;
	}

	return S_OK;
}

// �����_�����O.
void clsD3DXSKINMESH::Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vEye,	LPD3DXANIMATIONCONTROLLER pAC)
{
	m_mView = mView;
	m_mProj = mProj;
	m_vLight = vLight;
	m_vEye = vEye;

	// ��Ґi�s.
	if (pAC != NULL) {
		pAC->AdvanceTime(m_dAnimSpeed, NULL);
	}
	else {
		if (m_pD3dxMesh->m_pAnimController)
		{
			m_pD3dxMesh->m_pAnimController->AdvanceTime(m_dAnimSpeed, NULL);
		}
	}

	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	m_pD3dxMesh->UpdateFrameMatrices(m_pD3dxMesh->m_pFrameRoot, &m);
	DrawFrame(m_pD3dxMesh->m_pFrameRoot);
}

//�S�Ẵ��b�V�����쐬����.
void clsD3DXSKINMESH::BuildAllMesh(D3DXFRAME* pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		CreateAppMeshFromD3DXMesh(pFrame);
	}

	// �ċA�֐�.
	if (pFrame->pFrameSibling != NULL)
	{
		BuildAllMesh(pFrame->pFrameSibling);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		BuildAllMesh(pFrame->pFrameFirstChild);
	}
}

// ���b�V���쐬.
HRESULT clsD3DXSKINMESH::CreateAppMeshFromD3DXMesh(LPD3DXFRAME p)
{
	MYFRAME* pFrame = (MYFRAME*)p;

	//	LPD3DXMESH pD3DXMesh = pFrame->pMeshContainer->MeshData.pMesh;//D3DXү��(��������E�E�E)
	MYMESHCONTAINER* pContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	// ����ү��(�E�E�E�����Ƀ��b�V���f�[�^���R�s�[����)
	pAppMesh = new SKIN_PARTS_MESH;
	pAppMesh->bTex = false;

	// ���O�ɒ��_���A�|���S�������𒲂ׂ�.
	pAppMesh->dwNumVert = m_pD3dxMesh->GetNumVertices(pContainer);
	pAppMesh->dwNumFace = m_pD3dxMesh->GetNumFaces(pContainer);
	pAppMesh->dwNumUV = m_pD3dxMesh->GetNumUVs(pContainer);
	// Direct3D�ł�UV�̐��������_���K�v.
	if (pAppMesh->dwNumVert < pAppMesh->dwNumUV) {
		// ���L���_���ŁA���_������Ȃ��Ƃ�.
		MessageBox(NULL,
			"Direct3D�́AUV�̐��������_���K�v�ł�(UV��u���ꏊ���K�v�ł�)�e�N�X�`���͐������\���Ȃ��Ǝv���܂�",
			"Error", MB_OK);
		return E_FAIL;
	}
	// �ꎞ�I�ȃ������m��(���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@)
	MY_SKINVERTEX* pvVB = new MY_SKINVERTEX[pAppMesh->dwNumVert];
	int* piFaceBuffer = new int[pAppMesh->dwNumFace * 3];
	// 3���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)

//���_�ǂݍ���.
	for (DWORD i = 0; i < pAppMesh->dwNumVert; i++) {
		D3DXVECTOR3 v = m_pD3dxMesh->GetVertexCoord(pContainer, i);
		pvVB[i].vPos.x = v.x;
		pvVB[i].vPos.y = v.y;
		pvVB[i].vPos.z = v.z;
	}
	// �|���S�����(���_�C���f�b�N�X)�ǂݍ���.
	for (DWORD i = 0; i < pAppMesh->dwNumFace * 3; i++) {
		piFaceBuffer[i] = m_pD3dxMesh->GetIndex(pContainer, i);
	}
	// �@���ǂݍ���.
	for (DWORD i = 0; i < pAppMesh->dwNumVert; i++) {
		D3DXVECTOR3 v = m_pD3dxMesh->GetNormal(pContainer, i);
		pvVB[i].vNorm.x = v.x;
		pvVB[i].vNorm.y = v.y;
		pvVB[i].vNorm.z = v.z;
	}
	// �e�N�X�`�����W�ǂݍ���.
	for (DWORD i = 0; i < pAppMesh->dwNumVert; i++) {
		D3DXVECTOR2 v = m_pD3dxMesh->GetUV(pContainer, i);
		pvVB[i].vTex.x = v.x;
		pvVB[i].vTex.y = v.y;
	}
	// �}�e���A���ǂݍ���.
	pAppMesh->dwNumMaterial = m_pD3dxMesh->GetNumMaterials(pContainer);
	pAppMesh->pMaterial = new MY_SKINMATERIAL[pAppMesh->dwNumMaterial];	//�����[�N.

	// �}�e���A���̐������C���f�b�N�X�o�b�t�@���쐬.
	pAppMesh->ppIndexBuffer = new ID3D11Buffer*[pAppMesh->dwNumMaterial];//�����[�N.
	// �|���Z�ł͂Ȃ��uID3D11Buffer*�v�̔z��Ƃ����Ӗ�.
	for (DWORD i = 0; i < pAppMesh->dwNumMaterial; i++)
	{
		// ����(�A���r�G���g)
		pAppMesh->pMaterial[i].Ka.x = m_pD3dxMesh->GetAmbient(pContainer, i).y;
		pAppMesh->pMaterial[i].Ka.y = m_pD3dxMesh->GetAmbient(pContainer, i).z;
		pAppMesh->pMaterial[i].Ka.z = m_pD3dxMesh->GetAmbient(pContainer, i).w;
		pAppMesh->pMaterial[i].Ka.w = m_pD3dxMesh->GetAmbient(pContainer, i).x;
		// �g�U���ˌ�(�f�B�t���[�Y)
		pAppMesh->pMaterial[i].Kd.x = m_pD3dxMesh->GetDiffuse(pContainer, i).y;
		pAppMesh->pMaterial[i].Kd.y = m_pD3dxMesh->GetDiffuse(pContainer, i).z;
		pAppMesh->pMaterial[i].Kd.z = m_pD3dxMesh->GetDiffuse(pContainer, i).w;
		pAppMesh->pMaterial[i].Kd.w = m_pD3dxMesh->GetDiffuse(pContainer, i).x;
		// ���ʔ��ˌ�(�X�y�L����)
		pAppMesh->pMaterial[i].Ks.x = m_pD3dxMesh->GetSpecular(pContainer, i).y;
		pAppMesh->pMaterial[i].Ks.y = m_pD3dxMesh->GetSpecular(pContainer, i).z;
		pAppMesh->pMaterial[i].Ks.z = m_pD3dxMesh->GetSpecular(pContainer, i).w;
		pAppMesh->pMaterial[i].Ks.w = m_pD3dxMesh->GetSpecular(pContainer, i).x;

		// �e�N�X�`��(�f�B�t���[�Y�e�N�X�`���̂�)
		char* name = m_pD3dxMesh->GetTexturePath(pContainer, i);
		if (name) {
			char* ret = strrchr(m_FilePath, '\\');
			if (ret != NULL) {
				int check = ret - m_FilePath;
				char path[512];
				strcpy_s(path, 512, m_FilePath);
				path[check + 1] = '\0';

				strcat_s(path, sizeof(path), name);
				strcpy_s(pAppMesh->pMaterial[i].szTextureName,
					sizeof(pAppMesh->pMaterial[i].szTextureName),
					path);
			}
		}
		// �e�N�X�`�����쐬.
		if (pAppMesh->pMaterial[i].szTextureName[0] != 0
			&& FAILED(
				D3DX11CreateShaderResourceViewFromFileA(
					m_pDevice, pAppMesh->pMaterial[i].szTextureName,
					NULL, NULL, &pAppMesh->pMaterial[i].pTexture, NULL)))
		{
			MessageBox(NULL, "�e�N�X�`���ǂݍ��ݎ��s",
				"Error", MB_OK);
			return E_FAIL;
		}
		// ���̃}�e���A���ł���C���f�b�N�X�z����̊J�n�C���f�b�N�X�𒲂ׂ�.
		// ����ɃC���f�b�N�X�̌��𒲂ׂ�.
		int iCount = 0;
		int* pIndex = new int[pAppMesh->dwNumFace * 3];
		// �Ƃ肠�����A���b�V�����̃|���S��������؊m��.
		// (�����̂ۂ育�񂮂�[�Ղ͕K������ȉ��ɂȂ�)

		for (DWORD k = 0; k < pAppMesh->dwNumFace; k++)
		{
			// ���� i==k �Ԗڂ̃|���S���̃}�e���A���ԍ��Ȃ�.
			if (i == m_pD3dxMesh->GeFaceMaterialIndex(pContainer, k))
			{
				// k�Ԗڂ̃|���S������钸�_�̃C���f�b�N�X.
				pIndex[iCount]
					= m_pD3dxMesh->GetFaceVertexIndex(pContainer, k, 0);	// 1��.
				pIndex[iCount + 1]
					= m_pD3dxMesh->GetFaceVertexIndex(pContainer, k, 1);	// 2��.
				pIndex[iCount + 2]
					= m_pD3dxMesh->GetFaceVertexIndex(pContainer, k, 2);	// 3��.
				iCount += 3;
			}
		}
		if (iCount > 0) {
			// �C���f�b�N�X�o�b�t�@�쐬.
			CreateIndexBuffer(iCount * sizeof(int),
				pIndex, &pAppMesh->ppIndexBuffer[i]);
		}
		else {
			// ������̏����ɕs����o������.
			// �J�E���g����0�ȉ��̏ꍇ�́A�C���f�b�N�X�o�b�t�@�� NULL �ɂ��Ă���.
			pAppMesh->ppIndexBuffer[i] = NULL;
		}

		// ���̃}�e���A�����̃|���S����.
		pAppMesh->pMaterial[i].dwNumFace = iCount / 3;
		// �s�v�ɂȂ����̂ō폜.
		delete[] pIndex;
	}

	if (pContainer->pSkinInfo == NULL) {
		//char strDbg[128];
		//sprintf_s(strDbg, "ContainerName:[%s]", pContainer->Name);
		//MessageBox(NULL, strDbg, "Not SkinInfo", MB_OK);
		pAppMesh->bEnableBones = false;
	}
	else {
		// �X�L�����(�W���C���g�A�E�F�C�g)�ǂݍ���.
		ReadSkinInfo(pContainer, pvVB, pAppMesh);
	}



	// �o�[�e�b�N�X�o�b�t�@���쐬.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_SKINVERTEX) * (pAppMesh->dwNumVert);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVB;

	HRESULT hRslt = S_OK;
	if (FAILED(
		m_pDevice->CreateBuffer(
			&bd, &InitData, &pAppMesh->pVertexBuffer)))
	{
		hRslt = E_FAIL;
	}

	// �p�[�c���b�V���ɐݒ�.
	pFrame->pPartsMesh = pAppMesh;

	// �ꎞ�I�ȓ��ꕨ�͕s�v�Ȃ�̂ō폜.
	if (piFaceBuffer) {
		delete[] piFaceBuffer;
	}
	if (pvVB) {
		delete[] pvVB;
	}

	return hRslt;
}

// �{�[�������̃|�[�Y�ʒu�ɃZ�b�g����֐�.
void clsD3DXSKINMESH::SetNewPoseMatrices(SKIN_PARTS_MESH* pParts, int frame, MYMESHCONTAINER* pContainer)
{
	//�]�ރt���[����Update���邱��.
	//���Ȃ��ƍs�񂪍X�V����Ȃ�.
	// m_pD3dxMesh->UpdateFrameMatrices m_pD3dxMesh->m_pFrameRoot)�������_�����O���Ɏ��s���邱��.

	// �܂��A�A�j���[�V�������ԂɌ��������s����X�V����̂�D3DXMESH�ł�
	// �A�j���[�V�����R���g���[����(����)����Ă������̂Ȃ̂ŁA
	// �A�j���[�V�����R���g���[�����g���ăA�j����i�s�����邱�Ƃ��K�v.
	// m_pD3dxMesh->m_pAnimController->AdvanceTime(...)��.
	// �����_�����O���Ɏ��s���邱��.

	if (pParts->iNumBone <= 0) {
		//�{�[���� 0�@�ȉ��̏ꍇ.
	}

	for (int i = 0; i < pParts->iNumBone; i++)
	{
		pParts->pBoneArray[i].mNewPose
			= m_pD3dxMesh->GetNewPose(pContainer, i);
	}
}

// ����(���݂�)�|�[�Y�s���Ԃ��֐�.
D3DXMATRIX clsD3DXSKINMESH::GetCurrentPoseMatrix(SKIN_PARTS_MESH* pParts, int index)
{
	D3DXMATRIX ret =
		pParts->pBoneArray[index].mBindPose * pParts->pBoneArray[index].mNewPose;
	return ret;
}

// �t���[���̕`��.
VOID clsD3DXSKINMESH::DrawFrame(LPD3DXFRAME p)
{
	MYFRAME*			pFrame = (MYFRAME*)p;
	SKIN_PARTS_MESH*	pPartsMesh = pFrame->pPartsMesh;
	MYMESHCONTAINER*	pContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	if (pPartsMesh != NULL)
	{
		DrawPartsMesh(
			pPartsMesh,
			pFrame->CombinedTransformationMatrix,
			pContainer);
	}

	//�ċA�֐�.
	//(�Z��)
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling);
	}
	//(�e�q)
	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

// �p�[�c���b�V����`��.
void clsD3DXSKINMESH::DrawPartsMesh(SKIN_PARTS_MESH* pMesh, D3DXMATRIX World, MYMESHCONTAINER* pContainer)
{
	D3D11_MAPPED_SUBRESOURCE pData;

	//�g�p����V�F�[�_�̃Z�b�g.
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);


	D3DXMATRIX	Scale, Yaw, Pitch, Roll, Tran;
	// �g�k.
	D3DXMatrixScaling(&Scale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationY(&Yaw, m_fYaw);		// Y����].
	D3DXMatrixRotationX(&Pitch, m_fPitch);	// X����].
	D3DXMatrixRotationZ(&Roll, m_fRoll);		// Z����].
	// ��]�s��(����)
	m_mRotation = Yaw * Pitch * Roll;
	// ���s�ړ�.
	D3DXMatrixTranslation(&Tran, m_vPos.x, m_vPos.y, m_vPos.z);
	// ���[���h�s��.
	m_mWorld = Scale * m_mRotation * Tran;


	// �A�j���[�V�����t���[����i�߂� �X�L�����X�V.
	m_iFrame++;
	if (m_iFrame >= 3600) {
		m_iFrame = 0;
	}
	SetNewPoseMatrices(pMesh, m_iFrame, pContainer);

	if (SUCCEEDED(
		m_pDeviceContext->Map(
			m_pConstantBufferBone, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SHADER_GLOBAL_BONES sg;
		for (int i = 0; i < pMesh->iNumBone; i++)
		{
			D3DXMATRIX mat = GetCurrentPoseMatrix(pMesh, i);
			D3DXMatrixTranspose(&mat, &mat);
			sg.mBone[i] = mat;
		}
		memcpy_s(pData.pData, pData.RowPitch,
			(void*)&sg, sizeof(SHADER_GLOBAL_BONES));
		m_pDeviceContext->Unmap(m_pConstantBufferBone, 0);
	}
	m_pDeviceContext->VSSetConstantBuffers(2, 1, &m_pConstantBufferBone);
	m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pConstantBufferBone);

	// �o�[�e�b�N�X�o�b�t�@���Z�b�g.
	UINT stride = sizeof(MY_SKINVERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(
		0, 1, &pMesh->pVertexBuffer, &stride, &offset);

	// ���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pDeviceContext->IASetInputLayout(
		m_pVertexLayout);

	// �v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_pDeviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �J�����ʒu���V�F�[�_�ɓn��
	if (SUCCEEDED(
		m_pDeviceContext->Map(
			m_pConstantBuffer0, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SHADER_SKIN_GLOBAL0 sg;
		sg.vLightDir = D3DXVECTOR4(m_vLight.x, m_vLight.y, m_vLight.z, 0);
		sg.vEye = D3DXVECTOR4(m_vEye.x, m_vEye.y, m_vEye.z, 0);
		memcpy_s(pData.pData, pData.RowPitch,
			(void*)&sg, sizeof(SHADER_SKIN_GLOBAL0));
		m_pDeviceContext->Unmap(
			m_pConstantBuffer0, 0);
	}
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);

	// �}�e���A���̐������A
	// ���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@��`��.
	for (DWORD i = 0; i < pMesh->dwNumMaterial; i++)
	{
		// �g�p����Ă��Ȃ��}�e���A���΍�.
		if (pMesh->pMaterial[i].dwNumFace == 0)
		{
			continue;
		}
		// �C���f�b�N�X�o�b�t�@���Z�b�g.
		stride = sizeof(int);
		offset = 0;
		m_pDeviceContext->IASetIndexBuffer(
			pMesh->ppIndexBuffer[i],
			DXGI_FORMAT_R32_UINT, 0);

		// �}�e���A���̊e�v�f�ƕϊ��s����V�F�[�_�ɓn��.
		D3D11_MAPPED_SUBRESOURCE pDat;
		if (SUCCEEDED(
			m_pDeviceContext->Map(
				m_pConstantBuffer1, 0,
				D3D11_MAP_WRITE_DISCARD, 0, &pDat)))
		{
			SHADER_SKIN_GLOBAL1 sg;
			sg.mW = m_mWorld;
			D3DXMatrixTranspose(&sg.mW, &sg.mW);
			sg.mWVP = m_mWorld * m_mView * m_mProj;
			D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
			sg.vAmbient = pMesh->pMaterial[i].Ka;
			sg.vDiffuse = pMesh->pMaterial[i].Kd;
			sg.vSpecular = pMesh->pMaterial[i].Ks;
			memcpy_s(pDat.pData, pDat.RowPitch,
				(void*)&sg, sizeof(SHADER_SKIN_GLOBAL1));
			m_pDeviceContext->Unmap(
				m_pConstantBuffer1, 0);
		}
		m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);

		// �e�N�X�`�����V�F�[�_�ɓn��.
		if (pMesh->pMaterial[i].szTextureName[0] != NULL)
		{
			m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
			m_pDeviceContext->PSSetShaderResources(0, 1, &pMesh->pMaterial[i].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);
		}
		// Draw.
		m_pDeviceContext->DrawIndexed(pMesh->pMaterial[i].dwNumFace * 3, 0, 0);
	}
}

// ����֐�.
HRESULT clsD3DXSKINMESH::Release()
{
	if (m_pD3dxMesh) 
	{
		// �S�Ẵ��b�V���폜.
		DestroyAllMesh(m_pD3dxMesh->m_pFrameRoot);
		// �p�[�T�[�N���X�������.
		m_pD3dxMesh->Release();

		SAFE_DELETE(m_pD3dxMesh);
	}
	return S_OK;
}

// �S�Ẵ��b�V�����폜.
void clsD3DXSKINMESH::DestroyAllMesh(D3DXFRAME* pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		DestroyAppMeshFromD3DXMesh(pFrame);
	}

	//�ċA�֐�.
	if (pFrame->pFrameSibling != NULL)
	{
		DestroyAllMesh(pFrame->pFrameSibling);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		DestroyAllMesh(pFrame->pFrameFirstChild);
	}
}

// ���b�V���̍폜.
HRESULT clsD3DXSKINMESH::DestroyAppMeshFromD3DXMesh(LPD3DXFRAME p)
{
	MYFRAME* pFrame = (MYFRAME*)p;

	//	LPD3DXMESH pD3DXMesh = pFrame->pMeshContainer->MeshData.pMesh;//D3DXү��(��������E�E�E)
	MYMESHCONTAINER* pContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	// �C���f�b�N�X�o�b�t�@���.
	for (DWORD i = 0; i < pFrame->pPartsMesh->dwNumMaterial; i++) {
		if (pFrame->pPartsMesh->ppIndexBuffer[i] != NULL) {
			pFrame->pPartsMesh->ppIndexBuffer[i]->Release();
			pFrame->pPartsMesh->ppIndexBuffer[i] = NULL;
		}
	}
	delete[] pFrame->pPartsMesh->ppIndexBuffer[0];

	// ���_�o�b�t�@�J��.
	pFrame->pPartsMesh->pVertexBuffer->Release();

	// �p�[�c�}�e���A���J��.
	delete[] pFrame->pPartsMesh;

	return S_OK;
}

// �A�j���[�V�����Z�b�g�̐؂�ւ�(�Đ�).
void clsD3DXSKINMESH::ChangeAnimSet(int index, LPD3DXANIMATIONCONTROLLER pAC)
{
	if(m_pD3dxMesh == NULL)	return;		
	m_pD3dxMesh->ChangeAnimSet(index, pAC);
}

// �A�j���[�V�����Z�b�g�̐؂�ւ�(�J�n�t���[���w��\��).
void clsD3DXSKINMESH::ChangeAnimSet_StartPos(int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC)
{
	if (m_pD3dxMesh == NULL)	return;
	m_pD3dxMesh->ChangeAnimSet_StartPos(index, dStartFramePos, pAC);
}

// �A�j���[�V������~���Ԃ��擾.
double clsD3DXSKINMESH::GetAnimPeriod(int index)
{
	if (m_pD3dxMesh == NULL) {
		return 0.0f;
	}
	return m_pD3dxMesh->GetAnimPeriod(index);
}

// �A�j���[�V���������擾.
int clsD3DXSKINMESH::GetAnimMax(LPD3DXANIMATIONCONTROLLER pAC)
{
	if (m_pD3dxMesh != NULL) {
		return m_pD3dxMesh->GetAnimMax(pAC);
	}
	return -1;
}

// �w�肵���{�[�����(�s��)���擾����֐�.
bool clsD3DXSKINMESH::GetMatrixFromBone(char* sBoneName, D3DXMATRIX* pOutMat)
{
	if (m_pD3dxMesh != NULL) {
		if (m_pD3dxMesh->GetMatrixFromBone(sBoneName, pOutMat)) {
			return true;
		}
	}
	return false;
}

// �w�肵���{�[�����(���W)���擾����֐�.
bool clsD3DXSKINMESH::GetPosFromBone(char* sBoneName, D3DXVECTOR3* pOutPos)
{
	if (m_pD3dxMesh != NULL) {
		D3DXVECTOR3 tmpPos;
		if (m_pD3dxMesh->GetPosFromBone(sBoneName, &tmpPos)) {

			D3DXMATRIX mWorld, mScale, mTran;
			D3DXMATRIX mRot, mYaw, mPitch, mRoll;
			D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);
			D3DXMatrixRotationY(&mYaw, m_fYaw);
			D3DXMatrixRotationX(&mPitch, m_fPitch);
			D3DXMatrixRotationZ(&mRoll, m_fRoll);
			D3DXMatrixTranslation(&mTran, tmpPos.x, tmpPos.y, tmpPos.z);

			mRot = mYaw * mPitch * mRoll;
			mWorld = mTran * mScale * mRot;

			pOutPos->x = mWorld._41 + m_vPos.x;
			pOutPos->y = mWorld._42 + m_vPos.y;
			pOutPos->z = mWorld._43 + m_vPos.z;

			return true;
		}
	}
	return false;
}