#include "flashburn.h"			// ��������������ǰ�档

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

	// =============================һ��Ӳ����ʼ��=======================================

#ifdef FLASHBURN
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);		// ��仰��Ϊ�˳�ʼ��flash��������ram�����У�F28335.cmd
	MemCopy(&RamfuncsLoadStart2, &RamfuncsLoadEnd2, &RamfuncsRunStart2);
	InitFlash();					// ����TI����ʼ��FLASH�Ĵ���SYS
	memcpy(	&Flash28_API_RunStart,
			&Flash28_API_LoadStart,
			&Flash28_API_LoadEnd - &Flash28_API_LoadStart);
	memcpy(	&Progload28335_CAN_RunStart,
			&Progload28335_CAN_LoadStart,
			&Progload28335_CAN_LoadEnd - &Progload28335_CAN_LoadStart);

#endif 
	

#ifndef SIMULATOR	// û�������ʱ
	
	InitSysCtrl();					// ����TI����ʼ��ϵͳ�Ĵ�������Ҫ��������ֹ���Ź�����ʼ��PLLʱ��150MHZ������ʱ��

	// ISR + DOG
	DISABLE_INTS();
	InitPieCtrl();					// ����TI����ʼ��PIE�Ĵ���
	InitPieVectTable();				// ����TI����ʼ���жϺ���ʸ��
	drv_isr_init1(&zisr_fs);		// �жϳ�ʼ�������ø����ж�
	drv_dog_disable();				// �رտ��Ź�

	// IO + XINTF
	drv_io_init1();					// ��ʼ��io�Ĵ�����io����
	InitXintf();					// ����TI����ʼ���ⲿ�洢���Ĵ���������16λ�����ݺ͵�ַ��ʹ��

	// EXCEPT
	except_set_fname(chage_error, drv_cpld_reset_error);	// �쳣����ʱ���õĺ���
	except_clear_error();			// ����쳣
	
	// CPLD + PWM
	drv_cpld_init1();				// ��ʼ��cpld
	drv_pwm_stopA();				// �ر�pwm���
	drv_pwm_grid_stop();			// �ر�pwm���
	drv_pwm_init1();             	// ��ʼ��pwm�Ĵ���;pwm1,pwm2,pwm3����

	// AD
	drv_ad_init1();					// ��ʼ��ad�Ĵ�����ad���ã�У��ƫ
	
    // CAN + SCI +I2C
	drv_cana_init1();				// ��ʼ��cana�Ĵ���;cana����
	drv_canb_init1();				// ��ʼ��canb�Ĵ���;canb����
	drv_scib_init1();				// ��ʼ��sci�Ĵ���;scib����
	/*��ʼ������*/
	ProgramLoadInitCANB(0x7A1, 0x7C1); /*����0x7AAΪ����0������Ϣ��ID������0x7CCΪ����1������Ϣ��ID */
	/*��ʼ��ʱ��*/
	Flash_CPUScaleFactor = 	SCALE_FACTOR;  	

	// timer
	drv_timer_init1();				// ��ʱ��1��Ϊʱ��Դ

#endif

	// =============================���������ʼ�����������Ӳ��================================================
#ifdef SIMULATOR	// ��������
	cunit();		// ��Ԫ����ʱ
#endif	
	// CMM + ETHERNET + WLG
	cmm_init();                 	// ��ʼ��cmmͨѶ�����������ʼ���е�������Ҫ��д������Ķ��п��ܳ��ǰ�����ݡ�
	drv_ethernet_init1(); 
    
	// PRECHARGE
	precharge_init1();

	// RMS
	rms_init1();

	// DFT+POWER
	power_init1();

	// PLL
	pll_init1(&pll_grid, 3.0, 50.0);	//�ĳ�50�����ż������ס������

	chargebms_init1();
	// VC
	vc_ssc_init1();						//
	
	// BURNTEST
	burntest_init();

	charge_init1();

	protect_init();

	// cpld + dog + ISR				// ��������ܽ���Ӳ���ж�
	drv_cpld_init1();				// �ٴ����cpld���Ź����ϣ����ǰ���ʼ��ִ��ʱ�����
	drv_dog_enable();				// ʹ�ܿ��Ź�
	DRV_PWM_ISR_ACK();
	DRV_PWM_CLK_START();			// ��PWM�ж�ʱ�ӣ�PWMģ�鿪ʼ����������һ���жϣ������������ж�
	ENABLE_INTS();
	
	// ===================================������ѭ��===================================================

	for(;;){

#ifndef SIMULATOR	// û�������ʱ

		drv_io_main();		// ����Ű���Power����
		power_k = charge_power_relay(drv_io_bstart_ready&&protect_k);	// һ�����԰��� һ�����Ա���

		// CPLD + IO
		drv_cpld_read_judge_error();			// ��ȡCPLD���ж϶�ȡ��һЩ����
		drv_io_led_main();						// LED������

		// PRECHARGE
		precharge_reset();						// Ԥ���Ķ�ʱ; ��ʱ���բ����բ��Ȧ

		// AD + RMS + PROTECT
		drv_ad_read_temp(1);					// �ɼ��¶���	
		rms_calc();

		protect_over();

		// ͨѶ
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
		if (charge_laidian){		// ���ͻ�����

			fsm_dchg_bready = 0;	// һ��Ҫ�طŵ�Ŀ�������
			fsm_dchg_bstart = 0;
			cmm.out1 = 380;				//bltest
			charge_close_chg_clear_s();
			clk_Bjudgen(&clk_dchg, 0, 1000L);

			if (clk_Bjudgen(&clk_chg, 1, 1000L)){
				if (charge_close_dchg()){
					
					fsm_chg_bready = bmsA_1.EnableCharge;
					fsm_chg_bstart = bmsA_1.EnableCharge;

					drv_cpld_do_MCB_gridCAB(1);

					charge_dir     		= 1;	// ���
				    fsm_chg_bstate 		= 0;	// 0: û����
					PcsA_1.BatRunState 	= 1;	// ����ذ��������
				}
			}
			if (!bmsA_1.EnableCharge){
				fsm_chg_bready = bmsA_1.EnableCharge;
				fsm_chg_bstart = bmsA_1.EnableCharge;
			}
			// 3, �жϳ���Ƿ����
			fsm_chg_bstate = charge_chg_ok();
			if (fsm_chg_bstate){	// ������, ���ػ�
				fsm_chg_bready = 0;
				fsm_chg_bstart = 0;
			}

		}else{					// ���ͻ�û��

			fsm_chg_bready = 0;	//һ��Ҫ�س��Ŀ�������
			fsm_chg_bstart = 0;
			cmm.out1 = 0;//bltest
			charge_close_dchg_clear_s();
			clk_Bjudgen(&clk_chg, 0, 1000L);

			if (clk_Bjudgen(&clk_dchg, 1, 1000L)){
				if (charge_close_chg(0)){
					fsm_dchg_bready = bmsA_2.EnableDisCharge;
					fsm_dchg_bstart = bmsA_2.EnableDisCharge;

					drv_cpld_do_MCB_gridCAB(0);

					charge_dir      	= 0;	// �ŵ�
				    fsm_dchg_bstate 	= 0;	// 0: û����
					PcsA_1.BatRunState 	= 2;	// ����ذ��������
				}
			}
			if (!bmsA_2.EnableDisCharge){
				fsm_dchg_bready = bmsA_2.EnableDisCharge;
				fsm_dchg_bstart = bmsA_2.EnableDisCharge;
			}
			// 3, �жϷŵ��Ƿ����
			fsm_chg_bstate = charge_dchg_ok();
			if (fsm_dchg_bstate){	// �ŵ����, �ŵ�ػ�
				fsm_dchg_bready = 0;
				fsm_dchg_bstart = 0;
			}
		}
		fsm_chg_func();   // ���״̬��
		fsm_dchg_func();  // �ŵ�״̬��

		if (except.error){
			PcsA_1.BatRunState = 0;		// ���ͣ��״̬
		}

		EV_CAN_RxA();
		BMS_CAN_Tx();

		cmm_lemp_fan();

		power_func();

#endif		

#ifdef SIMULATOR	// ��������
		cunit();	// ��Ԫ����ʱ
#endif
		
#ifdef DEBUG		// ���Գ���
		test_main();
#endif

	}
}
