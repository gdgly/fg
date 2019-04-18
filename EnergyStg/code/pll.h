// ʾ�������������ⲿ���ö������ú���
//    PLL pll=PLL_DEFAULTS;
//    extern PLL pll;
//    pll_init(&pll);
//    pll_func(&pll);

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19


#ifndef __PLL_H__
#define __PLL_H__

typedef struct {
                //�ڲ�������
				FILTER2 filter2;
				PID pid;
				unsigned n;				
				float FC;
				
				float w, wt;
                float sin1, cos1, sin2, cos2;			//����

                //�����

                float f;
                float sin, cos;				//����
} PLL;

#define PLL_DEFAULTS {			FILTER1_DEFAULTS,		\
								PID_DEFAULTS,			\
 				      }

// ��������ָ��
typedef PLL *PLL_HANDLE;

extern PLL pll_grid;

// ���������ڲ�����
void pll_init1(PLL_HANDLE p, float FC, float f0);
void pll_func(PLL_HANDLE p, float alpha, float beta);

#define		PLL_ANGLE		(PI2/200)			//��ǰ����һ���ز������ڼ�ȥ����ز�����������ǰ��仰�����Ҿ���Ӧ�û��ǲ���һ���ز�



#endif  // __PLL_H__

