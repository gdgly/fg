#include "apply.h"
#include "relay.h"

void relay_init(RELAY_HANDLE p, void (*p1_fname)(unsigned) )
{
	p->frun_hook = p1_fname;
	p->bflag1 	   = 0;			p->bflag2      = 0;
	p->step        = 0;
}

void relay_hook_inner(unsigned bflag)
{
	//什么也不做，作用：如果忘记初始化，不会令指针乱指
}

//bonoff = 1合闸，0分闸
//bflag1记录当前需执行的瞬时命令。
//bflag2记录最终执行完的状态。
//时序：以最后命令为立即执行。另一种时序没实现：以第一个命令执行必须执行完，才执行第二个。
//主要验证逻辑包括两种：5小时、50ms来一次的命令如何执行
unsigned relay_level(RELAY_HANDLE p, unsigned bonoff)
{
	p->frun_hook(bonoff);
	p->bflag1 = bonoff;			

	if(p->bflag1 == p->bflag2)		
		return 1;
	else
		return 0;
}


// 应用举例：
//relay_pulse_ready();	//先打开，或再次打开。	不用管关闭，自动关闭
//relay_pulse(p, 1);
void relay_pulse_ready(RELAY_HANDLE p)
{
	p->step = 0;
}

//bonoff_back记录上次bonoff命令。
//step记录执行脉冲操作的步骤，共2步：0初始化操作，1给完第一个电平，2给完第二个电平（进入稳态）。
//发完一个脉冲，自动关闭
unsigned relay_pulse(RELAY_HANDLE p, unsigned bonoff)
{
	if(p->step==2)		return 1;
		
	switch (p->step){
		case 0:	if (relay_level(p, bonoff))									p->step++;	break;
		case 1:	if (relay_level(p, !bonoff))								p->step++;	break;
	}
	return 0;
}



// 描述：合闸分闸线圈需延时清掉才真正成立。这个函数放在主函数中实现此功能。
// 不能用bjudge1，必须bjudgen，因为能定时器能自动清零。    
// 错误发生情况：脉冲函数中，case 0 置1，case 1置0，且case 0后紧接着执行 case 1，没有机会执行reset函数时
void relay_reset(RELAY_HANDLE p, unsigned long ms)
{
	if (clk_Bjudgen(&p->clk,  p->bflag1 != p->bflag2, ms)){		
		p->bflag2 = p->bflag1;									
	}
}
