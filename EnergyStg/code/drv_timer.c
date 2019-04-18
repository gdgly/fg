#include "apply.h"
#include "drv_timer.h"

//DRV_TIMER drv_timer=DRV_TIMER_DEFAULTS;


// 描述：定时器初始化
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2014-05-08

void drv_timer_init1(void)
{
	CpuTimer0.RegsAddr = &CpuTimer0Regs;

	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;	//周期

	CpuTimer0Regs.TPR.bit.TDDR  	= 0x98;	//预定标设为15000(0x3A98),目的让TIM输出0.1ms的计数值
	CpuTimer0Regs.TPRH.bit.TDDRH 	= 0x3A;

	CpuTimer0Regs.TCR.bit.TSS = 1;

	CpuTimer0Regs.TCR.bit.TRB = 1;

	StartCpuTimer0();		//定时器开始工作
}

