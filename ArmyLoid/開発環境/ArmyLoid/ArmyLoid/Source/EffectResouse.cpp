#include"EffectResouse.h"

clsEffectsResouce::clsEffectsResouce()
{

}
clsEffectsResouce::~clsEffectsResouce()
{
	for (int i = 0; i < enEffects_Max; i++)
	{
		SAFE_DELETE(m_ppEffects[i]);
	}
	SAFE_DELETE(m_ppEffects);

}

void clsEffectsResouce::Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	m_hWnd = hWnd;//³¨ÝÄÞ³ÊÝÄÞÙ.
	m_pDevice11 = pDevice11;//ÃÞÊÞ²½µÌÞ¼Þª¸Ä.
	m_pContext11 = pContext11;//ÃÞÊÞ²½ºÝÃ·½Ä.
}

void clsEffectsResouce::Create()
{
	m_ppEffects = new clsEffects*[enEffects_Max];
	char tmp[256];//ˆêŽž‚Ì•¶Žšƒf[ƒ^•ÛŠÇêŠ
	for (int i = enEffects_Min; i < enEffects_Max; i++)
	{
		memcpy(tmp, m_pLoad->LoadFilePass("Data\\csv\\EffFilePass.csv", 1 + i), sizeof(char) * 255);		
		m_ppEffects[i] = new clsEffects;
		m_ppEffects[i]->Init();
		m_ppEffects[i]->Create(m_pDevice11, m_pContext11);
		m_ppEffects[i]->LoadData(tmp);
	}
}
