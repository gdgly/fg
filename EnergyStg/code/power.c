#include "apply.h"
#include "power.h"
#include "pu.h"

POWER power = POWER_DEFAULTS;


void power_init1(void)
{
	filter1_init_rapid(&power.filter1_p_ct, 0.0, drv_pwm.ts, 100.0);					// 截止频率50.0Hz
	
	power.p_ct 		=	power.q_ct 		=	power.s_ct 	=  	power.pf_ct		= 0;
	power.p_sys 	= 	power.q_sys 	=   power.s_sys =  	power.pf_sys 	= 0;
}




void power_func(void)
{	
	float p, q;
	static CLK clk0 = CLK_DEFAULTS;
	
	#define	 T_ms  		100
	
	if (!clk_Bjudgen(&clk0, 1, T_ms))					return;		// 定时执行下面程序

	p = protect_uouta*bmsA_3.N_systemCurrent*UDCPU;	                    // 
	FILTER1_FUNC_RAPID(power.filter1_p_ct,  p, power.p_ct);		    // 直流电压滤波


}
