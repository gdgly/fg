// 示例：声明对象、外部引用对象、引用函数
//    PLL pll=PLL_DEFAULTS;
//    extern PLL pll;
//    pll_init(&pll);
//    pll_func(&pll);

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19


#ifndef __PLL_H__
#define __PLL_H__

typedef struct {
                //内部变量：
				FILTER2 filter2;
				PID pid;
				unsigned n;				
				float FC;
				
				float w, wt;
                float sin1, cos1, sin2, cos2;			//对内

                //输出：

                float f;
                float sin, cos;				//对外
} PLL;

#define PLL_DEFAULTS {			FILTER1_DEFAULTS,		\
								PID_DEFAULTS,			\
 				      }

// 声明对象指针
typedef PLL *PLL_HANDLE;

extern PLL pll_grid;

// 声明对象内部函数
void pll_init1(PLL_HANDLE p, float FC, float f0);
void pll_func(PLL_HANDLE p, float alpha, float beta);

#define		PLL_ANGLE		(PI2/200)			//以前补偿一个载波，现在减去半个载波。看不懂以前这句话，但我觉得应该还是补偿一个载波



#endif  // __PLL_H__

