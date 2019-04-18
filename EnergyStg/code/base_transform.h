#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

// ������CLARK�任�����任��3/2��ֹ�任�����任��			ȫ��float
#define TRANSFORM_CLARK(a, b, c, alpha, beta)			\
{														\
	alpha = (2.0/3.0)    * ((a) - 0.5*(b) - 0.5*(c));	\
	beta  = (C_SQRT3/3.0)* (          (b)  -    (c));	\
}

#define TRANSFORM_ICLARK(alpha, beta, a, b, c)			\
{														\
	a = alpha;											\
	b = -0.5 * (alpha) + (C_SQRT3/2.0) * (beta);		\
	c = -0.5 * (alpha) - (C_SQRT3/2.0) * (beta);		\
}


// ������PARK�任�����任��2/2��ֹ->��ת�任�����任��		ȫ��float
#define TRANSFORM_PARK(alpha, beta, sin1, cos1, d, q)	\
{														\
	d = +(alpha) * (cos1) + (beta) * (sin1);			\
	q = -(alpha) * (sin1) + (beta) * (cos1);			\
}

#define TRANSFORM_IPARK(d, q, sin1, cos1, alpha, beta)	\
{														\
	alpha = (d) * (cos1) - (q) * (sin1);				\
	beta  = (d) * (sin1) + (q) * (cos1);				\
}


// ������PARK����任�����任��2/2��ֹ->��ת�任�����任��	ȫ��float
#define TRANSFORM_PARK_N(alpha, beta, sin1, cos1, d, q)	\
{														\
	d =  (alpha) * (cos1) - (beta) * (sin1);			\
	q = -(alpha) * (sin1) - (beta) * (cos1);			\
}

#define TRANSFORM_IPARK_N(d, q, sin1, cos1, alpha, beta)\
{														\
	alpha =  (d) * (cos1) - (q) * (sin1);				\
	beta  = -(d) * (sin1) - (q) * (cos1);				\
}


// ������PARK����任�����任��2/2��ֹ->��ת�任�����任��	ȫ��float
// i0=(a+b+c)*2/3����Ϊ�������2�����Դ˴�����2
// i0 =  2*((d) * (sin1) + (q) * (cos1));
#define TRANSFORM_PARK_Z(z0, sin1, cos1, d, q)			\
{														\
	d =  -(z0) * (sin1);								\
	q =   (z0) * (cos1);								\
}

#define TRANSFORM_IPARK_Z(d, q, sin1, cos1, z0)			\
{														\
	z0 =  ((d) * (sin1) + (q) * (cos1));				\
}


#define TRANSFORM_AHEAD		TRANSFORM_IPARK				// ������������������ǰ
#define TRANSFORM_LEG		TRANSFORM_PARK				// �����������������ͺ�

#define TRANSFORM_AHEAD_N	TRANSFORM_PARK				// ������������������ǰ
#define TRANSFORM_LEG_N		TRANSFORM_IPARK				// �����������������ͺ�


// ���������ڲ�����
//void transform_clark(float a, float b, float c, float *p_alpha, float *p_beta);
//void transform_park(float alpha, float beta, float sin1, float cos1, float *p_d, float *p_q);
//void transform_ipark(float d, float q, float sin1, float cos1, float *p_alpha, float *p_beta);

//void transform_clark2(float a, float b, float *p_alpha, float *p_beta);
//void transform_iclark(float alpha, float beta, float *p_a, float *p_b);
//void transform_iclark(float alpha, float beta, float *p_a, float *p_b, float *p_c);
//void transform_csin(float alpha, float beta, float *p_sin, float *p_cos);


#endif  // __TRANSFORM_H__

