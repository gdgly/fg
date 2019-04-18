#include "apply.h"
#include "pll.h"

PLL pll_grid = PLL_DEFAULTS;

// 描述：PLL初始化
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19


void pll_init1(PLL_HANDLE p, float FC, float f0)
{
    float Ton, tao, kp;


    p->FC = FC;
    p->f  = f0;				// if (cmm.fn==60.0)	    p->f = 60.0;	else    p->f = 50.0;
    p->sin = p->sin1 = p->sin2 = 0;
    p->cos = p->cos1 = p->cos2 = 1;

    p->w  = p->f*PI2;			// 必须初始化
    p->wt = 0;					// 必须初始化


	//滤波器
    filter2_init_rapid(&p->filter2,  0.0, drv_pwm.ts, p->FC);

	//PID
    Ton = 1.0/(PI2 * p->FC);
    tao = 5.0*Ton;
    kp  = (tao * 6.0/ 50.0)  /(Ton*Ton);
    pid_init_rapid(&p->pid, kp, 1.0/tao, p->w, 600.0, -600.0, drv_pwm.ts);	// 截止频率=参考下面，pid_init(p, kp, ki, out_reset, max, min)

	//
	p->n = 0;
}



// 描述：PLL工作
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19

//sin1是真实角度=ua，但实际有点滞后ua约0.01弧度，假设ua=0，sin1=-0.01
//sin2是sin1的next角度

//如果输入是正序，逆时针转，即：Ualpha =sin,Ubeta =-cos。锁出ud=0,uq=-1，f=+50HZ。
//如果输入是负序，顺时针转，即：Ualpha =sin,Ubeta = cos。锁出ud=0,uq=+1，f=-50HZ。
//前提：此处乘以-1，p->w = pid_func(&p->pid, drv_pwm.ts, w_ripple, 0.0);
//判断相序，可以这么做，theta = atan2(Ualpha, Ubeta),deta_theta增加正序，deta_theta减少负序。频率低多采几个点。

void pll_func(PLL_HANDLE p, float alpha, float beta)
{
	float d;
	float w_ripple;

	//烧写到FLASH后，1秒内不锁频，等待系统稳定，否则可能会有点问题。
	if(p->n < 1280){		p->n++;		return;	}


	p->sin1 = p->sin2;    	p->cos1 = p->cos2;								//其实sin1=sin2，不过为了编程概念清楚，所以定义两个变量

//	TRANSFORM_NEXT(p->sin1, p->cos1, -0.1564, 0.9877, p->sin, p->cos);	//此处仅需要补偿滞后当前ua的0.01弧度即可
	p->sin = p->sin1;    	p->cos = p->cos1;

	//2->2
	TRANSFORM_PARK(alpha, beta, p->sin1, p->cos1, d, w_ripple);			// 没用到q，所以用w_ripple代替

	// filter+PID
	FILTER2_FUNC_RAPID(p->filter2, d, w_ripple);				// 截止频率0.5HZ
    PID_FUNC_RAPID(p->pid, w_ripple, 0.0, p->w);				// 没有反馈所以=0。

	// 求sin/cos
    p->wt += drv_pwm.ts * p->w;

    if (p->wt >= PI)			p->wt -= PI2;
    else if (p->wt < -PI)		p->wt += PI2;

    p->sin2 = sin(p->wt);    	p->cos2 = cos(p->wt);
    // 求f和采样步长T
    p->f = p->w * (1.0/PI2);											// 没有设置此句：如果频率低于40HZ或者高于70HZ，丢弃

}

