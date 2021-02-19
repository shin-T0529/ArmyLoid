#ifndef EFFECT_RESOUCE_H
#define EFFECT_RESOUCE_H

#pragma warning(disable:4005)

#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

#include "C_LoadData.h"

#include"CEffectsMain.h"

//ײ����.
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )//�uD3DX�`�v�̒�`�g�p���ɕK�v.
//�G�t�F�N�g�̎��
enum enSprite2D
{
	enEffects_Exp,
	enEffects_ExpBullet,

	enEffects_Gurad,

	enEffects_Spark,

	enEffects_Max,				//�摜�ő�l(�����ōő�)
	enEffects_Min = 0			//�摜�ŏ��l
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
	HWND					m_hWnd;//����޳�����.
	ID3D11Device*			m_pDevice11;//���޲���޼ު��.
	ID3D11DeviceContext*	m_pContext11;//���޲���÷��.

	clsEffects**			m_ppEffects;
	clsLoadData*			m_pLoad;
	 
};

#endif
