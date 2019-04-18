#include "base_pid.h"
#include "base.h"


//Ti给的是Kp+Kp*Ki/s=Kp(1+Ki/s)，跟陈伯时的一样：Kp(tao*s+1)/(tao*s)
void pid_init(PID *p, float kp, float ki, float reset, float max, float min)
{				
	p->kp = kp;					
	p->KI = ki;
	p->max = max;
	p->min = min;
	p->ui = reset;
}

void pid_init_rapid(PID *p, float kp, float ki, float reset, float max, float min, float T_sample)
{				
	p->kp = kp;					
	p->KI = ki;
	p->max = max;
	p->min = min;
	p->ui = reset;
	p->T_sample = T_sample;
	p->ki = p->KI * p->T_sample;
}

void pid_kpki(PID *p, float kp, float ki)
{									
	p->kp = kp;			           	
	p->KI = ki;
	p->ki = ki * p->T_sample;                    	
}

void pid_reset(PID *p, float reset)
{
	p->ui = reset;
}

void pid_limit(PID *p, float max, float min)
{
	p->max = max;
	p->min = min;
}

// 描述：PID工作
float pid_func(PID *p, float T_sample, float ref, float feed)
{	
	float out;	
	
	p->ki = p->KI * T_sample;
										
	p->err = ref - feed; 						
	p->up = p->kp * p->err;							
	p->ui = p->ui + p->ki * p->up;					
	out = p->up + p->ui;							
	SAT_VOID(p->ui, p->max, p->min);				
	SAT_VOID(out, p->max, p->min);				
	
	return out;
}

