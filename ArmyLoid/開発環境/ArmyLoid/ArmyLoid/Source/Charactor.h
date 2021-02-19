#ifndef CHARACTOR_H
#define CHARACTOR_H

#include "DX9Mesh.h"
#include "ED3DXSKINMESH.h"
#include "DebugText.h"
#include "Global.h"
#include "DInput.h"

enum Anim
{	//���͉�.
	enStartUp				= 0,			//�N��.
	enWait					= 1,			//�ҋ@.

	enActionHirumi			= 2,			//�U���q�b�g���̂Ђ��.

	enAttackMode			= 3,			//�ߐڍU��.
	enShotMissile			= 4,			//�~�T�C������.
	enActionGuard			= 5,
	enActionGuardDuring		= 6,

	enHoverFlontStart		= 7,			//�O�i.
	enHoverFlontDuring		= 8,
	enHoverFlontEnd			= 9,

	enHoverRightStart		= 10,			//�E�ړ�.
	enHoverRightDuring		= 11,
	enHoverRightEnd			= 12,

	enHoverLeftStart		= 13,			//���ړ�.
	enHoverLeftDuring		= 14,
	enHoverLeftEnd			= 15,

	enHoverBackStart		= 16,			//���.
	enHoverBackDuring		= 17,
	enHoverBackEnd			= 18,

	
	enActionDown			= 19,			//�̗�0�̎�.
	enActionDownEnd			= 20,

	enWinnerPause			= 21,			//�����|�[�Y.
	enWinnerPauseEnd		= 22,

	enFreaze				= 23,
	enResetState,	//�ő�l�ݒ�.
};
//�L�����N���X.
class clsChara
{
public:
	struct HIT_POS
	{
		D3DXVECTOR3 vPos[4];
		HIT_POS()
			: vPos()
		{}
	};

	clsChara();
	virtual ~clsChara();

	//�g�����]�ȂǍ��W�̒l�����f���f�[�^�ɔ��f.
	void Update();
	void UpdateSkin();

	//----------------���f���֘A-----------------.
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye);
	void SkinRender(D3DXMATRIX &mView, D3DXMATRIX &mProj, 
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye);

	//���f���f�[�^�󂯓n��(�֘A�t��).
	void AttachModel(clsDx9Mesh* pModel);
	void AttachSkinModel(clsD3DXSKINMESH* pSkinModel);

	//���f���f�[�^�֘A�t������.
	void DetatchModel();
	void DetatchSkinModel();

	//------------------------------------------.


	void WallJudge(clsChara* pTackle, clsChara* pWall,float WSPACE);

	void dirCheck(float *fYaw);


	//-----------------�����蔻��-------------------.
	//������.
	bool SphereCollision(clsChara* pTarget, SSPHERE *pSSphere = NULL);
	//������.
	bool BBoxCollision(clsChara* pTarget, BBOX *pBBox = NULL);
	//�����Փ˔���.
	bool RayCollsion(clsChara* pAttacker, clsChara* pTarget);
	bool Intersect(clsChara* pAttacker, clsChara* pTarget, float* pfDistance, D3DXVECTOR3* pIntersect);

	//---------------------------------------------.

	HRESULT	FindVerticesOnPoly(LPD3DXMESH pMeshForRay, 
		DWORD dwPolyIndex, D3DXVECTOR3* pVecVertices);


	//Init�EAction�͊e�I�u�W�F�N�g�Ɋ��蓖�Ă�.
	virtual void Init() = 0;
	virtual void Action() = 0;

	virtual void Init_NotColl() {};
	virtual void Action2(){};


/*************�A�j���[�V����**************/
public:

private:
	D3DXMATRIX Target_Yaw;

public:
	double animTime;				//��Ұ��ݎ���.

	/********�ݒ�֐�*********/
	//���W�ݒ�.
	void SetPosition(D3DXVECTOR3 &vPos){ m_vPos = vPos; }
	//�傫���ݒ�.
	void SetScale(float Scale){ m_Scale = Scale; }
	//�X�L�����b�V���̑傫���ݒ�.
	void SetSkinSize(D3DXVECTOR3 SkinScale){ m_SkinScale = SkinScale; }
	//��]�l�ݒ�(x:Pitch y:Yaw z:Roll).
	void SetRot(D3DXVECTOR3 &vRot){ m_vRot = vRot; }

	/********�擾�֐�(x���f��)*********/
	clsDx9Mesh* GetStaicMesh(){ return m_pStaticMesh; }

	//�ʂłق����Ƃ���GetPos().x or y or z�łƂ��.
	D3DXVECTOR3 GetPos(){ return m_vPos; }
	//����.
	D3DXVECTOR3 GetRot() { return m_vRot; }
	//�傫���擾.
	float GetScale(){ return m_Scale; }
	//�X�L�����b�V���傫���擾.
	D3DXVECTOR3 Getm_SkinScale() { return m_SkinScale; }

	/********�擾�֐�(�X�L�����b�V�����f��)*********/

	//���̑����ʍ���.
	bool m_DeadFlag = false;
	int	 m_HP;
	int m_DamageHP;
	bool m_Stopping;

public:
	//�̨��쐬.
	HRESULT InitSphere(clsDx9Mesh* pMesh = NULL);

	//�޳��ިݸ��ޯ���쐬.
	HRESULT	InitBBox(clsDx9Mesh* pMesh = NULL);

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3	m_vRot;						//��]�l�̐ݒ�.
	D3DXVECTOR3 m_vCRot;					//��]�l��r�p.
	float		m_ScaleSize;				//�f�[�^�ǂݍ��ݎ��̃��f���T�C�Y����p.

	float		m_Yaw;
	float		m_Pitch;					//Intersect�֐��Ŏg�p.
	float		m_SaveYaw[10];

	D3DXVECTOR3	vIntersect;					//�������W(���C����) �e�̈ʒu�ɂ��g�p.

	HIT_POS		m_tagHitPos;

	int			ActState = 0;
	int			ActState2 = 0;

protected:
	//�����p���f���錾.
	clsDx9Mesh*			m_pStaticMesh;
	clsD3DXSKINMESH*	m_pStaticSkinMesh;

	float				m_Scale;			//�傫��.
	float				m_Roll;
	bool				bHit;				//����������������.
	D3DXVECTOR3			m_SkinScale;
	SSPHERE				m_SSphere;			//���萶���p.
	BBOX				m_BBox;				//����.

	D3DXVECTOR3			m_vRay;				//ڲ�̈ʒu.
	D3DXVECTOR3			m_vAxis;			//��]��.


};



#endif