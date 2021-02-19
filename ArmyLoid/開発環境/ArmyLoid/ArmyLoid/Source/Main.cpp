#include "Main.h"
#include "C_Player.h"
#include "Global.h"
#include "MyMacro.h"
#include <stdio.h>
#include <stdlib.h>

//#ifdef _DEBUG
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//
//#endif

clsMain* g_pClsMain = NULL;
//ｺﾝｽﾄﾗｸﾀ.
clsMain::clsMain()
{
	m_hWnd = NULL;
}

// ﾒｲﾝ関数.
INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevinstance,PSTR IpCmdLine,INT nCmdShow)
{
#ifdef _DEBUG

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

	g_pClsMain = new clsMain;

	if (g_pClsMain != NULL)
	{
		//ｳｨﾝﾄﾞｳ作成成功.
		if (SUCCEEDED(g_pClsMain->InitWindow(hInstance,0, 0,1280, 720,WND_TITLE)))
		{
			//3Dの作成は各シーンで行う.
			g_pClsMain->Loop();
		}
		//終了.

		delete g_pClsMain;//ｸﾗｽの破棄.
		g_pClsMain = NULL;
	}

	return 0;
}

// ｳｨﾝﾄﾞｳﾌﾟﾛｼｰｼﾞｬｰ.
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	//ﾌﾟﾛｼｰｼﾞｬｰ.
	return g_pClsMain->MsgProc(hWnd, uMsg, wParam, lParam);
}


//ｳｨﾝﾄﾞｳ初期化関数.
HRESULT clsMain::InitWindow(HINSTANCE hInstance,INT iX, INT iY,INT iWidth, INT iHeight,const char* WindowName)
{
	//ｳｨﾝﾄﾞｳ定義.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = APP_NAME;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//ｳｨﾝﾄﾞｳｸﾗｽをWindowsに登録.
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL,
			"ｳｨﾝﾄﾞｳｸﾗｽの登録に失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	//ｳｨﾝﾄﾞｳの作成.
	m_hWnd = CreateWindow(APP_NAME, APP_NAME,WS_OVERLAPPEDWINDOW,0, 0,iWidth, iHeight,NULL,NULL,hInstance,NULL);
	if (!m_hWnd)
	{
		MessageBox(NULL,"ｳｨﾝﾄﾞｳ作成に失敗", "エラー", MB_OK);
		return E_FAIL;
	}
	m_pCtrler = new clsDInput;
	m_pCtrler->InitDInput(hInstance);
	m_pCtrler->InitJoypad2();



	//ｳｨﾝﾄﾞｳの表示.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

//ｳｨﾝﾄﾞｳ関数(ﾒｯｾｰｼﾞ毎の処理)
LRESULT clsMain::MsgProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE://ESCｷｰ.
				DestroyWindow(hWnd);
			break;
		case VK_TAB://ESCｷｰ.
			if (MessageBox(NULL, "閉じる？", "警告", MB_YESNO) == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;

		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


