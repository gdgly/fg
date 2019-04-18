// 示例：声明对象、外部引用对象、引用函数
//    Tlimit Tlimit1 = Tlimit_DEFAULTS;
//    extern Tlimit Tlimit1;
//    参看函数

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19


#ifndef __Tlimit_H__
#define __Tlimit_H__

typedef struct {
                //内部变量：
				unsigned bresult;
				float sum;
				CLK clk0, clk1;
                //输入：

                //输出：
} Tlimit;

#define Tlimit_DEFAULTS { 0, 0, CLK_DEFAULTS, CLK_DEFAULTS  }

// 声明对象指针
typedef Tlimit *Tlimit_HANDLE;

// 声明对象内部函数
void Tlimit_init(Tlimit_HANDLE p);
unsigned Tlimit_fix(Tlimit_HANDLE p, float in, float threshold, unsigned long interval);
unsigned Tlimit_anti(Tlimit_HANDLE p, float in, float threshold, float limit1, unsigned long interval1);


// 描述：定时限欠保护
#define	Tlimit_fixn(p, in, limit1, interval)	Tlimit_fix(p, -in, -limit1, interval)



#endif  // __Tlimit_H__

