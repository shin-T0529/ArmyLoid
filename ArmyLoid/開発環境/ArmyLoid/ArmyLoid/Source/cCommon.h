#ifndef CCOMMON_H
#define	CCOMMON_H

#include "Global.h"
#include "CResource.h"	//Ø¿°½¸×½.
#include "Charactor.h"	//·¬×¸×½.
//#include "CRay.h"

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3D10.h>


//‹¤’ÊƒNƒ‰ƒX.
class clsCommon
{
public:
	clsCommon(){};
	virtual ~clsCommon(){};
	
	//“§‰ß(±ÙÌ§ÌŞÚİÄŞ)İ’è‚ÌØ‚è‘Ö‚¦.
	void SetBlend(bool bFlag);

	//•Ï”.
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	ID3D11BlendState*		m_pBlendState;
};

#endif //#ifndef CCOMMON_H
