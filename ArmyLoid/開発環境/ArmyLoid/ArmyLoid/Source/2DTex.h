#ifndef UI_D_H
#define UI_D_H

#include "cCommon.h"

#define ALIGN16 _declspec(align(16))

//-----------------------------------------------
//	�\����.
//-----------------------------------------------
//�ݽ����ޯ̧�̱��ؑ��̒�`(Simple.hlsl).
//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
struct SPRITESHADER_CBUFFER
{
	ALIGN16 D3DMATRIX	mW;					//���[���h�s��.
	ALIGN16 float		fViewPortWidth;		//�r���[�|�[�g��.
	ALIGN16 float		fViewPortHeight;	//�r���[�|�[�g����
	ALIGN16 float		fAlpha;				//�A���t�@�l(���߂Ŏg�p����).
	ALIGN16 D3DXVECTOR2	vUV;				//UV���W.

};
//���_�̍\����.
struct SpriteVertex
{
	D3DXVECTOR3 vPos;	//�ʒu.
	D3DXVECTOR2 vTex;	//ø������W(UV���W).
};

//���A�����\����(float�^).
struct WHSIZE_FLOAT
{
	float w;
	float h;
};

struct SPRITE_STATE
{
	float			Width;		//��.
	float			Height;		//����.
	float			U;			//U���W.
	float			V;			//V���W.
	WHSIZE_FLOAT	Disp;		//�\��������.
	WHSIZE_FLOAT	Base;		//���摜�̕�����.
	WHSIZE_FLOAT	Stride;		//�P�ϓ�����̕�����.

};
class cls2DTex :public clsCommon
{
public:

	D3DXVECTOR3		m_vPos;	

	int				m_AnimCount;	//�J�E���^
	bool			m_DispFlag;		//�\���t���O

	float			m_Alpha = 1.0f;		//��̧�l(�ő�l��1.0).
	POINTFLOAT		m_PatternNo;//Ͻ�ڔԍ�.

	cls2DTex();
	~cls2DTex();

	HRESULT Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11,
				const char* fileName,const SPRITE_STATE& ss);
	HRESULT InitShader();
	HRESULT InitTex(float Width,float Height,float U,float V);
	//�e�N�X�`���쐬.
	HRESULT	CreateTexture(const char* fileName, ID3D11ShaderResourceView** pTexture);

	void Render(SPRITE_STATE &ss, POINTFLOAT &PatNo);


	void AttachTex(cls2DTex* pTex);

	void DetatchTex();

	void TexRender(SPRITE_STATE &ss,POINTFLOAT &PatNo);


private:

	ID3D11VertexShader* m_pVertexShader;	//���_�����.
	ID3D11InputLayout*	m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*	m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*		m_pConstantBuffer;	//�ݽ����ޯ̧.
	ID3D11Buffer*		m_pVertexBuffer;	//���_�ޯ̧.

	ID3D11SamplerState*	m_pSampleLinear;	//ø����̻����.
	// ø����Ɋe��̨����������.
	ID3D11ShaderResourceView* m_pTexture;	//ø���.

	cls2DTex*			m_pStaticTex;
};
#endif