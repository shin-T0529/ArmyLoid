#include"CEffectsMain.h"
clsEffectsMain::clsEffectsMain()
{
	m_pEffects = new stEffectState;
	m_pEffects->Init();
}
clsEffectsMain::~clsEffectsMain()
{
	DettachEffect();
}

