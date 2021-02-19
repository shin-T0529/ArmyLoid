#ifndef DX9MESH_H
#define DX9MESH_H
//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���. 4005:�Ē�`.
#pragma warning( disable : 4005)

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>
#include <d3dx9.h>
#include "MyMacro.h"

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

//�̨��\����.
struct SSPHERE
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3	vCenter;//���S.
	float		Radius;	//���a.
	void Init()
	{
		vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Radius = 0.0f;
	}
};

//B�{�b�N�X�\����.
struct BBOX
{
	D3DXVECTOR3	vPosMax;
	D3DXVECTOR3	vPosMin;
	D3DXVECTOR3 vRot;
	D3DXVECTOR3	vAxisX;		//X��.
	D3DXVECTOR3	vAxisY;		//Y��.
	D3DXVECTOR3	vAxisZ;		//Z��.
	FLOAT		LengthX;	//x����.
	FLOAT		LengthY;	//y����.
	FLOAT		LengthZ;	//z����.
	FLOAT		vScale;
	//������.
	BBOX()
	{
		ZeroMemory(this, sizeof(BBOX));
		vAxisX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		vAxisY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vAxisZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		vScale = 0.0f;
	}

};

//����,
enum enDirectioin
{
	enDirection_Stop = 0,			//��~.
	enDirection_Forward,		//�O�i.
	enDirection_Backward,		//���.
	enDirection_Left,			//����].
	enDirection_Right,			//�E��].
	enDirection_FastBack,
	enDirection_FastFlont
};


//ү���ް���̧�ق�����o�����߂�����DirectX9���g�p����.
//�������ݸ�(�`��)�́ADirectX11�ōs��.
class clsDx9Mesh
{
public:
	//-----------------------------------------------
	//	�\����.
	//-----------------------------------------------
	//�ݽ����ޯ̧�̱��ؑ��̒�`(Mesh.hlsl).
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct MESHSHADER_CBUFFER0
	{
		D3DXMATRIX mW;		//ܰ���(�ʒu)���W.
		D3DXMATRIX mWVP;	//ܰ���,�ޭ�,�ˉe�̍����ϊ��s��.
		D3DXVECTOR4 vLightDir;//ײĕ���.
		D3DXVECTOR4	vEye;	//��׈ʒu(���_�ʒu).
	};
	struct MESHSHADER_CBUFFER1
	{
		D3DXVECTOR4	vAmbient;	//���F.
		D3DXVECTOR4 vDiffuse;	//�g�U���ːF.
		D3DXVECTOR4	vSpecular;	//���ʔ���.
	};
	//���_�̍\����.
	struct MeshVertex
	{
		D3DXVECTOR3 Pos;	//�ʒu.
		D3DXVECTOR3 Normal;	//�@��(�A�e�v�Z�ɕK�{).
		D3DXVECTOR2 Tex;	//ø������W(UV���W).
	};

	//��رٍ\����.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;	//���޴��.
		D3DXVECTOR4	Diffuse;	//�ި̭���.
		D3DXVECTOR4	Specular;	//��߷��.
		CHAR szTextureName[128];			//ø���̧�ٖ�.
		ID3D11ShaderResourceView* pTexture;	//ø���.
		DWORD dwNumFace;					//������رق���غ�ݐ�.

		//�ݽ�׸�.
		MY_MATERIAL(){
			ZeroMemory(this, sizeof(MY_MATERIAL));
		}
		//�޽�׸�.
		~MY_MATERIAL(){
			SAFE_RELEASE(pTexture);
		}
	};


	clsDx9Mesh();	//�ݽ�׸�.
	~clsDx9Mesh();	//�޽�׸�.

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPSTR fileName);
	
	//Dx9������.
	HRESULT InitDx9(HWND hWnd);
	//����֐�.
	void Release();

	//ү���Ǎ�.
	HRESULT LoadXMesh(LPSTR fileName);

	//����ލ쐬.
	HRESULT InitShader();

	//�����ݸ�(��Dx����Main���łQ���݂���̂Œ���).
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye);

	//����(��̧������)�ݒ�̐؂�ւ�.
	void SetBlend(bool bFlag);


	HWND			m_hWnd;	//����޳�����.

	//Dx9.
	LPDIRECT3D9			m_pD3d;	//Dx9��޼ު��.
	LPDIRECT3DDEVICE9	m_pDevice9;//���޲���޼ު��.

//	D3DMATERIAL9*		m_pMaterials;	//��رُ��.
//	LPDIRECT3DTEXTURE9* m_pTextures;	//ø������.
//	char				m_TexFileName[8][256];//ø���̧�ٖ�(8���܂�).
	DWORD				m_dwNumMaterials;//��رِ�.
	LPD3DXMESH			m_pMesh;		//ү����޼ު��.

	//Dx11.
	ID3D11Device*			m_pDevice11;		//���޲���޼ު��.
	ID3D11DeviceContext*	m_pContext11;		//���޲���÷��.

	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pConstantBuffer0;	//�ݽ����ޯ̧.
	ID3D11Buffer*			m_pConstantBuffer1;	//�ݽ����ޯ̧.


	ID3D11Buffer*	m_pVertexBuffer;		//���_(�ްï��)�ޯ̧.
	ID3D11Buffer**	m_ppIndexBuffer;			//���ޯ���ޯ̧.
	ID3D11SamplerState*	m_pSampleLinear;	//ø����̻����.
//	ID3D11ShaderResourceView* m_pTexture;	//ø���.

	MY_MATERIAL*	m_pMaterials;	//��رٍ\����.
	DWORD			m_NumAttr;		//������.
	DWORD			m_AttrID[300];	//����ID ��300�����܂�.

	bool			m_TextureFlag;	//ø����L��.


	float		m_Scale;	//�g�k.
	float		m_Yaw;		//��](Y��).
	float		m_Pitch;	//��](X��).
	float		m_Roll;		//��](Z��).
	D3DXVECTOR3	m_vPos;		//�ʒu(X,Y,Z���W).
	float		CenterX,CenterY,CenterZ;	//��]�̒��S���W.
	float		Direct;		//����.
	float		SpdSpin;	//��]���x.
	float		Distance;	//��]���a.
	


	SSPHERE		m_Sphere;	//�̨��\����.
	BBOX		m_BBox;		//�a�{�b�N�X�\����(�����蔻��@��).

	//�����޽ð�.
	ID3D11BlendState*	m_pBlendState;

	bool		m_ShotFlag;	//�����׸�.
	bool		m_DeadFlag;	//���S�׸�.

	enDirectioin	m_eDir;	//����.


	//ڲ.
	D3DXVECTOR3	m_vRay;			//ڲ�̈ʒu.
	D3DXVECTOR3 m_vRot;
	D3DXVECTOR3 m_vAxis;		//��]��.
	LPD3DXMESH	m_pMeshForRay;	//���C�ƃ��b�V���̔���.

};

#endif//#ifndef DX9MESH_H