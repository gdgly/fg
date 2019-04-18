#include "flashburn.h"		//由于此处需要用到SIMULATOR，所以放在此处。

#ifndef __CUNIT_H__
#define __CUNIT_H__

#ifdef SIMULATOR	//单元测试时
void cunit(void);

#endif 

#endif //__CUNIT_H__
