#include "apply.h"
#include "drv_timer.h"

//DRV_TIMER drv_timer=DRV_TIMER_DEFAULTS;


// ��������ʱ����ʼ��
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2014-05-08

void drv_timer_init1(void)
{
	CpuTimer0.RegsAddr = &CpuTimer0Regs;

	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;	//����

	CpuTimer0Regs.TPR.bit.TDDR  	= 0x98;	//Ԥ������Ϊ15000(0x3A98),Ŀ����TIM���0.1ms�ļ���ֵ
	CpuTimer0Regs.TPRH.bit.TDDRH 	= 0x3A;

	CpuTimer0Regs.TCR.bit.TSS = 1;

	CpuTimer0Regs.TCR.bit.TRB = 1;

	StartCpuTimer0();		//��ʱ����ʼ����
}

