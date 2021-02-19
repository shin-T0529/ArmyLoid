#ifndef DIRECT3D_H
#define DIRECT3D_H

#include "cCommon.h"
#include "MyMacro.h"
class clsDirect3D
{
public:
	clsDirect3D();
	~clsDirect3D();

	//各種取得関数(読込先で使いたい/空になると例外が出る).
	ID3D11Device* GetDevice()
	{
		return m_pDevice;
	}

	ID3D11DeviceContext* GetDeviceContext()
	{
		return m_pContext;
	}

	ID3D11RenderTargetView* GetTexRTV()
	{
		return m_pBackBuffer_TexRTV;
	}

	ID3D11DepthStencilView* GetDSTexDSV()
	{
		return m_pBackBuffer_DSTexDSV;
	}

	//Init3Dに当たるもの.
	void Create3D(HWND hWnd);

	void SetBlend(bool Flag);

	void SetBuffer();

	void RenderPresent();

private:
	HWND						m_hWnd;

	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;

	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;
	ID3D11Texture2D*			m_pBackBuffer_DSTex;
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;

	ID3D11BlendState*			m_pBlendState;


};


#endif