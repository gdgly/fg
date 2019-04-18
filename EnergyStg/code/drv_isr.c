#include "drv.h"
#include "drv_isr.h"


// �ж��������ģ������úã�	IERx=1����INTM=0����PIEIERx=1��������Ժ������䣩
// �����жϣ��Զ���ɣ�		IERx=0�أ�INTM=1�أ�PIEACKx=1�أ�IFR=0��PIEIFRx.y=0
// �˳��жϣ��Զ���ɣ�		IERx=1����INTM=0����PIEACKx=0��

// �ж���������뿪�����������жϵĻ������Ƽ����˳��жϵ����
// ������˵������������ǲ�ͬ��������жϵĻ���ֻҪ����һ��INTM=0�����ɣ�����ǰһ����


// �������жϳ�ʼ�������ø����ж�;ֻ��һ��,ͣ��ʱ�������³�ʼ��
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

void drv_isr_init1(void (*pf_pwm)(void))
{
	// ȫ���жϼĴ���
	IER = 0x0000;									// ȫ���ر�IER
	IFR = 0x0000;   								// ȫ�����IFR

   	EALLOW;  										// �ƺ�TINT0��PIE������Ҫ����䡣 TINT0 is needed to write to EALLOW protected registers
   	
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;				// ʹ��PIE
	
	PieCtrlRegs.PIEACK.all = 0xFFFF;				// д1������е�PIE�ж����PIEACK,ʹ�����Խ����ж�
   	
//	// INT1	 ��ʱ��CLK���ж�TINT0ʹ��
//	PieVectTable.TINT0 = pf_time;					// �жϺ�����
//	PieCtrlRegs.PIEIFR1.bit.INTx7 = 1;				// ���PIEIFR
//	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;				// ʹ��PIEIER
//	IER |= M_INT1;									// ʹ��IER


	// INT3
	// EPWM1_INT���ж�ʹ��	
	//PWM1�ж� <�л�> PWM5�жϡ�
//PWM�ж�
//	PieVectTable.EPWM1_INT = pf_pwm;
//	PieCtrlRegs.PIEIFR3.bit.INTx1 = 0;				// ���PIEIFR 
//	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

//PWM5�ж�
	PieVectTable.EPWM5_INT = pf_pwm;
	PieCtrlRegs.PIEIFR3.bit.INTx5 = 0;				// ���PIEIFR 
	PieCtrlRegs.PIEIER3.bit.INTx5 = 1;
	
	IER |= M_INT3;

//	// INT13   	// ��ʱ��1���ж�TINT1ʹ��   	
//	PieVectTable.XINT13 = pf_time;				// û��PIEIER
//	IER |= M_INT13;
	
   	// INT9
   	// SCI���ͺͽ����ж�ʹ��   	
    //PieVectTable.SCIRXINTB  =  &drv_scib_receive;
    //PieVectTable.SCITXINTB  =  pf_scitx;
   	//PieCtrlRegs.PIEIFR9.bit.INTx3 = 1;			// ���PIEIFR 
    //PieCtrlRegs.PIEIER9.bit.INTx3 = 1;       		// PIE Group 9,  INTx3
   	//PieCtrlRegs.PIEIFR9.bit.INTx4 = 1;			// ���PIEIFR 
    //PieCtrlRegs.PIEIER9.bit.INTx4 = 1;       		// PIE Group 9,  INTx4
    //IER |=  M_INT9;                            	// ʹ�ܵ�9��PIE�ж� CPU INT

	EDIS; 
}

/*
// ������PIE��1���ж�Ӧ��,������TINT0(CPU-��ʱ��0)�ж�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

void drv_isr_ack_int1(void)
{
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



// ������PIE��3���ж�Ӧ��,������ePWM�ж�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

void drv_isr_ack_int3(void)
{
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

*/
