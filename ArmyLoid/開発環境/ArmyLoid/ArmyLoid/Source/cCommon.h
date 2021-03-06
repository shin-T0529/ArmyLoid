#ifndef CCOMMON_H
#define	CCOMMON_H

#include "Global.h"
#include "CResource.h"	//ﾘｿｰｽｸﾗｽ.
#include "Charactor.h"	//ｷｬﾗｸﾗｽ.
//#include "CRay.h"

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3D10.h>


//共通クラス.
class clsCommon
{
public:
	clsCommon(){};
	virtual ~clsCommon(){};
	
	//透過(ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ)設定の切り替え.
	void SetBlend(bool bFlag);

	//変数.
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	ID3D11BlendState*		m_pBlendState;
};

#endif //#ifndef CCOMMON_H
