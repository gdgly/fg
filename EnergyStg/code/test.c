#include "apply.h"
#include "test.h"
#include "pu.h"

#pragma CODE_SECTION(test_fs					, "ramfuncs");

#ifdef DEBUG	//调试成立	//因为test.h中已经有条件编译，不能嵌套
unsigned test_break_point;
unsigned test_ua=0, test_ub=0, test_uc=0;
float    test_fa=0, test_fb=0, test_fc=0, test_fd=0,	test_fe=0,	test_ff=0;
float    test_fg=0, test_fh=0;


// 放在主程序中的子程序，一旦DEBUG关闭，就消失。
void test_main(void)
{
//    float a=0.3,b=0.4,c=0.5;

//    // 测DO
//    if (cmm.in1)  drv_cpld_do_cab_fan(1);            else   drv_cpld_do_cab_fan(0);     
//    if (cmm.in2)  drv_cpld_do_yellow(1);             else   drv_cpld_do_yellow(0);      
//    if (cmm.in3)  drv_cpld_do_blue(1);               else   drv_cpld_do_blue(0);        
//    if (cmm.in4)  drv_cpld_do_red(1);                else   drv_cpld_do_red(0);         
//    if (cmm.in5)  drv_cpld_do_MCB_grid(1);           else   drv_cpld_do_MCB_grid(0);    
//    if (cmm.in6)  drv_cpld_do_prechg_grid(1);          else   drv_cpld_do_prechg_grid(0);   
//    if (cmm.in7)  drv_cpld_do_MCB_bat(1);            else   drv_cpld_do_MCB_bat(0);     
//    if (cmm.in8)  drv_cpld_do_dis(1);                else   drv_cpld_do_dis(0);         
//	  if (cmm.in9)  drv_cpld_do_MCB_gridCAB(1);        else   drv_cpld_do_MCB_gridCAB(0); 
//    if (cmm.in10) drv_cpld_do_POWER(1);              else   drv_cpld_do_POWER(0);       
//    
//    cmm.out1 = DRV_CPLD_DI_POW();
//    cmm.out2 = DRV_CPLD_DI_MPS();
//    cmm.out3 = drv_cpld.error;
//	cmm.out4 = cmm.in11;
//	cmm.out5 = cmm.in12;
	
//    if (cmm.in11){
//        drv_pwm_bat_start();
//        DRV_PWM_BAT(a,b,c);
//    }else{
//        drv_pwm_bat_stop();  
//    }
//
//    if (cmm.in12){
//        drv_pwm_grid_start();
//        DRV_PWM_GRID(a,b,c);
//    }
//    else{   
//        drv_pwm_grid_stop(); 
//    }
}


// 放在中断程序中的子程序，一旦DEBUG关闭，就消失。
void test_fs(void)
{
/*	static unsigned bflag = 0;
	if(fsm_inv_bwork){
		DRV_IO_OUT_TEST(bflag); 	bflag = ~bflag;
	}else{
		DRV_IO_OUT_TEST(0);
	}
*/

}

#define	GET_ZERO(a, a_old, a_zero)					\
{													\
	if (a * a_old < 0) a_zero = 1; else a_zero = 0;	\
	a_old = a;										\
}

//单板调试时，取代drv_ad()，或放在cunit.c中
//生产三相电压和电流波形
void test_ad(void)
{
//	static SIN3 waveu = SIN3_DEFAULTS;
//	static SIN3 wavei = SIN3_DEFAULTS;
//	static SIN3 wavei_ct = SIN3_DEFAULTS;

	static unsigned bonce = 0;
	if (!bonce){
		bonce = 1;
//		waveu.theta = 0.0;
//		wavei.theta = 0.0;
//		wavei_ct.theta = 0.0;
	}
//	test_sin3_s(&waveu, drv_pwm.ts,	1, 1.0, &drv_ad.ua, &drv_ad.ub, &drv_ad.uc);


//	drv_ad.udc = 0.8;
	

}

//单次波形生成三相，s=single
void test_sin3_s(SIN3 *p, float ts, float h, float A, float *pa, float *pb, float *pc)
{
	float theta1, theta2, theta3;

	theta1 = p->theta;
	theta2 = p->theta - PI2_3;
	theta3 = p->theta + PI2_3;

	*pa = A*sin(h * theta1);
	*pb = A*sin(h * theta2);
	*pc = A*sin(h * theta3);

	p->theta += ts*(PI2*50.0);
	if(p->theta > PI2)	p->theta = 0;
						//p->theta -= PI2; 
}

//单次波形生成三相，s=single 产生正负序
void test_sin3_pn(SIN3 *p, float ts, float h, float A, float *pa, float *pb, float *pc)
{
	float theta1, theta2, theta3;

	theta1 = p->theta;
	theta2 = p->theta - PI2_3;
	theta3 = p->theta + PI2_3;

//	*pa = 2.9*A*sin(h * theta1);
//	*pb = A*sin(h * theta2);
//	*pc = 0.0 - *pa - *pb;//A*sin(h * theta3);

	*pa = A*sin(h * theta1) + 0.5*A*sin(h * theta1);
	*pb = A*sin(h * theta2) + 0.5*A*sin(h * theta3);
	*pc = A*sin(h * theta3) + 0.5*A*sin(h * theta2);

	p->theta += ts*(PI2*50.0);
	if(p->theta > PI2)	p->theta = 0;
						//p->theta -= PI2; 
}

//多次谐波波形生成三相，m=multi
void test_sin3_m(SIN3 *p, float ts,  float h1, float A1, float h3, float A3, float h5, float A5, float h7, float A7,   float *pa, float *pb, float *pc)
{
	float theta1, theta2, theta3;

	theta1 = p->theta;
	theta2 = p->theta - PI2_3;
	theta3 = p->theta + PI2_3;

	*pa = A1*sin(h1 * theta1) + A3*sin(h3 * theta1) + A5*sin(h5 * theta1) + A7*sin(h7 * theta1);
	*pb = A1*sin(h1 * theta2) + A3*sin(h3 * theta2) + A5*sin(h5 * theta2) + A7*sin(h7 * theta2);
	*pc = A1*sin(h1 * theta3) + A3*sin(h3 * theta3) + A5*sin(h5 * theta3) + A7*sin(h7 * theta3);
//	*pa = A1*sin(h1 * theta1) + 0.5*A1*sin(h1 * theta1) + A3*sin(h3 * theta1) + A5*sin(h5 * theta1) + A7*sin(h7 * theta1);
//	*pb = A1*sin(h1 * theta2) + 0.5*A1*sin(h1 * theta3) + A3*sin(h3 * theta2) + A5*sin(h5 * theta2) + A7*sin(h7 * theta2);
//	*pc = A1*sin(h1 * theta3) + 0.5*A1*sin(h1 * theta2) + A3*sin(h3 * theta3) + A5*sin(h5 * theta3) + A7*sin(h7 * theta3);
//	*a = A*sin(h * theta1) + A3*sin(h3 * theta1) + 0.829*sin(h5 * theta1) + 0.3967*sin(h7 * theta1) + 0.2885*sin(11 * theta1) + 0.2525*sin(13 * theta1) + 0.0721*sin(17 * theta1) + 0.036*sin(19 * theta1) + 0.0252*sin(23 * theta1) + 0.0216*sin(25 * theta1);
//	*b = A*sin(h * theta2) + A3*sin(h3 * theta2) + 0.829*sin(h5 * theta2) + 0.3967*sin(h7 * theta2) + 0.2885*sin(11 * theta2) + 0.2525*sin(13 * theta2) + 0.0721*sin(17 * theta2) + 0.036*sin(19 * theta2) + 0.0252*sin(23 * theta2) + 0.0216*sin(25 * theta2);
//	*c = A*sin(h * theta3) + A3*sin(h3 * theta3) + 0.829*sin(h5 * theta3) + 0.3967*sin(h7 * theta3) + 0.2885*sin(11 * theta3) + 0.2525*sin(13 * theta3) + 0.0721*sin(17 * theta3) + 0.036*sin(19 * theta3) + 0.0252*sin(23 * theta3) + 0.0216*sin(25 * theta3);

	p->theta += ts*(PI2*50.0);
//	p->theta += (0.049087385212340519350978802863742*0.001);//模拟频率漂移
	if(p->theta > PI2)	p->theta = 0;
						//p->theta -= PI2; 
}


#endif

