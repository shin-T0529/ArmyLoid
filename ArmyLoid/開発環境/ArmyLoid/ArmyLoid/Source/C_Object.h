#ifndef OBJECT_H
#define OBJECT_H

#include "Charactor.h"

//���삪�Ȃ�or�ȒP�ȓ���̂���I�u�W�F�N�g.
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