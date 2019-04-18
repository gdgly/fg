// 描述：头文件。
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19

#include "flashburn.h"			//必须放在这里，且最前面。

#ifndef __ZISR_FS_H__
#define __ZISR_FS_H__


// 声明对象内部函数
#ifndef SIMULATOR			

interrupt void zisr_fs(void);

#else

void zisr_fs(void);

#endif


#endif  // __ZISR_FS_H__

