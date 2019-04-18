// ������ͷ�ļ���һЩ�꺯�����塣
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-1-28

#ifndef __DRV_H__
#define __DRV_H__

// ����TI��ͷ�ļ�

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File

//----------------------- ����궨����--------------------------------//


#define  NOP1()				asm(" nop")
#define  NOP2()				asm(" nop; nop")
#define  NOP3()				asm(" nop; nop; nop")

//------------------------- �궨����---------------------------------//

#define DISABLE_INTS()				DINT
#define ENABLE_INTS()				EINT

/*
#define DISABLE_WRITE_REGISTER()	{asm("    ALLOW   ");
#define ENABLE_WRITE_REGISTER()		{asm("    ADIS    ");

#define  CPUINHZ					(150L*1000*1000)						// CPU������150MHZ
*/

//�˺꺯����������Ǳ��ʽ���Ƚ�����ĺ꺯�����������ô����Ŷ�����С���š�express����д��(express)��С���Ų�����ס���ֺŵ���䣬�������
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

//----------------------- ͷ�ļ�������--------------------------------//


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

//drv��=================================================================================
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
