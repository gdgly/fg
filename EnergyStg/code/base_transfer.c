#include "base.h"
#include "base_transfer.h"

void filter1_init(FILTER1 *p, float value)
{
	p->y_back = value;
	p->fc_T = -1;	//保证初始化
	p->ky = 0;
	p->ku = 1.0;
}

void filter1_init_rapid(FILTER1 *p, float value, float T_sample, float fc)
{
	p->y_back = value;
	p->ky = 1.0/(1 + PI2 * fc * T_sample);
	p->ku = 1.0 - p->ky;
}



// 描述：y = 1/(tao*s+1)  *u
//      	tao*s*y + y = u
//		=>  tao*[y(n)-y(n-1)]/T + y(n) =u(n)
//		=>  tao/T*y(n)-tao/T*y(n-1) + y(n) =u(n)
//		=>  (tao+T)/T*y(n) = tao/T*y(n-1) + u(n)
//		=>  y(n) = tao/(tao+T) *y(n-1) + T/(tao+T) *u(n)
//		=>  y(n) = 1/(1+T/tao) *y(n-1) + ? *u(n)
//		tao=1/wc=1/(2*pi*fc)		T/tao	=  T*2*pi*fc
// 		T : 采样周期，单位:s
// 		fc: 截止频率，单位:Hz
// 		u : 要滤波的ad数据。


float filter1_func(FILTER1 *p, float T_sample, float fc, float u)
{
	if(fabs(fc * T_sample -  p->fc_T) >1e-7){
		p->fc_T = fc * T_sample;
		p->ky = 1.0/(1 + PI2 * p->fc_T);
		p->ku = 1.0 - p->ky;
	}
	p->y_back = p->ky * p->y_back   +    p->ku * u;
	return p->y_back;
}


//============================================二阶滤波===========================================

void filter2_init(FILTER2 *p, float value)
{
	p->y_back2 = p->y_back = value;
	p->fc_T = -1;
	p->ky = 0;
	p->ku = 1.0;
}

float filter2_func(FILTER2 *p, float T_sample, float fc, float u)
{
	if(fabs(fc * T_sample -  p->fc_T) >1e-7){
		p->fc_T = fc * T_sample;
		p->ky = 1.0/(1 + PI2 * p->fc_T);
		p->ku = 1.0 - p->ky;
	}
	p->y_back  = p->ky * p->y_back   +    p->ku * u;
	p->y_back2 = p->ky * p->y_back2  +    p->ku * p->y_back;

	return p->y_back2;
}

//============================================二阶滤波快速===========================================

void filter2_init_rapid(FILTER2 *p, float value, float T_sample, float fc)
{
	p->y_back = value;
	p->y_back2 = value;
	p->ky = 1.0/(1 + PI2 * fc * T_sample);
	p->ku = 1.0 - p->ky;
}



// 描述：初始化integral
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void integral_init(INTEGRAL_HANDLE p, float value)
{
	p->y_back = value;
}



// 描述：y = 1/s  *u
//       s*y = u  =>  [y(n)-y(n-1)]/T =u(n)  =>  y(n) = y(n-1) + T*u(n)
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

float integral_func(INTEGRAL_HANDLE p, float T, float u)
{
	p->y_back =  p->y_back   +   T * u;
	return p->y_back;
}
