#include "apply.h"
#include "rms.h"

//输出变量
float rms_ua_in, 	rms_ub_in, 		rms_uc_in;
float rms_ia_in, 	rms_ib_in, 		rms_ic_in;
float rms_ua_out, 	rms_ub_out, 	rms_uc_out;
float rms_ia_out, 	rms_ib_out, 	rms_ic_out;
float rms_ie;
float rms_uac, rms_uacm;    // 有效值
float rms_uac1,rms_uacm1;   // 滤波后的有效值
float rms_uac_max, rms_uac_min;
float sum_uabc;

//内部变量
static FILTER1	filter1_rms_ua_in, 	filter1_rms_ub_in, 	filter1_rms_uc_in;
static FILTER1	filter1_rms_ia_in, 	filter1_rms_ib_in, 	filter1_rms_ic_in;
static FILTER1	filter1_rms_ua_out, filter1_rms_ub_out, filter1_rms_uc_out;
static FILTER1	filter1_rms_ia_out, filter1_rms_ib_out, filter1_rms_ic_out;
static float	sum2_ua_in, 		sum2_ub_in,	 		sum2_uc_in;
static float	sum2_ia_in, 		sum2_ib_in, 		sum2_ic_in;
static float	sum2_ua_out, 		sum2_ub_out,	 	sum2_uc_out;
static float	sum2_ia_out, 		sum2_ib_out, 		sum2_ic_out;

#define	RMS_FC	2.0

// 初始化
void rms_init1(void)
{
	rms_ua_in	= rms_ub_in	 = rms_uc_in  = 0;
	rms_ia_in 	= rms_ib_in  = rms_ic_in  = 0;
	rms_ua_out	= rms_ub_out = rms_uc_out = 0;
	rms_ia_out 	= rms_ib_out = rms_ic_out = 0;

	filter1_init_rapid(&filter1_rms_ua_in,      0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_ub_in,  	0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_uc_in,  	0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_ia_in,  	0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_ib_in,  	0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_ic_in,  	0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_ua_out,     0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_ub_out,  	0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_uc_out,  	0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_ia_out,  	0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_ib_out,  	0.0, drv_pwm.ts, RMS_FC);
	filter1_init_rapid(&filter1_rms_ic_out,  	0.0, drv_pwm.ts, RMS_FC);
}


// 输入。放在中断里
void rms_input_fs(void)
{
	
	FILTER1_FUNC_RAPID(filter1_rms_ua_in,  	SQ2(drv_ad.ua_in), sum2_ua_in);
	FILTER1_FUNC_RAPID(filter1_rms_ub_in,  	SQ2(drv_ad.ub_in), sum2_ub_in);
	FILTER1_FUNC_RAPID(filter1_rms_uc_in,  	SQ2(drv_ad.uc_in), sum2_uc_in);
	FILTER1_FUNC_RAPID(filter1_rms_ia_in,   SQ2(drv_ad.ia_in), sum2_ia_in);
	FILTER1_FUNC_RAPID(filter1_rms_ib_in,   SQ2(drv_ad.ib_in), sum2_ib_in);
	FILTER1_FUNC_RAPID(filter1_rms_ic_in,   SQ2(drv_ad.ic_in), sum2_ic_in);
	FILTER1_FUNC_RAPID(filter1_rms_ua_out,  SQ2(drv_ad.ua_out), sum2_ua_out);
	FILTER1_FUNC_RAPID(filter1_rms_ub_out,  SQ2(drv_ad.ub_out), sum2_ub_out);
	FILTER1_FUNC_RAPID(filter1_rms_uc_out,  SQ2(drv_ad.uc_out), sum2_uc_out);
	FILTER1_FUNC_RAPID(filter1_rms_ia_out,  SQ2(drv_ad.ia_out), sum2_ia_out);
	FILTER1_FUNC_RAPID(filter1_rms_ib_out,  SQ2(drv_ad.ib_out), sum2_ib_out);
	FILTER1_FUNC_RAPID(filter1_rms_ic_out,  SQ2(drv_ad.ic_out), sum2_ic_out);
		
}


// 有效值计算。放在主程序里
void rms_calc(void)
{
	static unsigned channel = 0;

	// 采样是按峰值归一化，有效值要乘1.414
	switch (channel){
		case 0:	rms_ua_in	 	= C_SQRT2*sqrt(sum2_ua_in);		break;
		case 1:	rms_ub_in	 	= C_SQRT2*sqrt(sum2_ub_in);		break;
		case 2:	rms_uc_in	 	= C_SQRT2*sqrt(sum2_uc_in);		break;
		case 3:	rms_ia_in 	    = C_SQRT2*sqrt(sum2_ia_in);		break;
		case 4:	rms_ib_in 	    = C_SQRT2*sqrt(sum2_ib_in);		break;
		case 5:	rms_ic_in 	    = C_SQRT2*sqrt(sum2_ic_in);		break;
		case 6:	rms_ua_out	 	= C_SQRT2*sqrt(sum2_ua_out);	break;
		case 7:	rms_ub_out	 	= C_SQRT2*sqrt(sum2_ub_out);	break;
		case 8:	rms_uc_out	 	= C_SQRT2*sqrt(sum2_uc_out);	break;
		case 9:	rms_ia_out 	    = C_SQRT2*sqrt(sum2_ia_out);	break;
		case 10:rms_ib_out 	    = C_SQRT2*sqrt(sum2_ib_out);	break;
		case 11:rms_ic_out 	    = C_SQRT2*sqrt(sum2_ic_out);	break;
		default:											    break;
	}

	if (channel >= 11)	channel = 0;	else	channel++;
}

