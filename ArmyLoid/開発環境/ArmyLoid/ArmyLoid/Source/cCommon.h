#ifndef CCOMMON_H
#define	CCOMMON_H

#include "Global.h"
#include "CResource.h"	//ؿ���׽.
#include "Charactor.h"	//��׸׽.
//#include "CRay.h"

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3D10.h>


//���ʃN���X.
class clsCommon
{
public:
	clsCommon(){};
	virtual ~clsCommon(){};
	
	//����(��̧������)�ݒ�̐؂�ւ�.
	void SetBlend(bool bFlag);

	//�ϐ�.
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	ID3D11BlendState*		m_pBlendState;
};

#endif //#ifndef CCOMMON_H
