// 示例：声明对象、外部引用对象、引用函数
//    POWER_init();
//    POWER_func();

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19


#ifndef __POWER_H__
#define __POWER_H__

typedef struct {
                //内部变量：
                FILTER1 filter1_p_ct, 		filter1_q_ct;
                FILTER1 filter1_p_sys, 		filter1_q_sys;
				
				//输出：
				float p_ct,  	q_ct,  		s_ct, 	pf_ct;
				float p_sys,	q_sys,  	s_sys, 	pf_sys;
} POWER;

#define POWER_DEFAULTS {		FILTER1_DEFAULTS,	FILTER1_DEFAULTS,	\
								FILTER1_DEFAULTS,	FILTER1_DEFAULTS	}

// 声明对象内部函数
void power_init1(void);
void power_func(void);


extern POWER power;

#endif  // __POWER_H__

