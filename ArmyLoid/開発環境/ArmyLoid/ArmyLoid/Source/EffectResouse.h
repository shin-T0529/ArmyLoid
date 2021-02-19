#ifndef EFFECT_RESOUCE_H
#define EFFECT_RESOUCE_H

#pragma warning(disable:4005)

#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "C_LoadData.h"

#include"CEffectsMain.h"

//ﾗｲﾌﾞﾗﾘ.
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )//「D3DX～」の定義使用時に必要.
//エフェクトの種類
enum enSprite2D
{
	enEffects_Exp,
	enEffects_ExpBullet,

	enEffects_Gurad,

	enEffects_Spark,

	enEffects_Max,				//画像最大値(自動で最大)
	enEffects_Min = 0			//画像最小値
};
/*
			Exp->SetPosition(Bullet->m_vPos);
			Exp->Start_up();
			Bullet->m_vPos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
*/
class clsEffectsResouce
{
public:
	clsEffectsResouce();
	~clsEffectsResouce();
	void		Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	void		Create();
	clsEffects* GetEffects(int setCode) 
	{
		return m_ppEffects[setCode];
	}
private:
	HWND					m_hWnd;//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
	ID3D11Device*			m_pDevice11;//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pContext11;//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.

	clsEffects**			m_ppEffects;
	clsLoadData*			m_pLoad;
	 
};

#endif
