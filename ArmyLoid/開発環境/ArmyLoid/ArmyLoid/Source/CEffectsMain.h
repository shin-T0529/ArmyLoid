#ifndef C_EFFECTS_MAIN_H
#define C_EFFECTS_MAIN_H


#include"CEffects.h"

struct stEffectState
{
	Effekseer::Handle handle;
	clsEffects* m_pEffects;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vRot;
	void Init()
	{
		handle = -1;
		m_pEffects=nullptr;
		m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}
};



/*****************************/
/*�G�t�F�N�g�I�u�W�F�N�g�N���X*/
/******************************/
class clsEffectsMain
{
public:
	clsEffectsMain();
	~clsEffectsMain();
	//�����X�V
	//���g�k���]�A���W�̒l�����f���f�[�^�ɔ��f
	
	void Start_up()
	{
		if (m_pEffects != nullptr)
		{
			//�̪�Ă��ƂɕK�v�������.
			m_pEffects->handle = m_pEffects->m_pEffects->Play(m_pEffects->m_vPos);
			m_pEffects->m_pEffects->SetRotation(m_pEffects->handle, m_pEffects->m_vRot);
		}
	}


	//�����_�����O�֐�
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj
		, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye)
	{
		if (m_pEffects != nullptr)
		{
			//Effekseer�����ݸ�.
			m_pEffects->m_pEffects->Render(mView, mProj, vEye);
		}
	}
	//���f���f�[�^�֘A�t���֐�
	void AttachEffect(clsEffects* pEffect)
	{
		if (pEffect != NULL)
		{
			m_pEffects->m_pEffects = pEffect;
		}
	}
	//���f���f�[�^�֘A�t�������֐�
	void DettachEffect()
	{
		if (m_pEffects != NULL)
		{
			m_pEffects = NULL;
		}
	}
	//���W�ݒ�֐�
	void SetPosition(D3DXVECTOR3 &vPos)
	{
		m_pEffects->m_vPos = vPos;
	}
	//���W�擾�֐�
	D3DXVECTOR3 GetPosition()
	{
		return m_pEffects->m_vPos;
	}
	//��]�ݒ�֐�
	void SetRotation(D3DXVECTOR3 &vRot)
	{
		m_pEffects->m_vRot = vRot;
	}
	//���W�擾�֐�
	D3DXVECTOR3 GetRotation()
	{
		return m_pEffects->m_vRot;
	}
private:
	stEffectState* m_pEffects;
};


#endif//#ifndef C_CHARACTER_H
