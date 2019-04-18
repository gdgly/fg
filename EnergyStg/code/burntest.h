// 示例：声明对象、外部引用对象、引用函数
//    BURNTEST burntest=BURNTEST_DEFAULTS;
//    extern BURNTEST burntest;
//    burntest_init();
//    burntest_func();

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19


#ifndef __BURNTEST_H__
#define __BURNTEST_H__

typedef struct {
                //内部变量：

                //SPEED
                SPEED speed_v;
				unsigned bwork;
				float vmax, v;

				//SVPWM
				float aduty, bduty, cduty;
} BURNTEST;


#define BURNTEST_DEFAULTS {  	SPEED_DEFAULTS, 0, 0}

// 声明对象指针
typedef BURNTEST *BURNTEST_HANDLE;

// 声明对象内部函数
void burntest_init(void);
void burntest_func(void);
void burntest_break(void);

extern BURNTEST burntest;


#endif  // __BURNTEST_H__

