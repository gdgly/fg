#include "apply.h"
#include "charge.h"
#include "test.h"
#include "pu.h"

FILTER1	filter1_iabc = FILTER1_DEFAULTS;

unsigned charge_bworkA = 0;
unsigned charge_startupA = 0;		//
float chargeA_ref = 0.0, UA_ref= 0.0,	IA_ref= 0.00, Aduty = -1.0;
PID charge_pid_UA = PID_DEFAULTS;
PID charge_pid_IA = PID_DEFAULTS;
SPEED charge_speed_UA = SPEED_DEFAULTS;
SPEED charge_speed_IA = SPEED_DEFAULTS;
unsigned charge_bflag = 0;
unsigned charge_dir = 1;            // 0: 放电      1: 充电
unsigned charge_laidian = 0;        // 0: 没来电    1: 来电
float charge_udc_ref = 0;           // boost时UDC设定值
float charge_udc_sef = 0.4;
float charge_iabc = 0;


#define KP_U	(cmm.in8*0.01)//0.5
#define KI_U	(cmm.in9*0.01)//34.0
#define KP_I	(cmm.in4*0.01)//0.5 //0.1//0.6			//1.092834
#define KI_I	(cmm.in5*0.01)//3.4

void charge_init1(void)
{
	#define TIMEUPDN  20.0		//20秒

	pid_init_rapid(&charge_pid_UA, KP_U, KI_U, 0.0,  1.0, -1.0, drv_pwm.ts);
	pid_init_rapid(&charge_pid_IA, KP_I, KI_I, 0.0,  1.0, -1.0, drv_pwm.ts);
    speed_set_rapid(&charge_speed_UA,	5.0,	5.0,	 	 1.5, 0.0, drv_pwm.ts);
	speed_set_rapid(&charge_speed_IA,	TIMEUPDN,	TIMEUPDN,	 1.0, -1.0, drv_pwm.ts);
	filter1_init_rapid(&filter1_iabc, 0.0, drv_pwm.ts, 500.0);					// 截止频率500.0Hz
	cmm.IA_ref = 0;
	charge_initA();
}


void charge_initA(void)
{
	pid_reset(&charge_pid_UA, -1.0);			speed_reset(&charge_speed_UA,	drv_ad.udc);
	pid_reset(&charge_pid_IA, -1.0);			speed_reset(&charge_speed_IA,	0.0);
	charge_bworkA = 0;
}

void charge_speedA(void)
{
//	static unsigned old = 0;
//
//	if (old == cmm.TimeDownI){
//		speed_set_rapid(&charge_speed_IA,	TIMEUPDN,	cmm.TimeDownI,	 1.0, -1.0, drv_pwm.ts);
//	}
//
//	old = cmm.TimeDownI;
}

void charge_loopA(void)
{
	static CLK clk_startup = CLK_DEFAULTS;
	unsigned fsm_bwork = 0;

	FILTER1_FUNC_RAPID(filter1_iabc,  drv_ad.iabc, charge_iabc);					// 直流电压滤波

	fsm_bwork = fsm_chg_bwork || fsm_dchg_bwork;
    if( !fsm_bwork ){

    	drv_pwm_stopA();
		charge_initA();
    	charge_bworkA = 0;

		charge_startupA = 0;		//初始化
		(void)clk_Bjudge1(&clk_startup, 0, 2000L);
	}else{

		if (!clk_Bjudge1(&clk_startup, 1, 2000L))	charge_startupA = 0;	else	charge_startupA = 1;
		
	   	if((!charge_bworkA) && charge_startupA)	{charge_bworkA = 1;		drv_pwm_startA(charge_dir);}	//延时2秒等继电器合
		if(!charge_bworkA)	return;

		pid_kpki(&charge_pid_IA, KP_I, KI_I);    // 实时改变PI参数
        pid_kpki(&charge_pid_UA, KP_U, KI_U);    // 实时改变PI参数

		//电流闭环
		IA_ref = cmm.IA_ref*(1.0/IDCPU);	SAT_VOID(IA_ref, 0.984, 0);//慢充0.588  快充0.984
		SPEED_UPDN_RAPID(charge_speed_IA, IA_ref, chargeA_ref);	
		if (charge_dir){
			PID_FUNC_RAPID(charge_pid_IA, chargeA_ref, charge_iabc,   Aduty);	//1: 充电 buck
		}else{
	    	charge_udc_sef = 1.3;   // boost时UDC设定值
		    SPEED_UPDN_RAPID(charge_speed_UA, charge_udc_sef, charge_udc_ref);	
			PID_FUNC_RAPID(charge_pid_UA, charge_udc_ref, drv_ad.udc, Aduty);	//0: 放电 boost  直流电压目标值130V
			if (drv_ad.udc > 1.5) Aduty = -1;   // 卸载时, 电压冲高, BOOST直接关了
		}

		drv_pwm_buck_duty(charge_dir, Aduty);
	}
}




// 运行中关电源:先关PWM，关开关，断控制电
void chage_power_key(void)
{
}

// 故障处理：先关PWM，关开关
void chage_error(void)
{
	drv_pwm_stop();
	precharge_release();
}

unsigned charge_close_dchg_ok = 0;
unsigned charge_close_dchg_s  = 0;
// 关放电 先断所有开关, 再关前后级PWM
unsigned charge_close_dchg(void)
{
	if (charge_close_dchg_ok)	return 1;

	switch(charge_close_dchg_s){
	case 0:	
		precharge_release();
		charge_close_dchg_s++;
		break;
	case 1:	
		drv_pwm_stop();
		charge_close_dchg_s++;
		charge_close_dchg_ok = 1;
		break;
	default:	
		break;
	}

	if (charge_close_dchg_ok)   return 1;   else    return 0;
}

// 清状态机量
void charge_close_dchg_clear_s(void)
{
	charge_close_dchg_ok = 0;
	charge_close_dchg_s  = 0;
}


unsigned charge_close_chg_ok = 0;
unsigned charge_close_chg_s  = 0;
// 如果正在充电, 先降电流, 再关PWM (有可能充满处于待机状态, 直接返回成功)
unsigned charge_close_chg(unsigned time)
{
	if (charge_close_chg_ok)	            return 1;
	if (!fsm_chg_bwork || !charge_bworkA)   return 1;

	switch(charge_close_chg_s){
	case 0:	
		precharge_release();
		charge_close_chg_s++;
		break;
	case 1:	
    	if (drv_ad.iabc < 0.1){
    		drv_pwm_stopA();
    		return 1;
    	}
		charge_close_chg_s++;
		charge_close_chg_ok = 1;
		break;
	default:	
		break;
	}

    if (charge_close_chg_ok)    return 1;   else    return 0;
}

// 清状态机量
void charge_close_chg_clear_s(void)
{
	charge_close_chg_ok = 0;
	charge_close_chg_s  = 0;
}

//快充满时或者快放完时,电流给定逐渐下降。判断条件用电池发来的总电压(以前用的是单体电压)
void charge_i_speed_down(float MaxBatCell, float MinBatCell, unsigned I_ref_T, 
                         float *xbox,float *xold1, float *xold2, float *I_ref, unsigned *bflag)
{
	float k, b;

#define UCSTART		(56.96-1.12)  //(4.15-0.07	)		//充电降电流区间
#define UCEND		(56.96+0.08)  //(4.15+0.005	)
#define UDSTART		(16.2 +1.12)  //(3.35+0.07	)		//放电降电流区间
#define UDEND		(16.2 -0.08)  //(3.35-0.005)
#define	I10A		(3.0	   )  //最低充电电流

	if (charge_dir){
		k = (I_ref_T-I10A)*(1.0/(UCSTART-UCEND));b = I10A - k*UCEND;				//充电 buck
		if (MaxBatCell > *xold1)	{*xbox = MaxBatCell; *xold1 = *xbox;}			//充电是电压上升, 只接收更大的值

		if ((MaxBatCell < UCSTART)&&(!(*bflag)))	{*I_ref = I_ref_T; }
		else if (MaxBatCell < UCEND)				{*bflag = 1; *I_ref = k*(*xbox) + b;	SAT_VOID(*I_ref, I_ref_T, 1);}	//快充满时,电流给定逐渐下降
		else										{*I_ref = 0; *bflag = 0; }		//快充满时,到电压截止点关机
	}else{
//		k = (I_ref_T-I10A)*(1.0/(UDSTART-UDEND));b = I10A - k*UDEND;				//放电 boost
//		if (MinBatCell < *xold2)	{*xbox = MinBatCell; *xold2 = *xbox;}			//放电是电压降, 只接收更小的值

//		if ((MinBatCell > UDSTART)&&(!(*bflag)))	{*I_ref = I_ref_T; }
//		else if (MinBatCell > UDEND)   				{*bflag = 1; *I_ref = k*(*xbox) + b;	SAT_VOID(*I_ref, I_ref_T, 1);}	//快放亏时,电流给定逐渐下降
//		else										{*I_ref = 0; *bflag = 0; }		//快放亏时,到电压截止点关机
	}
}


void charge_chg_setI(void)
{
	if (cmm.ChargeFastSlow){	// 快充
	    cmm.IA_ref_T = 80;
		cmm.IA_ref = (bmsA_1.N_currentMaxCharge*0.01)*cmm.IA_ref_T;     //正式代码:快充时 cmm.IA_ref_T = 80;
	}else{						// 快充
	    cmm.IA_ref_T = 98;
		cmm.IA_ref = (bmsA_1.N_currentMaxCharge*0.01*0.5)*cmm.IA_ref_T; //正式代码:慢充时 cmm.IA_ref_T = 98;
	}

	if (bmsA_3.WorkMode == 2){    // 加热时即使设置的慢充也要全给
		cmm.IA_ref = bmsA_1.N_currentMaxCharge*((float)cmm.IA_ref_T*0.01);
	}
}


void charge_chg_downI(void)
{
	static float cmm_xboxA 		= 0;
	static float cmm_xold1A 	= 0;
	static float cmm_xold2A 	= 56.96;
	static unsigned cmm_bflagA 	= 0;

	// 快充满或者快放完时电流逐渐减小
	if(!fsm_chg_bwork)    cmm_bflagA = 0;
	charge_i_speed_down(bmsA_3.N_systemVoltage, 0, cmm.IA_ref_T,  
	                 &cmm_xboxA, &cmm_xold1A, &cmm_xold2A, &cmm.IA_ref, &cmm_bflagA);
}

// 判断充电完成
unsigned charge_chg_ok(void)
{
	//充电截止
	if (  bmsA_3.N_systemVoltage		> bmsA_1.N_VoltMaxCharge	//电池充满了
		/*||drv_ad.ub 					> cmm.ChongVolt_end_A	*/	){	//自己采样的电压
		
		return 1;
	}
	
	return 0;
}


// 判断放电截止 放电不能降电流 完成后立即关机
unsigned charge_dchg_ok(void)
{
	//放电截止
	if (  bmsA_3.N_systemVoltage		< bmsA_2.N_VoltMinDischarge	//电池放完了
		/*||drv_ad.ub 			< cmm.ChongVolt_end_A	*/	){	//自己采样的电压
		
		return 1;
	}
	
	return 0;
}

// 电源继电器
unsigned charge_power_relay(unsigned bflag)
{
	static CLK clk1   = CLK_DEFAULTS;
    static unsigned s = 0;

	// 1,合的时候直接合
	if (bflag)   drv_cpld_do_POWER(1); 

	// 2,关的时候要确认关机,且没有电流 最好还延时1S. 此逻辑是防止电池包继电器带载切
	if (!bflag){

    	switch(s){
    	case 0:	    // 关机 进入相当于发关机命令 
    		s++;
    		break;
    	case 1:	    // 判断电流是否没有了
    	    clk_Bjudge1(&clk1, 0, 1000L);
    		if (fabs(drv_ad.iabc < 0.2))   s++;
    		break;
    	case 2:	    // 延时关继电器
    	    if (clk_Bjudge1(&clk1, 1, 1000L)){
        	    drv_cpld_do_POWER(0);
        		s=0;    // 最后一个状态,(其实不需要, 因为已经下电了)
    	    }
    		break;
    	default:
    	    s=0;
    		break;
    	}
    	return 0;
	}
	
	return 1;
}
