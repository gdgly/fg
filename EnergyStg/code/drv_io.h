// 描述：IO头文件
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

#ifndef __DRV_IO_H__
#define __DRV_IO_H__

#include "base.h"

// 给IO管脚输出值，LED1 低电平是亮,所以这里取反输出 
#define 	DRV_IO_OUT_LED1(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO59 = 1; else GpioDataRegs.GPBSET.bit.GPIO59 = 1;
#define 	DRV_IO_OUT_LED2(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1; else GpioDataRegs.GPBSET.bit.GPIO60 = 1;
#define 	DRV_IO_OUT_LED3(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO61 = 1; else GpioDataRegs.GPBSET.bit.GPIO61 = 1;
#define 	DRV_IO_OUT_LED4(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1; else GpioDataRegs.GPBSET.bit.GPIO48 = 1;
#define 	DRV_IO_OUT_LED5(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1; else GpioDataRegs.GPBSET.bit.GPIO49 = 1;
#define 	DRV_IO_OUT_TEST(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1; else GpioDataRegs.GPBSET.bit.GPIO34 = 1;


// 踢CPLD看门狗。如DSP不踢，CPLD封PWM。CPLD40KHZ检测1个点的速度，30个点没检测到，就封锁
#define 	DRV_IO_KICK_CPLD(bflag)  		if(bflag)	GpioDataRegs.GPASET.bit.GPIO24 = 1;	else GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;

// 以下是为测程序时间而用的，正式不用的
#define		DRV_IO_RESET_ETHERNET(bflag)	if(bflag)	GpioDataRegs.GPBSET.bit.GPIO35 = 1;	else GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;




typedef struct {
                // 内部变量：
				unsigned bflag_fs;
				CLK clk0;
                // 输入：

                // 输出：
                unsigned  	cmm_id;  	// 4个satck的功率等级
} DRV_IO;


#define DRV_IO_DEFAULTS {       0, CLK_DEFAULTS, 0	    }

// 声明对象指针
typedef DRV_IO *DRV_IO_HANDLE;




// 函数声明
void drv_io_init1(void);
void drv_io_cmm_id(void);
void drv_io_led_main(void);
void drv_io_led_fs(void);
void drv_io_main(void);

//------------------------- 全局变量定义区------------------------------//

extern DRV_IO drv_io;
extern unsigned drv_io_bstart_ready;


// 描述：zisr_fs()程序中，灯的闪灭。0.6s执行一次的初始化
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19
//  void drv_io_led_fs(void)
#define  DRV_IO_LED_FS()									\
{															\
	if (clk_Bjudgen(&drv_io.clk0, 1, 600L)){			    \
		DRV_IO_OUT_LED2(drv_io.bflag_fs);                   \
		drv_io.bflag_fs = ~drv_io.bflag_fs;                 \
	}														\
}






#endif  // __DRV_IO_H__

