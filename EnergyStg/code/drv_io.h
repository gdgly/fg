// ������IOͷ�ļ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

#ifndef __DRV_IO_H__
#define __DRV_IO_H__

#include "base.h"

// ��IO�ܽ����ֵ��LED1 �͵�ƽ����,��������ȡ����� 
#define 	DRV_IO_OUT_LED1(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO59 = 1; else GpioDataRegs.GPBSET.bit.GPIO59 = 1;
#define 	DRV_IO_OUT_LED2(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1; else GpioDataRegs.GPBSET.bit.GPIO60 = 1;
#define 	DRV_IO_OUT_LED3(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO61 = 1; else GpioDataRegs.GPBSET.bit.GPIO61 = 1;
#define 	DRV_IO_OUT_LED4(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1; else GpioDataRegs.GPBSET.bit.GPIO48 = 1;
#define 	DRV_IO_OUT_LED5(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1; else GpioDataRegs.GPBSET.bit.GPIO49 = 1;
#define 	DRV_IO_OUT_TEST(bflag)  	if(!bflag)	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1; else GpioDataRegs.GPBSET.bit.GPIO34 = 1;


// ��CPLD���Ź�����DSP���ߣ�CPLD��PWM��CPLD40KHZ���1������ٶȣ�30����û��⵽���ͷ���
#define 	DRV_IO_KICK_CPLD(bflag)  		if(bflag)	GpioDataRegs.GPASET.bit.GPIO24 = 1;	else GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;

// ������Ϊ�����ʱ����õģ���ʽ���õ�
#define		DRV_IO_RESET_ETHERNET(bflag)	if(bflag)	GpioDataRegs.GPBSET.bit.GPIO35 = 1;	else GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;




typedef struct {
                // �ڲ�������
				unsigned bflag_fs;
				CLK clk0;
                // ���룺

                // �����
                unsigned  	cmm_id;  	// 4��satck�Ĺ��ʵȼ�
} DRV_IO;


#define DRV_IO_DEFAULTS {       0, CLK_DEFAULTS, 0	    }

// ��������ָ��
typedef DRV_IO *DRV_IO_HANDLE;




// ��������
void drv_io_init1(void);
void drv_io_cmm_id(void);
void drv_io_led_main(void);
void drv_io_led_fs(void);
void drv_io_main(void);

//------------------------- ȫ�ֱ���������------------------------------//

extern DRV_IO drv_io;
extern unsigned drv_io_bstart_ready;


// ������zisr_fs()�����У��Ƶ�����0.6sִ��һ�εĳ�ʼ��
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19
//  void drv_io_led_fs(void)
#define  DRV_IO_LED_FS()									\
{															\
	if (clk_Bjudgen(&drv_io.clk0, 1, 600L)){			    \
		DRV_IO_OUT_LED2(drv_io.bflag_fs);                   \
		drv_io.bflag_fs = ~drv_io.bflag_fs;                 \
	}														\
}






#endif  // __DRV_IO_H__

