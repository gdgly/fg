// ������ͷ�ļ���
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19

#include "flashburn.h"			//��������������ǰ�档

#ifndef __ZISR_FS_H__
#define __ZISR_FS_H__


// ���������ڲ�����
#ifndef SIMULATOR			

interrupt void zisr_fs(void);

#else

void zisr_fs(void);

#endif


#endif  // __ZISR_FS_H__

