// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

#ifndef __DRV_ISR_H__
#define __DRV_ISR_H__

void drv_isr_init1(void (*pf_pwm)(void));

// 描述：PIE组3的中断应答,具体是ePWM中断
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19
// void drv_isr_ack_int3(void)
#define DRV_ISR_ACK_INT3()		(PieCtrlRegs.PIEACK.all = PIEACK_GROUP3)

// 描述：PIE组9的中断应答,具体是scib中断
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-6-27
// void drv_isr_ack_int9(void)
//#define DRV_ISR_ACK_INT9()	(PieCtrlRegs.PIEACK.all = PIEACK_GROUP9)


#endif  // __DRV_ISR_H__

