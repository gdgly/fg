#include "apply.h"
#include "pll.h"

PLL pll_grid = PLL_DEFAULTS;

// ������PLL��ʼ��
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19


void pll_init1(PLL_HANDLE p, float FC, float f0)
{
    float Ton, tao, kp;


    p->FC = FC;
    p->f  = f0;				// if (cmm.fn==60.0)	    p->f = 60.0;	else    p->f = 50.0;
    p->sin = p->sin1 = p->sin2 = 0;
    p->cos = p->cos1 = p->cos2 = 1;

    p->w  = p->f*PI2;			// �����ʼ��
    p->wt = 0;					// �����ʼ��


	//�˲���
    filter2_init_rapid(&p->filter2,  0.0, drv_pwm.ts, p->FC);

	//PID
    Ton = 1.0/(PI2 * p->FC);
    tao = 5.0*Ton;
    kp  = (tao * 6.0/ 50.0)  /(Ton*Ton);
    pid_init_rapid(&p->pid, kp, 1.0/tao, p->w, 600.0, -600.0, drv_pwm.ts);	// ��ֹƵ��=�ο����棬pid_init(p, kp, ki, out_reset, max, min)

	//
	p->n = 0;
}



// ������PLL����
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19

//sin1����ʵ�Ƕ�=ua����ʵ���е��ͺ�uaԼ0.01���ȣ�����ua=0��sin1=-0.01
//sin2��sin1��next�Ƕ�

//���������������ʱ��ת������Ualpha =sin,Ubeta =-cos������ud=0,uq=-1��f=+50HZ��
//��������Ǹ���˳ʱ��ת������Ualpha =sin,Ubeta = cos������ud=0,uq=+1��f=-50HZ��
//ǰ�᣺�˴�����-1��p->w = pid_func(&p->pid, drv_pwm.ts, w_ripple, 0.0);
//�ж����򣬿�����ô����theta = atan2(Ualpha, Ubeta),deta_theta��������deta_theta���ٸ���Ƶ�ʵͶ�ɼ����㡣

void pll_func(PLL_HANDLE p, float alpha, float beta)
{
	float d;
	float w_ripple;

	//��д��FLASH��1���ڲ���Ƶ���ȴ�ϵͳ�ȶ���������ܻ��е����⡣
	if(p->n < 1280){		p->n++;		return;	}


	p->sin1 = p->sin2;    	p->cos1 = p->cos2;								//��ʵsin1=sin2������Ϊ�˱�̸�����������Զ�����������

//	TRANSFORM_NEXT(p->sin1, p->cos1, -0.1564, 0.9877, p->sin, p->cos);	//�˴�����Ҫ�����ͺ�ǰua��0.01���ȼ���
	p->sin = p->sin1;    	p->cos = p->cos1;

	//2->2
	TRANSFORM_PARK(alpha, beta, p->sin1, p->cos1, d, w_ripple);			// û�õ�q��������w_ripple����

	// filter+PID
	FILTER2_FUNC_RAPID(p->filter2, d, w_ripple);				// ��ֹƵ��0.5HZ
    PID_FUNC_RAPID(p->pid, w_ripple, 0.0, p->w);				// û�з�������=0��

	// ��sin/cos
    p->wt += drv_pwm.ts * p->w;

    if (p->wt >= PI)			p->wt -= PI2;
    else if (p->wt < -PI)		p->wt += PI2;

    p->sin2 = sin(p->wt);    	p->cos2 = cos(p->wt);
    // ��f�Ͳ�������T
    p->f = p->w * (1.0/PI2);											// û�����ô˾䣺���Ƶ�ʵ���40HZ���߸���70HZ������

}

