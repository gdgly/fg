
#include "test.h"
#include "cunit.h"
#include "zisr_fs.h"
#include "apply.h"

//////////////观测,临时extern    begin  ////////////////////////////////
extern PID charge_pid_UA;
extern PID charge_pid_IA;

extern float chargeA_ref, UA_ref,	IA_ref, Aduty;
//////////////观测,临时extern    end     ////////////////////////////////


/****************************************
*	功能：解析接收到的CANB总线数据
*	参数：无
*	返回值：void
*	创建时间：20130216
*****************************************/
#pragma CODE_SECTION(EncoderDataFromCANB, "ramfuncs");
void EncoderDataFromCANB(void)
{
	if((ECanbRegs.CANRMP.all & 0x00003000)!=0)
	{
		// 接收自检、启控命令
		if(ECanbRegs.CANRMP.bit.RMP12 == 1)
		{
			ECanbRegs.CANRMP.all = 0x00001000; 					// 清除接收标志
		}
		// 接收控制指令
		if(ECanbRegs.CANRMP.bit.RMP13 == 1)		
		{
			ECanbRegs.CANRMP.all=0x00002000;  
		}
	}
	/*在线下载*/
	if((ECanbRegs.CANRMP.all & 0x00000001)!=0)
	{
		/*接收到CAN-A模块邮箱0的CAN总线消息*/	
		ProgramLoadEntryCANB();
	} 
}


#pragma CODE_SECTION(zisr_fs, "ramfuncs");

// 描述：载波中断，本质是个定时中断，采用PWM1的定时中断。 顶点中断计算，底点发送PWM
#ifndef SIMULATOR		//没有软仿真时

interrupt void zisr_fs(void)

#else					//软仿真成立时

void zisr_fs(void)

#endif
{
    static unsigned turn = 0;
	unsigned long bitval;

DRV_IO_OUT_TEST(1);//bltest
	if (!fsm_chg_bwork && !fsm_chg_bwork && drv_ad.udc<0.2)	EncoderDataFromCANB();	//不开机时才能下载程序 

	DRV_PWM_ISR_ACK();
	DRV_ISR_ACK_INT3();

	DRV_DOG_KICK();							// 踢狗1

	// 采样
	drv_ad_read();							// 数据采样及处理

	CLK_CLICK_FS(DRV_TIMER_GET_MS0_1());	// 时钟计数
	DRV_IO_LED_FS();						// LED灯亮
	drv_cpld_read_judge_error();			// 读取CPLD，判断读取的一些错误

	// 保护
	protect_fs();							// 一部分放到主函数

	// 电池包充放电闭环
	if (cmm.in7)
	charge_loopA();

	// PLL + VC
	pll_func(&pll_grid, drv_ad.ualpha, drv_ad.ubeta);		//锁相环。输出pll.f

    if (cmm.in6)
	vc_ssc_func();

	ACC_MOD2(turn, 1);
	
	// RMS
	rms_input_fs();

	#define F(val, n)	((unsigned long)(val) * (1L<<n))
	bitval	= F(except.error>0,22)
            + F(test_ub,20)                 + F(test_ua,19)
            + F(0,18)                       + F(charge_laidian,17)
            + F(bmsA_1.EnableCharge,16)     + F(bmsA_2.EnableDisCharge,15)
            + F(cmm.bstart_dchg,14)         + F(protect_close_dis,13)
            + F(vc_ssc.bwork,12)           	+ F(drv_io_bstart_ready,11)
            + F(protect_k,10)               + F(cmm.ChargeFastSlow,9)
			+ F(charge_bworkA,8)			+ F(charge_dir,7)			+ F(fsm_dchg_bstart,6)+ F(fsm_chg_bstart,5)
			+ F(PcsA_1.BatRunState,3)		+ F(fsm_dchg_bwork,2) 		+ F(fsm_chg_bwork,1);

	#define	ETH		DRV_ETHERNET_VAR_READ

	ETH(1,	protect_uouta, bmsA_3.N_systemVoltage, bmsA_3.N_systemCurrent, 	    drv_ad.ia_out, drv_ad.ib_out, drv_ad.ic_out);
	ETH(7,	bitval,	drv_ad.udc,	IA_ref,                	    chargeA_ref, charge_iabc,   Aduty);
	ETH(13,	drv_ad.ua_in, drv_ad.ub_in, drv_ad.uc_in, 		drv_ad.ia_in, drv_ad.ib_in, drv_ad.ic_in);
	ETH(19,	charge_pid_IA.kp,vc_Uamp2, vc_ssc.aduty,        charge_pid_IA.err,charge_pid_IA.up,charge_pid_IA.ui);
//	ETH(19,	vc_Uamp1,vc_Uamp2, bmsA_error.BmsState2.all,    vc_ssc.aduty,vc_ssc.bduty,bmsA_3.N_systemVoltage);
	ETH(25,	rms_ua_out, charge_udc_ref,  drv_ad.stacka_temp,	charge_pid_UA.err,charge_pid_UA.up,charge_pid_UA.ui);
  
	drv_ethernet_ram_write();

DRV_IO_OUT_TEST(0);//bltest

}



/////////////////////////测中断时间 开关频率 总时间
//测中断时间，        软件读时间：		示波器读时间：
//静态跑(含vc_init())   us              --us             
//fsm_inv_bwork=1:
//中断总时间:	       us              --us              

