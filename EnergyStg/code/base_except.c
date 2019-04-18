#include "base.h"
#include "base_except.h"
#include "drv_pwm.h"

EXCEPT except=EXCEPT_DEFAULTS;

#define SET		SET16_BIT
#define CLR		CLR16_BIT
#define SETCLR(ACT, n, var1, var2, var3, var4)	\
{												\
	if (n<=16*1)                				\
		ACT(var1, n-(1     )); 				    \
	else if (n<=16*2)           				\
		ACT(var2, n-(1+16*1));      			\
	else if (n<=16*3)           				\
		ACT(var3, n-(1+16*2));      			\
	else if (n<=16*4)           				\
		ACT(var4, n-(1+16*3));      			\
}

/*
#define CHKCLR(n, var1, var2, var3, var4)							\
{																	\
	if (n<=16*1)                									\
		if (BCHECK16_BIT(var1, n-1-16*0))	CLR(var1, n-1-16*0);    \
	else if (n<=16*2)           									\
		if (BCHECK16_BIT(var1, n-1-16*1))	CLR(var1, n-1-16*1);    \
	else if (n<=16*3)           									\
		if (BCHECK16_BIT(var1, n-1-16*2))	CLR(var1, n-1-16*2);    \
	else if (n<=16*4)           									\
		if (BCHECK16_BIT(var1, n-1-16*3))	CLR(var1, n-1-16*3);    \
}
*/

// 描述：清除故障
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void except_clear_error(void)
{
	if (except.error >= 1000) return;		//表中是2000，实际是1000。
	except.error_back = except.error;
	except.error = 0;
	except.error1 = except.error2 = except.error3 = except.error4 =0;
	except.reset_hook();
}



// 描述：判断、设置警告故障；清除警告于一身
// 		 警告要延时清除,延时200ms
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void except_judge_set_inner(unsigned berror_flag, unsigned n)
{

	if (!berror_flag){		//清除警告
		if (except.warn==n && clk_Bjudge1(&except.clk0, 1, 200L)){
			except.warn = 0;
			SETCLR(CLR, except.warn, except.warn1, except.warn2, except.warn3, except.warn4);
		}
		
	}else{

		if (n<1000){		//置警告
			(void)clk_Bjudge1(&except.clk0, 0, 200L);		//初始化
			except.warn = n;								//warn = 0-999
			SETCLR(SET, except.warn, except.warn1, except.warn2, except.warn3, except.warn4);
		}else{				//置故障
			except.shut_hook();
			if(except.error)	return;
			except.error = n - 1000;						//error赋值前先减去1000，因此error = 0-999
			SETCLR(SET, except.error, except.error1, except.error2, except.error3, except.error4);
		}

	}
}




// 描述：初始化钩子函数名
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void except_set_fname(void (*p1_fname)(void), void (*p2_fname)(void))
{
	except.shut_hook = p1_fname;					//p_fname是个函数指针，得到新的函数名称后，给except.shut_hook
	except.reset_hook = p2_fname;
}


// 描述：内部钩子函数
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void except_hook_inner(void)
{
	//什么也不做，作用：如果忘记初始化，不会令指针乱指
}

