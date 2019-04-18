// 示例：声明对象、外部引用对象、引用函数
// 统一了下编程风格，其实内部引用的是TI的PID，也没看太懂
//    PID pid=PID_DEFAULTS;
//    extern PID pid;
//	  pid_init(p, kp, ki, out_reset, max, min)
//    pid_init(&pid, kp, ki, kd, kc, max, min);
//    pid_func(&pid, ref, feed);

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19


#ifndef __PID_H__
#define __PID_H__

//#include "IQmathLib.h"
//#include "pid_reg3.h"

/*typedef struct {

} PID;*/

typedef struct {
			    //内部变量：
				  float  err;				// Variable: Error
				  float  up;				// Variable: Proportional output
				  float  ui;				// Variable: Integral output
//				  float  OutPreSat; 		// Variable: Pre-saturated output
				  float  T_sample;

                //输入：
				  float  kp;				// Parameter: Proportional gain
				  float  KI;			    // 此处积分系数，不考虑采样时间，算是临时输入变量。
				  float  ki;			    // Parameter: Integral gain		此处积分系数，考虑采样时间
				  float  max;			    // Parameter: Maximum output
				  float  min;	    		// Parameter: Minimum output

                //输出：

		 	 	} PID;

#define PID_DEFAULTS { 0.0, 			\
                       0.0, 			\
                       0.0, 			\
                       0.001, 			\
                       					\
                       0.0, 			\
                       0.0, 			\
                       0.0, 			\
                       0.0	 			\
              		}

// 声明对象内部函数
void pid_init_rapid(PID *p, float kp, float ki, float reset, float max, float min, float T_sample);
void pid_init(PID *p, float kp, float ki, float reset, float max, float min);
void pid_kpki(PID *p, float kp, float ki);
void pid_reset(PID *p, float reset);
void pid_limit(PID *p, float max, float min);
float pid_func(PID *p, float T_sample, float ref, float feed);



#define PID_FUNC_RAPID(P, ref, feed, out)		\
{												\
	P.err = ref - feed; 						\
	P.up= P.kp * P.err;							\
	P.ui= P.ui + P.ki * P.up;					\
	out= P.up + P.ui;							\
	SAT_VOID(P.ui, P.max, P.min);				\
	SAT_VOID(out, P.max, P.min);				\
}

			
#endif  // __PID_H__

