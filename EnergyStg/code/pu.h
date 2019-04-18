// ������PUͷ�ļ��������ⲿ����
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

#ifndef __PU_H__
#define __PU_H__


//------------------------- ���ۻ��ֵ---------------------------------
#define  ULRMSPU 		80.0							// �ߵ�ѹ��Чֵ	380
#define  UACRMSPU 		(ULRMSPU / C_SQRT3)				// ���ѹ��Чֵ	220
#define  IACRMSPU 		(50.0)							// �������Чֵ
#define  IDCPU	 		(100.0)							// ��س�ŵ����

#define  UACPU 			(UACRMSPU * C_SQRT2)			// ���ѹ��ֵ	65.3
#define  IACPU 			(IACRMSPU * C_SQRT2)			// �������ֵ	

#define  SPU 			(UACRMSPU * C_SQRT3 * IACRMSPU * C_SQRT3/1000.0) 	// ���ڹ��ʷ�ֵ����λKVA	=33KW

#define  UDCPU    		100.0							// �ֱ��ĸ�ߵ�ѹ



#endif  // __PU_H__

