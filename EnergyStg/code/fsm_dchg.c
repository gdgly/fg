#include "apply.h"
#include "fsm_dchg.h"


unsigned fsm_dchg_bwork  = 0;			// ����boost����
unsigned fsm_dchg_binv	 = 0;			// ��֧inv����
unsigned fsm_dchg_bready = 0;			// 
unsigned fsm_dchg_bstart = 0;			// 
unsigned fsm_dchg_bstate = 0;			// 0: û����
										// 1: ����

static enum {		//��ö������������
	S0 = 0,			//
	S1,				//
	S2,				//
	S3,				//
	S4,				//
					//
	S_END
} S=S0, S_OLD=S1;

#define	FSM1_GO_NEXT_RETURN()	 	{if(S>=(S_END-1)) 	S=S0;	else	S++;	return;}
#define	FSM1_GO_STOP()	 			{S=S0;	return;}


//=======================================================״̬0:STOP===============================================================
void fsm_dchg_f0_stop_in(void)
{
	drv_pwm_stopA();			//�����

	fsm_dchg_bwork    = 0;

	drv_cpld_do_cab_fan(0);		//�ػ��ط���

	drv_cpld_do_prechg_bat(0);	//�ŵ�ػ�, ģ���ڿ��ض��ɹ�
    drv_cpld_do_MCB_bat(0);

 	drv_cpld_do_prechg_grid(0);
	drv_cpld_do_MCB_grid(0);

}

void fsm_dchg_f0_stop(void)
{
		
	if(fsm_dchg_bready && except.warn == 0 && except.error == 0)	FSM1_GO_NEXT_RETURN();	//״̬0 =>״̬1
}



//=======================================================״̬1:READY===============================================================

void fsm_dchg_f1_prechg_in(void)
{
	precharge_ready_dchg();			//��բǰ�����ȸ�λ
}

void fsm_dchg_f1_prechg(void)
{
	unsigned bflag;
	
	if(!fsm_dchg_bready || except.error != 0)			FSM1_GO_STOP();	//״̬X =>״̬0

	drv_cpld_do_cab_fan(1);							//���ھͿ������з��ȣ���ͣ�����Ժ��ٸġ�
	
	bflag = precharge_dchg_func();
	if(bflag && fsm_dchg_bstart && except.warn == 0 && except.error == 0)	FSM1_GO_NEXT_RETURN(); 	//״̬1 =>״̬2
														
}


//=======================================================״̬2:STARTUP===============================================================
static CLK clk2_1 = CLK_DEFAULTS;
static CLK clk2_2 = CLK_DEFAULTS;
void fsm_dchg_f2_switch_in(void)
{
	drv_cpld_reset_error();							//�󿪹غ�բ���и�������cpld�������ϡ�
	(void)clk_Bjudge1(&clk2_1, 0, 1000L);
	(void)clk_Bjudge1(&clk2_2, 0, 1000L);
}

void fsm_dchg_f2_switch(void)
{
	if(except.error != 0)								FSM1_GO_STOP();	//״̬X =>״̬0

	if(!clk_Bjudge1(&clk2_1, 1, 1000L))	return;	                        // �ȶ�
	drv_cpld_do_MCB_grid(1);										    // �����࿪��

	if(!clk_Bjudge1(&clk2_2, 1, 1000L))	return;	                        // �ȶ�

								 						FSM1_GO_NEXT_RETURN();		//״̬2 =>״̬3
}



//=======================================================״̬3:BOOST===============================================================

void fsm_dchg_f3_boost_in(void)
{
}

void fsm_dchg_f3_boost(void)
{
	if(except.error != 0)								FSM1_GO_STOP();		//״̬X =>״̬0


	//�����ػ��������ػ�����
	if(drv_ad.udc > charge_udc_sef*(cmm.in7))						FSM1_GO_NEXT_RETURN();		//״̬3 =>״̬4
}


//=======================================================״̬4:���===============================================================

void fsm_dchg_f4_inv_in(void)
{
}

void fsm_dchg_f4_inv(void)
{
	if(except.error != 0)								FSM1_GO_STOP();		//״̬X =>״̬0

	//�����ػ��������ػ�����
	if(!fsm_dchg_bstart)								FSM1_GO_STOP();		//״̬3 =>״̬0
}





void fsm_dchg_func(void)
{
	do
	{
	    fsm_dchg_bwork = (S >= S3) ? 1 : 0;
	    fsm_dchg_binv  = (S >= S4) ? 1 : 0;
		if(except.error != 0 || !fsm_dchg_bstart)		S = S0;				//���Ϻ� =>״̬0

		switch(S)
		{
			case S0:	if(S != S_OLD)	{S_OLD = S;		fsm_dchg_f0_stop_in();	}		fsm_dchg_f0_stop();				break;
			case S1:	if(S != S_OLD)	{S_OLD = S;		fsm_dchg_f1_prechg_in();}		fsm_dchg_f1_prechg();			break;
			case S2:	if(S != S_OLD)	{S_OLD = S;		fsm_dchg_f2_switch_in();}		fsm_dchg_f2_switch();			break;
			case S3:	if(S != S_OLD)	{S_OLD = S;		fsm_dchg_f3_boost_in();  }		fsm_dchg_f3_boost();			break;
			case S4:	if(S != S_OLD)	{S_OLD = S;		fsm_dchg_f4_inv_in();  }		fsm_dchg_f4_inv();			    break;
		
		}
	}while(S != S_OLD);
}
