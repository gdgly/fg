// 描述：头文件。一些宏函数定义。
// 作者：何超
// 版本：4.0
// 日期：星期四,2013-1-24

#ifndef __APPLY_H__
#define __APPLY_H__

//------------------------- 头文件定义区------------------------------//
#include "FPU.h"

#include "base.h"
#include "drv.h"
#include "math.h"

#include "cmm.h"

#include "relay.h"
#include "precharge.h"

#include "tlimit.h"
#include "protect.h"

#include "chargebms.h"
#include "charge.h"
#include "fsm_chg.h"
#include "fsm_dchg.h"
#include "power.h"

#include "pll.h"
#include "rms.h"
#include "vc_ssc.h"

#include "svpwm.h"
#include "burntest.h"

//#include "led.h"

#include "test.h"

//APPLY层=================================================================================

#pragma CODE_SECTION(protect_fs   				, "ramfuncs");
//#pragma CODE_SECTION(Tlimit_fix		   			, "ramfuncs");
//#pragma CODE_SECTION(Tlimit_anti	   			, "ramfuncs");

#pragma CODE_SECTION(pll_func       			, "ramfuncs");
#pragma CODE_SECTION(burntest_break	        	, "ramfuncs");
#pragma CODE_SECTION(charge_loopA     			, "ramfuncs");
//#pragma CODE_SECTION(vc_n_ithd_gen        		, "ramfuncs");
//#pragma CODE_SECTION(vc_fwd        				, "ramfuncs");
//#pragma CODE_SECTION(vc_limit_get_max      		, "ramfuncs");
//#pragma CODE_SECTION(vc_limit_kin_i_ct			, "ramfuncs");

#pragma CODE_SECTION(vc_ssc_init       			, "ramfuncs");
#pragma CODE_SECTION(vc_ssc_func        		, "ramfuncs");
#pragma CODE_SECTION(svpwm_dq		   			, "ramfuncs");
//#pragma CODE_SECTION(svpwm_mf		   			, "ramfuncs");
#pragma CODE_SECTION(rms_input_fs		   	    , "ramfuncs");

//#pragma CODE_SECTION(fft_input					, "ramfuncs");

//#pragma CODE_SECTION(dft_calc			   		, "ramfuncs");//测试用

#endif  // __APPLY_H__
