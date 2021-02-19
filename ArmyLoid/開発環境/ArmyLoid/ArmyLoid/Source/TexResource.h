#ifndef TEXRESOURCE_H
#define TEXRESOURCE_H

#include <vector>
#include "2DTex.h"
#include "C_LoadData.h"
enum enStatic2DTex
{//�����ꃊ�X�g�����O���f�[�^�ɂ��邱��.
	//*************�V�[���E�V�[��UI*************//
	enScreenDark,			//�Ö�.
	enScreenWhite,			//����.

	enSceneTexTitile,		//�^�C�g��
	enSceneUITexMenu,		//�Q�[���I���{�^��.
	enSceneUITexCurSor,		//���j���[����O�̃{�^��.
	enSceneUITexStart,		//�A���J�n�h�~�p.

	enSceneTexManual,		//�������.
	enSceneTexManualRule,	//���[������.
	enSceneUITexReady,		//��������E�m�F�����{�^��.
	enSceneUITexOK,			//����.
	enSceneUIP_1,
	enSceneUIP_2,

	enSceneTex1PWin,		//1P����.
	enSceneTex2PWin,		//2P����.
	enSceneTexDraw,			//��������.
	enSceneUIRetry,			//���g���C.
	enSceneUIExit,			//�I��.
	enSceneUISelectCursor,	//�I���A�C�R��.
	enSceneUIDecCursor,		//�`�F�b�N�}�[�N.

	enSceneTexCredit,		//�N���W�b�g���.
	enSceneTexCreditBack,	//�N���W�b�g�w�i.

	//*************���C��UI*************//

	//HP.
	enUITexHP1_FINE,
	enUITexHP_CAUTION,		//�d�l�ύX�ɔ����摜���ύX.
	enUITexHP_DAMAGE,
	enUITexHP1_Bar,

	enUITexHP2_FINE,
	enUITexHP2_Bar,

	//��.
	enUITexNamePrate1P,
	enUITexNamePrate2P,

	//�u�[�X�g�Q�[�W.
	enUITex1Boost,				//�u�[�X�g�Q�[�W���g.
	enUITex2Boost,				//�u�[�X�g�Q�[�W���g.
	enUITex1Gauge,				//�Q�[�W�g.
	enUITex2Gauge,				//�Q�[�W�g.

	enUITexGaugeHeat,			//�I�[�o�[�q�[�g��.
	enUITexHeat,				//�I�[�o�[�q�[�g���Q�[�W���g.

	//�����ꗗ.
	enUIShoot1PMaWeponTex,		//�����ꗗ�K�g�����O1P.
	enUIShoot1PMiWeponTex,		//�����ꗗ�~�T�C��1P.
	enUIShoot2PMaWeponTex,		//�����ꗗ�K�g�����O2P.
	enUIShoot2PMiWeponTex,		//�����ꗗ�~�T�C��2P.

	enUIWepon1PBack,
	enUIWepon2PBack,

	enUIWeponReloading,			//�����[�h�\��.

	//��������.
	enUITimeMi,					//��������  ��
	enUITimeSe1,				//�������ԁ@�b10.
	enUITimese2,				//�������ԁ@�b1.
	enUITimeBackTex,
	//�J�n���}�e�N�X�`��.
	enSceneUIStart,
	enSceneUIEnd,
	enSceneUIFinish,
	enSceneUISigunalBack,
	enStaticTex_Max,			//���������玩���ōő�l.
	enStaticTex_Min = 0,		//�����l�̂��� 0 �Œ�.
};

//���W�f�[�^�ێ�.
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
	//�e�N�X�`���쐬.
	HRESULT CreateStaticTex(int enTex, const char* fileName,SPRITE_STATE &ss);
	//�e�N�X�`���f�[�^�̊J��.
	HRESULT ReleaseStaticTex(int enTex);
	//�e�N�X�`���f�[�^�擾.
	cls2DTex* GetStaticTex(int enTex);
	//�e�N�X�`���f�[�^�Ǎ�.
	HRESULT LoadStaticTexAll(SPRITE_STATE &ss);
	//�e�N�X�`���̍��W�f�[�^.
	D3DXVECTOR3	TexPos(int enTex);

public:
	//�f�[�^�ێ��p.
	D3DXVECTOR3		m_tPos;
private:

	int itex = 0;			//�e�N�X�`���ԍ�(�摜No).
	int Filei = 0;			//�t�@�C���p�X�ǔԍ�.
	int Linei = 0;			//�t�@�C���p�X�s�ԍ�.

	int texno = 0;
	int Posi = 0;			//�|�W�V�����ǔԍ�.
	int PosLi = 0;			//�|�W�V�����ǔԍ�.

	bool CheckRangeMainStaticTex(int enTex);

 /*�ǂݍ��ނƂ��̊e�V�[���e�N�X�`�������̒萔.
 (�ǂ��̃e�N�X�`���œǍ�����ߕʂ̂Ƃ���ŊJ�n���邩)*/
	int TitleNo = enSceneTexTitile;
	int ManualNo = enSceneTexManual;
	int ResultNo = enSceneTex1PWin;
	int CreditNo = enSceneTexCredit;
	int MainNo = enUITexHP1_FINE;

	//�{�̃t�@�C���p�X.
	const char*		S_TexPass = "Data\\csv\\Texture\\FilePass\\SceneTexPass.csv";
	const char*		S_TexPos = "Data\\csv\\Texture\\TexPosData.csv";
private:	//�s�ϕ���.
	HWND					m_hWnd;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;
	clsLoadData*			m_Load;

	cls2DTex**				m_ppStaticTex;

	DataList				TexData;
	SceneIndex*				Index;
};

#endif // !TEXRESOURCE_H