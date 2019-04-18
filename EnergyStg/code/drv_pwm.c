#include "drv.h"
#include "drv_pwm.h"

DRV_PWM     drv_pwm             = DRV_PWM_DEFAULTS; //  中断时间，目前等同inv的时间

DRV_PWM 	drv_pwm_inv 		= DRV_PWM_DEFAULTS; // 逆变
DRV_PWM 	drv_pwm_grid 		= DRV_PWM_DEFAULTS; // 网侧, 

#define drv_pwm123x_init(EPwmxRegs)																									\
{                                                                                                                                   \
	EPwmxRegs.TBCTR = 0x0001;                                                                                                       \
   	EPwmxRegs.TBPHS.half.TBPHS = 0;						/* 同步后加载的相位寄存器值*/                                               \
   	                                                                                                                                \
   	EPwmxRegs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;		/* =0x3	禁止SYNCO输出*/   	                                                \
	EPwmxRegs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;     /* 递增递减计数模式*/                                                       \
	/* 设置时钟  */                                                                                                                 \
	EPwmxRegs.TBCTL.bit.CLKDIV    = 0;					/* TBCLK = SYSCLKOUT / (HSPCLKDIV × CLKDIV) = SYSCLKOUT = 150MHz*/         \
	EPwmxRegs.TBCTL.bit.HSPCLKDIV = 0;                                                                                              \
	                                                                                                                                \
	/* 设置映射模式  */                                                                                                             \
    EPwmxRegs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;			/* 0 : 设置成映射模式,作为双缓冲操作，所有的写操作通过CPU访问映射寄存器*/   \
    EPwmxRegs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  		/* 00: 当CTR=0时加载*/                                                      \
    EPwmxRegs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                                                                                     \
    EPwmxRegs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;  		/* Load on Zero*/                                                           \
                                                                                                                                    \
	/* 设置动作限定寄存器AQ，电平设置为：CMPA = 最大时，PWM输出 = 1， CMPA = 0时，PWM输出 = 0*/    		                            \
    EPwmxRegs.AQCTLA.bit.CAU = AQ_SET;            		/* 计数器递增时，跟比较器A一致时，此时PWMxA=1*/                             \
    EPwmxRegs.AQCTLA.bit.CAD = AQ_CLEAR;           		/* 计数器递减时，跟比较器A一致时，此时PWMxA=0*/                             \
    EPwmxRegs.AQCTLB.bit.CBU = AQ_SET;            	/* 计数器递增时，跟比较器A一致时，此时PWMxB=0*/                             \
    EPwmxRegs.AQCTLB.bit.CBD = AQ_CLEAR;           		/* 计数器递减时，跟比较器A一致时，此时PWMxB=1*/                             \
	/* EPwmxRegs.AQCTLA.bit.CAU = AQ_CLEAR;            	 计数器递增时，跟比较器A一致时，此时PWMxA=0*/                             \
    /* EPwmxRegs.AQCTLA.bit.CAD = AQ_SET;           	 计数器递减时，跟比较器A一致时，此时PWMxA=1*/                             \
    /* EPwmxRegs.AQCTLB.bit.CAU = AQ_SET;           	 计数器递增时，跟比较器A一致时，此时PWMxB=1*/                             \
    /* EPwmxRegs.AQCTLB.bit.CAD = AQ_CLEAR;        		 计数器递减时，跟比较器A一致时，此时PWMxB=0*/                             \
	EPwmxRegs.AQSFRC.all 	 = 0;                                                                                                   \
	/*  产生互补的PWM波形 高电平有效互补(AHC)				*/                                                                      \
	/*                                                      */                                                                      \
	/*  ＿＿＿＿￣￣￣￣ ＿＿＿＿￣￣￣￣ ＿＿＿＿＿  EPWMA */                                                                      \
	/*                                                      */                                                                      \
	/* ￣￣￣￣ ＿＿＿＿￣￣￣￣ ＿＿＿＿￣￣￣￣￣   EPWMB */                                                                      \
	/*                                                      */                                                                      \
                                                                                                                                    \
    EPwmxRegs.CMPA.half.CMPA = DRV_PWM_CMPA;                                                                                        \
    EPwmxRegs.CMPB           = DRV_PWM_CMPB;                                                                                        \
                                                                                                                                    \
	/* 设置死区*/                                                                                                                   \
   /*EPwmxRegs.DBRED          	 = DRV_PWM_DBR;      */                                                                                  \
   /*EPwmxRegs.DBFED          	 = DRV_PWM_DBF;        */                                                                                 \
 	/*EPwmxRegs.DBCTL.bit.POLSEL   = DRV_PWM_DBP;		 	死区极性 高电平有效互补(AHC)    */                                       \
    /*	  EPwmxRegs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  使能死区        */                                                      \
                                                                                                                                    \
	EPwmxRegs.PCCTL.all 		 = 0;                                                                                               \
                                                                                                                                    \
	EPwmxRegs.TZSEL.all 		 = 0;                                                                                               \
	EPwmxRegs.TZCTL.all 		 = 0;                                                                                               \
	EPwmxRegs.TZEINT.all 		 = 0;                                                                                               \
	EPwmxRegs.TZCLR.all 		 = 0;                                                                                               \
	EPwmxRegs.TZFRC.all 		 = 0;                                                                                               \
                                                                                                                                    \
}


// 描述：pwm寄存器初始化, 这里用一段宏定义来函数, 实现几个pwm初始化只调用这一个函数
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-6-26

#define drv_pwmx_init(EPwmxRegs)																									\
{                                                                                                                                   \
	EPwmxRegs.TBCTR = 0x0001;                                                                                                       \
   	EPwmxRegs.TBPHS.half.TBPHS = 0;						/* 同步后加载的相位寄存器值*/                                               \
   	                                                                                                                                \
   	EPwmxRegs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;		/* =0x3	禁止SYNCO输出*/   	                                                \
	EPwmxRegs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;     /* 递增递减计数模式*/                                                       \
	/* 设置时钟  */                                                                                                                 \
	EPwmxRegs.TBCTL.bit.CLKDIV    = 0;					/* TBCLK = SYSCLKOUT / (HSPCLKDIV × CLKDIV) = SYSCLKOUT = 150MHz*/         \
	EPwmxRegs.TBCTL.bit.HSPCLKDIV = 0;                                                                                              \
	                                                                                                                                \
	/* 设置映射模式  */                                                                                                             \
    EPwmxRegs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;			/* 0 : 设置成映射模式,作为双缓冲操作，所有的写操作通过CPU访问映射寄存器*/   \
    EPwmxRegs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  		/* 00: 当CTR=0时加载*/                                                      \
    EPwmxRegs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                                                                                     \
    EPwmxRegs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;  		/* Load on Zero*/                                                           \
                                                                                                                                    \
	/* 设置动作限定寄存器AQ，电平设置为：CMPA = 最大时，PWM输出 = 1， CMPA = 0时，PWM输出 = 0*/    		                            \
    EPwmxRegs.AQCTLA.bit.CAU = AQ_SET;            		/* 计数器递增时，跟比较器A一致时，此时PWMxA=1*/                             \
    EPwmxRegs.AQCTLA.bit.CAD = AQ_CLEAR;           		/* 计数器递减时，跟比较器A一致时，此时PWMxA=0*/                             \
    EPwmxRegs.AQCTLB.bit.CAU = AQ_CLEAR;            	/* 计数器递增时，跟比较器A一致时，此时PWMxB=0*/                             \
    EPwmxRegs.AQCTLB.bit.CAD = AQ_SET;           		/* 计数器递减时，跟比较器A一致时，此时PWMxB=1*/                             \
	/* EPwmxRegs.AQCTLA.bit.CAU = AQ_CLEAR;            	 计数器递增时，跟比较器A一致时，此时PWMxA=0*/                             \
    /* EPwmxRegs.AQCTLA.bit.CAD = AQ_SET;           	 计数器递减时，跟比较器A一致时，此时PWMxA=1*/                             \
    /* EPwmxRegs.AQCTLB.bit.CAU = AQ_SET;           	 计数器递增时，跟比较器A一致时，此时PWMxB=1*/                             \
    /* EPwmxRegs.AQCTLB.bit.CAD = AQ_CLEAR;        		 计数器递减时，跟比较器A一致时，此时PWMxB=0*/                             \
	EPwmxRegs.AQSFRC.all 	 = 0;                                                                                                   \
	/*  产生互补的PWM波形 高电平有效互补(AHC)				*/                                                                      \
	/*                                                      */                                                                      \
	/*  ＿＿＿＿￣￣￣￣ ＿＿＿＿￣￣￣￣ ＿＿＿＿＿  EPWMA */                                                                      \
	/*                                                      */                                                                      \
	/* ￣￣￣￣ ＿＿＿＿￣￣￣￣ ＿＿＿＿￣￣￣￣￣   EPWMB */                                                                      \
	/*                                                      */                                                                      \
                                                                                                                                    \
    EPwmxRegs.CMPA.half.CMPA = DRV_PWM_CMPA;                                                                                        \
    EPwmxRegs.CMPB           = DRV_PWM_CMPB;                                                                                        \
                                                                                                                                    \
	/* 设置死区*/                                                                                                                   \
    EPwmxRegs.DBRED          	 = DRV_PWM_DBR;                                                                                     \
    EPwmxRegs.DBFED          	 = DRV_PWM_DBF;                                                                                     \
	EPwmxRegs.DBCTL.bit.POLSEL   = DRV_PWM_DBP;			/* 死区极性 高电平有效互补(AHC)   */                                        \
    EPwmxRegs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  	/* 使能死区                       */                                        \
                                                                                                                                    \
	EPwmxRegs.PCCTL.all 		 = 0;                                                                                               \
                                                                                                                                    \
	EPwmxRegs.TZSEL.all 		 = 0;                                                                                               \
	EPwmxRegs.TZCTL.all 		 = 0;                                                                                               \
	EPwmxRegs.TZEINT.all 		 = 0;                                                                                               \
	EPwmxRegs.TZCLR.all 		 = 0;                                                                                               \
	EPwmxRegs.TZFRC.all 		 = 0;                                                                                               \
                                                                                                                                    \
}


// 开关频率设置。	开关频率不能由外面调用设置了，因为必须由宏函数输入寄存器
#define	DRV_PWM_PERIOD(EPwmxRegs, drv_pwm, fs)   {drv_pwm_period_inner(&drv_pwm, fs);	EPwmxRegs.TBPRD = drv_pwm.period_2;}
void drv_pwm_period_inner(DRV_PWM *p, unsigned long fs)
{
   	if (p->fs == fs)	return;	// 直接返回
SECURE_EXPRESS(
   	p->fs = fs;
   	p->ts = 1.0/fs;
   	p->period_2 = (unsigned)(75000000L * p->ts);
   	p->period_4 = (unsigned)(p->period_2 * 0.5);
)
}


// 描述：pwm初始化
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-1-30

void drv_pwm_init1(void)
{

	InitEPwm1Gpio();									// 来自TI，初始化EPWM寄存器
   	InitEPwm2Gpio();			
   	InitEPwm3Gpio();			
	InitEPwm4Gpio();			
	InitEPwm5Gpio();			
	InitEPwm6Gpio();			

    EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      		// Stop all the TB clocks
    EDIS;

    drv_pwm123x_init(EPwm1Regs);
    drv_pwm123x_init(EPwm2Regs);
    drv_pwm123x_init(EPwm3Regs);
    drv_pwmx_init(EPwm4Regs); 
	drv_pwmx_init(EPwm5Regs); 							// 
	drv_pwmx_init(EPwm6Regs); 							// 

   	// 设置PWM4启动AD
	//EPwm5Regs.TBCTR = 376;		                        // 计数器重设,提前5us,10K开关频率时，一个周期是7500，则5us就是375
	EPwm5Regs.ETSEL.bit.SOCAEN  = 1;        		    // 使能A组的SOC
	EPwm5Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;		    // 当计数器=0事件时，启动ADC转换
	EPwm5Regs.ETPS.bit.SOCAPRD  = 1;        		    // 第一次事件发生就产生SOC脉冲，去启动ADC转换
                                              
	EPwm5Regs.ETSEL.bit.SOCBEN  = 1;        		    // 使能B组的SOC
	EPwm5Regs.ETSEL.bit.SOCBSEL = ET_CTR_ZERO;		    // 当计数器=0事件时，启动ADC转换
	EPwm5Regs.ETPS.bit.SOCBPRD  = 1;        		    // 第一次事件发生就产生SOC脉冲，去启动ADC转换
                                                        

    // 中断
	EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     		// 使能计数器=0事件，
	EPwm5Regs.ETSEL.bit.INTEN  = 1;  					// 使能中断，Enable INT
	EPwm5Regs.ETPS.bit.INTPRD  = ET_1ST;           		// 在第一个事件时就中断
	EPwm5Regs.ETFRC.all		   = 0;
    
    EPwm1Regs.AQCSFRC.all = 5;           				// 开驱动A
    EPwm2Regs.AQCSFRC.all = 5;           				// 开驱动B
    EPwm3Regs.AQCSFRC.all = 5;           				// 开驱动C
    EPwm4Regs.AQCSFRC.all = 0; 			 				// 开驱动A
    EPwm5Regs.AQCSFRC.all = 0;           				// 开驱动B
    EPwm6Regs.AQCSFRC.all = 0; 			 				// 开驱动C
    
	drv_pwm_period_inner(&drv_pwm, 		INV_SWITH_FREQ);// 中断时间，

	DRV_PWM_PERIOD(EPwm1Regs, drv_pwm_inv, 			INV_SWITH_FREQ);		// 逆变A相
 	DRV_PWM_PERIOD(EPwm2Regs, drv_pwm_inv, 			INV_SWITH_FREQ);		// 逆变B相
	DRV_PWM_PERIOD(EPwm3Regs, drv_pwm_inv, 	        INV_SWITH_FREQ);		// 逆变C相
 	DRV_PWM_PERIOD(EPwm4Regs, drv_pwm_grid, 		INV_SWITH_FREQ);		// 网侧A相
	DRV_PWM_PERIOD(EPwm5Regs, drv_pwm_grid,		    INV_SWITH_FREQ);		// 网侧B相
	DRV_PWM_PERIOD(EPwm6Regs, drv_pwm_grid,		    INV_SWITH_FREQ);		// 网侧C相

	EPwm1Regs.TBCTR = 1;
	EPwm2Regs.TBCTR = drv_pwm_inv.period_2*2.0/3.0;
	EPwm3Regs.TBCTR = EPwm2Regs.TBCTR*2.0;

   	DISABLE_INTS();
}




void drv_pwm_grid_start(void)
{
//    EPwm1Regs.AQCSFRC.all = 0;            // 开驱动A
//    EPwm2Regs.AQCSFRC.all = 0;            // 开驱动B
//    EPwm3Regs.AQCSFRC.all = 0;            // 开驱动C

    DRV_CPLD_DO_START_GRID_PWM();
}


void drv_pwm_grid_stop(void)
{
    DRV_CPLD_DO_STOP_GRID_PWM();

//    EPwm1Regs.AQCSFRC.all = 5;            // 强制低
//    EPwm2Regs.AQCSFRC.all = 5;            // 强制低
//    EPwm3Regs.AQCSFRC.all = 5;            // 强制低
}




void drv_pwm_startA(unsigned buck)
{
	if(buck){    
		EPwm1Regs.AQCSFRC.all = 4;            // 开驱动A上管
		EPwm2Regs.AQCSFRC.all = 4;            // 开驱动A上管
		EPwm3Regs.AQCSFRC.all = 4;            // 开驱动A上管
	}else{
		EPwm1Regs.AQCSFRC.all = 1;            // 开驱动A下管
		EPwm2Regs.AQCSFRC.all = 1;            // 开驱动A下管
		EPwm3Regs.AQCSFRC.all = 1;            // 开驱动A下管
	}

    DRV_CPLD_DO_START_PWM();
}



// 描述：pwm停止
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-1-24

void drv_pwm_stopA(void)
{
    DRV_CPLD_DO_STOP_PWM();

    EPwm1Regs.AQCSFRC.all = 5;            // 强制低
	EPwm2Regs.AQCSFRC.all = 5;            // 强制低
	EPwm3Regs.AQCSFRC.all = 5;            // 强制低
}


void drv_pwm_stop(void)
{
	drv_pwm_stopA();
	drv_pwm_grid_stop();
}


//			上管 		下管
//buck： 	MOS			二极管
//boost：	二极管		MOS
void drv_pwm_buck_duty(unsigned buck, float duty)
{
	unsigned tmp;
	
	SAT_VOID(duty, 1.0, -1.0);
	tmp  = (unsigned)(1+drv_pwm.period_4 * (1.0-duty));

	if(buck==1){						    //buck
		EPwm1Regs.CMPA.half.CMPA = tmp;
    	EPwm1Regs.CMPB           = 0;
    	EPwm1Regs.AQCSFRC.all 	 = 4;

		EPwm2Regs.CMPA.half.CMPA = tmp;
    	EPwm2Regs.CMPB           = 0;
    	EPwm2Regs.AQCSFRC.all    = 4;

		EPwm3Regs.CMPA.half.CMPA = tmp;
    	EPwm3Regs.CMPB           = 0;
    	EPwm3Regs.AQCSFRC.all    = 4;
	}else{									//boost
    	EPwm1Regs.CMPA.half.CMPA = 0;
    	EPwm1Regs.CMPB           = tmp;
    	EPwm1Regs.AQCSFRC.all    = 1;

    	EPwm2Regs.CMPA.half.CMPA = 0;
    	EPwm2Regs.CMPB           = tmp;
    	EPwm2Regs.AQCSFRC.all    = 1;

    	EPwm3Regs.CMPA.half.CMPA = 0;
    	EPwm3Regs.CMPB           = tmp;
    	EPwm3Regs.AQCSFRC.all    = 1;
  	}
}
