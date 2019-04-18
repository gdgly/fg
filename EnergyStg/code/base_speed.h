// 示例：声明对象、外部引用对象、引用函数
//    SPEED speed=SPEED_DEFAULTS;
//    extern SPEED speed;


// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19


#ifndef __SPEED_H__
#define __SPEED_H__


typedef struct {
                //内部变量：
				float up, dn;
				float up1s, dn1s;
				float max, min;
				float out;
  				//输入
                //输出：
} SPEED;

#define SPEED_DEFAULTS 	{	0,	0,		\
							0,	0,		\
							0,	0,		\
							0			}

// 声明对象内部函数
void speed_set(SPEED *p, float timeup, float timedn, float max, float min);
void speed_reset(SPEED *p, float u);
float speed_updn(SPEED *p, float T_sample, float in);

void speed_set_rapid(SPEED *p, float timeup, float timedn, float max, float min, float T_sample);

#define SPEED_UPDN_RAPID(P, in, result)			\
{												\
	if(in == P.out){                          	\
		/*不考虑限幅*/     					    \
	}else if(in > P.out){                      	\
		P.out += P.up;							\
		if(P.out > in)		P.out = in;         \
		SAT_VOID(P.out, P.max, P.min);			\
	}else{                                      \
		P.out -= P.dn;                         	\
		if(P.out < in)		P.out = in;         \
		SAT_VOID(P.out, P.max, P.min);			\
	}											\
	/*SAT_VOID(P.out, P.max, P.min);*/			\
	result = P.out;                             \
}

#endif  // __SPEED_H__
