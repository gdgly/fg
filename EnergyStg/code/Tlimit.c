#include "apply.h"
#include "Tlimit.h"


#define FLOAT_LIMIT           1E+30F				// #define FLT_MAX   3.402823466E+38F

// 描述：时限保护的初始化
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19

void Tlimit_init(Tlimit_HANDLE p)
{
	p->sum = p->bresult = 0;
	(void)clk_Bjudgen(&p->clk0, 0, 1L);					// 初始化
	(void)clk_Bjudge1(&p->clk1, 0, 1L);					// 初始化
}


// 描述：定时限过保护

unsigned Tlimit_fix(Tlimit_HANDLE p, float in, float threshold, unsigned long interval)
{	
	if (!clk_Bjudgen(&p->clk0, 1, 1L))	return p->bresult;	// 1ms执行下面程序一次	
	
	p->sum += in - threshold;								// 超过的值，就开始累计
	if (p->sum < 0 )				p->sum=0;				// 当sum<0时，sum=0
	else if (p->sum > FLOAT_LIMIT)	p->sum= FLOAT_LIMIT;	// 当sum数据过大时，就锁死
	
	p->bresult = clk_Bjudge1(&p->clk1, p->sum > 0, interval);
	return p->bresult;
}


// 描述：反时限过保护。都是可放在主程序中的。interval1是limit1对应的时间

unsigned Tlimit_anti(Tlimit_HANDLE p, float in, float threshold, float limit1, unsigned long interval1)
{
	if (!clk_Bjudgen(&p->clk0, 1, 1L))	return p->bresult;	// 1ms执行下面程序一次	

	p->sum += SQ2(in) - SQ2(threshold);						// 超过的值，就开始累计
	if (p->sum < 0 )				p->sum=0;				// 当sum<0时，sum=0
	else if (p->sum > FLOAT_LIMIT)	p->sum= FLOAT_LIMIT;	// 当sum数据过大时，就锁死
	
	if (p->sum > (SQ2(limit1)- SQ2(threshold)) * interval1 )		
		p->bresult = 1;	
	else 	
		p->bresult = 0;
	return p->bresult;
}
