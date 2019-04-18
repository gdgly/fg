// 示例：声明对象、外部引用对象、引用函数
//    PRECHARGE precharge=PRECHARGE_DEFAULTS;
//    extern PRECHARGE precharge;
//    precharge_init(&precharge);
//    precharge_func(&precharge);

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19


#ifndef __PRECHARGE_H__
#define __PRECHARGE_H__

// 声明对象内部函数
void precharge_init1(void);
void precharge_ready_chg(void);
void precharge_ready_dchg(void);
unsigned precharge_chg_func(void);
unsigned precharge_dchg_func(void);
unsigned precharge_release(void);
void precharge_reset(void);
unsigned precharge_MCB_onoff_func(unsigned bonoff);

#endif  // __PRECHARGE_H__
