// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

#ifndef __DRV_DOG_H__
#define __DRV_DOG_H__

void drv_dog_enable(void);
void drv_dog_disable(void);
//void drv_dog_kick(void);

extern unsigned drv_dog_bflag;

// �������߹���������CPLD������,��ֹ�����������ͳ����ܷ�,��CPLD��PWM��CPLD40KHZ���1������ٶȣ�30����û��⵽���ͷ���;
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-7-11
// void drv_dog_kick(void)
#define DRV_DOG_KICK()						\
{											\
	drv_dog_bflag = ~drv_dog_bflag;         \
	DRV_IO_KICK_CPLD(drv_dog_bflag);        \
                                            \
	EALLOW;                                 \
	SysCtrlRegs.WDKEY = 0x0055;             \
	SysCtrlRegs.WDKEY = 0x00AA;             \
	EDIS;									\
}

#endif  // __DRV_DOG_H__
