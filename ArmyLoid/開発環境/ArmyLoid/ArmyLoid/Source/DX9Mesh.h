#ifndef DX9MESH_H
#define DX9MESH_H
//警告についてのｺｰﾄﾞ分析を無効にする. 4005:再定義.
#pragma warning( disable : 4005)

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>
#include <d3dx9.h>
#include "MyMacro.h"

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )	//「D3DX～」の定義使用時に必要.
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

//ｽﾌｨｱ構造体.
struct SSPHERE
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3	vCenter;//中心.
	float		Radius;	//半径.
	void Init()
	{
		vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Radius = 0.0f;
	}
};

//Bボックス構造体.
struct BBOX
{
	D3DXVECTOR3	vPosMax;
	D3DXVECTOR3	vPosMin;
	D3DXVECTOR3 vRot;
	D3DXVECTOR3	vAxisX;		//X軸.
	D3DXVECTOR3	vAxisY;		//Y軸.
	D3DXVECTOR3	vAxisZ;		//Z軸.
	FLOAT		LengthX;	//x長さ.
	FLOAT		LengthY;	//y長さ.
	FLOAT		LengthZ;	//z長さ.
	FLOAT		vScale;
	//初期化.
	BBOX()
	{
		ZeroMemory(this, sizeof(BBOX));
		vAxisX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		vAxisY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vAxisZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		vScale = 0.0f;
	}

};

//方向,
enum enDirectioin
{
	enDirection_Stop = 0,			//停止.
	enDirection_Forward,		//前進.
	enDirection_Backward,		//後退.
	enDirection_Left,			//左回転.
	enDirection_Right,			//右回転.
	enDirection_FastBack,
	enDirection_FastFlont
};


//ﾒｯｼｭﾃﾞｰﾀをﾌｧｲﾙから取り出すためだけにDirectX9を使用する.
//※ﾚﾝﾀﾞﾘﾝｸﾞ(描画)は、DirectX11で行う.
class clsDx9Mesh
{
public:
	//-----------------------------------------------
	//	構造体.
	//-----------------------------------------------
	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Mesh.hlsl).
	//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
	struct MESHSHADER_CBUFFER0
	{
		D3DXMATRIX mW;		//ﾜｰﾙﾄﾞ(位置)座標.
		D3DXMATRIX mWVP;	//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,射影の合成変換行列.
		D3DXVECTOR4 vLightDir;//ﾗｲﾄ方向.
		D3DXVECTOR4	vEye;	//ｶﾒﾗ位置(視点位置).
	};
	struct MESHSHADER_CBUFFER1
	{
		D3DXVECTOR4	vAmbient;	//環境色.
		D3DXVECTOR4 vDiffuse;	//拡散反射色.
		D3DXVECTOR4	vSpecular;	//鏡面反射.
	};
	//頂点の構造体.
	struct MeshVertex
	{
		D3DXVECTOR3 Pos;	//位置.
		D3DXVECTOR3 Normal;	//法線(陰影計算に必須).
		D3DXVECTOR2 Tex;	//ﾃｸｽﾁｬ座標(UV座標).
	};

	//ﾏﾃﾘｱﾙ構造体.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;	//ｱﾝﾋﾞｴﾝﾄ.
		D3DXVECTOR4	Diffuse;	//ﾃﾞｨﾌｭｰｽﾞ.
		D3DXVECTOR4	Specular;	//ｽﾍﾟｷｭﾗ.
		CHAR szTextureName[128];			//ﾃｸｽﾁｬﾌｧｲﾙ名.
		ID3D11ShaderResourceView* pTexture;	//ﾃｸｽﾁｬ.
		DWORD dwNumFace;					//そのﾏﾃﾘｱﾙのﾎﾟﾘｺﾞﾝ数.

		//ｺﾝｽﾄﾗｸﾀ.
		MY_MATERIAL(){
			ZeroMemory(this, sizeof(MY_MATERIAL));
		}
		//ﾃﾞｽﾄﾗｸﾀ.
		~MY_MATERIAL(){
			SAFE_RELEASE(pTexture);
		}
	};


	clsDx9Mesh();	//ｺﾝｽﾄﾗｸﾀ.
	~clsDx9Mesh();	//ﾃﾞｽﾄﾗｸﾀ.

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPSTR fileName);
	
	//Dx9初期化.
	HRESULT InitDx9(HWND hWnd);
	//解放関数.
	void Release();

	//ﾒｯｼｭ読込.
	HRESULT LoadXMesh(LPSTR fileName);

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();

	//ﾚﾝﾀﾞﾘﾝｸﾞ(※Dx内とMain内で２つ存在するので注意).
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye);

	//透過(ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ)設定の切り替え.
	void SetBlend(bool bFlag);


	HWND			m_hWnd;	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.

	//Dx9.
	LPDIRECT3D9			m_pD3d;	//Dx9ｵﾌﾞｼﾞｪｸﾄ.
	LPDIRECT3DDEVICE9	m_pDevice9;//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.

//	D3DMATERIAL9*		m_pMaterials;	//ﾏﾃﾘｱﾙ情報.
//	LPDIRECT3DTEXTURE9* m_pTextures;	//ﾃｸｽﾁｬ情報.
//	char				m_TexFileName[8][256];//ﾃｸｽﾁｬﾌｧｲﾙ名(8枚まで).
	DWORD				m_dwNumMaterials;//ﾏﾃﾘｱﾙ数.
	LPD3DXMESH			m_pMesh;		//ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.

	//Dx11.
	ID3D11Device*			m_pDevice11;		//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pContext11;		//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.

	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer0;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	ID3D11Buffer*			m_pConstantBuffer1;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.


	ID3D11Buffer*	m_pVertexBuffer;		//頂点(ﾊﾞｰﾃｯｸｽ)ﾊﾞｯﾌｧ.
	ID3D11Buffer**	m_ppIndexBuffer;			//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ.
	ID3D11SamplerState*	m_pSampleLinear;	//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ.
//	ID3D11ShaderResourceView* m_pTexture;	//ﾃｸｽﾁｬ.

	MY_MATERIAL*	m_pMaterials;	//ﾏﾃﾘｱﾙ構造体.
	DWORD			m_NumAttr;		//属性数.
	DWORD			m_AttrID[300];	//属性ID ※300属性まで.

	bool			m_TextureFlag;	//ﾃｸｽﾁｬ有無.


	float		m_Scale;	//拡縮.
	float		m_Yaw;		//回転(Y軸).
	float		m_Pitch;	//回転(X軸).
	float		m_Roll;		//回転(Z軸).
	D3DXVECTOR3	m_vPos;		//位置(X,Y,Z座標).
	float		CenterX,CenterY,CenterZ;	//回転の中心座標.
	float		Direct;		//方向.
	float		SpdSpin;	//回転速度.
	float		Distance;	//回転半径.
	


	SSPHERE		m_Sphere;	//ｽﾌｨｱ構造体.
	BBOX		m_BBox;		//Ｂボックス構造体(当たり判定　■).

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ.
	ID3D11BlendState*	m_pBlendState;

	bool		m_ShotFlag;	//ｼｮｯﾄﾌﾗｸﾞ.
	bool		m_DeadFlag;	//死亡ﾌﾗｸﾞ.

	enDirectioin	m_eDir;	//方向.


	//ﾚｲ.
	D3DXVECTOR3	m_vRay;			//ﾚｲの位置.
	D3DXVECTOR3 m_vRot;
	D3DXVECTOR3 m_vAxis;		//回転軸.
	LPD3DXMESH	m_pMeshForRay;	//レイとメッシュの判定.

};

#endif//#ifndef DX9MESH_H