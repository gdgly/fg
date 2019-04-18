// 比较特殊的.c文件里定义了个实例

// 示例：声明对象、外部引用对象、引用函数
// 严格来说本模块包括两个模块：bell模块和clk模块，因此对bell模块操作不需要clk地址
//    CLK clk0 = CLK_DEFAULTS;
//    clk_click(nkHZ);
//	  clk_Bjudge1(&clk0, ON, 20);
//	  clk_Bjudgen(&clk0, OFF, 20);


//	3、定时执行一次，且死循环等待，这种情况比较少见
//	定义timer0是局部变量即可
//	while( !clk_Bjudgen(&clk0, 1, 20) ) ;				//死循环等待20ms
//	clk_Bjudgen(&clk0, 0, 20);							//初始化
//	......执行判断成功后的程序;

//	1、定时执行一次，或PLC定时器输出
//	定义一个静态或全局变量clk0。
//	z=clk_Bjudge1(&clk0, 1, 20);

//	2、定时执行无穷次
//	定义一个静态或全局变量clk0。
//	if (!clk_Bjudgen(&clk0, 1, 20))	 	return;
//	......执行判断成功后的定时程序;



// 描述：时钟模块头文件
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

#ifndef __CLK_H__
#define __CLK_H__

typedef struct {
	unsigned long  bell;				// 闹钟
	unsigned       Bstate;				// 进入开关标识,0=能进入闹钟设置(开)，不能进入闹钟判断；1=不能进入闹钟设置(关)，能进入闹钟判断；
	unsigned int   Bout;				// Bout是clk_Bclk1()用到的输出状态，在此一起定义。
} CLK;
#define CLK_DEFAULTS {	0,0,0}


typedef struct {
	// 输出
	//unsigned long  ms;				// 时钟基本步长, 1ms一步
	unsigned long  ms0_1;				// 时钟基本步长, 0.1ms一步
} CLK_INNER;

#define CLK_INNER_DEFAULTS {0}		// 采用1

// 声明对象指针
typedef CLK *CLK_HANDLE;

// 声明对象内部函数
//void clk_click(unsigned nkhz);
unsigned int clk_Bjudge1(CLK  *pclk, unsigned Bon,	unsigned long  interval);
unsigned int clk_Bjudgen(CLK  *pclk, unsigned Bon,	unsigned long  interval);

extern CLK clk;
extern CLK_INNER clk_inner;


// 描述：时钟计时
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19
//void 	clk_click(unsigned value)
#define	CLK_CLICK_FS(value)		CLK_CLICK(value)
#define	CLK_CLICK(value)		(clk_inner.ms0_1 = value)			//clk_click()函数写成一个宏
#define	CLK_CLICK_TEST()		(clk_inner.ms0_1++)					//仅用于测试，比如cunit.c中


#endif  // __CLK_H__
