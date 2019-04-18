#include "flashburn.h"			// 必须放在这里，且最前面。

#include "test.h"
#include "cunit.h"
#include "zisr_fs.h"

#include "apply.h"

void main(void)
{
	static CLK cmm_clk = CLK_DEFAULTS;
	static unsigned cmm_turn = 0;
	unsigned power_k = 0;
	static CLK clk_chg  = CLK_DEFAULTS;
	static CLK clk_dchg = CLK_DEFAULTS;

	// =============================一、硬件初始化=======================================

#ifdef FLASHBURN
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);		// 这句话是为了初始化flash，调程序到ram中运行，F28335.cmd
	MemCopy(&RamfuncsLoadStart2, &RamfuncsLoadEnd2, &RamfuncsRunStart2);
	InitFlash();					// 来自TI，初始化FLASH寄存器SYS
	memcpy(	&Flash28_API_RunStart,
			&Flash28_API_LoadStart,
			&Flash28_API_LoadEnd - &Flash28_API_LoadStart);
	memcpy(	&Progload28335_CAN_RunStart,
			&Progload28335_CAN_LoadStart,
			&Progload28335_CAN_LoadEnd - &Progload28335_CAN_LoadStart);

#endif 
	

#ifndef SIMULATOR	// 没有软仿真时
	
	InitSysCtrl();					// 来自TI，初始化系统寄存器，主要包括：禁止看门狗、初始化PLL时钟150MHZ、外设时钟

	// ISR + DOG
	DISABLE_INTS();
	InitPieCtrl();					// 来自TI，初始化PIE寄存器
	InitPieVectTable();				// 来自TI，初始化中断函数矢量
	drv_isr_init1(&zisr_fs);		// 中断初始化，配置各种中断
	drv_dog_disable();				// 关闭看门狗

	// IO + XINTF
	drv_io_init1();					// 初始化io寄存器，io配置
	InitXintf();					// 来自TI，初始化外部存储器寄存器，包括16位的数据和地址口使能

	// EXCEPT
	except_set_fname(chage_error, drv_cpld_reset_error);	// 异常处理时调用的函数
	except_clear_error();			// 清除异常
	
	// CPLD + PWM
	drv_cpld_init1();				// 初始化cpld
	drv_pwm_stopA();				// 关闭pwm输出
	drv_pwm_grid_stop();			// 关闭pwm输出
	drv_pwm_init1();             	// 初始化pwm寄存器;pwm1,pwm2,pwm3配置

	// AD
	drv_ad_init1();					// 初始化ad寄存器，ad配置，校零偏
	
    // CAN + SCI +I2C
	drv_cana_init1();				// 初始化cana寄存器;cana配置
	drv_canb_init1();				// 初始化canb寄存器;canb配置
	drv_scib_init1();				// 初始化sci寄存器;scib配置
	/*初始化部分*/
	ProgramLoadInitCANB(0x7A1, 0x7C1); /*参数0x7AA为邮箱0接收消息的ID，参数0x7CC为邮箱1发送消息的ID */
	/*初始化时钟*/
	Flash_CPUScaleFactor = 	SCALE_FACTOR;  	

	// timer
	drv_timer_init1();				// 定时器1作为时钟源

#endif

	// =============================二、软件初始化，先软件后硬件================================================
#ifdef SIMULATOR	// 软仿真成立
	cunit();		// 单元测试时
#endif	
	// CMM + ETHERNET + WLG
	cmm_init();                 	// 初始化cmm通讯参数，这个初始化有点慢，需要回写。后面的读有可能冲掉前面数据。
	drv_ethernet_init1(); 
    
	// PRECHARGE
	precharge_init1();

	// RMS
	rms_init1();

	// DFT+POWER
	power_init1();

	// PLL
	pll_init1(&pll_grid, 3.0, 50.0);	//改成50，解决偶尔锁不住的问题

	chargebms_init1();
	// VC
	vc_ssc_init1();						//
	
	// BURNTEST
	burntest_init();

	charge_init1();

	protect_init();

	// cpld + dog + ISR				// 最后开启，能进入硬件中断
	drv_cpld_init1();				// 再次清除cpld看门狗故障，如果前面初始化执行时间过长
	drv_dog_enable();				// 使能看门狗
	DRV_PWM_ISR_ACK();
	DRV_PWM_CLK_START();			// 开PWM中断时钟，PWM模块开始计数，避免一开中断，就连续几次中断
	ENABLE_INTS();
	
	// ===================================三、主循环===================================================

	for(;;){

#ifndef SIMULATOR	// 没有软仿真时

		drv_io_main();		// 检测门按键Power开关
		power_k = charge_power_relay(drv_io_bstart_ready&&protect_k);	// 一个来自按键 一个来自保护

		// CPLD + IO
		drv_cpld_read_judge_error();			// 读取CPLD，判断读取的一些错误
		drv_io_led_main();						// LED灯闪灭

		// PRECHARGE
		precharge_reset();						// 预充电的定时; 延时清合闸、分闸线圈

		// AD + RMS + PROTECT
		drv_ad_read_temp(1);					// 采集温度量	
		rms_calc();

		protect_over();

		// 通讯
		if (clk_Bjudgen(&cmm_clk, 1, 30L)) {
			switch (cmm_turn) {
				case 0:	cmm_read_pnl(fsm_dchg_bwork||fsm_dchg_bwork);  cmm_turn++;  break;
				case 1:	cmm_write_pnl();			 	  cmm_turn++;  break;
				case 2:	cmm_reset_parameter();		 	  cmm_turn = 0;break;
				default:								  cmm_turn = 0;break;
			}
		}
		cmm_arb();

		test_ua = bmsA_1.EnableCharge;
		test_ub = bmsA_2.EnableDisCharge;
bmsA_1.EnableCharge    = power_k&&cmm.bstart_dchg&&bmsA_1.EnableCharge;//bltest
bmsA_2.EnableDisCharge = power_k&&cmm.bstart_dchg&&bmsA_2.EnableDisCharge&&protect_close_dis;//bltest
		charge_laidian = drv_cpld_laidian();
		if (charge_laidian){		// 柴油机来电

			fsm_dchg_bready = 0;	// 一定要关放电的开机命令
			fsm_dchg_bstart = 0;
			cmm.out1 = 380;				//bltest
			charge_close_chg_clear_s();
			clk_Bjudgen(&clk_dchg, 0, 1000L);

			if (clk_Bjudgen(&clk_chg, 1, 1000L)){
				if (charge_close_dchg()){
					
					fsm_chg_bready = bmsA_1.EnableCharge;
					fsm_chg_bstart = bmsA_1.EnableCharge;

					drv_cpld_do_MCB_gridCAB(1);

					charge_dir     		= 1;	// 充电
				    fsm_chg_bstate 		= 0;	// 0: 没充满
					PcsA_1.BatRunState 	= 1;	// 给电池包充电请求
				}
			}
			if (!bmsA_1.EnableCharge){
				fsm_chg_bready = bmsA_1.EnableCharge;
				fsm_chg_bstart = bmsA_1.EnableCharge;
			}
			// 3, 判断充电是否完成
			fsm_chg_bstate = charge_chg_ok();
			if (fsm_chg_bstate){	// 充电完成, 充电关机
				fsm_chg_bready = 0;
				fsm_chg_bstart = 0;
			}

		}else{					// 柴油机没电

			fsm_chg_bready = 0;	//一定要关充电的开机命令
			fsm_chg_bstart = 0;
			cmm.out1 = 0;//bltest
			charge_close_dchg_clear_s();
			clk_Bjudgen(&clk_chg, 0, 1000L);

			if (clk_Bjudgen(&clk_dchg, 1, 1000L)){
				if (charge_close_chg(0)){
					fsm_dchg_bready = bmsA_2.EnableDisCharge;
					fsm_dchg_bstart = bmsA_2.EnableDisCharge;

					drv_cpld_do_MCB_gridCAB(0);

					charge_dir      	= 0;	// 放电
				    fsm_dchg_bstate 	= 0;	// 0: 没放完
					PcsA_1.BatRunState 	= 2;	// 给电池包充电请求
				}
			}
			if (!bmsA_2.EnableDisCharge){
				fsm_dchg_bready = bmsA_2.EnableDisCharge;
				fsm_dchg_bstart = bmsA_2.EnableDisCharge;
			}
			// 3, 判断放电是否完成
			fsm_chg_bstate = charge_dchg_ok();
			if (fsm_dchg_bstate){	// 放电完成, 放电关机
				fsm_dchg_bready = 0;
				fsm_dchg_bstart = 0;
			}
		}
		fsm_chg_func();   // 充电状态机
		fsm_dchg_func();  // 放电状态机

		if (except.error){
			PcsA_1.BatRunState = 0;		// 电池停机状态
		}

		EV_CAN_RxA();
		BMS_CAN_Tx();

		cmm_lemp_fan();

		power_func();

#endif		

#ifdef SIMULATOR	// 软仿真成立
		cunit();	// 单元测试时
#endif
		
#ifdef DEBUG		// 调试成立
		test_main();
#endif

	}
}
