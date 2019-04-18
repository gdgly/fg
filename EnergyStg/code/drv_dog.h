// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

#ifndef __DRV_DOG_H__
#define __DRV_DOG_H__

void drv_dog_enable(void);
void drv_dog_disable(void);
//void drv_dog_kick(void);

extern unsigned drv_dog_bflag;

// 描述：踢狗。包括给CPLD的握手,防止仿真器死机和程序跑飞,让CPLD封PWM，CPLD40KHZ检测1个点的速度，30个点没检测到，就封锁;
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-7-11
// void drv_dog_kick(void)
#define DRV_DOG_KICK()						\
{											\
	drv_dog_bflag = ~drv_dog_bflag;         \
	DRV_IO_KICK_CPLD(drv_dog_bflag);        \
                                            \
	EALLOW;                                 \
	SysCtrlRegs.WDKEY = 0x0055;             \
	SysCtrlRegs.WDKEY = 0x00AA;             \
	EDIS;									\
}

#endif  // __DRV_DOG_H__
