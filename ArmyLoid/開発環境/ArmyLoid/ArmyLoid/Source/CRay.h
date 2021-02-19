#ifndef CRAY_H
#define CRAY_H

#include "CCommon.h"	//���ʃN���X

//���C�N���X
class clsRay : public clsCommon
{
public:
	//-----------------------------------------------
	//	�\����.
	//-----------------------------------------------
	//�ݽ����ޯ̧�̱��ؑ��̒�`(Simple.hlsl).
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct RAYSHADER_CBUFFER
	{
		D3DXMATRIX	mWVP;	//ܰ���,�ޭ�,�ˉe�̍����ϊ��s��.
		D3DXVECTOR4	vColor;	//�װ(RGBA�̌^�ɍ��킹��).
	};
	//���_�\����
	struct RAY_VERTEX
	{
		D3DXVECTOR3 vPos;	//�ʒu
	};
	//���C�\����
	struct RAY
	{
		D3DXVECTOR3 vPoint[2];	//�n�_�I�_
		D3DXVECTOR3 vDirection;	//����
		D3DXVECTOR3 vPos;		//�ʒu
		FLOAT		Yaw;		//Y����]�l
		RAY()
		{ ZeroMemory(this, sizeof(RAY)); }
	};

	clsRay();	//�R���X�g���N�^
	~clsRay();	//�f�X�g���N�^

	//������
	HRESULT Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	//�V�F�[�_�[������
	HRESULT InitShader();
	//���C������
	HRESULT InitModel();
	//�`��
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj);
	//����֐�
	HRESULT Release();
	//���C�\����
	RAY					m_Ray;

private:
	//�����َ�ނ��Ƃɗp��.
	ID3D11VertexShader* m_pVertexShader;	//���_�����.
	ID3D11InputLayout*	m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*	m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*		m_pConstantBuffer;	//�ݽ����ޯ̧.

	//�����ق��Ƃɗp��.
	ID3D11Buffer*		m_pVertexBuffer;	//���_�ޯ̧.




};


#endif //#ifndef CRAY_H