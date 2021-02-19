#include "C_LoadData.h"

//スプライトステート取得.
D3DXVECTOR2 clsLoadData::LoadSpriteState(LPCTSTR fileName, int Line)
{
	FILE *fp;
	D3DXVECTOR2 PosS = { 0,0 };
	error_status_t err;
	err = fopen_s(&fp, fileName, "r");

	//エラーチェック.
	if (err != 0)
	{
		MessageBox(NULL, "データ読込失敗", "error", MB_OK);
	}

	//行列の読み込み.
	for (int i = 0; i < Line; i++)
	{
		if (fscanf(fp, "%f,%f", &PosS.x, &PosS.y) == EOF)
		{
			MessageBox(NULL, "データ取得失敗", "error", MB_OK);

		}
		//読み込んだらD3DXVECTOR3からfloatへ変換する.
		Convert_float(PosS.x, PosS.y);
	}

	//開いたファイルを閉じる.
	fclose(fp);
	return PosS;
}

//大きさが全て同じ物.
D3DXVECTOR3 clsLoadData::LoadData(LPCTSTR fileName, int Line)
{
	FILE *fp;
	D3DXVECTOR3 PosS = {0,0,0};
	error_status_t err;
	err = fopen_s(&fp, fileName, "r");

	//エラーチェック.
	if (err != 0)
	{
		MessageBox(NULL, "データ読込失敗", "error", MB_OK);
	}
	//行列の読み込み.
	for (int i = 0; i < Line; i++)
	{
		if (fscanf(fp, "%f,%f,%f", &PosS.x, &PosS.y, &PosS.z) == EOF)
		{
			MessageBox(NULL, "データ取得失敗", "error", MB_OK);

		}
		//読み込んだらD3DXVECTOR3からfloatへ変換する.
		Convert_float(PosS.x, PosS.y, PosS.z);
	}
	//開いたファイルを閉じる.
	fclose(fp);
	return PosS;
}

//配置場所によって大きさが変わるもの.
D3DXVECTOR4 clsLoadData::LoadDataScale(LPCTSTR fileName, int Line)
{
	FILE *fp;
	D3DXVECTOR4 PosS = { 0, 0, 0 ,0 };
	error_status_t err;
	err = fopen_s(&fp, fileName, "r");

	//エラーチェック.
	if (err != 0)
	{
		MessageBox(NULL, "データ読込失敗", "error", MB_OK);
	}
	//行列の読み込み.
	for (int i = 0; i < Line; i++)
	{
		if (fscanf(fp, "%f,%f,%f,%f", &PosS.x, &PosS.y, &PosS.z,&PosS.w) == EOF)
		{
			MessageBox(NULL, "データ取得失敗", "error", MB_OK);

		}
		//読み込んだらD3DXVECTOR3からfloatへ変換する.
		Convert_float(PosS.x, PosS.y, PosS.z,PosS.w);
	}
	//開いたファイルを閉じる.
	fclose(fp);
	return PosS;
}

char* clsLoadData::LoadFilePass(const char* fileName, int Line)
{

	//ファイル読み込みよう
	char FileData[255];	//一時の文字データ保管場所
	FILE *fp;			//読み込みファイル
	error_status_t err;
	err = fopen_s(&fp, fileName, "r");

	//エラーチェック
	if (err != 0)
	{
		MessageBox(NULL, "データ読み込み失敗(char)", "エラー", MB_OK);
	}

	//データ取得
	for (int i = 0; i < Line; i++)
	{
		if (fscanf(fp, "%s", FileData) == EOF)
		{
			MessageBox(NULL, "データ取得失敗(char)", "エラー", MB_OK);
			break;
		}
	}

	//閉じる
	fclose(fp);

	return FileData;
}

//数値変換　(float型).
D3DXVECTOR2 clsLoadData::Convert_float(float x, float y)
{
	//代入データ.
	D3DXVECTOR2 tempData;

	tempData.x = x;
	tempData.y = y;

	return tempData;
}

//数値変換　(float型).
D3DXVECTOR3 clsLoadData::Convert_float(float x, float y, float z)
{
	//代入データ.
	D3DXVECTOR3 tempData;

	tempData.x = x;
	tempData.y = y;
	tempData.z = z;

	return tempData;
}

//数値変換　(float型座標+float型大きさ).
D3DXVECTOR4 clsLoadData::Convert_float(float x, float y, float z,float w)
{
	//代入データ.
	D3DXVECTOR4 tempData;

	tempData.x = x;
	tempData.y = y;
	tempData.z = z;
	tempData.w = w;
	return tempData;
}

