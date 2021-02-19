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
/*エフェクトオブジェクトクラス*/
/******************************/
class clsEffectsMain
{
public:
	clsEffectsMain();
	~clsEffectsMain();
	//情報を更新
	//※拡縮や回転、座標の値をモデルデータに反映
	
	void Start_up()
	{
		if (m_pEffects != nullptr)
		{
			//ｴﾌｪｸﾄごとに必要なﾊﾝﾄﾞﾙ.
			m_pEffects->handle = m_pEffects->m_pEffects->Play(m_pEffects->m_vPos);
			m_pEffects->m_pEffects->SetRotation(m_pEffects->handle, m_pEffects->m_vRot);
		}
	}


	//レンダリング関数
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj
		, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye)
	{
		if (m_pEffects != nullptr)
		{
			//Effekseerﾚﾝﾀﾞﾘﾝｸﾞ.
			m_pEffects->m_pEffects->Render(mView, mProj, vEye);
		}
	}
	//モデルデータ関連付け関数
	void AttachEffect(clsEffects* pEffect)
	{
		if (pEffect != NULL)
		{
			m_pEffects->m_pEffects = pEffect;
		}
	}
	//モデルデータ関連付け解除関数
	void DettachEffect()
	{
		if (m_pEffects != NULL)
		{
			m_pEffects = NULL;
		}
	}
	//座標設定関数
	void SetPosition(D3DXVECTOR3 &vPos)
	{
		m_pEffects->m_vPos = vPos;
	}
	//座標取得関数
	D3DXVECTOR3 GetPosition()
	{
		return m_pEffects->m_vPos;
	}
	//回転設定関数
	void SetRotation(D3DXVECTOR3 &vRot)
	{
		m_pEffects->m_vRot = vRot;
	}
	//座標取得関数
	D3DXVECTOR3 GetRotation()
	{
		return m_pEffects->m_vRot;
	}
private:
	stEffectState* m_pEffects;
};


#endif//#ifndef C_CHARACTER_H
