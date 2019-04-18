// ʾ�������������ⲿ���ö������ú���
//    DRV_CPLD drv_cpld = DRV_CPLD_DEFAULTS;
//    extern DRV_CPLD drv_cpld;
//    drv_cpld_init();
//    drv_cpld_read();

// ������ͷ�ļ���
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

#ifndef __DRV_CPLD_H__
#define __DRV_CPLD_H__


typedef struct {
                // �ڲ�������
				unsigned di;
				unsigned error;		

                // ���룺

                // �����
				unsigned version;		//16����
				unsigned id;
				unsigned bMCB_on;
				unsigned bMCB_off;
				unsigned bdischarge, bprecharge;
} DRV_CPLD;

#define DRV_CPLD_DEFAULTS {0, 0, 0, 0, 	0, 0, 0, 0}


// ���������ڲ�����
void drv_cpld_init1(void);
void drv_cpld_read_judge_error(void);
void drv_cpld_reset_error(void);

// д
void drv_cpld_do_cab_fan(unsigned bflag);
void drv_cpld_do_yellow(unsigned bflag);
void drv_cpld_do_blue(unsigned bflag);
void drv_cpld_do_red(unsigned bflag);
void drv_cpld_do_MCB_grid(unsigned bflag);
void drv_cpld_do_prechg_grid(unsigned bflag);
void drv_cpld_do_MCB_bat(unsigned bflag);
void drv_cpld_do_dis(unsigned bflag);
void drv_cpld_do_MCB_gridCAB(unsigned bflag);
void drv_cpld_do_POWER(unsigned bflag);
void drv_cpld_do_prechg_bat(unsigned bflag);
unsigned drv_cpld_laidian(void);

//----------------------------2����ַ����----------------------------
#define XINF6					0x100000		// Ŀǰ�õ���XINF6����10 0000h-20 0000h����1M*16bit

// ��(����)
#define CPLD_TABLE1				(XINF6)

#define CPLD_BUS_TEST1			( *((volatile unsigned int *)(CPLD_TABLE1 + 90)) )		// �������ߣ�������Ӧ����AA55
#define CPLD_BUS_TEST2			( *((volatile unsigned int *)(CPLD_TABLE1 + 165)) )		// �������ߣ�������Ӧ����55AA
#define CPLD_VERSION			( *((volatile unsigned int *)(CPLD_TABLE1 + 2)) )		// CPLD�汾��16���ƣ�0110����01.10
#define CPLD_DI					( *((volatile unsigned int *)(CPLD_TABLE1 + 3)) )		// DIN,��SVG����10��DIN
#define CPLD_ERROR				( *((volatile unsigned int *)(CPLD_TABLE1 + 4)) )		// ���ϱ�־��������λ����һ�����ϱ�־��15-0��


// д(���)                                                                              
#define CPLD_TABLE2				(30 + XINF6)

#define CPLD_RELAY1				( *((volatile unsigned int *)(CPLD_TABLE2 + 1)) )		// J1_DO--1  ���
#define CPLD_RELAY2				( *((volatile unsigned int *)(CPLD_TABLE2 + 2)) )		// J1_DO--2  �Ƶ�
#define CPLD_RELAY3				( *((volatile unsigned int *)(CPLD_TABLE2 + 3)) )		// J1_DO--3  ����
#define CPLD_RELAY4				( *((volatile unsigned int *)(CPLD_TABLE2 + 4)) )		// J1_DO--4  ���
#define CPLD_RELAY5				( *((volatile unsigned int *)(CPLD_TABLE2 + 5)) )		// J1_DO--5  ģ���������������غ�բ
#define CPLD_RELAY6				( *((volatile unsigned int *)(CPLD_TABLE2 + 6)) )		// J1_DO--6  ģ����������Ԥ���
#define CPLD_RELAY7				( *((volatile unsigned int *)(CPLD_TABLE2 + 7)) )		// J1_DO--7  ģ���ز������غ�բ
#define CPLD_RELAY8				( *((volatile unsigned int *)(CPLD_TABLE2 + 8)) )		// J1_DO--8  NC


#define CPLD_MUX				( *((volatile unsigned int *)(CPLD_TABLE2 + 11)))		// �ر�����MUX
#define CPLD_PWM				( *((volatile unsigned int *)(CPLD_TABLE2 + 13)))		// �ر�����PWM��FFB0:�رգ�FFB1����
#define CPLD_RESET_ERROR		( *((volatile unsigned int *)(CPLD_TABLE2 + 14)))		// ���CPLD������ϢFF50+0000
#define CPLD_PWM_GRID			( *((volatile unsigned int *)(CPLD_TABLE2 + 15)))		// �ر���������PWM��FCA0:�رգ�FCA1����
#define CPLD_DIS    			( *((volatile unsigned int *)(CPLD_TABLE2 + 17)))		// 
#define CPLD_RELAY9				( *((volatile unsigned int *)(CPLD_TABLE2 + 18)) )		// J2_DO--9  �������Ӵ���contact
#define CPLD_RELAY10			( *((volatile unsigned int *)(CPLD_TABLE2 + 19)) )		// J2_DO--10  ��Դ�̵���relay
#define CPLD_RELAY11			( *((volatile unsigned int *)(CPLD_TABLE2 + 20)) )		// J2_DO--  ��ز�Ԥ���

//----------------------------3���꺯������----------------------------

// �ⲿ�õ��ĺ꺯������

// ��
#define DRV_CPLD_DI_POW()		(	BCHECK16_BIT(drv_cpld.di, 0)	)				//	J1_DI--1  ������ť
#define DRV_CPLD_DI_MPS()		(	BCHECK16_BIT(drv_cpld.di, 1)	)				//	J1_DI--2  ����ָʾ

	// drv_cpld.error��λ����
	// 0λ��A���������ϣ�1�������
	// 1λ��B���������ϣ�1�������
	// 2λ��C���������ϣ�1�������
	// 3λ��������UVW�������ϣ�1�������
	// 
	// 4λ��A��������ϣ�1�������
	// 5λ��B��������ϣ�1�������
	// 6λ��C��������ϣ�1�������
	// 7λ��������UVW�������ϣ�1�������
	// 
	// 8λ�� 
	// 9λ�� 
	// 10λ��
	// 11λ��
	// 
	// 12λ��ֱ����ѹ1���ϣ�1�������
	// 13λ��CPLD���Ź���1�����Ź��������DSP����CPLD���Ź�(io.h)��CPLD��PWM��CPLD40KHZ���1������ٶȣ�30����û��⵽���ͷ����ˡ�ĿǰDSPû�д���
	// 14λ������
	// 15λ������
#define DRV_CPLD_A_DRV_ERROR()	(drv_cpld.error & 0x0001)	// �������� bit0
#define DRV_CPLD_B_DRV_ERROR()	(drv_cpld.error & 0x0002)	// �������� bit1
#define DRV_CPLD_C_DRV_ERROR()	(drv_cpld.error & 0x0004)	// �������� bit2
#define DRV_CPLD_D_DRV_ERROR()	(drv_cpld.error & 0x0008)	// �������� bit3
#define DRV_CPLD_A_OUT_OI()		(drv_cpld.error & 0x0010)	// �������� bit4
#define DRV_CPLD_B_OUT_OI()		(drv_cpld.error & 0x0020)	// �������� bit5
#define DRV_CPLD_C_OUT_OI()		(drv_cpld.error & 0x0040)	// �������� bit6
#define DRV_CPLD_D_OUT_OI()		(drv_cpld.error & 0x0080)	// �������� bit7
#define DRV_CPLD_A_OT()		    (drv_cpld.error & 0x0100)	// ���¹��� bit8
#define DRV_CPLD_B_OT()		    (drv_cpld.error & 0x0200)	// ���¹��� bit9
#define DRV_CPLD_C_OT()		    (drv_cpld.error & 0x0400)	// ���¹��� bit10
#define DRV_CPLD_D_OT()		    (drv_cpld.error & 0x0800)	// ���¹��� bit11
#define DRV_CPLD_ODC()		    (drv_cpld.error & 0x1000)	// ֱ������ bit12
#define DRV_CPLD_DSP()    		(drv_cpld.error & 0x2000)	// ֱ������ bit13

// д                                   
#define DRV_CPLD_DO_START_PWM() 		(CPLD_PWM=0xFFB1		)	// ������ز�PWM
#define DRV_CPLD_DO_STOP_PWM()			(CPLD_PWM=0xFFB0		)	// �رյ�ز�PWM
                                                              
#define DRV_CPLD_DO_START_GRID_PWM()	(CPLD_PWM_GRID=0xFCA1	)	// ���������PWM
#define DRV_CPLD_DO_STOP_GRID_PWM()		(CPLD_PWM_GRID=0xFCA0	)	// �ر������PWM


// ����������AD��ѡ�񿪹�,DRV_IO_MUX_AD1�ǵ�λ.		��IO�����ļ��ĳ�ʼ��ʱ��IO12��IO13��IO14��IO15,��ʼ�������Ϊ0��
#define	DRV_CPLD_MUX(data)				(CPLD_MUX = data		)


extern DRV_CPLD drv_cpld;

#endif  // __DRV_CPLD_H__

