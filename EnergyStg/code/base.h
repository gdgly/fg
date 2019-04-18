// �������ײ�����ͷ�ļ��������ⲿ����
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

#ifndef __BASE_H__
#define __BASE_H__

//------------------------- �궨����---------------------------------//

// ��������
#define C_1_3 					0.3333333333333333333333333333
#define C_SQRT3					1.732050807568877
#define C_SQRT2 				1.414213562373095
#define PI						3.141592653589793
#define PI2						(PI*2.0)
#define PI2_3					(PI*2.0/3.0)

// �򵥺���
#define SIGN(x)					(((x)>=(0))?		(1):	(-1))
#define SQ2(x)					((x)*(x))
#define SQ2_SUM(x,y)			((x)*(x) + (y)*(y))
#define MAX2(x,y)				(((x)>=(y))?		(x):	(y))
#define MIN2(x,y)				(((x)<=(y))?		(x):	(y))
#define MAX3(x,y,z)				MAX2(MAX2(x, y), z) 			//ʵ�ʵ���3��?����
#define MIN3(x,y,z)				MIN2(MIN2(x, y), z)
#define MAXMIN(x,y,z,max,min)	{max = MAX2(x, y);		min = x + y - max;		\
								max = MAX2(z, max);		min = MIN2(z, min);		}
#define SAT_OUT(a,max,min)		MAX2(MIN2(a,max),min)
#define SAT_VOID(a,max,min)		{if(a > max) a = max;	else if ( a < min)	a =min;}
#define ACC_MOD2(a,n)			a=(++a)&n						//n=2^k - 1, k=2,3,4,5...
#define MODN(a,N)				((a) - (int)((a)*(1.0/N))*N )	//������

// λ������bit=0-15��0-31
#define SET16_BIT(data,bit)  	(	(data) = (data) |  ((unsigned int)1  << (bit))				)	//a=a|80h		ĳλ��1�����ı������λ
#define SET32_BIT(data,bit)  	(	(data) = (data) |  ((unsigned long)1 << (bit))				)
#define CLR16_BIT(data,bit)    	(	(data) = (data) & ~((unsigned int)1  << (bit))				)	//a=a&~80h		ĳλ��0�����ı������λ  ~ֻ������unsigned�ϣ�502
#define CLR32_BIT(data,bit)    	(	(data) = (data) & ~((unsigned long)1 << (bit))				)
#define BCHECK16_BIT(data,bit)  (		   ( (data) &  ((unsigned int)1  << (bit)) )  >> (bit)	)	//a&80h==80h	����ĳλ�����ı�a��ֵ
#define BCHECK32_BIT(data,bit)  (		   ( (data) &  ((unsigned long)1 << (bit)) )  >> (bit)	)



//----------------------- ͷ�ļ�������--------------------------------//
#include "math.h"

#include "base_clk.h"
#include "base_except.h"

#include "base_pid.h"
#include "base_transfer.h"
#include "base_transform.h"
#include "base_speed.h"

//base��=================================================================================
//#pragma CODE_SECTION(clk_click   				, "ramfuncs");
#pragma CODE_SECTION(clk_Bjudge1   				, "ramfuncs");
#pragma CODE_SECTION(clk_Bjudgen   				, "ramfuncs");
#pragma CODE_SECTION(except_judge_set_inner		, "ramfuncs");

#pragma CODE_SECTION(filter1_func				, "ramfuncs");
#pragma CODE_SECTION(filter2_func				, "ramfuncs");

#pragma CODE_SECTION(pid_kpki   				, "ramfuncs");
#pragma CODE_SECTION(pid_limit  	 			, "ramfuncs");
#pragma CODE_SECTION(pid_func					, "ramfuncs");

#pragma CODE_SECTION(speed_updn   				, "ramfuncs");

//#pragma CODE_SECTION(transform_clark			, "ramfuncs");
//#pragma CODE_SECTION(transform_iclark			, "ramfuncs");
//#pragma CODE_SECTION(transform_park			, "ramfuncs");
//#pragma CODE_SECTION(transform_ipark			, "ramfuncs");
//#pragma CODE_SECTION(transform_park_n			, "ramfuncs");
//#pragma CODE_SECTION(transform_ipark_n		, "ramfuncs");
//#pragma CODE_SECTION(transform_park_z			, "ramfuncs");
//#pragma CODE_SECTION(transform_ipark_z		, "ramfuncs");

#endif  // __BASE_H__

