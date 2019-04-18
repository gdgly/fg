// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19

#include "flashburn.h"			//��������������ǰ�档


#ifndef __TEST_H__
#define __TEST_H__

#ifdef DEBUG		//���Գ���

typedef struct {
                //�ڲ�������
				float theta;
} SIN3;

#define 	SIN3_DEFAULTS 	{ 0 }



// ���������ڲ�����
void test_ad(void);
void test_sin3_s(SIN3 *p, float ts, float h, float A, float *pa, float *pb, float *pc);
void test_sin3_pn(SIN3 *p, float ts, float h, float A, float *pa, float *pb, float *pc);
void test_sin3_m(SIN3 *p, float ts,  float h1, float A1, float h3, float A3, float h5, float A5, float h7, float A7,   float *pa, float *pb, float *pc);
void test_main(void);
void test_fs(void);

extern unsigned test_break_point;
extern unsigned test_ua, test_ub,  test_uc;
extern float    test_fa, test_fb,  test_fc,  test_fd,	test_fe,	test_ff;
extern float    test_fg, test_fh;


#define  TP()						TEST_POINT()
#define  TEST_POINT()				(test_break_point++)

#define  TM(EXPRESS)				TEST_MASK(EXPRESS)
#define  TEST_MASK(EXPRESS)			if(0)	{EXPRESS}


#endif

#endif  // __TEST_H__

