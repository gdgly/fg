#include "apply.h"
#include "precharge.h"
#include "pu.h"

static unsigned precharge_chg_n  = 0;
static unsigned precharge_dchg_n = 0;

RELAY relay_precharge = RELAY_DEFAULTS;
RELAY relay_MCB_on    = RELAY_DEFAULTS;


void precharge_init1(void)
{
 	relay_init(&relay_precharge, 	drv_cpld_do_prechg_grid);
	relay_init(&relay_MCB_on, 		drv_cpld_do_MCB_grid);

	relay_level(&relay_MCB_on, 0);	//上电时，所有继电器放0
	relay_level(&relay_precharge, 0);

	relay_pulse_ready(&relay_MCB_on);
	precharge_ready_chg();			//
	precharge_ready_dchg();			//
	(void)precharge_release();

}

void precharge_ready_chg(void)
{
	precharge_chg_n =0;	
}

void precharge_ready_dchg(void)
{
	precharge_dchg_n=0;	
}


// 描述：PRECHARGE工作
unsigned precharge_chg_func(void)
{
	//0：预充电接触器合闸
	//1：10s充不上电——直流电压低，报故障
	//2：大开关合闸
	//3：最后还要，预充电接触器分闸
	//4：直流电压低，报故障

	switch (precharge_chg_n){
		case 0:																			precharge_chg_n++;	break;	//预充电时不能放电
		case 1:	if (relay_level(&relay_precharge, 1))									precharge_chg_n++;	break;	//
		case 2:	if (drv_ad.udc < 0.6)
				{EXCEPT_JUDGE_SET(1, ERROR_PREGRID);	}else 	
																						precharge_chg_n++;	break;
		case 3:	if (precharge_MCB_onoff_func(1))										precharge_chg_n++;	break;
		case 4:	relay_level(&relay_precharge, 0);										precharge_chg_n++;	break;	//这里不再延时
		case 5:	if (drv_ad.udc < 0){	EXCEPT_JUDGE_SET(1, ERROR_PREGRID);	}else	    				    return 1;
//		case 4:																							    return 1;
	}
	return 0;
}

unsigned precharge_dchg_func(void)
{
    static CLK clk1 = CLK_DEFAULTS;

	switch (precharge_dchg_n){
		case 0:	drv_cpld_do_dis(0);														precharge_dchg_n++;	break;	//预充电时不能放电
		case 1:	drv_cpld_do_prechg_bat(1);	(void)clk_Bjudge1(&clk1, 0, 5000L);		    precharge_dchg_n++;	break;	//预充电合闸
		case 2:	if (clk_Bjudge1(&clk1, 1, 5000L))  									    precharge_dchg_n++;	break;	//延时10S
		case 3:	if (drv_ad.udc < 0.2){EXCEPT_JUDGE_SET(1, ERROR_PREBAT);	}else 	    precharge_dchg_n++;	break;  //是否充好
		case 4:	drv_cpld_do_MCB_bat(1);         										precharge_dchg_n++;	break;  //合大开关
		case 5:	drv_cpld_do_prechg_bat(0);		        								precharge_dchg_n++;	break;	//预充电断开
		case 6:	if (drv_ad.udc < 0){	EXCEPT_JUDGE_SET(1, ERROR_PREBAT);	}else		precharge_dchg_n=0;	return 1;
	}
	return 0;
}


//断所有开关
unsigned precharge_release(void)
{
//	drv_cpld_do_MCB_gridCAB(0);

    drv_cpld_do_prechg_bat(0);	
    drv_cpld_do_MCB_bat(0);

 	drv_cpld_do_prechg_grid(0);
	drv_cpld_do_MCB_grid(0);

	return(1);
}


void precharge_reset(void)
{
	relay_reset(&relay_precharge, 10000L);			//10s
}




unsigned precharge_MCB_onoff_func(unsigned bonoff)
{
	if (bonoff){							// bonoff=1:合闸、不放电；bonoff=0:分闸、放电
		relay_level(&relay_MCB_on, 1);		
		return 1;
	}else{
		relay_level(&relay_MCB_on, 0);		// 分闸时, 先分闸, 再放电
		return 1;
	}
	
	return 0;
}
