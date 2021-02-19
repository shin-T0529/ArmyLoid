#ifndef OBJECT_H
#define OBJECT_H

#include "Charactor.h"

//動作がないor簡単な動作のあるオブジェクト.
class clsObject : public clsChara
{
public:
	clsObject() {};
	~clsObject() 
	{
		DetatchModel();
	};

	void Init()
	{
		
	}
	void Action() {}

};

#endif 