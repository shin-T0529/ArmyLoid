#include "DInput.h"

LPDIRECTINPUT8			g_pDI;				//DirectInputオブジェクト
LPDIRECTINPUTDEVICE8	g_pPad[PadNumber];				//デバイスオブジェクト（コントローラー）

//クラス関数ではない関数
//ジョイスティック列挙関数
BOOL CALLBACK EnumJoysticksCallBack(const DIDEVICEINSTANCE *pDiDInstance,VOID *pContext)
{
		HRESULT hRlt;	//関数の復帰地
		for (int i = 0; i < PadNumber; i++)
		{
			//デバイスの作成
			hRlt = g_pDI->CreateDevice(
				pDiDInstance->guidInstance,	//デバイス番号
				&g_pPad[i],					//(out)作成されたデバイスオブジェクト
				NULL);
			//エラーチェック
			if (hRlt != DI_OK)
			{
				return DIENUM_CONTINUE;	//次のデバイスを要求
			}
		}

	return DIENUM_STOP;	//列挙停止
}

//オブジェクト列挙関数
BOOL CALLBACK EnumObjectsCallBack(const DIDEVICEOBJECTINSTANCE *pDiDObjectInstance, VOID*pContext)
{
	for (int i = 0; i < PadNumber; i++)
	{
		//軸(Axis,アナログスティック)を持っているか？
		if (pDiDObjectInstance->dwType & DIDFT_AXIS)
		{
			//入力情報の範囲を設定する
			DIPROPRANGE diprg;		//範囲指定構造体

			diprg.diph.dwSize = sizeof(DIPROPRANGE);			//構造体サイズ
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);		//ヘッダー情報のサイズ
			diprg.diph.dwObj = pDiDObjectInstance->dwType;		//オブジェクトタイプを設定する（Axisタイプ）
			diprg.diph.dwHow = DIPH_BYID;						//dwObjに設定されたものがオブジェクトの種類か？インスタンス番号か？判断

			diprg.lMax = +1000;			//軸の最大値
			diprg.lMin = -1000;			//軸の最小値

			//範囲の設定
			if (FAILED(g_pPad[i]->SetProperty(DIPROP_RANGE, &diprg.diph)))
			{
				return DIENUM_STOP;
			}
		}
	}
	return DIENUM_CONTINUE;
}

DInput::DInput()
{
	for (int i = 0; i < PadNumber; i++)
	{
		m_pPad[i] = NULL;

	}
	m_pDI = NULL;

}

DInput::~DInput()
{

}

//DirectInputの初期化
bool DInput::initDI(HWND hWnd)
{
	for (int i = 0; i < PadNumber; i++)
	{
		HRESULT hRlt;	//関数の復帰地
		hRlt = DirectInput8Create(
			GetModuleHandle(NULL),	//DxInputを作成するアプリハンドル
			DIRECTINPUT_VERSION,	//固定：DxInputのバージョン
			IID_IDirectInput8,		//固定：UnicodeとAnsiの区別用
			(VOID**)&m_pDI,			//(out)DxInputオブジェクト
			NULL);

		//エラーチェック
		if (hRlt != DI_OK)
		{
			MessageBox(NULL, "DxInputの作成に失敗", "エラー", MB_OK);
			return false;
		}

		g_pDI = m_pDI;

		//使用可能なコントローラーを探す
		hRlt = (*m_pDI).EnumDevices(
			DI8DEVCLASS_GAMECTRL,//すべてのゲームコントローラー
			EnumJoysticksCallBack,//コントローラーの列挙関数
			NULL,				 //コールバック関数からの値
			DIEDFL_ATTACHEDONLY);//繋がっているものからのみ

	//エラーチェック
		if (hRlt != DI_OK)
		{
			MessageBox(NULL, "コントローラーの確認に失敗", "エラー", MB_OK);
			return false;
		}
		m_pPad[i] = g_pPad[i];

		//コントローラーの接続確認
		//接続されている場合
		if (g_pPad[i] != NULL)
		{
			//コントローラー構造体のデータフォーマットを作成
			hRlt = g_pPad[i]->SetDataFormat(&c_dfDIJoystick2);
			if (hRlt != DI_OK)
			{
				MessageBox(NULL, "データフォーマットを作成に失敗", "エラー", MB_OK);
			}

			//ほかのデバイスとの協調レベルを設定
			hRlt = g_pPad[i]->SetCooperativeLevel(hWnd,
				DISCL_EXCLUSIVE |	//排他アクセス
				DISCL_FOREGROUND);	//フォアグラウンド

			if (hRlt != DI_OK)
			{
				MessageBox(NULL, "協調レベルの設定に失敗", "エラー", MB_OK);
			}

			//使用可能なオブジェクト（ボタンなど）の列挙を作成
			hRlt = g_pPad[i]->EnumObjects(
				EnumObjectsCallBack,	//オブジェクト関数
				(VOID*)hWnd,			//コールバック関数に送る情報
				DIDFT_ALL);				//すべてのオブジェクト 

			if (hRlt != DI_OK)
			{
				MessageBox(NULL, "オブジェクトの列挙に失敗", "エラー", MB_OK);
			}

		}
	}
	return true;
}

//入力情報更新
HRESULT DInput :: UpdateInputState()
{
	HRESULT hRlt;		//関数復帰地
	DIJOYSTATE2 js;		//ジョイスティック情報構造体

	for (int i = 0; i < PadNumber; i++)
	{
		//入力情報の初期化
		InitInputState();

		//ジョイスティックの接続確認
		if (m_pPad[i] == NULL)
		{
			return S_OK;
		}

		//ジョイスティックが変更されているかを確認
		hRlt = m_pPad[i]->Poll();

		if (hRlt != DI_OK && hRlt != DI_NOEFFECT)		//DI_NOEFFECT：確認不要なデバイス
		{
			//コントローラーのアクセス権を取得する
			hRlt = m_pPad[i]->Acquire();
			while (hRlt == DIERR_INPUTLOST)
			{
				hRlt = m_pPad[i]->Acquire();
			}
			return S_OK;
		}

		//コントローラーから直接データを取得する
		hRlt = m_pPad[i]->GetDeviceState(
			sizeof(DIJOYSTATE2),			//取得するサイズ
			&js);						//（out）取得したデータの結果
		if (hRlt == DIERR_INPUTLOST || hRlt == DIERR_NOTACQUIRED)
		{
			hRlt = m_pPad[i]->Acquire();
			while (hRlt == DIERR_INPUTLOST)
			{
				hRlt = m_pPad[i]->Acquire();
			}
		}
		if (hRlt != DI_OK)
		{
			return hRlt;
		}

		//入力結果.
		//右
		if (js.lX > N_OFFSET){ AddInputState(enPkey_R); }
		//左
		else if (js.lX < -N_OFFSET){ AddInputState(enPkey_L); }
		//下
		if (js.lY > N_OFFSET){ AddInputState(enPkey_D); }
		//上
		else if (js.lY < -N_OFFSET){ AddInputState(enPkey_U); }

		//ボタン（RGB Buttons）
		//ループ開始位置をチェック対象のボタンに変更する
		for (int i = enPkey_01; i < enPkey_Max; i++)
		{
			//コントローラーのボタンの配列開始番号が0からなので開始位置を戻す
			if (js.rgbButtons[i - enPkey_01] & 0x80)
			{
				//キャストを行いループ変数を入力値として格納
				AddInputState((enPkey)i);
			}
		}
	}
	return S_OK;

}

//入力情報更新
HRESULT DInput::UpdateInputState2()
{
	HRESULT hRlt;		//関数復帰地
	DIJOYSTATE2 js;		//ジョイスティック情報構造体

	for (int i = 0; i < PadNumber; i++)
	{
		//入力情報の初期化
		InitInputState();

		//ジョイスティックの接続確認
		if (m_pPad[i] == NULL)
		{
			return S_OK;
		}

		//ジョイスティックが変更されているかを確認
		hRlt = m_pPad[i]->Poll();

		if (hRlt != DI_OK && hRlt != DI_NOEFFECT)		//DI_NOEFFECT：確認不要なデバイス
		{
			//コントローラーのアクセス権を取得する
			hRlt = m_pPad[i]->Acquire();
			while (hRlt == DIERR_INPUTLOST)
			{
				hRlt = m_pPad[i]->Acquire();
			}
			return S_OK;
		}

		//コントローラーから直接データを取得する
		hRlt = m_pPad[i]->GetDeviceState(
			sizeof(DIJOYSTATE2),			//取得するサイズ
			&js);							//（out）取得したデータの結果
		if (hRlt == DIERR_INPUTLOST || hRlt == DIERR_NOTACQUIRED)
		{
			hRlt = m_pPad[i]->Acquire();
			while (hRlt == DIERR_INPUTLOST)
			{
				hRlt = m_pPad[i]->Acquire();
			}
		}
		if (hRlt != DI_OK)
		{
			return hRlt;
		}

		//入力結果.
			//右
			if (js.lX > N_OFFSET) { AddInputState(enPkey_R); }
			//左
			else if (js.lX < -N_OFFSET) { AddInputState(enPkey_L); }
			//下
			if (js.lY > N_OFFSET) { AddInputState(enPkey_D); }
			//上
			else if (js.lY < -N_OFFSET) { AddInputState(enPkey_U); }



		//ボタン（RGB Buttons）
		//ループ開始位置をチェック対象のボタンに変更する
		for (int i = enPkey_01; i < enPkey_Max; i++)
		{
			//コントローラーのボタンの配列開始番号が0からなので開始位置を戻す
			if (js.rgbButtons[i - enPkey_01] & 0x80)
			{
				//キャストを行いループ変数を入力値として格納
				AddInputState((enPkey)i);
			}
		}
	}
	return S_OK;

}

//入力情報を追加
void DInput :: AddInputState(enPkey key)
{
	//シフト演算子：<<または>>
	m_uInputState |= 1 << key;

}

//入力情報の初期化
void DInput :: InitInputState()
{
	m_uInputState = 0;
}

//入力チェック
bool DInput :: IsPressKey(enPkey key)
{
	if((m_uInputState >> key) & 1)
	{
		return true;
	}

	return false;
}