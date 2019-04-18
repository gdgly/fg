// 示例：声明对象、外部引用对象、引用函数
//    RELAY relay=RELAY_DEFAULTS;
//    extern RELAY relay;

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：3.1
// 日期：星期三,2011-12-28


#ifndef __RELAY_H__
#define __RELAY_H__

typedef struct {
                //内部变量：
				CLK clk;
				void (*frun_hook)(unsigned);
				unsigned bflag1, bflag2;
				unsigned step;
} RELAY;

#define RELAY_DEFAULTS {	CLK_DEFAULTS,				\
							relay_hook_inner,			\
							0, 0,		                \
							0			                }

// 声明对象指针
typedef RELAY *RELAY_HANDLE;

// 声明对象内部函数
void relay_init(RELAY_HANDLE p, void (*p1_fname)(unsigned));
void relay_hook_inner(unsigned);
void relay_reset(RELAY_HANDLE p, unsigned long ms);
unsigned relay_level(RELAY_HANDLE p, unsigned bonoff);

void relay_pulse_ready(RELAY_HANDLE p);
unsigned relay_pulse(RELAY_HANDLE p, unsigned bonoff);


#endif  // __RELAY_H__

