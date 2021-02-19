#ifndef DEBUG_TEXT_H
#define DEBUG_TEXT_H
//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���. 4005:�Ē�`.
#pragma warning( disable : 4005 )
#pragma warning(disable:4018)

#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//�uD3DX�`�v�̒�`�g�p���ɕK�v.

#pragma warning(disable:4005)

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )


//���ޯ��÷�ĸ׽.
class clsDebugText
{
public:
	//�ݽ����ޯ̧�\����.
	struct TEXT_CBUFFER
	{
		D3DXMATRIX	mWVP;
		D3DXVECTOR4	vColor;
		D3DXVECTOR4	Alpha;	//x�l�����g��Ȃ�.
	};
	//÷�Ă̈ʒu���.
	struct TextVertex
	{
		D3DXVECTOR3	vPos;	//�ʒu.
		D3DXVECTOR2	vTex;	//ø������W.
	};


	clsDebugText();		//�ݽ�׸�.
	~clsDebugText();	//�޽�׸�.

	HRESULT Init(ID3D11DeviceContext* pContext,
		DWORD Width, DWORD Height,
		float Size, D3DXVECTOR4 vColor);

	//����(��̧������)�ݒ�̐؂�ւ�.
	void SetBlend(bool bFlag);

	//�����ݸފ֐�.
	void Render(char* text, float x, float y);
	//̫�������ݸފ֐�.
	void RenderFont(int FontIndex, float x, float y);

private:
	ID3D11Device*	m_pDevice11;//���޲���޼ު��.
	ID3D11DeviceContext* m_pContext11;//���޲���÷��.

	ID3D11VertexShader* m_pVertexShader;	//���_�����.
	ID3D11InputLayout*	m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*	m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*		m_pConstantBuffer;	//�ݽ����ޯ̧.

	ID3D11Buffer*		m_pVertexBuffer[100];	//���_�ޯ̧(100��).

	ID3D11SamplerState*	m_pSampleLinear;		//ø����̻����.
	ID3D11ShaderResourceView* m_pAsciiTexture;	//ø���.

	ID3D11BlendState*	m_pBlendState;	//�����޽ð�.

	DWORD	m_WindowWidth;	//����޳��.
	DWORD	m_WindowHeight;	//����޳����.

	float	m_Kerning[100];	//���ݸ�(100��).
	float	m_Scale;		//�g�k�l(25pixel��� 25pixel=1.0).
	float	m_Alpha;		//���ߒl.
	D3DXVECTOR4	m_vColor;	//�F.

	D3DXMATRIX m_mView;	//�ޭ��s��.
	D3DXMATRIX m_mProj;	//��ۼު���ݍs��.
};


#endif//#ifndef DEBUG_TEXT_H