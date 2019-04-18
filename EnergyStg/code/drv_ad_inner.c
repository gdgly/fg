#include "drv.h"
#include "base.h"
#include "drv_ad.h"
#include "pu.h"

//�����ڲ�����
void drv_ad_read_inner(void);


// �������ϵ�У��ƫ��
void drv_ad_check(void)
{
	unsigned i,j;

	#define	DRV_F_D		FILTER1_DEFAULTS
	FILTER1 iaref_filter1_in  = DRV_F_D,	ibref_filter1_in  = DRV_F_D,	icref_filter1_in  = DRV_F_D;
	FILTER1 iaref_filter1_out = DRV_F_D,	ibref_filter1_out = DRV_F_D,	icref_filter1_out = DRV_F_D;

	for(i=0;i<500;i++){				        // ADԼ20us��1�Σ���ȡ5000������Լ10ms
		AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1; 		// Ϊ������һ��SOC����ʱ�ظ��������,��������һ��SOC֮ǰʹ��RST_SEQ1��λ������
		AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;	  	// ���� SEQ1 ���� Cascade
		drv_ad_read_inner();					// У׼����
		drv_ad_read_temp(0);					// �ɼ�ȫ���¶��� + STACK_size
		for (j=0;j<9000;j++)	NOP1();	        // RAM�ܵ�ʱ��û�£�FLASH������??

		#define		T_S		20e-6
		#define		FC		1e3

		//LEMУ��ƫ================================================================================
		drv_ad.iaref_in  = filter1_func(&iaref_filter1_in, 		T_S,	FC,		drv_ad.iaref1_in);
		drv_ad.ibref_in  = filter1_func(&ibref_filter1_in, 		T_S,	FC,		drv_ad.ibref1_in);
		drv_ad.icref_in  = filter1_func(&icref_filter1_in, 		T_S,	FC,		drv_ad.icref1_in);

		drv_ad.iaref_out = filter1_func(&iaref_filter1_out, 	T_S,	FC,		drv_ad.iaref1_out);
		drv_ad.ibref_out = filter1_func(&ibref_filter1_out, 	T_S,	FC,		drv_ad.ibref1_out);
		drv_ad.icref_out = filter1_func(&icref_filter1_out, 	T_S,	FC,		drv_ad.icref1_out);
	}
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ 	= 1;    // ePWM_SOCA_SEQ2������ת��
	for (i=0;i<60000;i++)	NOP1();			    // Ҫ�ȵ�AD��Ӳ�������ȶ�����ܲ���; һ��forѭ����5��ָ���6.66*5 = 30ns,��ʱ��60000L*30ns = 1.8ms

	EXCEPT_JUDGE_SET( abs(drv_ad.iaref1_in -3072) > 350 || abs(drv_ad.ibref1_in -3072) > 350 || abs(drv_ad.icref1_in -3072) > 350, ERROR_AD);
	EXCEPT_JUDGE_SET( abs(drv_ad.iaref1_out-3072) > 350 || abs(drv_ad.ibref1_out-3072) > 350 || abs(drv_ad.icref1_out-3072) > 350, ERROR_AD);
}


//�ڲ�����,ר����У׼�õġ�
void drv_ad_read_inner(void)
{
	int    i = 6, j = 0;

    do{
	    while ((int)(AdcRegs.ADCASEQSR.bit.SEQ_CNTR) > i) ; // �ȴ���Ӧͨ��ת�����, ����ͨ��������
		NOP3();NOP3();
		switch (j){
	    	case 0:	drv_ad.icref1_in = AdcMirror.ADCRESULT0;
	    			break;

	    	case 1:	drv_ad.ibref1_in = AdcMirror.ADCRESULT2;
	    			break;

	    	case 2:	drv_ad.iaref1_in = AdcMirror.ADCRESULT4;
	    			break;

	    	case 3:	
					break;

	    	case 4:	
	    			break;

	    	case 5:	drv_ad.icref1_out= AdcMirror.ADCRESULT10;
	    			break;

	    	case 6:	drv_ad.ibref1_out= AdcMirror.ADCRESULT12;
	    			break;

			case 7:	drv_ad.iaref1_out= AdcMirror.ADCRESULT14;
	    			break;
	    }

		j++;
		if (i>0)   i--;

    }while(j<8);
}
