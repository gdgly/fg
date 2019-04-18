
#include "test.h"
#include "cunit.h"
#include "zisr_fs.h"
#include "apply.h"

//////////////�۲�,��ʱextern    begin  ////////////////////////////////
extern PID charge_pid_UA;
extern PID charge_pid_IA;

extern float chargeA_ref, UA_ref,	IA_ref, Aduty;
//////////////�۲�,��ʱextern    end     ////////////////////////////////


/****************************************
*	���ܣ��������յ���CANB��������
*	��������
*	����ֵ��void
*	����ʱ�䣺20130216
*****************************************/
#pragma CODE_SECTION(EncoderDataFromCANB, "ramfuncs");
void EncoderDataFromCANB(void)
{
	if((ECanbRegs.CANRMP.all & 0x00003000)!=0)
	{
		// �����Լ졢��������
		if(ECanbRegs.CANRMP.bit.RMP12 == 1)
		{
			ECanbRegs.CANRMP.all = 0x00001000; 					// ������ձ�־
		}
		// ���տ���ָ��
		if(ECanbRegs.CANRMP.bit.RMP13 == 1)		
		{
			ECanbRegs.CANRMP.all=0x00002000;  
		}
	}
	/*��������*/
	if((ECanbRegs.CANRMP.all & 0x00000001)!=0)
	{
		/*���յ�CAN-Aģ������0��CAN������Ϣ*/	
		ProgramLoadEntryCANB();
	} 
}


#pragma CODE_SECTION(zisr_fs, "ramfuncs");

// �������ز��жϣ������Ǹ���ʱ�жϣ�����PWM1�Ķ�ʱ�жϡ� �����жϼ��㣬�׵㷢��PWM
#ifndef SIMULATOR		//û�������ʱ

interrupt void zisr_fs(void)

#else					//��������ʱ

void zisr_fs(void)

#endif
{
    static unsigned turn = 0;
	unsigned long bitval;

DRV_IO_OUT_TEST(1);//bltest
	if (!fsm_chg_bwork && !fsm_chg_bwork && drv_ad.udc<0.2)	EncoderDataFromCANB();	//������ʱ�������س��� 

	DRV_PWM_ISR_ACK();
	DRV_ISR_ACK_INT3();

	DRV_DOG_KICK();							// �߹�1

	// ����
	drv_ad_read();							// ���ݲ���������

	CLK_CLICK_FS(DRV_TIMER_GET_MS0_1());	// ʱ�Ӽ���
	DRV_IO_LED_FS();						// LED����
	drv_cpld_read_judge_error();			// ��ȡCPLD���ж϶�ȡ��һЩ����

	// ����
	protect_fs();							// һ���ַŵ�������

	// ��ذ���ŵ�ջ�
	if (cmm.in7)
	charge_loopA();

	// PLL + VC
	pll_func(&pll_grid, drv_ad.ualpha, drv_ad.ubeta);		//���໷�����pll.f

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



/////////////////////////���ж�ʱ�� ����Ƶ�� ��ʱ��
//���ж�ʱ�䣬        �����ʱ�䣺		ʾ������ʱ�䣺
//��̬��(��vc_init())   us              --us             
//fsm_inv_bwork=1:
//�ж���ʱ��:	       us              --us              

