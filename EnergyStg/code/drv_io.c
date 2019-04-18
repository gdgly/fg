#include "drv.h"
#include "drv_io.h"
#include "base.h"


DRV_IO drv_io = DRV_IO_DEFAULTS;
unsigned drv_io_bstart_ready = 0;


// ������io�Ĵ�����ʼ��;ֻ��һ��,ͣ��ʱ�������³�ʼ��
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-1-17
void drv_io_init1(void)
{
    InitGpio();              	// ����TI��IO�Ĵ���

    EALLOW;
    GpioCtrlRegs.GPACTRL.all  = 0xFFFFFFFF;    // GPIO0-GPIO31 ��������SYSCLKOUT*510
    GpioCtrlRegs.GPBCTRL.all  = 0xFFFFFFFF;    // GPIO32-GPIO63 ��������SYSCLKOUT*510
    GpioCtrlRegs.GPAQSEL1.all = 0xFFFFFFFF;    // GPIO0-GPIO15  ����Ϊ����Ϊ��ͬ��;����ΪGPIO����ϵͳʱ��ͬ��
    GpioCtrlRegs.GPAQSEL2.all = 0xFFFFFFFF;    // GPIO16-GPIO31 ����Ϊ����Ϊ��ͬ��;����ΪGPIO����ϵͳʱ��ͬ��
    GpioCtrlRegs.GPBQSEL1.all = 0xFFFFFFFF;    // GPIO32-GPIO39 ����Ϊ����Ϊ��ͬ��;����ΪGPIO����ϵͳʱ��ͬ��
    GpioCtrlRegs.GPBQSEL2.all = 0xFFFFFFFF;    // GPIO48-GPIO63 ����Ϊ����Ϊ��ͬ��;����ΪGPIO����ϵͳʱ��ͬ��

	GpioDataRegs.GPASET.all    = 0;
	GpioDataRegs.GPBSET.all    = 0;
	GpioDataRegs.GPCSET.all    = 0;
	GpioDataRegs.GPACLEAR.all  = 0;
	GpioDataRegs.GPBCLEAR.all  = 0;
	GpioDataRegs.GPCCLEAR.all  = 0;
	GpioDataRegs.GPATOGGLE.all = 0;
	GpioDataRegs.GPBTOGGLE.all = 0;
	GpioDataRegs.GPCTOGGLE.all = 0;

	// ����DI  id
    GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0;   	// ���ó�����
    GpioCtrlRegs.GPBDIR.bit.GPIO55 = 0;   	// ���ó�����
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 0;   	// ���ó�����
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 0;   	// ���ó�����
    GpioCtrlRegs.GPBDIR.bit.GPIO58 = 0;   	// ���ó�����

	// ����DO	LED
    GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;   	// ���ó���� LED1
    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;   	// ���ó���� LED2
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;   	// ���ó���� LED3
    GpioCtrlRegs.GPBDIR.bit.GPIO62 = 1;   	// ���ó���� LED4
    GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;   	// ���ó���� LED5

    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;   	// ���Ե�
	
	// Ԥ��
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;		// TZ1��TZ2��TZ3��TZ4��Ŀǰû�õ������ó����룬��ȫ��
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 0;
    //GpioDataRegs.GPADAT.bit.GPIO12 = 0;		
	//GpioDataRegs.GPADAT.bit.GPIO13 = 0;
	//GpioDataRegs.GPADAT.bit.GPIO14 = 0;
	//GpioDataRegs.GPADAT.bit.GPIO15 = 0;

    GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;   	// ��̫��оƬ��λ
	GpioDataRegs.GPBSET.bit.GPIO35 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;   	// ���ó���� CPLD�߹�


	// ����DI  ��;��cpld��
	// ����DO ��;��cpld��

    EDIS;

	drv_io_cmm_id();
}


// ��������DSP���ϵ�id��,��Ϊͨ��id
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-3-6

void drv_io_cmm_id(void)
{
    drv_io.cmm_id = (GpioDataRegs.GPBDAT.bit.GPIO54)       	// ADDR1
       			  + (GpioDataRegs.GPBDAT.bit.GPIO55 << 1)  	// ADDR2
       			  + (GpioDataRegs.GPBDAT.bit.GPIO56 << 2)  	// ADDR3
       			  + (GpioDataRegs.GPBDAT.bit.GPIO57 << 3) 	// ADDR4
       			  + (GpioDataRegs.GPBDAT.bit.GPIO58 << 4); 	// ADDR5
}



// ������main()�����У��Ƶ�����
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

void drv_io_led_main(void)
{
	static unsigned bflag = 0;
	static CLK clk0       = CLK_DEFAULTS;

	if (!clk_Bjudgen(&clk0, 1, 2000L)) 	return;			// 1sִ��һ�εĳ�ʼ��

	DRV_IO_OUT_LED1(bflag);
	bflag = ~bflag;
}



// ������zisr_fs()�����У��Ƶ�����
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19
/*
void drv_io_led_fs(void)
{
	static unsigned bflag = 0;
	static CLK clk0       = CLK_DEFAULTS;

	if (clk_Bjudgen(&clk0, 1, 600L)){			// 0.6sִ��һ�εĳ�ʼ��
		DRV_IO_OUT_LED2(bflag);
		bflag = ~bflag;
	}
}
*/


// ����������main()������
void drv_io_main(void)
{
	       unsigned	io_key    = 0					  ;		//io_key����static������io���ĵ�ǰ˲ʱֵ
	static unsigned					io_key_back    = 0;
	static unsigned	io_level  = 0, 	io_level_back  = 0;		//io���ĵ�ƽֵ��������������

	static CLK clk1 = CLK_DEFAULTS;
	
	//��ͣ����
	io_key = DRV_CPLD_DI_POW();

	//1���������������õ���ƽ
	if (io_key != io_key_back){
		clk_Bjudge1(&clk1, 0, 50L);
	}if(io_key==1){
		if(clk_Bjudge1(&clk1, 1, 50L))		io_level = 1;	//����ʱ����Աȹػ�ʱ�䳤Щ�������ܶ����ĳ�ʼ��������ˡ�
	}else{
		if(clk_Bjudge1(&clk1, 1, 350L))   	io_level = 0;
	}
	
	//����
	io_key_back   = io_key;     
	
	//2�����������أ��л����ػ�
	if(io_level_back == 0 && io_level == 1){
		drv_io_bstart_ready = 1 - drv_io_bstart_ready;		//��ʽ����ʱҪ��
	}

	//����
	io_level_back = io_level;
	
	// ��Դ�̵����ű���
//	drv_cpld_do_POWER(drv_io_bstart_ready); 		        //��Դ�̵�������

}

