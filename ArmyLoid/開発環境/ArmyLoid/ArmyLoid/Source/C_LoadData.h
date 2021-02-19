#ifndef C_LOADDATA_H
#define	C_LOADDATA_H

#pragma warning(disable:4996)
#pragma warning(disable:4172)


#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Charactor.h"

class clsLoadData : public clsChara
{
public:
	D3DXVECTOR2 LoadSpriteState(LPCTSTR file
		, int Line);
	D3DXVECTOR3 LoadData(LPCTSTR fileName,int Line);
	D3DXVECTOR4 LoadDataScale(LPCTSTR fileName, int Line);
	
	char*		LoadFilePass(const char* fileName, int Line);
	

protected:
	D3DXVECTOR2 Convert_float(float x, float y);
	D3DXVECTOR3 Convert_float(float x, float y, float z);
	D3DXVECTOR4 Convert_float(float x, float y, float z,float w);
private:

};

#endif //#ifndef C_LOADDATA_H
