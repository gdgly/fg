#include "base.h"
#include "base_clk.h"

CLK_INNER clk_inner = CLK_INNER_DEFAULTS;		//比较特殊的.c文件里定义了个实例

// 描述：时钟计时
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

//void clk_click(unsigned value)	
//{
//	clk_inner.ms0_1 = value;		//内部时钟如果以毫秒的速度，溢出为2^32/1000/3600s/24h/30d=1.65月
//}


// 描述：设置时钟时间，只会进入一次
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

//void clk_start_bell(CLK  *pclk)
//{
//	pclk->bell=clk_inner.ms;
//}
#define clk_start_bell(pclk)   {pclk->bell = clk_inner.ms0_1;}


// 描述：时钟判断
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19
//unsigned int clk_Bjudge_bell(const CLK  *pclk, unsigned long  interval)
//{
//	if ( (clk_inner.ms - pclk->bell) >= interval){
//		return 1;
//	}else{
//		return 0;
//	}
//}
											//在此处乘以10,因为是0.1ms计数
#define clk_Bjudge_bell(pclk, interval)		(  ( (clk_inner.ms0_1 - pclk->bell) >= interval*10 )?	1:	0  )




// 因为每个clk_Bjudge1函数都能令OFF，使能初始化。所以不用专门编写初始化函数。


// 描述：一次定时。Bon=OFF能初始化,Bon=ON能启动
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19
unsigned int clk_Bjudge1(CLK  *pclk, unsigned Bon,	unsigned long  interval)
{
	if (!Bon)								{							pclk->Bout=0;	pclk->Bstate=0;	return 0;}	// 初始化; 定时不启动,下面几句是启动
	if (pclk->Bout)							{															return 1;}	// 多此句
	if (!pclk->Bstate)						{clk_start_bell(pclk);						pclk->Bstate=1;	return 0;}	// 闹钟开始定时;记录当前时刻,开关标识置关
	if (!clk_Bjudge_bell(pclk,interval))	{															return 0;}	// 闹钟没到时间,返回0,下次还能进入	
																		pclk->Bout=1;	pclk->Bstate=0;	return 1;	// 闹钟到时间,返回1,闹钟定时完成
	
//	if (Bon==0)															{							pclk->Bout=0;	pclk->Bstate=0;	return pclk->Bout;}
//	if (Bon==1 && pclk->Bout==1)										{							pclk->Bout=1;	pclk->Bstate=0;	return pclk->Bout;}
//	if (Bon==1 && pclk->Bout==0 && pclk->Bstate==0)						{clk_start_bell(pclk);	pclk->Bout=0;	pclk->Bstate=1;	return pclk->Bout;}
//	
//	if (Bon==1 && pclk->Bout==0 && pclk->Bstate==1 && clk_Bjudge_bell(pclk,interval)==0)	{	pclk->Bout=0;	pclk->Bstate=1;	return pclk->Bout;}
//	
//	if (Bon==1 && pclk->Bout==0 && pclk->Bstate==1 && clk_Bjudge_bell(pclk,interval)==1)	{	pclk->Bout=1;	pclk->Bstate=0;	return pclk->Bout;}
}



// 描述：重复定时。Bon=0能初始化,Bon=1能启动
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19
unsigned int clk_Bjudgen(CLK  *pclk, unsigned Bon,	unsigned long  interval)
{
	if (!Bon)								{							pclk->Bstate=0;	return 0;}
	if (!pclk->Bstate)						{clk_start_bell(pclk);		pclk->Bstate=1;	return 0;}	
	if (!clk_Bjudge_bell(pclk,interval))	{											return 0;}	
																		pclk->Bstate=0;	return 1;
	
//	if (Bon==0)																	{							pclk->Bstate=0;	return 0;}
//	if (Bon==1 && pclk->Bstate==0)												{clk_start_bell(pclk);	pclk->Bstate=1;	return 0;}	
//	if (Bon==1 && pclk->Bstate==1 && clk_Bjudge_bell(pclk,interval)==0)			{							pclk->Bstate=1;	return 0;}	
//	if (Bon==1 && pclk->Bstate==1 && clk_Bjudge_bell(pclk,interval)==1)			{							pclk->Bstate=0;	return 1;}
}

