#include "apply.h"
#include "burntest.h"


BURNTEST burntest = BURNTEST_DEFAULTS;


// �������������翹����ͭ�Ŷ�·�������ջ����ԣ����������cmm.in1���㿪ʼ����

void burntest_init(void)
{
	burntest.vmax = 1.0;
//	cmm.bzd = 0;
	
	//speed_set(SPEED_HANDLE p, float timeup, float timedn, float max, float min)	
	//									����ʱ�䣬�½�ʱ�䣬������f�������Сf
	speed_set(&burntest.speed_v, 			60.0,  20.0,    1.0,       0.0);	
	speed_reset(&burntest.speed_v, 0.0);	//��ʼ��
}


void burntest_func(void)
{
//	float tmp;
	if(except.error)						return;
		
																						
//	if(!cmm.in3)							{burntest.bwork = 0;	burntest_init();	drv_pwm_bat_stop();	return;}

//	if(cmm.in3 && !burntest.bwork)			{burntest.bwork = 1;	drv_pwm_bat_start();}
	
	//�������բ
	//drv_cpld_do_MCB_on_out(1);            // fsm_inv.c�к���������բ

#define IN1_PRO	1000

	//������ѹ
//	if (cmm.in1 > IN1_PRO) cmm.in1 = IN1_PRO;
//	tmp =  burntest.vmax * cmm.in1 * (1.0/IN1_PRO);
	
	//SPEED
//	burntest.v = speed_updn(&burntest.speed_v,   drv_pwm.ts, tmp);
	
	//SVPWM				 Ƶ��ֱ����Ϊ1.0=50HZ, 0.0��ֱ��ƫ����
	//svpwm_mf(float T_sample, float f, float gain, float offset, float *p_ta, float *p_tb, float *p_tc)
	svpwm_mf(drv_pwm.ts, 1.0, burntest.v, 0.0, &burntest.aduty, &burntest.bduty, &burntest.cduty);	

	//PWM���
	//DRV_PWM_DUTY(burntest.aduty, burntest.bduty, burntest.cduty, burntest.cduty);
//	DRV_PWM_BAT(burntest.aduty, burntest.bduty, burntest.cduty);
}


// �ƶ�
void burntest_break(void)
{
}

