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
unsigned charge_dir = 1;            // 0: �ŵ�      1: ���
unsigned charge_laidian = 0;        // 0: û����    1: ����
float charge_udc_ref = 0;           // boostʱUDC�趨ֵ
float charge_udc_sef = 0.4;
float charge_iabc = 0;


#define KP_U	(cmm.in8*0.01)//0.5
#define KI_U	(cmm.in9*0.01)//34.0
#define KP_I	(cmm.in4*0.01)//0.5 //0.1//0.6			//1.092834
#define KI_I	(cmm.in5*0.01)//3.4

void charge_init1(void)
{
	#define TIMEUPDN  20.0		//20��

	pid_init_rapid(&charge_pid_UA, KP_U, KI_U, 0.0,  1.0, -1.0, drv_pwm.ts);
	pid_init_rapid(&charge_pid_IA, KP_I, KI_I, 0.0,  1.0, -1.0, drv_pwm.ts);
    speed_set_rapid(&charge_speed_UA,	5.0,	5.0,	 	 1.5, 0.0, drv_pwm.ts);
	speed_set_rapid(&charge_speed_IA,	TIMEUPDN,	TIMEUPDN,	 1.0, -1.0, drv_pwm.ts);
	filter1_init_rapid(&filter1_iabc, 0.0, drv_pwm.ts, 500.0);					// ��ֹƵ��500.0Hz
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

	FILTER1_FUNC_RAPID(filter1_iabc,  drv_ad.iabc, charge_iabc);					// ֱ����ѹ�˲�

	fsm_bwork = fsm_chg_bwork || fsm_dchg_bwork;
    if( !fsm_bwork ){

    	drv_pwm_stopA();
		charge_initA();
    	charge_bworkA = 0;

		charge_startupA = 0;		//��ʼ��
		(void)clk_Bjudge1(&clk_startup, 0, 2000L);
	}else{

		if (!clk_Bjudge1(&clk_startup, 1, 2000L))	charge_startupA = 0;	else	charge_startupA = 1;
		
	   	if((!charge_bworkA) && charge_startupA)	{charge_bworkA = 1;		drv_pwm_startA(charge_dir);}	//��ʱ2��ȼ̵�����
		if(!charge_bworkA)	return;

		pid_kpki(&charge_pid_IA, KP_I, KI_I);    // ʵʱ�ı�PI����
        pid_kpki(&charge_pid_UA, KP_U, KI_U);    // ʵʱ�ı�PI����

		//�����ջ�
		IA_ref = cmm.IA_ref*(1.0/IDCPU);	SAT_VOID(IA_ref, 0.984, 0);//����0.588  ���0.984
		SPEED_UPDN_RAPID(charge_speed_IA, IA_ref, chargeA_ref);	
		if (charge_dir){
			PID_FUNC_RAPID(charge_pid_IA, chargeA_ref, charge_iabc,   Aduty);	//1: ��� buck
		}else{
	    	charge_udc_sef = 1.3;   // boostʱUDC�趨ֵ
		    SPEED_UPDN_RAPID(charge_speed_UA, charge_udc_sef, charge_udc_ref);	
			PID_FUNC_RAPID(charge_pid_UA, charge_udc_ref, drv_ad.udc, Aduty);	//0: �ŵ� boost  ֱ����ѹĿ��ֵ130V
			if (drv_ad.udc > 1.5) Aduty = -1;   // ж��ʱ, ��ѹ���, BOOSTֱ�ӹ���
		}

		drv_pwm_buck_duty(charge_dir, Aduty);
	}
}




// �����йص�Դ:�ȹ�PWM���ؿ��أ��Ͽ��Ƶ�
void chage_power_key(void)
{
}

// ���ϴ����ȹ�PWM���ؿ���
void chage_error(void)
{
	drv_pwm_stop();
	precharge_release();
}

unsigned charge_close_dchg_ok = 0;
unsigned charge_close_dchg_s  = 0;
// �طŵ� �ȶ����п���, �ٹ�ǰ��PWM
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

// ��״̬����
void charge_close_dchg_clear_s(void)
{
	charge_close_dchg_ok = 0;
	charge_close_dchg_s  = 0;
}


unsigned charge_close_chg_ok = 0;
unsigned charge_close_chg_s  = 0;
// ������ڳ��, �Ƚ�����, �ٹ�PWM (�п��ܳ������ڴ���״̬, ֱ�ӷ��سɹ�)
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

// ��״̬����
void charge_close_chg_clear_s(void)
{
	charge_close_chg_ok = 0;
	charge_close_chg_s  = 0;
}

//�����ʱ���߿����ʱ,�����������½����ж������õ�ط������ܵ�ѹ(��ǰ�õ��ǵ����ѹ)
void charge_i_speed_down(float MaxBatCell, float MinBatCell, unsigned I_ref_T, 
                         float *xbox,float *xold1, float *xold2, float *I_ref, unsigned *bflag)
{
	float k, b;

#define UCSTART		(56.96-1.12)  //(4.15-0.07	)		//��罵��������
#define UCEND		(56.96+0.08)  //(4.15+0.005	)
#define UDSTART		(16.2 +1.12)  //(3.35+0.07	)		//�ŵ罵��������
#define UDEND		(16.2 -0.08)  //(3.35-0.005)
#define	I10A		(3.0	   )  //��ͳ�����

	if (charge_dir){
		k = (I_ref_T-I10A)*(1.0/(UCSTART-UCEND));b = I10A - k*UCEND;				//��� buck
		if (MaxBatCell > *xold1)	{*xbox = MaxBatCell; *xold1 = *xbox;}			//����ǵ�ѹ����, ֻ���ո����ֵ

		if ((MaxBatCell < UCSTART)&&(!(*bflag)))	{*I_ref = I_ref_T; }
		else if (MaxBatCell < UCEND)				{*bflag = 1; *I_ref = k*(*xbox) + b;	SAT_VOID(*I_ref, I_ref_T, 1);}	//�����ʱ,�����������½�
		else										{*I_ref = 0; *bflag = 0; }		//�����ʱ,����ѹ��ֹ��ػ�
	}else{
//		k = (I_ref_T-I10A)*(1.0/(UDSTART-UDEND));b = I10A - k*UDEND;				//�ŵ� boost
//		if (MinBatCell < *xold2)	{*xbox = MinBatCell; *xold2 = *xbox;}			//�ŵ��ǵ�ѹ��, ֻ���ո�С��ֵ

//		if ((MinBatCell > UDSTART)&&(!(*bflag)))	{*I_ref = I_ref_T; }
//		else if (MinBatCell > UDEND)   				{*bflag = 1; *I_ref = k*(*xbox) + b;	SAT_VOID(*I_ref, I_ref_T, 1);}	//��ſ�ʱ,�����������½�
//		else										{*I_ref = 0; *bflag = 0; }		//��ſ�ʱ,����ѹ��ֹ��ػ�
	}
}


void charge_chg_setI(void)
{
	if (cmm.ChargeFastSlow){	// ���
	    cmm.IA_ref_T = 80;
		cmm.IA_ref = (bmsA_1.N_currentMaxCharge*0.01)*cmm.IA_ref_T;     //��ʽ����:���ʱ cmm.IA_ref_T = 80;
	}else{						// ���
	    cmm.IA_ref_T = 98;
		cmm.IA_ref = (bmsA_1.N_currentMaxCharge*0.01*0.5)*cmm.IA_ref_T; //��ʽ����:����ʱ cmm.IA_ref_T = 98;
	}

	if (bmsA_3.WorkMode == 2){    // ����ʱ��ʹ���õ�����ҲҪȫ��
		cmm.IA_ref = bmsA_1.N_currentMaxCharge*((float)cmm.IA_ref_T*0.01);
	}
}


void charge_chg_downI(void)
{
	static float cmm_xboxA 		= 0;
	static float cmm_xold1A 	= 0;
	static float cmm_xold2A 	= 56.96;
	static unsigned cmm_bflagA 	= 0;

	// ��������߿����ʱ�����𽥼�С
	if(!fsm_chg_bwork)    cmm_bflagA = 0;
	charge_i_speed_down(bmsA_3.N_systemVoltage, 0, cmm.IA_ref_T,  
	                 &cmm_xboxA, &cmm_xold1A, &cmm_xold2A, &cmm.IA_ref, &cmm_bflagA);
}

// �жϳ�����
unsigned charge_chg_ok(void)
{
	//����ֹ
	if (  bmsA_3.N_systemVoltage		> bmsA_1.N_VoltMaxCharge	//��س�����
		/*||drv_ad.ub 					> cmm.ChongVolt_end_A	*/	){	//�Լ������ĵ�ѹ
		
		return 1;
	}
	
	return 0;
}


// �жϷŵ��ֹ �ŵ粻�ܽ����� ��ɺ������ػ�
unsigned charge_dchg_ok(void)
{
	//�ŵ��ֹ
	if (  bmsA_3.N_systemVoltage		< bmsA_2.N_VoltMinDischarge	//��ط�����
		/*||drv_ad.ub 			< cmm.ChongVolt_end_A	*/	){	//�Լ������ĵ�ѹ
		
		return 1;
	}
	
	return 0;
}

// ��Դ�̵���
unsigned charge_power_relay(unsigned bflag)
{
	static CLK clk1   = CLK_DEFAULTS;
    static unsigned s = 0;

	// 1,�ϵ�ʱ��ֱ�Ӻ�
	if (bflag)   drv_cpld_do_POWER(1); 

	// 2,�ص�ʱ��Ҫȷ�Ϲػ�,��û�е��� ��û���ʱ1S. ���߼��Ƿ�ֹ��ذ��̵���������
	if (!bflag){

    	switch(s){
    	case 0:	    // �ػ� �����൱�ڷ��ػ����� 
    		s++;
    		break;
    	case 1:	    // �жϵ����Ƿ�û����
    	    clk_Bjudge1(&clk1, 0, 1000L);
    		if (fabs(drv_ad.iabc < 0.2))   s++;
    		break;
    	case 2:	    // ��ʱ�ؼ̵���
    	    if (clk_Bjudge1(&clk1, 1, 1000L)){
        	    drv_cpld_do_POWER(0);
        		s=0;    // ���һ��״̬,(��ʵ����Ҫ, ��Ϊ�Ѿ��µ���)
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
