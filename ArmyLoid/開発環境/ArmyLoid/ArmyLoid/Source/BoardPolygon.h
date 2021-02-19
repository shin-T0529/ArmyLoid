#ifndef BOARDPOLYGON_H
#define BOARDPOLYGON_H

#include "cCommon.h"
/*************************************************
*	���ײĸ׽.
**/
class clsBoardPolygon :public clsCommon
{
public://public �O�����籸���\.

	D3DXVECTOR3		m_vPos;	//�ʒu.

	int				m_AnimCount;//����.
	bool			m_DispFlag;	//�\���׸�.

	clsBoardPolygon();	//�ݽ�׸�.
	~clsBoardPolygon();	//�޽�׸�.

	//������.
	HRESULT Init(ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPSTR fileName);

	//����ލ쐬.
	HRESULT InitShader();
	//���ٍ쐬.
	HRESULT InitModel(LPSTR fileName);

	//�`��(�����ݸ�).
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vEye);


	//-----------------------------------------------
	//	�\����.
	//-----------------------------------------------
	//�ݽ����ޯ̧�̱��ؑ��̒�`(Simple.hlsl).
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct SPRITESHADER_CBUFFER
	{
		D3DXMATRIX	mWVP;	//ܰ���,�ޭ�,�ˉe�̍����ϊ��s��.
		D3DXVECTOR4	vColor;	//�װ(RGBA�̌^�ɍ��킹��).
		D3DXVECTOR4	vUV;	//UV���W.
	};
	//���_�̍\����.
	struct SpriteVertex
	{
		D3DXVECTOR3 vPos;	//�ʒu.
		D3DXVECTOR2 vTex;	//ø������W(UV���W).
	};

private:

	//�����َ�ނ��Ƃɗp��.
	ID3D11VertexShader* m_pVertexShader;	//���_�����.
	ID3D11InputLayout*	m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*	m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*		m_pConstantBuffer;	//�ݽ����ޯ̧.

	//�����ق��Ƃɗp��.
	ID3D11Buffer*		m_pVertexBuffer;	//���_�ޯ̧.

	ID3D11SamplerState*	m_pSampleLinear;	//ø����̻����.
	// ø����Ɋe��̨����������.
	ID3D11ShaderResourceView* m_pTexture;	//ø���.

	//�����޽ð�.
	ID3D11BlendState*	m_pBlendState;
};

#endif//#ifndef SPRITE_H