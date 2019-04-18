#include "apply.h"
#include "vc_ssc.h"
#include "pu.h"

static float soft =0.0;
VC_SSC vc_ssc = VC_SSC_DEFAULTS;

//PID pid_uamp   = PID_DEFAULTS;
PID pid_MA   = PID_DEFAULTS;
PID pid_MB   = PID_DEFAULTS;
PID pid_MC   = PID_DEFAULTS;

FILTER1	filter1_udc2 = FILTER1_DEFAULTS;
SPEED 	speed_V;
SPEED 	speed_K;

float vc_Uamp_ref;
float vc_Uamp;
float vc_Uamp1;
float vc_Uamp2;
unsigned bfirst_start = 0;
float MA, MB, MC;
float M3, Z3;

#define UAMP_TEST	1.0									 // 测试代码=0.8 正式代码=1.0
#define	 	K0      (1.07*UAMP_TEST*(113.1/UDCPU))		 //  113.1=80*1.414				 (113/100)/udc
float knew=0, k=0;

void vc_ssc_init1(void)
{
	vc_ssc.aduty = 0;
	vc_ssc.bduty = 0;
	vc_ssc.cduty = 0;
	vc_Uamp_ref  = 0;
	vc_Uamp      = 0;
	vc_Uamp1     = 0;
	vc_Uamp2     = 0;

	vc_ssc.samlpe_num = INV_SWITH_FREQ/50.0;		//一个工频周期的采样点数,开关频率除以工频频率

   	#define		KP_uam			8.0     //0.5     //3       //30  //0.2	//0.05
	#define		KI_uam			12.0    //5.0     //12.57   //1	//0.1
	//pid_init_rapid(&pid_uamp,   KP_uam, KI_uam, 0.0,  0.05, -0.05, drv_pwm.ts);
	pid_init_rapid(&pid_MA,   KP_uam, KI_uam, 0.0,  0.05, -0.05, drv_pwm.ts);
	pid_init_rapid(&pid_MB,   KP_uam, KI_uam, 0.0,  0.05, -0.05, drv_pwm.ts);
	pid_init_rapid(&pid_MC,   KP_uam, KI_uam, 0.0,  0.05, -0.05, drv_pwm.ts);
	
	filter1_init_rapid(&filter1_udc2, 0.0, drv_pwm.ts, 500.0);	 // 初值是0，截止频率500.0Hz  硬件滤波是3.12KHz

	#define	TIMEUPDN_V	10.0					//3s全程。
	speed_set_rapid(&speed_V,	TIMEUPDN_V,	TIMEUPDN_V,	1.0,	0.2, drv_pwm.ts);
	
	#define	TIMEUPDN_K	1.4					
	speed_set_rapid(&speed_K,	TIMEUPDN_K,	TIMEUPDN_K,	1.0,	0.3, drv_pwm.ts);

}

void vc_ssc_init(void)
{
	soft =0.0;
	vc_Uamp2     = 0;
	bfirst_start        = 0;

	//pid_reset(&pid_uamp, 	0); 
	pid_reset(&pid_MA, 	0); 
	pid_reset(&pid_MB, 	0); 
	pid_reset(&pid_MC, 	0); 
	
	speed_reset(&speed_V,	0.0);
    speed_reset(&speed_K,	K0);
    k=K0;
}

void vc_ssc_func(void)
{
	static float theta = 0;
	float thetaA, thetaB, thetaC;
	float vc_Uamp2_A = 0.0, vc_Uamp2_B = 0.0, vc_Uamp2_C = 0.0;
	
	static CLK clk_uam = CLK_DEFAULTS;
	static unsigned bonce    = 0;
	float udc;

	if (charge_laidian)	{drv_pwm_grid_stop();return;}//如果来电, 网侧PWM一定要关,不能运行 安全一点
	if(!fsm_dchg_binv){
	    if (!bonce){
	        drv_pwm_grid_stop();
	        vc_ssc.bwork=0;
	        vc_ssc_init();
	        bonce=1;
	        return;
	    }
	}else if(!vc_ssc.bwork){
	    vc_ssc.bwork=1;
	    drv_pwm_grid_start();
	    bonce=0;
	}

	// 第一种方法：PI稳定输出电压
	// 电压闭环以输出380V为目标, PI调节输出目标直流电压值
	#define UAMP_MS		1000L
	if(fsm_dchg_binv){
		if(!clk_Bjudge1(&clk_uam, 1, UAMP_MS )){				// 开机后延时1秒,认为交流输出有了,PI可以界入了
			vc_Uamp2_A = 0.0; vc_Uamp2_B = 0.0; vc_Uamp2_C = 0.0;	// 刚开机, 考虑交流还没有发出,发出理论值
		}else{
			//PID_FUNC_RAPID(pid_uamp, (1.0*UAMP_TEST), rms_ua_out, vc_Uamp2);	// 
			PID_FUNC_RAPID(pid_MA, (1.0*UAMP_TEST), rms_ua_out, vc_Uamp2_A);
			PID_FUNC_RAPID(pid_MB, (1.0*UAMP_TEST), rms_ub_out, vc_Uamp2_B);
			PID_FUNC_RAPID(pid_MC, (1.0*UAMP_TEST), rms_uc_out, vc_Uamp2_C);
		}
	}else{
		(void)clk_Bjudge1(&clk_uam, 0, UAMP_MS);
	}
	//1、对直流电压滤波
	FILTER1_FUNC_RAPID(filter1_udc2,  drv_ad.udc, udc);

	//2、直流电压补偿
	if(K0 >= udc) 	knew = udc;			                     // m= K0/udc，此时直流电压很低，相当于要限制m=1;
	else			knew = K0;
			
	if(knew < k) 	{speed_reset(&speed_K,	knew);			k=knew;}
	else			{SPEED_UPDN_RAPID(speed_K, knew, k);			}

#define AMPK  1.15
	//vc_Uamp1= k/udc;
	vc_Uamp1= cmm.in10*0.01*(AMPK*K0)/udc;
	
	//vc_Uamp = vc_Uamp1 + vc_Uamp2;
	//if (vc_Uamp > 1.0)	vc_Uamp = 1.0;
	MA = vc_Uamp1 + vc_Uamp2_A;         vc_Uamp2 = vc_Uamp2_A;  //仅观测
	MB = vc_Uamp1 + vc_Uamp2_B;
	MC = vc_Uamp1 + vc_Uamp2_C;
	if (MA > AMPK)	MA = AMPK;
	if (MB > AMPK)	MB = AMPK;
	if (MC > AMPK)	MC = AMPK;

	// 加个电压缓启动, 应对启动时电流冲击
	if(fsm_dchg_binv){
    	if(!bfirst_start){
    		SPEED_UPDN_RAPID(speed_V, 1.0, soft); // 缓冲给定
    		MA = soft * MA;
    		MB = soft * MB;
    		MC = soft * MC;
            if(fabs(soft - 1.0) < 0.0001)       bfirst_start=1;
    	}else{
    	    vc_Uamp_ref = vc_Uamp;
    	}
    }


	//1、SPWM
	theta += (2.0*PI*50.0) * drv_pwm.ts;
	if(theta > PI2)		theta = -PI2;
	
	thetaA = theta;
	thetaB = theta - PI2_3;
	thetaC = theta + PI2_3;
	
	M3 = 1.0;    //MIN3(MA, MB, MC);
	Z3 = M3 * 0.15 * sin(3 * thetaA)* (AMPK > 1.01); // AMPK为1.0时 不加三次谐波
				
	vc_ssc.aduty =  MA * sin(thetaA) + Z3; 
	vc_ssc.bduty =  MB * sin(thetaB) + Z3; 
	vc_ssc.cduty =  MC * sin(thetaC) + Z3;
	//2、SVPWM				 频率直接设为1.0=50HZ, 0.0无直流偏移量
	//svpwm_mf(float T_sample, float f, float gain, float offset, float *p_ta, float *p_tb, float *p_tc)
	//svpwm_mf(drv_pwm.ts, 1.0, vc_Uamp_ref, 0.0, &vc_ssc.aduty, &vc_ssc.bduty, &vc_ssc.cduty);	
	DRV_PWM_GRID(vc_ssc.aduty, vc_ssc.bduty, vc_ssc.cduty);

}
