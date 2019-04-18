#include "drv.h"
#include "drv_io.h"
#include "base.h"


DRV_IO drv_io = DRV_IO_DEFAULTS;
unsigned drv_io_bstart_ready = 0;


// 描述：io寄存器初始化;只能一次,停机时不能重新初始化
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-1-17
void drv_io_init1(void)
{
    InitGpio();              	// 来自TI，IO寄存器

    EALLOW;
    GpioCtrlRegs.GPACTRL.all  = 0xFFFFFFFF;    // GPIO0-GPIO31 采样周期SYSCLKOUT*510
    GpioCtrlRegs.GPBCTRL.all  = 0xFFFFFFFF;    // GPIO32-GPIO63 采样周期SYSCLKOUT*510
    GpioCtrlRegs.GPAQSEL1.all = 0xFFFFFFFF;    // GPIO0-GPIO15  配置为外设为不同步;配置为GPIO仅与系统时钟同步
    GpioCtrlRegs.GPAQSEL2.all = 0xFFFFFFFF;    // GPIO16-GPIO31 配置为外设为不同步;配置为GPIO仅与系统时钟同步
    GpioCtrlRegs.GPBQSEL1.all = 0xFFFFFFFF;    // GPIO32-GPIO39 配置为外设为不同步;配置为GPIO仅与系统时钟同步
    GpioCtrlRegs.GPBQSEL2.all = 0xFFFFFFFF;    // GPIO48-GPIO63 配置为外设为不同步;配置为GPIO仅与系统时钟同步

	GpioDataRegs.GPASET.all    = 0;
	GpioDataRegs.GPBSET.all    = 0;
	GpioDataRegs.GPCSET.all    = 0;
	GpioDataRegs.GPACLEAR.all  = 0;
	GpioDataRegs.GPBCLEAR.all  = 0;
	GpioDataRegs.GPCCLEAR.all  = 0;
	GpioDataRegs.GPATOGGLE.all = 0;
	GpioDataRegs.GPBTOGGLE.all = 0;
	GpioDataRegs.GPCTOGGLE.all = 0;

	// 对内DI  id
    GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0;   	// 设置成输入
    GpioCtrlRegs.GPBDIR.bit.GPIO55 = 0;   	// 设置成输入
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 0;   	// 设置成输入
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 0;   	// 设置成输入
    GpioCtrlRegs.GPBDIR.bit.GPIO58 = 0;   	// 设置成输入

	// 对内DO	LED
    GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;   	// 设置成输出 LED1
    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;   	// 设置成输出 LED2
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;   	// 设置成输出 LED3
    GpioCtrlRegs.GPBDIR.bit.GPIO62 = 1;   	// 设置成输出 LED4
    GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;   	// 设置成输出 LED5

    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;   	// 测试点
	
	// 预留
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;		// TZ1，TZ2，TZ3，TZ4，目前没用到，设置成输入，安全。
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 0;
    //GpioDataRegs.GPADAT.bit.GPIO12 = 0;		
	//GpioDataRegs.GPADAT.bit.GPIO13 = 0;
	//GpioDataRegs.GPADAT.bit.GPIO14 = 0;
	//GpioDataRegs.GPADAT.bit.GPIO15 = 0;

    GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;   	// 以太网芯片复位
	GpioDataRegs.GPBSET.bit.GPIO35 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;   	// 设置成输出 CPLD踢狗


	// 对外DI  无;在cpld上
	// 对外DO 无;在cpld上

    EDIS;

	drv_io_cmm_id();
}


// 描述：读DSP板上的id码,做为通信id
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-3-6

void drv_io_cmm_id(void)
{
    drv_io.cmm_id = (GpioDataRegs.GPBDAT.bit.GPIO54)       	// ADDR1
       			  + (GpioDataRegs.GPBDAT.bit.GPIO55 << 1)  	// ADDR2
       			  + (GpioDataRegs.GPBDAT.bit.GPIO56 << 2)  	// ADDR3
       			  + (GpioDataRegs.GPBDAT.bit.GPIO57 << 3) 	// ADDR4
       			  + (GpioDataRegs.GPBDAT.bit.GPIO58 << 4); 	// ADDR5
}



// 描述：main()程序中，灯的闪灭
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

void drv_io_led_main(void)
{
	static unsigned bflag = 0;
	static CLK clk0       = CLK_DEFAULTS;

	if (!clk_Bjudgen(&clk0, 1, 2000L)) 	return;			// 1s执行一次的初始化

	DRV_IO_OUT_LED1(bflag);
	bflag = ~bflag;
}



// 描述：zisr_fs()程序中，灯的闪灭
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19
/*
void drv_io_led_fs(void)
{
	static unsigned bflag = 0;
	static CLK clk0       = CLK_DEFAULTS;

	if (clk_Bjudgen(&clk0, 1, 600L)){			// 0.6s执行一次的初始化
		DRV_IO_OUT_LED2(bflag);
		bflag = ~bflag;
	}
}
*/


// 描述：放在main()程序中
void drv_io_main(void)
{
	       unsigned	io_key    = 0					  ;		//io_key不用static变量，io键的当前瞬时值
	static unsigned					io_key_back    = 0;
	static unsigned	io_level  = 0, 	io_level_back  = 0;		//io键的电平值（消除键抖动后）

	static CLK clk1 = CLK_DEFAULTS;
	
	//启停按键
	io_key = DRV_CPLD_DI_POW();

	//1、消除键抖动，得到电平
	if (io_key != io_key_back){
		clk_Bjudge1(&clk1, 0, 50L);
	}if(io_key==1){
		if(clk_Bjudge1(&clk1, 1, 50L))		io_level = 1;	//启动时间可以比关机时间长些，这样很多程序的初始化就完成了。
	}else{
		if(clk_Bjudge1(&clk1, 1, 350L))   	io_level = 0;
	}
	
	//备份
	io_key_back   = io_key;     
	
	//2、根据上升沿，切换开关机
	if(io_level_back == 0 && io_level == 1){
		drv_io_bstart_ready = 1 - drv_io_bstart_ready;		//正式代码时要改
	}

	//备份
	io_level_back = io_level;
	
	// 电源继电器放别处了
//	drv_cpld_do_POWER(drv_io_bstart_ready); 		        //电源继电器吸合

}

