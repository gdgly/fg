
#ifndef __DRV_TIMER_H__
#define __DRV_TIMER_H__


// ���������ڲ�����
void drv_timer_init1(void);

// ��������0.1ms	= ��TIM��������ʱ����0.1ms
//unsigned long drv_timer_get_ms0_1(void)
#define		DRV_TIMER_GET_MS0_1()		(CpuTimer0Regs.PRD.all - CpuTimer0Regs.TIM.all)

#endif  // __DRV_TIMER_H__

