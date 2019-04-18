// 描述：头文件。一些宏函数定义。
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-1-28

#ifndef __DRV_H__
#define __DRV_H__

// 来自TI的头文件

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File

//----------------------- 特殊宏定义区--------------------------------//


#define  NOP1()				asm(" nop")
#define  NOP2()				asm(" nop; nop")
#define  NOP3()				asm(" nop; nop; nop")

//------------------------- 宏定义区---------------------------------//

#define DISABLE_INTS()				DINT
#define ENABLE_INTS()				EINT

/*
#define DISABLE_WRITE_REGISTER()	{asm("    ALLOW   ");
#define ENABLE_WRITE_REGISTER()		{asm("    ADIS    ");

#define  CPUINHZ					(150L*1000*1000)						// CPU工作于150MHZ
*/

//此宏函数输入参数是表达式。比较特殊的宏函数。最外面用大扩号而不是小扩号。express不能写成(express)。小扩号不能扩住带分号的语句，否则错误。
#define	SECURE_EXPRESS(express)		\
{									\
		DISABLE_INTS();				\
		express						\
		ENABLE_INTS();				\
}

#define	SECURE_REGISTER(express)	\
{									\
		DISABLE_WRITE_REGISTER();	\
		express						\
		ENABLE_WRITE_REGISTER();	\
}

//----------------------- 头文件定义区--------------------------------//


#include "drv_io.h"
#include "drv_cpld.h"

#include "drv_can.h"
#include "drv_sci.h"
#include "drv_ethernet.h"

#include "drv_dog.h"
#include "drv_timer.h"
#include "drv_isr.h"
#include "drv_ad.h"
#include "drv_pwm.h"

#include "Progload28335_CANlib.h"

//drv层=================================================================================
#pragma CODE_SECTION(drv_ad_read    			, "ramfuncs");
#pragma CODE_SECTION(drv_ad_calibrate           , "ramfuncs");
#pragma CODE_SECTION(drv_io_led_fs              , "ramfuncs");
#pragma CODE_SECTION(drv_pwm_startA          	, "ramfuncs");
#pragma CODE_SECTION(drv_pwm_stopA           	, "ramfuncs");
#pragma CODE_SECTION(drv_pwm_stop          		, "ramfuncs");
#pragma CODE_SECTION(drv_pwm_buck_duty          , "ramfuncs");
#pragma CODE_SECTION(drv_pwm_grid_start         , "ramfuncs");
#pragma CODE_SECTION(drv_pwm_grid_stop          , "ramfuncs");
#pragma CODE_SECTION(drv_ethernet_ram_write     , "ramfuncs");
#pragma CODE_SECTION(drv_cpld_read_judge_error 	, "ramfuncs");


#endif  // __DRV_H__
