#include "apply.h"
#include "Tlimit.h"


#define FLOAT_LIMIT           1E+30F				// #define FLT_MAX   3.402823466E+38F

// ������ʱ�ޱ����ĳ�ʼ��
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19

void Tlimit_init(Tlimit_HANDLE p)
{
	p->sum = p->bresult = 0;
	(void)clk_Bjudgen(&p->clk0, 0, 1L);					// ��ʼ��
	(void)clk_Bjudge1(&p->clk1, 0, 1L);					// ��ʼ��
}


// ��������ʱ�޹�����

unsigned Tlimit_fix(Tlimit_HANDLE p, float in, float threshold, unsigned long interval)
{	
	if (!clk_Bjudgen(&p->clk0, 1, 1L))	return p->bresult;	// 1msִ���������һ��	
	
	p->sum += in - threshold;								// ������ֵ���Ϳ�ʼ�ۼ�
	if (p->sum < 0 )				p->sum=0;				// ��sum<0ʱ��sum=0
	else if (p->sum > FLOAT_LIMIT)	p->sum= FLOAT_LIMIT;	// ��sum���ݹ���ʱ��������
	
	p->bresult = clk_Bjudge1(&p->clk1, p->sum > 0, interval);
	return p->bresult;
}


// ��������ʱ�޹����������ǿɷ����������еġ�interval1��limit1��Ӧ��ʱ��

unsigned Tlimit_anti(Tlimit_HANDLE p, float in, float threshold, float limit1, unsigned long interval1)
{
	if (!clk_Bjudgen(&p->clk0, 1, 1L))	return p->bresult;	// 1msִ���������һ��	

	p->sum += SQ2(in) - SQ2(threshold);						// ������ֵ���Ϳ�ʼ�ۼ�
	if (p->sum < 0 )				p->sum=0;				// ��sum<0ʱ��sum=0
	else if (p->sum > FLOAT_LIMIT)	p->sum= FLOAT_LIMIT;	// ��sum���ݹ���ʱ��������
	
	if (p->sum > (SQ2(limit1)- SQ2(threshold)) * interval1 )		
		p->bresult = 1;	
	else 	
		p->bresult = 0;
	return p->bresult;
}
