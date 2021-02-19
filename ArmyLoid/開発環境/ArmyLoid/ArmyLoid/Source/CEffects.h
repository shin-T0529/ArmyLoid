#ifndef CEFFECTS_H
#define CEFFECTS_H
//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable : 4005)

//#define ENABLE_XAUDIO

//------------------------------------------------
//	先に読み込む ﾍｯﾀﾞｰ,ﾗｲﾌﾞﾗﾘ.
//------------------------------------------------
#include <stdio.h>
#include <D3D11.h>
#include <uchar.h>
#pragma comment( lib, "d3d11.lib" )
#pragma warning(disable:4005)

//------------------------------------------------
//	ここからEffekseer関係 ﾍｯﾀﾞｰ,ﾗｲﾌﾞﾗﾘ.
//------------------------------------------------
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#ifdef _WIN32
#ifdef _DEBUG
	#pragma comment( lib, "VS2017\\Debug\\Effekseer.lib" )
	#pragma comment( lib, "VS2017\\Debug\\EffekseerRendererDX11.lib" )
#else//#ifdef _DEBUG
	#pragma comment( lib, "VS2017\\Release\\Effekseer.lib" )
	#pragma comment( lib, "VS2017\\Release\\EffekseerRendererDX11.lib" )
#endif//#ifdef _DEBUG
#else//#ifdef _WIN32
#ifdef _DEBUG
	#pragma comment( lib, "VS2017\\Debug\\Effekseer.lib" )
	#pragma comment( lib, "VS2017\\Debug\\EffekseerRendererDX11.lib" )
#else//#ifdef _DEBUG
	#pragma comment( lib, "VS2017\\Release\\Effekseer.lib" )
	#pragma comment( lib, "VS2017\\Release\\EffekseerRendererDX11.lib" )
#endif//#ifdef _DEBUG
#endif//#ifdef _WIN32

//「D3DX～」使用するのに必須.
//注意:Effekseer関係より後に読み込むこと.
#include <D3D10.h>
#include <D3DX10math.h>
#pragma comment( lib, "d3dx10.lib" )

using namespace Effekseer;
/*************************************************
*
*	ﾌﾘｰｿﾌﾄ Effekseerのﾃﾞｰﾀを使うためのｸﾗｽ.
*
**/
class clsEffects
{
public:
	//ｺﾝｽﾄﾗｸﾀ.
	clsEffects();
	//ﾃﾞｽﾄﾗｸﾀ.
	~clsEffects();

	//初期化関数.
	HRESULT Init();
	//構築関数.
	HRESULT Create(ID3D11Device* pDevice11,ID3D11DeviceContext* pContext11);
	//破棄関数.
	HRESULT Destroy();
	//ﾃﾞｰﾀ読込関数.
	HRESULT LoadData(const char* cFilepass);
	//ﾃﾞｰﾀ解放関数.
	HRESULT ReleaseData();
	//描画.
	void Render(const D3DXMATRIX& mView, const D3DXMATRIX& mProj, const D3DXVECTOR3& vCamPos);

	//再生.
	Handle Play(const D3DXVECTOR3& vPos) {
		return m_pManager->Play(m_pEffect, vPos.x, vPos.y, vPos.z);
	}
	//一時停止関数
	void Paused(::Handle handle, bool flag) {
		m_pManager->SetPaused(handle, flag);
	}
	//停止関数
	void Stop(::Handle handle) {
		m_pManager->StopEffect(handle);
	}
	//すべて停止
	void StopAll() {
		m_pManager->StopAllEffects();
	}
	//サイズを指定する
	void SetScale(::Handle handle, D3DXVECTOR3 vScale)
	{
		m_pManager->SetScale(handle, vScale.x, vScale.y, vScale.z);
	}

	//回転を指定する
	void SetRotation(::Handle handle, D3DXVECTOR3 vRot)
	{
		m_pManager->SetRotation(handle, vRot.x, vRot.y, vRot.z);
	}
	void SetRotation(::Handle handle, D3DXVECTOR3 vAxis, float angle)
	{
		m_pManager->SetRotation(handle,
			::Effekseer::Vector3D(vAxis.x, vAxis.y, vAxis.z), angle);
	}
	//位置を指定する
	void SetPosition(::Handle handle, D3DXVECTOR3 vPos)
	{
		m_pManager->SetLocation(handle,
			::Effekseer::Vector3D(vPos.x, vPos.y, vPos.z));

	}
	//再生速度を設定する
	//※pause();関数で一時停止ができない場合は
	//SetSpeed()関数で速度を0にすれば一時停止できる
	void SetSpeed(::Handle handle, float speed)
	{
		m_pManager->SetSpeed(handle, speed);
	}

private:
	//生成やｺﾋﾟｰを禁止する.
	clsEffects(const clsEffects& rhs);
	clsEffects& operator = (const clsEffects& rhs) {};

	//---------------------------------------------
	//	DirectX↔Effekseer変換系.
	//---------------------------------------------
	//ﾍﾞｸﾀｰ変換関数.
	Effekseer::Vector3D ConvertToVector3Efk(const D3DXVECTOR3& SrcVec3Dx);
	D3DXVECTOR3 ConvertToVector3Dx(const Effekseer::Vector3D& SrcVec3Efk);
	//行列変換関数.
	Effekseer::Matrix44 ConvertToMatrixEfk(const D3DXMATRIX& SrcMatDx);
	D3DXMATRIX ConvertToMatrixDx(const Effekseer::Matrix44& SrcMatEfk);


	//ﾋﾞｭｰ行列を設定.
	void SetViewMatrix(const D3DXMATRIX& mView);
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列を設定.
	void SetProjectionMatrix(const D3DXMATRIX& mProj);

	//ｴﾌｪｸﾄを動作させるために必要.
	Manager*						m_pManager;
	EffekseerRenderer::Renderer*	m_pRender;
	//ｴﾌｪｸﾄの種類ごとに必要.
	Effect*							m_pEffect;
	//ｴﾌｪｸﾄごとに必要.
	//※同じｴﾌｪｸﾄを同時に３つ出すなら３つ必要.
	Handle							m_Handle;

};


#endif//#ifndef CEFFECTS_H