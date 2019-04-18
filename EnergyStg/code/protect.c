#include "apply.h"
#include "protect.h"
#include "pu.h"
#include "test.h"

static 	Tlimit ia_Tlimit     = Tlimit_DEFAULTS,	ib_Tlimit     = Tlimit_DEFAULTS,	ic_Tlimit     = Tlimit_DEFAULTS;
static	Tlimit ia_out_Tlimit = Tlimit_DEFAULTS,	ib_out_Tlimit = Tlimit_DEFAULTS,	ic_out_Tlimit = Tlimit_DEFAULTS;
unsigned protect_k         = 1;
unsigned protect_close_dis = 1;
FILTER1	filter1_uouta      = FILTER1_DEFAULTS;
float   protect_uouta      = 0;

void protect_init(void)
{
	Tlimit_init(&ia_Tlimit);	Tlimit_init(&ib_Tlimit);	Tlimit_init(&ic_Tlimit);
	Tlimit_init(&ia_out_Tlimit);Tlimit_init(&ib_out_Tlimit);Tlimit_init(&ic_out_Tlimit);

	filter1_init_rapid(&filter1_uouta, 0.0, drv_pwm.ts, 5.0);	 // 截止频率5.0Hz  
}


void judge_ACB(void);

void protect_over(void)
{
	static CLK clk_bprotest = CLK_DEFAULTS;
	static CLK clk_disov    = CLK_DEFAULTS;
	static CLK clk0         = CLK_DEFAULTS;
	
	unsigned bfalg_dis_over = 0;
	
	if (!clk_Bjudge1(&clk_bprotest, 1, 5000L))		return;

	// 瞬时电压、电流平衡过保护
	EXCEPT_JUDGE_SET(fabs(drv_ad.ua_in + drv_ad.ub_in + drv_ad.uc_in) > 0.3, ERROR_U_IMBALANCE);
	EXCEPT_JUDGE_SET(MAX3(rms_ua_in, rms_ub_in, rms_uc_in) - MIN3(rms_ua_in, rms_ub_in, rms_uc_in) > 0.3, ERROR_PHASE_LOSS);
	EXCEPT_JUDGE_SET(PcsA_1.BatRunState && fabs(drv_ad.ia_in + drv_ad.ib_in + drv_ad.ic_in) > 0.8, ERROR_I_IMBALANCE);

    #define TEMPOVER    60
	EXCEPT_JUDGE_SET(fabs(drv_ad.stacka_temp) > TEMPOVER, ERROR_DRV_CPLD_A_OT);	//过温
//	EXCEPT_JUDGE_SET(fabs(drv_ad.stackb_temp) > TEMPOVER, ERROR_DRV_CPLD_B_OT);	//过温
//	EXCEPT_JUDGE_SET(fabs(drv_ad.stackc_temp) > TEMPOVER, ERROR_DRV_CPLD_C_OT);	//过温

	// 过压有效值
	#define		IOVER	    1.1
	EXCEPT_JUDGE_SET(Tlimit_fix(&ia_Tlimit, drv_ad.ia_in, IOVER, 200), ERROR_I1A_OVER);
	EXCEPT_JUDGE_SET(Tlimit_fix(&ib_Tlimit, drv_ad.ib_in, IOVER, 200), ERROR_I1B_OVER);
	EXCEPT_JUDGE_SET(Tlimit_fix(&ic_Tlimit, drv_ad.ic_in, IOVER, 200), ERROR_I1C_OVER);

	#define		IOUTOVER	1.1
	EXCEPT_JUDGE_SET(Tlimit_fix(&ia_out_Tlimit, drv_ad.ia_out, IOUTOVER, 200), ERROR_I1A_OUT_OVER);
	EXCEPT_JUDGE_SET(Tlimit_fix(&ib_out_Tlimit, drv_ad.ib_out, IOUTOVER, 200), ERROR_I1B_OUT_OVER);
	EXCEPT_JUDGE_SET(Tlimit_fix(&ic_out_Tlimit, drv_ad.ic_out, IOUTOVER, 200), ERROR_I1C_OUT_OVER);

	EXCEPT_JUDGE_SET(fsm_chg_bwork &&
	                 protect_uouta > bmsA_1.N_VoltMaxCharge+2.1  &&
	                 bmsA_1.N_VoltMaxCharge > 10,     ERROR_CHG_OVER ); // > 10 是防止发过来的是0 充满了电池包发的是0
//	EXCEPT_JUDGE_SET(bmsA_3.N_systemVoltage < bmsA_2.N_VoltMinDischarge-2,  ERROR_DCHG_OVER);
	EXCEPT_JUDGE_SET(fsm_dchg_bwork &&
	                 drv_ad.iabc            > bmsA_2.N_currentMaxDischarge, ERROR_DISI_OVER);

	// 电池电压太低, 一定要关上电源继电器 // 放电到2.7*16节 = 43.2V 我们的高1.1V
	bfalg_dis_over = protect_uouta < (44.1/IDCPU);                //过放条件
	if (clk_Bjudge1(&clk_disov, bfalg_dis_over, 10*60*1000L)){
		protect_k = 0;       // 到关电源继电器电压点了,要先关放电, 再延时, 再关电源继电器 为0是关机
	}
    if (bfalg_dis_over){
        protect_close_dis = 0;                                    // 置关机命令 为0是关机
    }
    EXCEPT_JUDGE_SET(bfalg_dis_over, E2W(ERROR_DCHG_OVER));       // 置过放告警
    if (protect_uouta > (49.1/IDCPU)){
        protect_close_dis = 1;                                    // 电池充起来后, 置起, 这样可以再转放电模式
    }

	
	if (clk_Bjudgen(&clk0, 1, 100L)){					// 电池防反接

		EXCEPT_JUDGE_SET(drv_ad.ua_out < -10, ERROR_BATAA_REVERSE);
		EXCEPT_JUDGE_SET(drv_ad.ub_out < -10, ERROR_BATAB_REVERSE);
		EXCEPT_JUDGE_SET(drv_ad.uc_out < -10, ERROR_BATAC_REVERSE);
	}

	EXCEPT_JUDGE_SET(bmsA_error.BmsState2.bits.other				, ERROR_BAT_OTHER              );
	EXCEPT_JUDGE_SET(bmsA_error.BmsState2.bits.Curr_High_DisCharge  , ERROR_BAT_CURR_HIGH_DISCHARGE);
	EXCEPT_JUDGE_SET(bmsA_error.BmsState2.bits.Curr_High_Charge	    , ERROR_BAT_CURR_HIGH_CHARGE   );
	EXCEPT_JUDGE_SET(bmsA_error.BmsState2.bits.Temp_Low			    , ERROR_BAT_TEMP_LOW           );
	EXCEPT_JUDGE_SET(bmsA_error.BmsState2.bits.Temp_Over			, ERROR_BAT_TEMP_OVER          );
	EXCEPT_JUDGE_SET(bmsA_error.BmsState2.bits.Cell_Vlot_Unblance	, ERROR_BAT_CELL_VLOT_UNBLANCE );
	EXCEPT_JUDGE_SET(bmsA_error.BmsState2.bits.Cell_Vlot_low		, ERROR_BAT_CELL_VLOT_LOW      );
	EXCEPT_JUDGE_SET(bmsA_error.BmsState2.bits.Cell_Vlot_High		, ERROR_BAT_CELL_VLOT_HIGH     );
	
}


// 描述：在放到中断中的快速保护

void protect_fs(void)
{
    float val_ov = 0;
    
	// 直流电容瞬时过压保护——直流电容
	EXCEPT_JUDGE_SET(drv_ad.udc > 2.0, ERROR_UDC_OVER);		//

	EXCEPT_JUDGE_SET(fabs(drv_ad.ua_in) > (cmm.in13*0.01), ERROR_UA_IN_OVER);	//硬件保护 变压器后面是80V 80/380 = 0.21
	EXCEPT_JUDGE_SET(fabs(drv_ad.ub_in) > (cmm.in13*0.01), ERROR_UB_IN_OVER);
	EXCEPT_JUDGE_SET(fabs(drv_ad.uc_in) > (cmm.in13*0.01), ERROR_UC_IN_OVER);

	EXCEPT_JUDGE_SET(fabs(drv_ad.ia_in) > (cmm.in11*0.01), ERROR_IA_IN_OVER);	//硬件保护
	EXCEPT_JUDGE_SET(fabs(drv_ad.ib_in) > (cmm.in11*0.01), ERROR_IB_IN_OVER);
	EXCEPT_JUDGE_SET(fabs(drv_ad.ic_in) > (cmm.in11*0.01), ERROR_IC_IN_OVER);

    val_ov = (bmsA_3.N_systemVoltage + 10.0)*(1.0/UDCPU);
	EXCEPT_JUDGE_SET(fabs(drv_ad.ua_out) > val_ov && bmsA_3.N_systemVoltage>0, ERROR_UA_OUT_OVER);	//硬件保护
	EXCEPT_JUDGE_SET(fabs(drv_ad.ub_out) > val_ov && bmsA_3.N_systemVoltage>0, ERROR_UB_OUT_OVER);
	EXCEPT_JUDGE_SET(fabs(drv_ad.uc_out) > val_ov && bmsA_3.N_systemVoltage>0, ERROR_UC_OUT_OVER);

	EXCEPT_JUDGE_SET(fabs(drv_ad.ia_out) > (cmm.in12*0.01), ERROR_IA_OUT_OVER);	//硬件保护
	EXCEPT_JUDGE_SET(fabs(drv_ad.ib_out) > (cmm.in12*0.01), ERROR_IB_OUT_OVER);
	EXCEPT_JUDGE_SET(fabs(drv_ad.ic_out) > (cmm.in12*0.01), ERROR_IC_OUT_OVER);

	FILTER1_FUNC_RAPID(filter1_uouta,  drv_ad.ua_out, protect_uouta);

}



//static float af[12], bf[12], cf[12];
//static unsigned iu = 0;
// ==============================================================================================
// 内部函数。判断相序
// bok：	0开始。1检测完毕。
void judge_ACB(void)
{
	static unsigned bok=0;
	static unsigned a=0, b=0, c=0, order=0;
	static float    old=0;
	static unsigned n=0;

	if(bok)	return;
	n++;
	if (n < 100)	return;		// AD采样第一个点有问题，放过去，因为在主循环中，所以多放几个点

	//检测缺相
	if (drv_ad.ua_in > 0.2)	a=1;
	if (drv_ad.ub_in > 0.2)	b=1;
	if (drv_ad.uc_in > 0.2)	c=1;
	EXCEPT_JUDGE_SET(n>10000, ERROR_UACB);
	
	//检测相序
	if( old < 0 && drv_ad.ua_in > 0){
		//只进去一次，判断相序
		order= 1;
		EXCEPT_JUDGE_SET((!( drv_ad.ub_in < 0 && drv_ad.uc_in > 0)), ERROR_AD);
	}
	old = drv_ad.ua_in;
	
	//汇总检测
	if(a && b && c && order) 	bok= 1;
}

