#include "apply.h"
#include "burntest.h"


BURNTEST burntest = BURNTEST_DEFAULTS;


// 描述：输出接入电抗器用铜排短路，满载烧机测试，面板上设置cmm.in1从零开始加载

void burntest_init(void)
{
	burntest.vmax = 1.0;
//	cmm.bzd = 0;
	
	//speed_set(SPEED_HANDLE p, float timeup, float timedn, float max, float min)	
	//									上升时间，下降时间，输出最大f，输出最小f
	speed_set(&burntest.speed_v, 			60.0,  20.0,    1.0,       0.0);	
	speed_reset(&burntest.speed_v, 0.0);	//初始化
}


void burntest_func(void)
{
//	float tmp;
	if(except.error)						return;
		
																						
//	if(!cmm.in3)							{burntest.bwork = 0;	burntest_init();	drv_pwm_bat_stop();	return;}

//	if(cmm.in3 && !burntest.bwork)			{burntest.bwork = 1;	drv_pwm_bat_start();}
	
	//先输出合闸
	//drv_cpld_do_MCB_on_out(1);            // fsm_inv.c有合逆变输出合闸

#define IN1_PRO	1000

	//给定电压
//	if (cmm.in1 > IN1_PRO) cmm.in1 = IN1_PRO;
//	tmp =  burntest.vmax * cmm.in1 * (1.0/IN1_PRO);
	
	//SPEED
//	burntest.v = speed_updn(&burntest.speed_v,   drv_pwm.ts, tmp);
	
	//SVPWM				 频率直接设为1.0=50HZ, 0.0无直流偏移量
	//svpwm_mf(float T_sample, float f, float gain, float offset, float *p_ta, float *p_tb, float *p_tc)
	svpwm_mf(drv_pwm.ts, 1.0, burntest.v, 0.0, &burntest.aduty, &burntest.bduty, &burntest.cduty);	

	//PWM输出
	//DRV_PWM_DUTY(burntest.aduty, burntest.bduty, burntest.cduty, burntest.cduty);
//	DRV_PWM_BAT(burntest.aduty, burntest.bduty, burntest.cduty);
}


// 制动
void burntest_break(void)
{
}

