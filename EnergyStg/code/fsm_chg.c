#include "apply.h"
#include "fsm_chg.h"

unsigned fsm_chg_bwork  = 0;			// 
unsigned fsm_chg_bready = 0;			// 
unsigned fsm_chg_bstart = 0;			// 
unsigned fsm_chg_bstate = 0;			// 0: 没充满
										// 1: 充满

static enum {		//此枚举名称无意义
	S0 = 0,			//
	S1,				//
	S2,				//
	S3,				//
					//
	S_END
} S=S0, S_OLD=S1;

#define	FSM_GO_NEXT_RETURN()	 	{if(S>=(S_END-1)) 	S=S0;	else	S++;	return;}
#define	FSM_GO_STOP()	 			{S=S0;	return;}


//=======================================================状态0:STOP===============================================================
void fsm_chg_f0_stop_in(void)
{
	drv_pwm_stopA();			//

	fsm_chg_bwork    = 0;

	drv_cpld_do_cab_fan(0);		//关机关风扇

	drv_cpld_do_prechg_bat(0);	//放电关机, 模块内开关都可关
    drv_cpld_do_MCB_bat(0);

 	drv_cpld_do_prechg_grid(0);
	drv_cpld_do_MCB_grid(0);
}

void fsm_chg_f0_stop(void)
{
	if(fsm_chg_bready && except.warn == 0 && except.error == 0)	FSM_GO_NEXT_RETURN();	//状态0 =>状态1
}



//=======================================================状态1:READY===============================================================

void fsm_chg_f1_prechg_in(void)
{
	precharge_ready_chg();			//合闸前必须先复位
}

void fsm_chg_f1_prechg(void)
{
	unsigned bflag;

	if(!fsm_chg_bready || except.error != 0)			FSM_GO_STOP();	//状态X =>状态0

	drv_cpld_do_cab_fan(1);							//现在就开机运行风扇，不停机。以后再改。
	
	bflag = precharge_chg_func();
	if(bflag && fsm_chg_bstart && except.warn == 0 && except.error == 0)	FSM_GO_NEXT_RETURN(); 	//状态1 =>状态2
														
}


//=======================================================状态2:STARTUP===============================================================
static CLK clk2_1 = CLK_DEFAULTS;
static CLK clk2_2 = CLK_DEFAULTS;
void fsm_chg_f2_switch_in(void)
{
	drv_cpld_reset_error();							//大开关合闸后，有干扰引起cpld过流故障。
	(void)clk_Bjudge1(&clk2_1, 0, 1000L);
	(void)clk_Bjudge1(&clk2_2, 0, 1000L);
}

void fsm_chg_f2_switch(void)
{
	if(except.error != 0)								FSM_GO_STOP();	//状态X =>状态0

	if(!clk_Bjudge1(&clk2_1, 1, 1000L))	return;	                        // 稳定
	drv_cpld_do_MCB_bat(1);										        // 合电池侧开关

	if(!clk_Bjudge1(&clk2_2, 1, 1000L))	return;	                        // 稳定

								 						FSM_GO_NEXT_RETURN();		//状态2 =>状态3
}



//=======================================================状态3:WORK===============================================================

void fsm_chg_f3_work_in(void)
{
}

void fsm_chg_f3_work(void)
{
	if(except.error != 0)								FSM_GO_STOP();		//状态X =>状态0

	// 充电管理
	// 1, 充电电流设置
	charge_chg_setI();
	// 2, 快充满时降电流
//bltest 还要从单体电压改成总电压	charge_chg_downI();

	//正常关机条件：关机命令 或者充满电
	if(!fsm_chg_bstart) FSM_GO_STOP();		//状态3 =>状态0
}



void fsm_chg_func(void)
{
	do
	{
	    fsm_chg_bwork = (S >= S3) ? 1 : 0;
		if(except.error != 0 || !fsm_chg_bstart)		S = S0;				//故障后 =>状态0

		switch(S)
		{
			case S0:	if(S != S_OLD)	{S_OLD = S;		fsm_chg_f0_stop_in();	}		fsm_chg_f0_stop();				break;
			case S1:	if(S != S_OLD)	{S_OLD = S;		fsm_chg_f1_prechg_in();	}		fsm_chg_f1_prechg();			break;
			case S2:	if(S != S_OLD)	{S_OLD = S;		fsm_chg_f2_switch_in();	}		fsm_chg_f2_switch();			break;
			case S3:	if(S != S_OLD)	{S_OLD = S;		fsm_chg_f3_work_in();   }		fsm_chg_f3_work();				break;
		
		}
	}while(S != S_OLD);
}
