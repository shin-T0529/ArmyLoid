#include "Charactor.h"

clsChara::clsChara()
	:m_pStaticMesh(NULL)
	, m_pStaticSkinMesh(NULL)
	, m_vPos(0.0f,0.0f,0.0f)
	, m_vRot(0.0f, 0.0f, 0.0f)
	, m_vCRot(0.0f, 0.0f, 0.0f)
	, m_Yaw(0.0f),m_Pitch(0.0f),m_Roll(0.0f)
	, m_Scale(1.0f)
	, m_ScaleSize(1.0f)
	, m_SSphere()
	//HP.
	, m_HP(100), m_DamageHP(m_HP)
	//����p�t���O.
	, m_DeadFlag(false), m_Stopping(false)
	//�A�j���[�V����.
	//, ActState(0), ActState2(0)
{

}

clsChara::~clsChara()
{

}

void clsChara::Update()
{
	if (m_pStaticMesh == NULL)
	{ return; }
	m_pStaticMesh->m_Scale = m_Scale;
	m_pStaticMesh->m_Yaw = m_vRot.y;
	m_pStaticMesh->m_Pitch = m_vRot.x;
	m_pStaticMesh->m_Roll = m_vRot.z;
	m_pStaticMesh->m_vPos = m_vPos;
}
void clsChara::UpdateSkin()
{
	if (m_pStaticSkinMesh == NULL)
	{
		return;
	}
	m_pStaticSkinMesh->m_vScale = m_SkinScale;
	m_pStaticSkinMesh->m_fYaw = m_vRot.y;
	m_pStaticSkinMesh->m_fPitch = m_vRot.x;
	m_pStaticSkinMesh->m_fRoll = m_vRot.z;
	m_pStaticSkinMesh->m_vPos = m_vPos;
}

void clsChara::Render(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye)
{
	if (m_pStaticMesh == NULL)
	{
		return;
	}
	//�X�V.
	Update();
	//���b�V���t�@�C���̃����_�����O�֐����Ă�,
	m_pStaticMesh->Render(mView, mProj, vLight, vEye);
}
void clsChara::SkinRender(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye)
{
	if (m_pStaticSkinMesh == NULL)
	{
		return;
	}
	//�X�V.
	UpdateSkin();
	//���b�V���t�@�C���̃����_�����O�֐����Ă�,
	m_pStaticSkinMesh->Render(mView, mProj, vLight, vEye,0);
}

//���f���f�[�^�󂯓n��(�֘A�t��).
void clsChara::AttachModel(clsDx9Mesh* pModel)
{
	if (pModel != NULL)
	{
		m_pStaticMesh = pModel;
	}
}
void clsChara::AttachSkinModel(clsD3DXSKINMESH* pSkinModel)
{
	if (pSkinModel != NULL)
	{
		m_pStaticSkinMesh = pSkinModel;
	}
}

//���f���f�[�^�֘A�t������.
void clsChara::DetatchModel()
{
	if (m_pStaticMesh != NULL)
	{
		m_pStaticMesh = NULL;
	}
}
void clsChara::DetatchSkinModel()
{
	if (m_pStaticSkinMesh != NULL)
	{
		m_pStaticSkinMesh = NULL;
	}
}

//�o�E���f�B���O�X�t�B�A(����p��)�쐬.
HRESULT clsChara::InitSphere(clsDx9Mesh* pMesh)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;//���_�ޯ̧.
	void*		pVertices = NULL;//���_.
	D3DXVECTOR3	vCenter;	//���S.
	float		Radius;		//���a.

	if (pMesh == NULL){	pMesh = m_pStaticMesh; }

	//���_�ޯ̧���擾.
	if (FAILED(pMesh->m_pMesh->GetVertexBuffer(&pVB))){	return E_FAIL; }
	//ү���̒��_�ޯ̧��ۯ�����.
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}

	//ү���̊O�ډ~�̒��S�Ɣ��a���v�Z����.
	D3DXComputeBoundingSphere((D3DXVECTOR3*)pVertices,pMesh->m_pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(pMesh->m_pMesh->GetFVF()),	&vCenter,&Radius);

	//��ۯ�.
	pVB->Unlock();
	SAFE_RELEASE(pVB);

	//���S�Ɣ��a���\���̂ɐݒ�.
	pMesh->m_Sphere.vCenter = vCenter;
	pMesh->m_Sphere.Radius = Radius;

	//����p�ɋL�^.
	m_SSphere.vCenter = pMesh->m_Sphere.vCenter;
	m_SSphere.Radius = pMesh->m_Sphere.Radius;

	return S_OK;
}

//�o�E���f�B���O�{�b�N�X(����p��)�쐬.
HRESULT	clsChara::InitBBox(clsDx9Mesh* pMesh)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	VOID* pVertices = NULL;
	D3DXVECTOR3 Max, Min;
	if (pMesh == NULL){	pMesh = m_pStaticMesh; }

	//ү���̒��_�o�b�t�@�����b�N����.
	if (FAILED(pMesh->m_pMesh->GetVertexBuffer(&pVB)))
	{
		MessageBox(NULL, "���_�o�b�t�@�擾���s", "error", MB_OK);
		return E_FAIL;
	}
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		MessageBox(NULL, "���_�o�b�t�@���b�N���s", "error", MB_OK);
		return E_FAIL;
	}
	
	//���b�V�����̒��_�ʒu�̍ő�ƍŏ�����������
	D3DXComputeBoundingBox(
		(D3DXVECTOR3*)pVertices,pMesh->m_pMesh->GetNumVertices(),D3DXGetFVFVertexSize(pMesh->m_pMesh->GetFVF()),
		&Min, &Max);


	pVB->Unlock();//�A�����b�N.
	SAFE_RELEASE(pVB);

	m_BBox.vPosMax = Max;
	m_BBox.vPosMin = Min;
	m_BBox.vScale = m_Scale;

	//���f���̃T�C�Y�ōő�l�E�ŏ��l�𐧌�����.
	m_BBox.vPosMax.x = m_BBox.vPosMax.x * m_Scale;
	m_BBox.vPosMin.x = m_BBox.vPosMin.x * m_Scale;
	m_BBox.vPosMax.y = m_BBox.vPosMax.y * m_Scale;
	m_BBox.vPosMin.y = m_BBox.vPosMin.y * m_Scale;
	m_BBox.vPosMax.z = m_BBox.vPosMax.z * m_Scale;
	m_BBox.vPosMin.z = m_BBox.vPosMin.z * m_Scale;

	//���x�N�g���E���̒���(���̏ꍇ�A�{�b�N�X�̊e���a)������������.
	m_BBox.LengthX = (Max.x - Min.x) / 2.0f;
	m_BBox.LengthY = (Max.y - Min.y) / 2.0f;
	m_BBox.LengthZ = (Max.z - Min.z) / 2.0f;

	return S_OK;
}

//������.
bool clsChara::SphereCollision(clsChara* pTarget, SSPHERE *pSSphere)
{
	D3DXVECTOR3 tempPos(0.0f,0.0f,0.0f);
	float		tempRadius = 0.0f;
	if (pSSphere != NULL)
	{
		tempPos = pSSphere->vPos + m_vPos;
		tempRadius = pSSphere->Radius;
	}
	else
	{
		tempPos = m_SSphere.vPos + m_vPos;
		tempRadius = m_SSphere.Radius;
	}
	//�Q�̕��̂̒��S�Ԃ̋��������߂�.
	D3DXVECTOR3 vLength = pTarget->m_vPos - tempPos;

	//�����ɕϊ�����.
	float Length = D3DXVec3Length(&vLength);

	//�Q�̕��̂̋������A�Q�̕��̂̔��a�����������̂�菬�����Ƃ������Ƃ�.
	//�̨����m���d�Ȃ��Ă���(�Փ˂��Ă���)�Ƃ�������.
	if (Length <= tempRadius + pTarget->m_SSphere.Radius)
	{
		return true;//�Փ�.
	}
	return false;//�Փ˂��Ă��Ȃ�.
}

//������.
bool clsChara::BBoxCollision(clsChara* pTarget, BBOX *pBBox)
{
	//�U���p��2�̒��_��p�ӂ�����������.
	D3DXVECTOR3 vecAtkMax, vecAtkMin;	//�ő�A�ŏ����_.

	if (pBBox != NULL)
	{
		vecAtkMax = pBBox->vPosMax + m_vPos;
		vecAtkMin = pBBox->vPosMin + m_vPos;
	}
	else
	{
		vecAtkMax = m_BBox.vPosMax + m_vPos;
		vecAtkMin = m_BBox.vPosMin + m_vPos;
	}

	//�ڕW�p�p��2�̒��_��p�ӂ�����������.
	D3DXVECTOR3 vecTrgMax, vecTrgMin;	//�ő�A�ŏ����_.
	vecTrgMax = pTarget->m_BBox.vPosMax + pTarget->m_vPos;
	vecTrgMin = pTarget->m_BBox.vPosMin + pTarget->m_vPos;

	//2�̃��b�V�����ꂼ��̍ő�A�ŏ��ʒu���g��.
	//4�̏�񂪂���΁A�Փ˂����o�ł���.
	if( vecAtkMin.x<vecTrgMax.x &&vecAtkMax.x>vecTrgMin.x
	  &&vecAtkMin.y<vecTrgMax.y &&vecAtkMax.y>vecTrgMin.y
	  &&vecAtkMin.z<vecTrgMax.z &&vecAtkMax.z>vecTrgMin.z)
	{
		return true;	//�Փ˂��Ă���.
	}
	return false;		//�Փ˂��Ă��Ȃ�.
}

//��������.
bool clsChara::RayCollsion(clsChara* pAttacker, clsChara* pTarget)
{
	FLOAT	fDistance;	//����.

	//���݈ʒu�̃R�s�[.
	pAttacker->m_vRay = pAttacker->m_vPos;
	//���C�̍��������@�̈ʒu����ɂ���.
	pAttacker->m_vRay.y = pAttacker->m_vPos.y + 3.0f;
	//���x�N�g���͐����ŉ�����.
	pAttacker->m_vAxis = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	bHit = Intersect(pAttacker, pTarget, &fDistance, &vIntersect);

	if (bHit == true)
	{
		return true;
	}
	return false;


	//��_�̍��W����y���W�Ƃ��ăZ�b�g.
	pAttacker->m_vPos.y = vIntersect.y + 1.0f;
	return bHit;

 }

bool clsChara::Intersect(
	clsChara*	 pAttacker,		//��̕���.
	clsChara*	 pTarget,		//�Ώۂ̕���.
	float*		 pfDistance,	//(�o��)����.
	D3DXVECTOR3* pIntersect)	//(�o��)�������W.
{
	D3DXMATRIXA16	matRot;		//��]�s��.

	//��]�s����v�Z.
	D3DXMatrixRotationY(&matRot, pAttacker->m_Yaw);

	//���x�N�g���̗p��.
	D3DXVECTOR3	vecAxisZ;

	//Z�x�N�g�����̂��̂����݂̉�]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord
	(&vecAxisZ, &pAttacker->m_vAxis, &matRot);

	D3DXVECTOR3	vecStart, vecEnd, vecDistance;
	vecStart = vecEnd = pAttacker->m_vRay;	//���C�̈ʒu��ݒ�.

	vecEnd += vecAxisZ * 1.0f;

	//��L�����̍��W�ɉ�]���W����������.
	//�t�����p����ΐ��������C��������.
	D3DXMATRIX matWorld, matScale, matTrans;
	D3DXMATRIX matYaw, matPitch, matRoll;
	D3DXMatrixScaling(&matScale,
		pTarget->m_Scale, pTarget->m_Scale, pTarget->m_Scale);

	D3DXMatrixRotationY(&matYaw, pTarget->m_Yaw);
 	D3DXMatrixRotationX(&matPitch, pTarget->m_Pitch);
	D3DXMatrixRotationZ(&matRoll, pTarget->m_Roll);

	D3DXMatrixTranslation(&matTrans,
		pTarget->m_vPos.x, pTarget->m_vPos.y, pTarget->m_vPos.z);


	//matWorld =
	//	matScale * matYaw * matPitch * matRoll * matTrans;

	matWorld= matScale * matYaw * matTrans;

	//�t�s������߂�.
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(&vecStart, &vecStart, &matWorld);
	D3DXVec3TransformCoord(&vecEnd, &vecEnd, &matWorld);

	//���������߂�.
	vecDistance = vecEnd - vecStart;

	BOOL	bHit = false;//�����t���O.

	DWORD	dwIndex = 0;	//�C���f�b�N�X�ԍ�.
	D3DXVECTOR3 vVertex[3];	//���_���W.
	FLOAT U = 0, V = 0;		//(out)�d�S�q�b�g���W.

	//���C�ƃ��b�V���̌����𒲂ׂ�.
		D3DXIntersect(pTarget->m_pStaticMesh->m_pMeshForRay,	//�Ώۂ̃��b�V��.
			&vecStart,							//���C�̊J�n�ʒu.
			&vecDistance,						//���C�̋���.
			&bHit,								//(out)���茋��
			&dwIndex,							//(out)bHit��true���A
			//���C�̎��_�ɍł��߂��̖ʂ̲��ޯ���l�ւ̃|�C���^.
			&U, &V,								//(out)�d�S�q�b�g���W.
			pfDistance,							//�^�[�Q�b�g�Ƃ̋���.
			NULL, NULL);


	if (bHit == TRUE)
	{
		//������.
		FindVerticesOnPoly
		(pTarget->m_pStaticMesh->m_pMeshForRay, dwIndex, vVertex);
		//�d�S���W�����_���Z�o����.
		//���[�J����_��v0 + U*(v1-v0)+v*(v2-v0)�ŋ��߂���/
		*pIntersect = vVertex[0]
			+ U * (vVertex[1] - vVertex[0])
			+ V * (vVertex[2] - vVertex[0]);

		D3DXMATRIX matHit;
		D3DXMatrixTranslation(&matHit,
			pIntersect->x, pIntersect->y, pIntersect->z);

		//matWorld =
		//	matHit * matScale * matYaw * matPitch * matRoll * matTrans;

		matWorld = matHit * matScale * matYaw * matTrans;

		pIntersect->x = matWorld._41;
		pIntersect->y = matWorld._42;
		pIntersect->z = matWorld._43;

		return true;
	}
	return false;
}


//�����ʒu�̃|���S���̒��_��������.
//(���_���W�̓��[�J�����W)
HRESULT	clsChara::FindVerticesOnPoly(
	LPD3DXMESH	pMeshForRay, DWORD	dwPolyIndex,
	D3DXVECTOR3* pVecVertices)
{
	//���_���̃o�C�g�����擾.
	DWORD dwStride = pMeshForRay->GetNumBytesPerVertex();
	//���_�����擾����.
	DWORD dwVertexAmt = pMeshForRay->GetNumVertices();
	//�ʐ��擾.
	DWORD dwPolyAmt = pMeshForRay->GetNumFaces();

	WORD* pwPoly = NULL;

	//�C���f�b�N�X�o�b�t�@�����b�N(�Ǎ����[�h)
	pMeshForRay->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pwPoly);
	BYTE* pbVertices = NULL;
	FLOAT* pfVertices = NULL;
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;
	pMeshForRay->GetVertexBuffer(&VB);

	if (SUCCEEDED(VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		//�|���S���̒��_�̂ЂƂڂ��擾.
		pfVertices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pVecVertices[0].x = pfVertices[0];
		pVecVertices[0].y = pfVertices[1];
		pVecVertices[0].z = pfVertices[2];

		//�|���S���̒��_�̂ӂ��ڂ��擾.
		pfVertices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pVecVertices[1].x = pfVertices[0];
		pVecVertices[1].y = pfVertices[1];
		pVecVertices[1].z = pfVertices[2];

		//�|���S���̒��_�݂̂��ڂ��擾.
		pfVertices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pVecVertices[2].x = pfVertices[0];
		pVecVertices[2].y = pfVertices[1];
		pVecVertices[2].z = pfVertices[2];

		//���b�N����.
		pMeshForRay->UnlockIndexBuffer();
		VB->Unlock();

	}

	VB->Release();

	return S_OK;

}



//�ǔ���.
void clsChara::WallJudge(clsChara* pTackle, clsChara* pWall, float WSPACE)
{
	FLOAT		fDistance[4];	//����.
	D3DXVECTOR3	vIntersect[4];	//��_���W.
	float		fDis, fYaw;		//�����Ɖ�].

	pTackle->m_vRay = pTackle->m_vPos;
	pTackle->m_vRay.y = 0.0f;	//ڲ��y�ʒu�𒲐�(�ǂ��o���邾���Ⴍ���ĕǊ����������߂ɒ��).

	//ڲ�̌����ɂ�蓖����ǂ܂ł̋��������߂�.
	pTackle->m_vAxis = D3DXVECTOR3(0.0f, 0.0f, 3.0f);	//���޸�ّO.
	Intersect(pTackle, pWall, &fDistance[0], &vIntersect[0]);
	pTackle->m_vAxis = D3DXVECTOR3(0.0f, 0.0f, -3.0f);	//���޸�ٌ�.
	Intersect(pTackle, pWall, &fDistance[1], &vIntersect[1]);
	pTackle->m_vAxis = D3DXVECTOR3(3.0f, 0.0f, 0.0f);	//���޸�ىE.
	Intersect(pTackle, pWall, &fDistance[2], &vIntersect[2]);
	pTackle->m_vAxis = D3DXVECTOR3(-3.0f, 0.0f, 0.0f);	//���޸�ٍ�.
	Intersect(pTackle, pWall, &fDistance[3], &vIntersect[3]);

#ifdef _DEBUG
	for (int i = 0; i < 4; i++)
	{
		m_tagHitPos.vPos[i] = vIntersect[i];
	}
#endif // _DEBUG

	fYaw = fabsf(pTackle->m_Yaw);	//fabs:��Βl(flaot��).
	dirCheck(&fYaw);	//0�`2�΂̊ԂɎ��߂�.

	//�O���ǂɐڋ�.
	fDis = fDistance[0];
	if (0.01f < fDis && fDis < WSPACE)
	{
		//���v���.
		if (pTackle->m_Yaw < 0.0f)
		{
			if (0.785f <= fYaw && fYaw < 2.355f) 
			{		//�E����.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f) 
			{	//�O����.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f)
			{	//������.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else
			{	//������.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
		}
		//�����v���.
		else {
			if (0.785f <= fYaw && fYaw < 2.355f)
			{		//�E����.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f)
			{	//�O����.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) 
			{	//������.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else
			{	//������.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
		}
	}

	//��낪�ǂɐڋ�.
	fDis = fDistance[1];
	if (0.01f < fDis && fDis < WSPACE) 
	{
		//���v���.
		if (pTackle->m_Yaw < 0.0f) 
		{
			if (0.785f <= fYaw && fYaw < 2.355f)
			{		//�E����.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f)
			{	//�O����.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) 
			{	//������.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else 
			{										//������.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
		}
		//�����v���.
		else 
		{
			if (0.785f <= fYaw && fYaw < 2.355f)
			{		//�E����.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f)
			{	//�O����.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f)
			{	//������.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else
			{	//������.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
		}
	}

	//�E���ǂɐڋ�.
	fDis = fDistance[2];
	if (0.01f < fDis && fDis < WSPACE) 
	{
		//���v���.
		if (pTackle->m_Yaw < 0.0f) {
			if (0.785f <= fYaw && fYaw < 2.355f) {		//�E����.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f) {	//�O����.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) {	//������.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else {										//������.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
		}
		//�����v���.
		else {
			if (0.785f <= fYaw && fYaw < 2.355f) {		//�E����.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f) {	//�O����.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) {	//������.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else {										//������.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
		}
	}

	//�����ǂɐڋ�.
	fDis = fDistance[3];
	if (0.01f < fDis && fDis < WSPACE)
	{
		//���v���.
		if (pTackle->m_Yaw < 0.0f) {
			if (0.785f <= fYaw && fYaw < 2.355f) {		//�E����.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f) {	//�O����.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) {	//������.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else {										//������.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
		}
		//�����v���.
		else {
			if (0.785f <= fYaw && fYaw < 2.355f) {		//�E����.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f) {	//�O����.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) {	//������.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else {										//������.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
		}
	}

}

//��]�l����.
//(Yaw��1���ȏサ�Ă��鎞�̒���).
void clsChara::dirCheck(float *fYaw)
{
	if (*fYaw > static_cast<float>(D3DX_PI * 2.0f)) {
		//1���ȏサ�Ă���.
		*fYaw -= static_cast<float>(D3DX_PI * 2.0f);//2��(360��)������.
	}

	//�ċA�֐�.
	if (*fYaw > static_cast<float>(D3DX_PI * 2.0f)) {
		dirCheck(fYaw);
	}
}

