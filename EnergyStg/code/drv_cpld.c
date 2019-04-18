#include "drv_cpld.h"
#include "base.h"
#include "drv.h"

DRV_CPLD drv_cpld = DRV_CPLD_DEFAULTS;

// 描述：
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void drv_cpld_init1(void)
{
	drv_cpld_reset_error();
	DRV_CPLD_DO_STOP_PWM();
	DRV_CPLD_DO_STOP_GRID_PWM();

	EXCEPT_JUDGE_SET(!(CPLD_BUS_TEST1 == 0xAA55 && CPLD_BUS_TEST2 == 0x55AA), ERROR_DRV_CPLD_BUS_TEST);
	drv_cpld.version = CPLD_VERSION;
}


// 描述：读cpld
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void drv_cpld_read_judge_error(void)
{
	drv_cpld.error = CPLD_ERROR;
	drv_cpld.di    = CPLD_DI;

	EXCEPT_JUDGE_SET(DRV_CPLD_A_DRV_ERROR() ,	 ERROR_DRV_CPLD_A_STACK_DRV   );
	EXCEPT_JUDGE_SET(DRV_CPLD_B_DRV_ERROR() ,	 ERROR_DRV_CPLD_B_STACK_DRV   );
	EXCEPT_JUDGE_SET(DRV_CPLD_C_DRV_ERROR() ,	 ERROR_DRV_CPLD_C_STACK_DRV   );
	EXCEPT_JUDGE_SET(DRV_CPLD_D_DRV_ERROR() ,	 ERROR_DRV_CPLD_D_STACK_DRV   );
//	EXCEPT_JUDGE_SET(DRV_CPLD_A_OUT_OI()	,	 ERROR_DRV_CPLD_A_OUT_OI      );
//	EXCEPT_JUDGE_SET(DRV_CPLD_B_OUT_OI()	,	 ERROR_DRV_CPLD_B_OUT_OI      );
//	EXCEPT_JUDGE_SET(DRV_CPLD_C_OUT_OI()	,	 ERROR_DRV_CPLD_C_OUT_OI      );
//	EXCEPT_JUDGE_SET(DRV_CPLD_D_OUT_OI()	,	 ERROR_DRV_CPLD_D_OUT_OI      );
	EXCEPT_JUDGE_SET(DRV_CPLD_A_OT()		,    ERROR_DRV_CPLD_A_OT          );
	EXCEPT_JUDGE_SET(DRV_CPLD_B_OT()		,    ERROR_DRV_CPLD_B_OT          );
	EXCEPT_JUDGE_SET(DRV_CPLD_C_OT()		,    ERROR_DRV_CPLD_C_OT          );
	EXCEPT_JUDGE_SET(DRV_CPLD_D_OT()		,    ERROR_DRV_CPLD_D_OT          );
	EXCEPT_JUDGE_SET(DRV_CPLD_ODC()		    ,    ERROR_DRV_CPLD_ODC           );

}

void drv_cpld_reset_error(void)
{
	CPLD_RESET_ERROR = 0xFF50;			// 清除CPLD故障信息
	CPLD_RESET_ERROR = 0x55AA;
}


//以下用函数形式，因为relay吸合有延时，所以单独有函数来处理，需要函数指针导入如下函数。

void drv_cpld_do_cab_fan(unsigned bflag)
{
	if(bflag)	CPLD_RELAY1 = 0xBB21;	else	CPLD_RELAY1 = 0xBB20;		// DO--1  风机
}

void drv_cpld_do_yellow(unsigned bflag)
{
	if(bflag)	CPLD_RELAY2 = 0xBB21;	else	CPLD_RELAY2 = 0xBB20;		// DO--2  黄灯
}

void drv_cpld_do_blue(unsigned bflag)
{
	if(bflag)	CPLD_RELAY3 = 0xBB21;	else	CPLD_RELAY3 = 0xBB20;		// DO--3  蓝灯
}

void drv_cpld_do_red(unsigned bflag)
{
	if(bflag)	CPLD_RELAY4 = 0xBB21;	else	CPLD_RELAY4 = 0xBB20;		// DO--4  红灯
}

void drv_cpld_do_MCB_grid(unsigned bflag)
{
	if(bflag)	CPLD_RELAY5 = 0xBB21;	else	CPLD_RELAY5 = 0xBB20;		// DO--5  模块网侧输入主开关合闸
}

void drv_cpld_do_prechg_grid(unsigned bflag)
{
	if(bflag)	CPLD_RELAY6 = 0xBB21;	else	CPLD_RELAY6 = 0xBB20;		// DO--6  模块网侧输入预充电
}

void drv_cpld_do_MCB_bat(unsigned bflag)
{
	if(bflag)	CPLD_RELAY7 = 0xBB21;	else	CPLD_RELAY7 = 0xBB20;		// DO--7  模块电池侧主开关合闸
}

void drv_cpld_do_dis(unsigned bflag)
{
	if(bflag)	CPLD_DIS    = 0xBB21;	else	CPLD_DIS    = 0xBB20;		// 放电
}

void drv_cpld_do_MCB_gridCAB(unsigned bflag)
{
	if(bflag)	CPLD_RELAY9    = 0xBB21;	else	CPLD_RELAY9    = 0xBB20;		// 机柜主接触器
}

void drv_cpld_do_POWER(unsigned bflag)
{
	if(bflag)	CPLD_RELAY10    = 0xBB21;	else	CPLD_RELAY10    = 0xBB20;		// 控制电接触器
}

void drv_cpld_do_prechg_bat(unsigned bflag)
{
	if(bflag)	CPLD_RELAY11 = 0xBB21;	else	CPLD_RELAY11 = 0xBB20;		        // DO--6  模块网侧输入预充电
}

// 判断柴油机是否有电 这里最好要防抖反复确认
unsigned drv_cpld_laidian(void)
{
    return (!DRV_CPLD_DI_MPS());	// 电气图有反向
}


