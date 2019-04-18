#include "apply.h"
#include "cunit.h"
#include "test.h"

#ifdef SIMULATOR	//有单元测试时




void cunit(void)
{
	static unsigned bfirst = 0;

	//需要初始化都放在这里。在开机初始化时，会被调用一次。
	if (!bfirst){
		bfirst = 1;
		drv_pwm.ts = 1.0/10000;	//纯软件仿真初始化时，不会初始化硬件层，所以必须放这里
		fsm_inv_bwork = 0;
		return;
	}

	//正常工作
	test_ad();	//生产三相电压和电流波形

	// cmm.bstaie = 1;	// 也可手动置1控制开关机
	excita_loop();

	// fsm_inv_bwork = 1;	// 也可手动置1控制开关机
	vc_ssc_func();	

	CLK_CLICK_TEST();
}



#endif
/*
	static FILTER2	filter2_id;
	static FILTER2	filter2_iq;
	static FILTER2	filter2_idr = FILTER2_DEFAULTS;
	static FILTER2	filter2_iqr = FILTER2_DEFAULTS;

		#define FW_BD	5.0
		filter2_init_rapid(&filter2_id,  0.0, drv_pwm.ts, FW_BD);
		filter2_init_rapid(&filter2_iq,  0.0, drv_pwm.ts, FW_BD);
		filter2_init_rapid(&filter2_idr,  0.0, drv_pwm.ts, FW_BD);
		filter2_init_rapid(&filter2_iqr,  0.0, drv_pwm.ts, FW_BD);

//	FILTER2_FUNC_RAPID(filter2_id, id, id_f);
//	FILTER2_FUNC_RAPID(filter2_iq, iq, iq_f);
//	FILTER2_FUNC_RAPID(filter2_idr, idr, idr_f);
//	FILTER2_FUNC_RAPID(filter2_iqr, iqr, iqr_f);

*/

//	scope窗口 排序方式
//	鼠标点击顺序		=>		横排vertical	=>		竖排horizon
//	1	2	3				6	4	2				6	3
//	4	5	6				5	3	1				5	2
//													4	1


