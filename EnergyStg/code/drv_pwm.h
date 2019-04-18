// 示例：声明对象、外部引用对象、引用函数
// drv_pwm_init()		pwm寄存器初始化;	停机时不能重新初始化
// drv_pwm_start()		pwm启动
// drv_pwm_stop()		pwm停止
// drv_pwm_isr_ack()	PWM1中断应答

// 描述：头文件。如何应用，结构定义等都在此
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-2-28


#ifndef __DRV_PWM_H__
#define __DRV_PWM_H__


//------------------------- 宏定义区---------------------------------//
#define   INV_SWITH_FREQ	    10000			// 10K开关频率
#define   EXC_SWITH_FREQ	    25000			// 25K开关频率

#define   DRV_PWM_FS_3KHZ   	25000			// 150MHZ/3KHZ/2  = 50K/2 = 25000，切记，因为是对称PWM，所以时钟必须除以2
#define   DRV_PWM_DBR       	300             // 定义死区时间上升时间  6.667*300 = 2us
#define   DRV_PWM_DBF       	300             // 定义死区时间下降时间  6.667*300 = 2us
#define   DRV_PWM_DBP       	2               // 定义死区极性 高电平有效互补(AHC)
#define   DRV_PWM_CMPA      	0				// PWM比较器A的初始值
#define   DRV_PWM_CMPB      	0				// PWM比较器B的初始值

//------------------------- 变量定义区---------------------------------//

typedef struct {
                // 内部变量：
   				unsigned period_2;	// 载波周期的1/2。
				unsigned period_4;	// 载波周期的1/4。输出PWM时pwm_period_4 * (1+aduty),因为(1+aduty)范围是0~1,所以要1/4周期数据
				// 输出
				unsigned fs;	// 载波频率,
				float ts;
} DRV_PWM;

#define DRV_PWM_DEFAULTS {    DRV_PWM_FS_3KHZ>>1,	DRV_PWM_FS_3KHZ>>2,  1201.0, 1.0/1201.0}	//必须改成1201HZ，这样才能第一次赋值时进去。

// 声明对象指针
typedef DRV_PWM *DRV_PWM_HANDLE;

extern DRV_PWM  drv_pwm         ; // 中断时间
extern DRV_PWM 	drv_pwm_inv     ; // 逆变
extern DRV_PWM 	drv_pwm_grid    ; // 


//----------------------- 内部函数定义区--------------------------------//

void drv_pwm_init1(void);
void drv_pwm_grid_start(void);
void drv_pwm_grid_stop(void);
void drv_pwm_startA(unsigned buck);
void drv_pwm_stopA(void);
void drv_pwm_stop(void);
void drv_pwm_buck_duty(unsigned buck, float duty);


// 描述：PWM1中断应答
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19
// void drv_pwm_isr_ack()

//#define DRV_PWM_ISR_ACK()		{EPwm1Regs.ETCLR.bit.INT = 1;} // 清除ETCLR[INT]标志位并且使更多中断脉冲生成
#define DRV_PWM_ISR_ACK()		{EPwm5Regs.ETCLR.bit.INT = 1;} // 清除ETCLR[INT]标志位并且使更多中断脉冲生成


// 描述：启动PWM时钟以启动PWM模块开始计数; 放在主循环前，开总中断前一句，避免PWM模块提前开始计数后，连续几次中断
#define	DRV_PWM_CLK_START()						\
{                                               \
	EALLOW;                                     \
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;      \
    EDIS;                                       \
}


#define DRV_PWM_ZD_START()		 DRV_CPLD_DO_START_N_PWM()      // 制动的开启


// 描述：pwm正常输出，xduty取值范围[-1 1]
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19
#define DRV_PWM_BAT(aduty, bduty, cduty)											    \
{																					    \
		SAT_VOID(aduty, 1.0, -1.0);                                                     \
		SAT_VOID(bduty, 1.0, -1.0);                                                     \
		SAT_VOID(cduty, 1.0, -1.0);                                                     \
		EPwm1Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_inv.period_4 * (1.0+aduty));	\
		EPwm2Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_inv.period_4 * (1.0+bduty));	\
		EPwm3Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_inv.period_4 * (1.0+cduty));	\
}


                                                                                        
// 网侧                                                                 
#define DRV_PWM_GRID(aduty, bduty, cduty)										    \
{																					    \
		SAT_VOID(aduty, 0.99, -0.99);                                                   \
		SAT_VOID(bduty, 0.99, -0.99);                                                   \
		SAT_VOID(cduty, 0.99, -0.99);                                      			    \
		EPwm4Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_grid.period_4 * (1.0+aduty)); \
		EPwm5Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_grid.period_4 * (1.0+bduty));	\
		EPwm6Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_grid.period_4 * (1.0+cduty));	\
}

#endif  // __DRV_PWM_H__

