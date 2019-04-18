#include "drv_cpld.h"
#include "base.h"
#include "drv.h"

DRV_CPLD drv_cpld = DRV_CPLD_DEFAULTS;

// ������
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

void drv_cpld_init1(void)
{
	drv_cpld_reset_error();
	DRV_CPLD_DO_STOP_PWM();
	DRV_CPLD_DO_STOP_GRID_PWM();

	EXCEPT_JUDGE_SET(!(CPLD_BUS_TEST1 == 0xAA55 && CPLD_BUS_TEST2 == 0x55AA), ERROR_DRV_CPLD_BUS_TEST);
	drv_cpld.version = CPLD_VERSION;
}


// ��������cpld
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

void drv_cpld_read_judge_error(void)
{
	drv_cpld.error = CPLD_ERROR;
	drv_cpld.di    = CPLD_DI;

	EXCEPT_JUDGE_SET(DRV_CPLD_A_DRV_ERROR() ,	 ERROR_DRV_CPLD_A_STACK_DRV   );
	EXCEPT_JUDGE_SET(DRV_CPLD_B_DRV_ERROR() ,	 ERROR_DRV_CPLD_B_STACK_DRV   );
	EXCEPT_JUDGE_SET(DRV_CPLD_C_DRV_ERROR() ,	 ERROR_DRV_CPLD_C_STACK_DRV   );
	EXCEPT_JUDGE_SET(DRV_CPLD_D_DRV_ERROR() ,	 ERROR_DRV_CPLD_D_STACK_DRV   );
//	EXCEPT_JUDGE_SET(DRV_CPLD_A_OUT_OI()	,	 ERROR_DRV_CPLD_A_OUT_OI      );
//	EXCEPT_JUDGE_SET(DRV_CPLD_B_OUT_OI()	,	 ERROR_DRV_CPLD_B_OUT_OI      );
//	EXCEPT_JUDGE_SET(DRV_CPLD_C_OUT_OI()	,	 ERROR_DRV_CPLD_C_OUT_OI      );
//	EXCEPT_JUDGE_SET(DRV_CPLD_D_OUT_OI()	,	 ERROR_DRV_CPLD_D_OUT_OI      );
	EXCEPT_JUDGE_SET(DRV_CPLD_A_OT()		,    ERROR_DRV_CPLD_A_OT          );
	EXCEPT_JUDGE_SET(DRV_CPLD_B_OT()		,    ERROR_DRV_CPLD_B_OT          );
	EXCEPT_JUDGE_SET(DRV_CPLD_C_OT()		,    ERROR_DRV_CPLD_C_OT          );
	EXCEPT_JUDGE_SET(DRV_CPLD_D_OT()		,    ERROR_DRV_CPLD_D_OT          );
	EXCEPT_JUDGE_SET(DRV_CPLD_ODC()		    ,    ERROR_DRV_CPLD_ODC           );

}

void drv_cpld_reset_error(void)
{
	CPLD_RESET_ERROR = 0xFF50;			// ���CPLD������Ϣ
	CPLD_RESET_ERROR = 0x55AA;
}


//�����ú�����ʽ����Ϊrelay��������ʱ�����Ե����к�����������Ҫ����ָ�뵼�����º�����

void drv_cpld_do_cab_fan(unsigned bflag)
{
	if(bflag)	CPLD_RELAY1 = 0xBB21;	else	CPLD_RELAY1 = 0xBB20;		// DO--1  ���
}

void drv_cpld_do_yellow(unsigned bflag)
{
	if(bflag)	CPLD_RELAY2 = 0xBB21;	else	CPLD_RELAY2 = 0xBB20;		// DO--2  �Ƶ�
}

void drv_cpld_do_blue(unsigned bflag)
{
	if(bflag)	CPLD_RELAY3 = 0xBB21;	else	CPLD_RELAY3 = 0xBB20;		// DO--3  ����
}

void drv_cpld_do_red(unsigned bflag)
{
	if(bflag)	CPLD_RELAY4 = 0xBB21;	else	CPLD_RELAY4 = 0xBB20;		// DO--4  ���
}

void drv_cpld_do_MCB_grid(unsigned bflag)
{
	if(bflag)	CPLD_RELAY5 = 0xBB21;	else	CPLD_RELAY5 = 0xBB20;		// DO--5  ģ���������������غ�բ
}

void drv_cpld_do_prechg_grid(unsigned bflag)
{
	if(bflag)	CPLD_RELAY6 = 0xBB21;	else	CPLD_RELAY6 = 0xBB20;		// DO--6  ģ����������Ԥ���
}

void drv_cpld_do_MCB_bat(unsigned bflag)
{
	if(bflag)	CPLD_RELAY7 = 0xBB21;	else	CPLD_RELAY7 = 0xBB20;		// DO--7  ģ���ز������غ�բ
}

void drv_cpld_do_dis(unsigned bflag)
{
	if(bflag)	CPLD_DIS    = 0xBB21;	else	CPLD_DIS    = 0xBB20;		// �ŵ�
}

void drv_cpld_do_MCB_gridCAB(unsigned bflag)
{
	if(bflag)	CPLD_RELAY9    = 0xBB21;	else	CPLD_RELAY9    = 0xBB20;		// �������Ӵ���
}

void drv_cpld_do_POWER(unsigned bflag)
{
	if(bflag)	CPLD_RELAY10    = 0xBB21;	else	CPLD_RELAY10    = 0xBB20;		// ���Ƶ�Ӵ���
}

void drv_cpld_do_prechg_bat(unsigned bflag)
{
	if(bflag)	CPLD_RELAY11 = 0xBB21;	else	CPLD_RELAY11 = 0xBB20;		        // DO--6  ģ����������Ԥ���
}

// �жϲ��ͻ��Ƿ��е� �������Ҫ��������ȷ��
unsigned drv_cpld_laidian(void)
{
    return (!DRV_CPLD_DI_MPS());	// ����ͼ�з���
}


