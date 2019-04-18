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

	relay_level(&relay_MCB_on, 0);	//�ϵ�ʱ�����м̵�����0
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


// ������PRECHARGE����
unsigned precharge_chg_func(void)
{
	//0��Ԥ���Ӵ�����բ
	//1��10s�䲻�ϵ硪��ֱ����ѹ�ͣ�������
	//2���󿪹غ�բ
	//3�����Ҫ��Ԥ���Ӵ�����բ
	//4��ֱ����ѹ�ͣ�������

	switch (precharge_chg_n){
		case 0:																			precharge_chg_n++;	break;	//Ԥ���ʱ���ܷŵ�
		case 1:	if (relay_level(&relay_precharge, 1))									precharge_chg_n++;	break;	//
		case 2:	if (drv_ad.udc < 0.6)
				{EXCEPT_JUDGE_SET(1, ERROR_PREGRID);	}else 	
																						precharge_chg_n++;	break;
		case 3:	if (precharge_MCB_onoff_func(1))										precharge_chg_n++;	break;
		case 4:	relay_level(&relay_precharge, 0);										precharge_chg_n++;	break;	//���ﲻ����ʱ
		case 5:	if (drv_ad.udc < 0){	EXCEPT_JUDGE_SET(1, ERROR_PREGRID);	}else	    				    return 1;
//		case 4:																							    return 1;
	}
	return 0;
}

unsigned precharge_dchg_func(void)
{
    static CLK clk1 = CLK_DEFAULTS;

	switch (precharge_dchg_n){
		case 0:	drv_cpld_do_dis(0);														precharge_dchg_n++;	break;	//Ԥ���ʱ���ܷŵ�
		case 1:	drv_cpld_do_prechg_bat(1);	(void)clk_Bjudge1(&clk1, 0, 5000L);		    precharge_dchg_n++;	break;	//Ԥ����բ
		case 2:	if (clk_Bjudge1(&clk1, 1, 5000L))  									    precharge_dchg_n++;	break;	//��ʱ10S
		case 3:	if (drv_ad.udc < 0.2){EXCEPT_JUDGE_SET(1, ERROR_PREBAT);	}else 	    precharge_dchg_n++;	break;  //�Ƿ���
		case 4:	drv_cpld_do_MCB_bat(1);         										precharge_dchg_n++;	break;  //�ϴ󿪹�
		case 5:	drv_cpld_do_prechg_bat(0);		        								precharge_dchg_n++;	break;	//Ԥ���Ͽ�
		case 6:	if (drv_ad.udc < 0){	EXCEPT_JUDGE_SET(1, ERROR_PREBAT);	}else		precharge_dchg_n=0;	return 1;
	}
	return 0;
}


//�����п���
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
	if (bonoff){							// bonoff=1:��բ�����ŵ磻bonoff=0:��բ���ŵ�
		relay_level(&relay_MCB_on, 1);		
		return 1;
	}else{
		relay_level(&relay_MCB_on, 0);		// ��բʱ, �ȷ�բ, �ٷŵ�
		return 1;
	}
	
	return 0;
}
