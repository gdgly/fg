#include "drv.h"
#include "drv_pwm.h"

DRV_PWM     drv_pwm             = DRV_PWM_DEFAULTS; //  �ж�ʱ�䣬Ŀǰ��ͬinv��ʱ��

DRV_PWM 	drv_pwm_inv 		= DRV_PWM_DEFAULTS; // ���
DRV_PWM 	drv_pwm_grid 		= DRV_PWM_DEFAULTS; // ����, 

#define drv_pwm123x_init(EPwmxRegs)																									\
{                                                                                                                                   \
	EPwmxRegs.TBCTR = 0x0001;                                                                                                       \
   	EPwmxRegs.TBPHS.half.TBPHS = 0;						/* ͬ������ص���λ�Ĵ���ֵ*/                                               \
   	                                                                                                                                \
   	EPwmxRegs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;		/* =0x3	��ֹSYNCO���*/   	                                                \
	EPwmxRegs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;     /* �����ݼ�����ģʽ*/                                                       \
	/* ����ʱ��  */                                                                                                                 \
	EPwmxRegs.TBCTL.bit.CLKDIV    = 0;					/* TBCLK = SYSCLKOUT / (HSPCLKDIV �� CLKDIV) = SYSCLKOUT = 150MHz*/         \
	EPwmxRegs.TBCTL.bit.HSPCLKDIV = 0;                                                                                              \
	                                                                                                                                \
	/* ����ӳ��ģʽ  */                                                                                                             \
    EPwmxRegs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;			/* 0 : ���ó�ӳ��ģʽ,��Ϊ˫������������е�д����ͨ��CPU����ӳ��Ĵ���*/   \
    EPwmxRegs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  		/* 00: ��CTR=0ʱ����*/                                                      \
    EPwmxRegs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                                                                                     \
    EPwmxRegs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;  		/* Load on Zero*/                                                           \
                                                                                                                                    \
	/* ���ö����޶��Ĵ���AQ����ƽ����Ϊ��CMPA = ���ʱ��PWM��� = 1�� CMPA = 0ʱ��PWM��� = 0*/    		                            \
    EPwmxRegs.AQCTLA.bit.CAU = AQ_SET;            		/* ����������ʱ�����Ƚ���Aһ��ʱ����ʱPWMxA=1*/                             \
    EPwmxRegs.AQCTLA.bit.CAD = AQ_CLEAR;           		/* �������ݼ�ʱ�����Ƚ���Aһ��ʱ����ʱPWMxA=0*/                             \
    EPwmxRegs.AQCTLB.bit.CBU = AQ_SET;            	/* ����������ʱ�����Ƚ���Aһ��ʱ����ʱPWMxB=0*/                             \
    EPwmxRegs.AQCTLB.bit.CBD = AQ_CLEAR;           		/* �������ݼ�ʱ�����Ƚ���Aһ��ʱ����ʱPWMxB=1*/                             \
	/* EPwmxRegs.AQCTLA.bit.CAU = AQ_CLEAR;            	 ����������ʱ�����Ƚ���Aһ��ʱ����ʱPWMxA=0*/                             \
    /* EPwmxRegs.AQCTLA.bit.CAD = AQ_SET;           	 �������ݼ�ʱ�����Ƚ���Aһ��ʱ����ʱPWMxA=1*/                             \
    /* EPwmxRegs.AQCTLB.bit.CAU = AQ_SET;           	 ����������ʱ�����Ƚ���Aһ��ʱ����ʱPWMxB=1*/                             \
    /* EPwmxRegs.AQCTLB.bit.CAD = AQ_CLEAR;        		 �������ݼ�ʱ�����Ƚ���Aһ��ʱ����ʱPWMxB=0*/                             \
	EPwmxRegs.AQSFRC.all 	 = 0;                                                                                                   \
	/*  ����������PWM���� �ߵ�ƽ��Ч����(AHC)				*/                                                                      \
	/*                                                      */                                                                      \
	/*  �ߣߣߣߣ������� �ߣߣߣߣ������� �ߣߣߣߣ�  EPWMA */                                                                      \
	/*                                                      */                                                                      \
	/* �������� �ߣߣߣߣ������� �ߣߣߣߣ���������   EPWMB */                                                                      \
	/*                                                      */                                                                      \
                                                                                                                                    \
    EPwmxRegs.CMPA.half.CMPA = DRV_PWM_CMPA;                                                                                        \
    EPwmxRegs.CMPB           = DRV_PWM_CMPB;                                                                                        \
                                                                                                                                    \
	/* ��������*/                                                                                                                   \
   /*EPwmxRegs.DBRED          	 = DRV_PWM_DBR;      */                                                                                  \
   /*EPwmxRegs.DBFED          	 = DRV_PWM_DBF;        */                                                                                 \
 	/*EPwmxRegs.DBCTL.bit.POLSEL   = DRV_PWM_DBP;		 	�������� �ߵ�ƽ��Ч����(AHC)    */                                       \
    /*	  EPwmxRegs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  ʹ������        */                                                      \
                                                                                                                                    \
	EPwmxRegs.PCCTL.all 		 = 0;                                                                                               \
                                                                                                                                    \
	EPwmxRegs.TZSEL.all 		 = 0;                                                                                               \
	EPwmxRegs.TZCTL.all 		 = 0;                                                                                               \
	EPwmxRegs.TZEINT.all 		 = 0;                                                                                               \
	EPwmxRegs.TZCLR.all 		 = 0;                                                                                               \
	EPwmxRegs.TZFRC.all 		 = 0;                                                                                               \
                                                                                                                                    \
}


// ������pwm�Ĵ�����ʼ��, ������һ�κ궨��������, ʵ�ּ���pwm��ʼ��ֻ������һ������
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-6-26

#define drv_pwmx_init(EPwmxRegs)																									\
{                                                                                                                                   \
	EPwmxRegs.TBCTR = 0x0001;                                                                                                       \
   	EPwmxRegs.TBPHS.half.TBPHS = 0;						/* ͬ������ص���λ�Ĵ���ֵ*/                                               \
   	                                                                                                                                \
   	EPwmxRegs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;		/* =0x3	��ֹSYNCO���*/   	                                                \
	EPwmxRegs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;     /* �����ݼ�����ģʽ*/                                                       \
	/* ����ʱ��  */                                                                                                                 \
	EPwmxRegs.TBCTL.bit.CLKDIV    = 0;					/* TBCLK = SYSCLKOUT / (HSPCLKDIV �� CLKDIV) = SYSCLKOUT = 150MHz*/         \
	EPwmxRegs.TBCTL.bit.HSPCLKDIV = 0;                                                                                              \
	                                                                                                                                \
	/* ����ӳ��ģʽ  */                                                                                                             \
    EPwmxRegs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;			/* 0 : ���ó�ӳ��ģʽ,��Ϊ˫������������е�д����ͨ��CPU����ӳ��Ĵ���*/   \
    EPwmxRegs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  		/* 00: ��CTR=0ʱ����*/                                                      \
    EPwmxRegs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                                                                                     \
    EPwmxRegs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;  		/* Load on Zero*/                                                           \
                                                                                                                                    \
	/* ���ö����޶��Ĵ���AQ����ƽ����Ϊ��CMPA = ���ʱ��PWM��� = 1�� CMPA = 0ʱ��PWM��� = 0*/    		                            \
    EPwmxRegs.AQCTLA.bit.CAU = AQ_SET;            		/* ����������ʱ�����Ƚ���Aһ��ʱ����ʱPWMxA=1*/                             \
    EPwmxRegs.AQCTLA.bit.CAD = AQ_CLEAR;           		/* �������ݼ�ʱ�����Ƚ���Aһ��ʱ����ʱPWMxA=0*/                             \
    EPwmxRegs.AQCTLB.bit.CAU = AQ_CLEAR;            	/* ����������ʱ�����Ƚ���Aһ��ʱ����ʱPWMxB=0*/                             \
    EPwmxRegs.AQCTLB.bit.CAD = AQ_SET;           		/* �������ݼ�ʱ�����Ƚ���Aһ��ʱ����ʱPWMxB=1*/                             \
	/* EPwmxRegs.AQCTLA.bit.CAU = AQ_CLEAR;            	 ����������ʱ�����Ƚ���Aһ��ʱ����ʱPWMxA=0*/                             \
    /* EPwmxRegs.AQCTLA.bit.CAD = AQ_SET;           	 �������ݼ�ʱ�����Ƚ���Aһ��ʱ����ʱPWMxA=1*/                             \
    /* EPwmxRegs.AQCTLB.bit.CAU = AQ_SET;           	 ����������ʱ�����Ƚ���Aһ��ʱ����ʱPWMxB=1*/                             \
    /* EPwmxRegs.AQCTLB.bit.CAD = AQ_CLEAR;        		 �������ݼ�ʱ�����Ƚ���Aһ��ʱ����ʱPWMxB=0*/                             \
	EPwmxRegs.AQSFRC.all 	 = 0;                                                                                                   \
	/*  ����������PWM���� �ߵ�ƽ��Ч����(AHC)				*/                                                                      \
	/*                                                      */                                                                      \
	/*  �ߣߣߣߣ������� �ߣߣߣߣ������� �ߣߣߣߣ�  EPWMA */                                                                      \
	/*                                                      */                                                                      \
	/* �������� �ߣߣߣߣ������� �ߣߣߣߣ���������   EPWMB */                                                                      \
	/*                                                      */                                                                      \
                                                                                                                                    \
    EPwmxRegs.CMPA.half.CMPA = DRV_PWM_CMPA;                                                                                        \
    EPwmxRegs.CMPB           = DRV_PWM_CMPB;                                                                                        \
                                                                                                                                    \
	/* ��������*/                                                                                                                   \
    EPwmxRegs.DBRED          	 = DRV_PWM_DBR;                                                                                     \
    EPwmxRegs.DBFED          	 = DRV_PWM_DBF;                                                                                     \
	EPwmxRegs.DBCTL.bit.POLSEL   = DRV_PWM_DBP;			/* �������� �ߵ�ƽ��Ч����(AHC)   */                                        \
    EPwmxRegs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  	/* ʹ������                       */                                        \
                                                                                                                                    \
	EPwmxRegs.PCCTL.all 		 = 0;                                                                                               \
                                                                                                                                    \
	EPwmxRegs.TZSEL.all 		 = 0;                                                                                               \
	EPwmxRegs.TZCTL.all 		 = 0;                                                                                               \
	EPwmxRegs.TZEINT.all 		 = 0;                                                                                               \
	EPwmxRegs.TZCLR.all 		 = 0;                                                                                               \
	EPwmxRegs.TZFRC.all 		 = 0;                                                                                               \
                                                                                                                                    \
}


// ����Ƶ�����á�	����Ƶ�ʲ�����������������ˣ���Ϊ�����ɺ꺯������Ĵ���
#define	DRV_PWM_PERIOD(EPwmxRegs, drv_pwm, fs)   {drv_pwm_period_inner(&drv_pwm, fs);	EPwmxRegs.TBPRD = drv_pwm.period_2;}
void drv_pwm_period_inner(DRV_PWM *p, unsigned long fs)
{
   	if (p->fs == fs)	return;	// ֱ�ӷ���
SECURE_EXPRESS(
   	p->fs = fs;
   	p->ts = 1.0/fs;
   	p->period_2 = (unsigned)(75000000L * p->ts);
   	p->period_4 = (unsigned)(p->period_2 * 0.5);
)
}


// ������pwm��ʼ��
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-1-30

void drv_pwm_init1(void)
{

	InitEPwm1Gpio();									// ����TI����ʼ��EPWM�Ĵ���
   	InitEPwm2Gpio();			
   	InitEPwm3Gpio();			
	InitEPwm4Gpio();			
	InitEPwm5Gpio();			
	InitEPwm6Gpio();			

    EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      		// Stop all the TB clocks
    EDIS;

    drv_pwm123x_init(EPwm1Regs);
    drv_pwm123x_init(EPwm2Regs);
    drv_pwm123x_init(EPwm3Regs);
    drv_pwmx_init(EPwm4Regs); 
	drv_pwmx_init(EPwm5Regs); 							// 
	drv_pwmx_init(EPwm6Regs); 							// 

   	// ����PWM4����AD
	//EPwm5Regs.TBCTR = 376;		                        // ����������,��ǰ5us,10K����Ƶ��ʱ��һ��������7500����5us����375
	EPwm5Regs.ETSEL.bit.SOCAEN  = 1;        		    // ʹ��A���SOC
	EPwm5Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;		    // ��������=0�¼�ʱ������ADCת��
	EPwm5Regs.ETPS.bit.SOCAPRD  = 1;        		    // ��һ���¼������Ͳ���SOC���壬ȥ����ADCת��
                                              
	EPwm5Regs.ETSEL.bit.SOCBEN  = 1;        		    // ʹ��B���SOC
	EPwm5Regs.ETSEL.bit.SOCBSEL = ET_CTR_ZERO;		    // ��������=0�¼�ʱ������ADCת��
	EPwm5Regs.ETPS.bit.SOCBPRD  = 1;        		    // ��һ���¼������Ͳ���SOC���壬ȥ����ADCת��
                                                        

    // �ж�
	EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     		// ʹ�ܼ�����=0�¼���
	EPwm5Regs.ETSEL.bit.INTEN  = 1;  					// ʹ���жϣ�Enable INT
	EPwm5Regs.ETPS.bit.INTPRD  = ET_1ST;           		// �ڵ�һ���¼�ʱ���ж�
	EPwm5Regs.ETFRC.all		   = 0;
    
    EPwm1Regs.AQCSFRC.all = 5;           				// ������A
    EPwm2Regs.AQCSFRC.all = 5;           				// ������B
    EPwm3Regs.AQCSFRC.all = 5;           				// ������C
    EPwm4Regs.AQCSFRC.all = 0; 			 				// ������A
    EPwm5Regs.AQCSFRC.all = 0;           				// ������B
    EPwm6Regs.AQCSFRC.all = 0; 			 				// ������C
    
	drv_pwm_period_inner(&drv_pwm, 		INV_SWITH_FREQ);// �ж�ʱ�䣬

	DRV_PWM_PERIOD(EPwm1Regs, drv_pwm_inv, 			INV_SWITH_FREQ);		// ���A��
 	DRV_PWM_PERIOD(EPwm2Regs, drv_pwm_inv, 			INV_SWITH_FREQ);		// ���B��
	DRV_PWM_PERIOD(EPwm3Regs, drv_pwm_inv, 	        INV_SWITH_FREQ);		// ���C��
 	DRV_PWM_PERIOD(EPwm4Regs, drv_pwm_grid, 		INV_SWITH_FREQ);		// ����A��
	DRV_PWM_PERIOD(EPwm5Regs, drv_pwm_grid,		    INV_SWITH_FREQ);		// ����B��
	DRV_PWM_PERIOD(EPwm6Regs, drv_pwm_grid,		    INV_SWITH_FREQ);		// ����C��

	EPwm1Regs.TBCTR = 1;
	EPwm2Regs.TBCTR = drv_pwm_inv.period_2*2.0/3.0;
	EPwm3Regs.TBCTR = EPwm2Regs.TBCTR*2.0;

   	DISABLE_INTS();
}




void drv_pwm_grid_start(void)
{
//    EPwm1Regs.AQCSFRC.all = 0;            // ������A
//    EPwm2Regs.AQCSFRC.all = 0;            // ������B
//    EPwm3Regs.AQCSFRC.all = 0;            // ������C

    DRV_CPLD_DO_START_GRID_PWM();
}


void drv_pwm_grid_stop(void)
{
    DRV_CPLD_DO_STOP_GRID_PWM();

//    EPwm1Regs.AQCSFRC.all = 5;            // ǿ�Ƶ�
//    EPwm2Regs.AQCSFRC.all = 5;            // ǿ�Ƶ�
//    EPwm3Regs.AQCSFRC.all = 5;            // ǿ�Ƶ�
}




void drv_pwm_startA(unsigned buck)
{
	if(buck){    
		EPwm1Regs.AQCSFRC.all = 4;            // ������A�Ϲ�
		EPwm2Regs.AQCSFRC.all = 4;            // ������A�Ϲ�
		EPwm3Regs.AQCSFRC.all = 4;            // ������A�Ϲ�
	}else{
		EPwm1Regs.AQCSFRC.all = 1;            // ������A�¹�
		EPwm2Regs.AQCSFRC.all = 1;            // ������A�¹�
		EPwm3Regs.AQCSFRC.all = 1;            // ������A�¹�
	}

    DRV_CPLD_DO_START_PWM();
}



// ������pwmֹͣ
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2013-1-24

void drv_pwm_stopA(void)
{
    DRV_CPLD_DO_STOP_PWM();

    EPwm1Regs.AQCSFRC.all = 5;            // ǿ�Ƶ�
	EPwm2Regs.AQCSFRC.all = 5;            // ǿ�Ƶ�
	EPwm3Regs.AQCSFRC.all = 5;            // ǿ�Ƶ�
}


void drv_pwm_stop(void)
{
	drv_pwm_stopA();
	drv_pwm_grid_stop();
}


//			�Ϲ� 		�¹�
//buck�� 	MOS			������
//boost��	������		MOS
void drv_pwm_buck_duty(unsigned buck, float duty)
{
	unsigned tmp;
	
	SAT_VOID(duty, 1.0, -1.0);
	tmp  = (unsigned)(1+drv_pwm.period_4 * (1.0-duty));

	if(buck==1){						    //buck
		EPwm1Regs.CMPA.half.CMPA = tmp;
    	EPwm1Regs.CMPB           = 0;
    	EPwm1Regs.AQCSFRC.all 	 = 4;

		EPwm2Regs.CMPA.half.CMPA = tmp;
    	EPwm2Regs.CMPB           = 0;
    	EPwm2Regs.AQCSFRC.all    = 4;

		EPwm3Regs.CMPA.half.CMPA = tmp;
    	EPwm3Regs.CMPB           = 0;
    	EPwm3Regs.AQCSFRC.all    = 4;
	}else{									//boost
    	EPwm1Regs.CMPA.half.CMPA = 0;
    	EPwm1Regs.CMPB           = tmp;
    	EPwm1Regs.AQCSFRC.all    = 1;

    	EPwm2Regs.CMPA.half.CMPA = 0;
    	EPwm2Regs.CMPB           = tmp;
    	EPwm2Regs.AQCSFRC.all    = 1;

    	EPwm3Regs.CMPA.half.CMPA = 0;
    	EPwm3Regs.CMPB           = tmp;
    	EPwm3Regs.AQCSFRC.all    = 1;
  	}
}
