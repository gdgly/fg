// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

#ifndef __DRV_ISR_H__
#define __DRV_ISR_H__

void drv_isr_init1(void (*pf_pwm)(void));

// ������PIE��3���ж�Ӧ��,������ePWM�ж�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19
// void drv_isr_ack_int3(void)
#define DRV_ISR_ACK_INT3()		(PieCtrlRegs.PIEACK.all = PIEACK_GROUP3)

// ������PIE��9���ж�Ӧ��,������scib�ж�
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-6-27
// void drv_isr_ack_int9(void)
//#define DRV_ISR_ACK_INT9()	(PieCtrlRegs.PIEACK.all = PIEACK_GROUP9)


#endif  // __DRV_ISR_H__

