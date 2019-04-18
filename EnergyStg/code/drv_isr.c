#include "drv.h"
#include "drv_isr.h"


// 中断是这样的，先设置好：	IERx=1开，INTM=0开，PIEIERx=1开（这个以后永不变）
// 进入中断，自动完成：		IERx=0关，INTM=1关，PIEACKx=1关，IFR=0，PIEIFRx.y=0
// 退出中断，自动完成：		IERx=1开，INTM=0开，PIEACKx=0开

// 中断内如果还想开启所有其他中断的话，估计加入退出中断的语句
// 但简单来说，如果开启的是不同组的其他中断的话，只要加入一句INTM=0开即可，跟以前一样。


// 描述：中断初始化，配置各种中断;只能一次,停机时不能重新初始化
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void drv_isr_init1(void (*pf_pwm)(void))
{
	// 全局中断寄存器
	IER = 0x0000;									// 全部关闭IER
	IFR = 0x0000;   								// 全部清楚IFR

   	EALLOW;  										// 似乎TINT0的PIE设置需要此语句。 TINT0 is needed to write to EALLOW protected registers
   	
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;				// 使能PIE
	
	PieCtrlRegs.PIEACK.all = 0xFFFF;				// 写1清除所有的PIE中断组的PIEACK,使它可以接收中断
   	
//	// INT1	 定时器CLK的中断TINT0使能
//	PieVectTable.TINT0 = pf_time;					// 中断函数名
//	PieCtrlRegs.PIEIFR1.bit.INTx7 = 1;				// 清除PIEIFR
//	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;				// 使能PIEIER
//	IER |= M_INT1;									// 使能IER


	// INT3
	// EPWM1_INT的中断使能	
	//PWM1中断 <切换> PWM5中断。
//PWM中断
//	PieVectTable.EPWM1_INT = pf_pwm;
//	PieCtrlRegs.PIEIFR3.bit.INTx1 = 0;				// 清除PIEIFR 
//	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

//PWM5中断
	PieVectTable.EPWM5_INT = pf_pwm;
	PieCtrlRegs.PIEIFR3.bit.INTx5 = 0;				// 清除PIEIFR 
	PieCtrlRegs.PIEIER3.bit.INTx5 = 1;
	
	IER |= M_INT3;

//	// INT13   	// 定时器1的中断TINT1使能   	
//	PieVectTable.XINT13 = pf_time;				// 没有PIEIER
//	IER |= M_INT13;
	
   	// INT9
   	// SCI发送和接收中断使能   	
    //PieVectTable.SCIRXINTB  =  &drv_scib_receive;
    //PieVectTable.SCITXINTB  =  pf_scitx;
   	//PieCtrlRegs.PIEIFR9.bit.INTx3 = 1;			// 清除PIEIFR 
    //PieCtrlRegs.PIEIER9.bit.INTx3 = 1;       		// PIE Group 9,  INTx3
   	//PieCtrlRegs.PIEIFR9.bit.INTx4 = 1;			// 清除PIEIFR 
    //PieCtrlRegs.PIEIER9.bit.INTx4 = 1;       		// PIE Group 9,  INTx4
    //IER |=  M_INT9;                            	// 使能第9组PIE中断 CPU INT

	EDIS; 
}

/*
// 描述：PIE组1的中断应答,具体是TINT0(CPU-定时器0)中断
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void drv_isr_ack_int1(void)
{
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



// 描述：PIE组3的中断应答,具体是ePWM中断
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void drv_isr_ack_int3(void)
{
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

*/
