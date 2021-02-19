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

//’è”.
#define WND_TITLE	"Army Loid"
#define APP_NAME	"Army Loid"

/*************************************************
*	Ò²İ¸×½.
**/
class clsMain : public clsGame
{
public:
	clsMain();
	~clsMain(){};

	//³¨İÄŞ³‰Šú‰»ŠÖ”.
	HRESULT InitWindow(HINSTANCE hInstance,
		INT iX, INT iY, INT iWidth, INT iHeight,const char* WindowName);

	LRESULT MsgProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam);
private:

};