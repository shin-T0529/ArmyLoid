#pragma warning(disable:4005)
#pragma warning(disable:4996)

#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3D10.h>
#include "Game.h"
#include "cCommon.h"
#include "Direct3D.h"

//定数.
#define WND_TITLE	"Army Loid"
#define APP_NAME	"Army Loid"

/*************************************************
*	ﾒｲﾝｸﾗｽ.
**/
class clsMain : public clsGame
{
public:
	clsMain();
	~clsMain(){};

	//ｳｨﾝﾄﾞｳ初期化関数.
	HRESULT InitWindow(HINSTANCE hInstance,
		INT iX, INT iY, INT iWidth, INT iHeight,const char* WindowName);

	LRESULT MsgProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam);
private:

};