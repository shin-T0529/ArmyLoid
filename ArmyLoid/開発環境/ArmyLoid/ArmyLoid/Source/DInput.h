#ifndef __DIINPUT__
#define __DIINPUT__

#include<dinput.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")	//コントローラー操作で使用

//定数
#define N_OFFSET	500	//ジョイスティックのニュートラル

//DInput2用
#define PadNumber	2	//パッドの数

//キー情報の列挙体
enum enPkey
{
	enPkey_U,		//Up
	enPkey_D,		//Down
	enPkey_L,		//Left
	enPkey_R,		//Right		

	enPkey_UR,		//UpR
	enPkey_DR,		//DownR
	enPkey_LR,		//LeftR
	enPkey_RR,		//RightR	

	enPkey_01,		
	enPkey_02,		
	enPkey_03,	
	enPkey_04,		
	enPkey_05,		
	enPkey_06,		
	enPkey_07,	
	enPkey_08,		
	enPkey_09,		
	enPkey_10,		

	enPkey_Max,		//入力の最大値
};

class DInput
{
public:
	DInput();
	~DInput();

	//DirectInput 初期化設定
	bool initDI(HWND hWnd);

	//入力情報更新関数
	HRESULT UpdateInputState();
	HRESULT UpdateInputState2();

	//入力チェック関数
	bool IsPressKey(enPkey key);

	LPDIRECTINPUTDEVICE8	m_pPad[2];//デバイスオブジェクト（コントローラー）

private:
	LPDIRECTINPUT8			m_pDI;					//DxInputオブジェクト

	//LPDIRECTINPUTDEVICE8	m_pPad;					//デバイスオブジェクト（コントローラー）

	DIJOYSTATE JoyState;

	unsigned int m_uInputState;	//入力情報の保管用

	//入力情報の追加
	void AddInputState(enPkey key);
	//入力情報の初期化
	void InitInputState();

};



#endif	//__DIINPUT__