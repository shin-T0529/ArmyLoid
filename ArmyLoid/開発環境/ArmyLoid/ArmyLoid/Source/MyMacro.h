#ifndef MY_MACRO_H
#define MY_MACRO_H

//================================================
//
//	Ï¸Û.
//
//================================================

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif


//´×°Ò¯¾.
#define ERR_MSG(str,title){MessageBox(NULL,str,title,MB_OK);}
//‰ð•ú.
#define SAFE_RELEASE(p)		{ if(p!=nullptr) { (p)->Release(); (p)=nullptr; } }
#define SAFE_DELETE(p)		{ if(p!= nullptr) { delete (p);     (p)=nullptr; } }
#define SAFE_DELETE_ARRAY(p){ if(p!= nullptr) { delete[] (p);   (p)=nullptr; } }


#endif//#ifndef MY_MACRO_H