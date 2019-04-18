#include "drv.h"
#include "drv_dog.h"

unsigned drv_dog_bflag = 0;


// ������ʹ�ܿ��Ź�
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-7-11

void drv_dog_enable(void)
{
    EALLOW;
	SysCtrlRegs.WDCR = 0x00E8;
	// bit 15-8      0's:    reserved
	// bit 7         1:      WDFLAG, write 1 to clear
	// bit 6         1:      WDDIS, 1=disable WD
	// bit 5-3       101:    WDCHK, WD check bits, always write as 101b
	// bit 2-0       000:    WDPS, WD prescale bits, 000: WDCLK=OSCCLK/512/1
	SysCtrlRegs.SCSR = 0x0000;
	// bit 15-3      0's:    reserved
	// bit 2         0:      WDINTS, WD interrupt status bit (read-only)
	// bit 1         0:      WDENINT, 0=WD causes reset, 1=WD causes WDINT
	// bit 0         0:      WDOVERRIDE, write 1 to disable disabling of the WD (clear-only)
    EDIS;
}


// �������رտ��Ź�
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-7-11

void drv_dog_disable(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}


// �������߹���������CPLD������,��ֹ�����������ͳ����ܷ�,��CPLD��PWM��CPLD40KHZ���1������ٶȣ�30����û��⵽���ͷ���;
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-7-11

/*
void drv_dog_kick(void)
{
	drv_dog_bflag = ~drv_dog_bflag;
	DRV_IO_KICK_CPLD(drv_dog_bflag);

	EALLOW;    
	SysCtrlRegs.WDKEY = 0x0055;    
	SysCtrlRegs.WDKEY = 0x00AA;    
	EDIS;
	
}
*/
