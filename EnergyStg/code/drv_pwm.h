// ʾ�������������ⲿ���ö������ú���
// drv_pwm_init()		pwm�Ĵ�����ʼ��;	ͣ��ʱ�������³�ʼ��
// drv_pwm_start()		pwm����
// drv_pwm_stop()		pwmֹͣ
// drv_pwm_isr_ack()	PWM1�ж�Ӧ��

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-2-28


#ifndef __DRV_PWM_H__
#define __DRV_PWM_H__


//------------------------- �궨����---------------------------------//
#define   INV_SWITH_FREQ	    10000			// 10K����Ƶ��
#define   EXC_SWITH_FREQ	    25000			// 25K����Ƶ��

#define   DRV_PWM_FS_3KHZ   	25000			// 150MHZ/3KHZ/2  = 50K/2 = 25000���мǣ���Ϊ�ǶԳ�PWM������ʱ�ӱ������2
#define   DRV_PWM_DBR       	300             // ��������ʱ������ʱ��  6.667*300 = 2us
#define   DRV_PWM_DBF       	300             // ��������ʱ���½�ʱ��  6.667*300 = 2us
#define   DRV_PWM_DBP       	2               // ������������ �ߵ�ƽ��Ч����(AHC)
#define   DRV_PWM_CMPA      	0				// PWM�Ƚ���A�ĳ�ʼֵ
#define   DRV_PWM_CMPB      	0				// PWM�Ƚ���B�ĳ�ʼֵ

//------------------------- ����������---------------------------------//

typedef struct {
                // �ڲ�������
   				unsigned period_2;	// �ز����ڵ�1/2��
				unsigned period_4;	// �ز����ڵ�1/4�����PWMʱpwm_period_4 * (1+aduty),��Ϊ(1+aduty)��Χ��0~1,����Ҫ1/4��������
				// ���
				unsigned fs;	// �ز�Ƶ��,
				float ts;
} DRV_PWM;

#define DRV_PWM_DEFAULTS {    DRV_PWM_FS_3KHZ>>1,	DRV_PWM_FS_3KHZ>>2,  1201.0, 1.0/1201.0}	//����ĳ�1201HZ���������ܵ�һ�θ�ֵʱ��ȥ��

// ��������ָ��
typedef DRV_PWM *DRV_PWM_HANDLE;

extern DRV_PWM  drv_pwm         ; // �ж�ʱ��
extern DRV_PWM 	drv_pwm_inv     ; // ���
extern DRV_PWM 	drv_pwm_grid    ; // 


//----------------------- �ڲ�����������--------------------------------//

void drv_pwm_init1(void);
void drv_pwm_grid_start(void);
void drv_pwm_grid_stop(void);
void drv_pwm_startA(unsigned buck);
void drv_pwm_stopA(void);
void drv_pwm_stop(void);
void drv_pwm_buck_duty(unsigned buck, float duty);


// ������PWM1�ж�Ӧ��
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19
// void drv_pwm_isr_ack()

//#define DRV_PWM_ISR_ACK()		{EPwm1Regs.ETCLR.bit.INT = 1;} // ���ETCLR[INT]��־λ����ʹ�����ж���������
#define DRV_PWM_ISR_ACK()		{EPwm5Regs.ETCLR.bit.INT = 1;} // ���ETCLR[INT]��־λ����ʹ�����ж���������


// ����������PWMʱ��������PWMģ�鿪ʼ����; ������ѭ��ǰ�������ж�ǰһ�䣬����PWMģ����ǰ��ʼ���������������ж�
#define	DRV_PWM_CLK_START()						\
{                                               \
	EALLOW;                                     \
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;      \
    EDIS;                                       \
}


#define DRV_PWM_ZD_START()		 DRV_CPLD_DO_START_N_PWM()      // �ƶ��Ŀ���


// ������pwm���������xdutyȡֵ��Χ[-1 1]
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19
#define DRV_PWM_BAT(aduty, bduty, cduty)											    \
{																					    \
		SAT_VOID(aduty, 1.0, -1.0);                                                     \
		SAT_VOID(bduty, 1.0, -1.0);                                                     \
		SAT_VOID(cduty, 1.0, -1.0);                                                     \
		EPwm1Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_inv.period_4 * (1.0+aduty));	\
		EPwm2Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_inv.period_4 * (1.0+bduty));	\
		EPwm3Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_inv.period_4 * (1.0+cduty));	\
}


                                                                                        
// ����                                                                 
#define DRV_PWM_GRID(aduty, bduty, cduty)										    \
{																					    \
		SAT_VOID(aduty, 0.99, -0.99);                                                   \
		SAT_VOID(bduty, 0.99, -0.99);                                                   \
		SAT_VOID(cduty, 0.99, -0.99);                                      			    \
		EPwm4Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_grid.period_4 * (1.0+aduty)); \
		EPwm5Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_grid.period_4 * (1.0+bduty));	\
		EPwm6Regs.CMPA.half.CMPA = (unsigned)(1+drv_pwm_grid.period_4 * (1.0+cduty));	\
}

#endif  // __DRV_PWM_H__

