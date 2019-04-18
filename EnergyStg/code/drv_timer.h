
#ifndef __DRV_TIMER_H__
#define __DRV_TIMER_H__


// 声明对象内部函数
void drv_timer_init1(void);

// 描述：读0.1ms	= 从TIM读出来的时间是0.1ms
//unsigned long drv_timer_get_ms0_1(void)
#define		DRV_TIMER_GET_MS0_1()		(CpuTimer0Regs.PRD.all - CpuTimer0Regs.TIM.all)

#endif  // __DRV_TIMER_H__

