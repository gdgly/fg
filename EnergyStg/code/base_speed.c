#include "base.h"
#include "base_speed.h"

// 描述：初始化
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void speed_set(SPEED *p, float timeup, float timedn, float max, float min)
{
	p->up1s = (max - min) / timeup;
	p->dn1s = (max - min) / timedn;
	
	p->max = max;
	p->min = min;
 }

void speed_reset(SPEED *p, float u)
{
	p->out = SAT_OUT(u, p->max, p->min);
}

float speed_updn(SPEED *p, float T_sample, float in)
{															
	p->up = T_sample * p->up1s;								
	p->dn = T_sample * p->dn1s;								
	if(in >= p->out){                                       	
                                                            
		 p->out += p->up;                                   	
		 if(p->out > in)		p->out = in;                		
	}else{                                                  
                                                            
		 p->out -= p->dn;                                   	
		 if(p->out < in)		p->out = in;                		
	}                                                       
	                                                        
	SAT_VOID(p->out, p->max, p->min);							
	return p->out;									
}

void speed_set_rapid(SPEED *p, float timeup, float timedn, float max, float min, float T_sample)
{
	p->up1s = (max - min) / timeup;
	p->dn1s = (max - min) / timedn;
	
	p->max = max;
	p->min = min;

	p->up = T_sample * p->up1s;								
	p->dn = T_sample * p->dn1s;								
}
