//#include "base_transform.h"

/*
// 描述：CLARK变换，3/2静止变换
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19
void transform_clark(float a, float b, float c, float *p_alpha, float *p_beta)
{
	*p_alpha = (2.0/3.0)    * ((a - 0.5*b) - 0.5*c);			//chenboshi变换
	*p_beta  = (C_SQRT3/3.0)* (         b  -     c);

	// *p_beta   = (2.0/3.0)    * (a - 0.5*b - 0.5*c);		//bose变换
	// *p_alpha  = (C_SQRT3/3.0)* (0 -     b +     c);
}

// 描述：PARK变换，2/2静止->旋转变换
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19

void transform_park(float alpha, float beta, float sin1, float cos1, float *p_d, float *p_q)
{
	*p_d = +alpha * cos1 + beta * sin1;
    *p_q = -alpha * sin1 + beta * cos1;
}


// 描述：PARK变换，2/2旋转->静止变换
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19

void transform_ipark(float d, float q, float sin1, float cos1, float *p_alpha, float *p_beta)
{
	*p_alpha = d * cos1 - q * sin1;
	*p_beta  = d * sin1 + q * cos1;
}

// 描述：
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19
//void transform_next(float sin1, float cos1, float SIN, float COS, float *p_sin, float *p_cos)
void transform_next(float alpha, float beta, float SIN, float COS, float *p_aplha, float *p_beta)
{
	*p_aplha = alpha * COS - beta  * SIN;
	*p_beta = beta  * COS + alpha * SIN;
	
//	*p_sin = sin1 * COS + cos1 * SIN;
//	*p_cos = cos1 * COS - sin1 * SIN;
}*/




//======================================================================================================
// 描述：CLARK变换，3/2静止变换
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19
/*#define MINUS_SQRT3_3		-0.57735026918962576450914878050196		//=-sqrt(3)/3

void transform_clark2(float a, float b, float *p_alpha, float *p_beta)
{
	*p_beta   = a;
	*p_alpha  = (a+2*b)*MINUS_SQRT3_3;
}

// 描述：CLARK逆变换，2/3静止变换，基本用来观测而已
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19
#define SQRT3_2		0.86602540378443864676372317075294		//=sqrt(3)/2
void transform_iclark(float alpha, float beta, float *p_a, float *p_b)
//void transform_iclark(float alpha, float beta, float *p_a, float *p_b, float *p_c)
{
	*p_a = beta;
	*p_b = -SQRT3_2 * alpha - 0.5 * beta;
//	*p_c = +SQRT3_2 * alpha - 0.5 * beta;
}

// 描述：求cos1和sin1的变换，可能用不到
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19

void transform_csin1(float alpha, float beta, float *p_sin1, float *p_cos1)
{
	float z;
	z = alpha * alpha + beta * beta;
	if (z==0){
		*p_sin1=0;
		*p_cos1=1;
	}else{
		z=sqrt(z);
		*p_sin1 = beta  / z;
		*p_cos1 = alpha / z;
	}
}
*/

