#include "drv.h"
#include "base.h"
#include "drv_ad.h"
#include "pu.h"

//定义内部函数
void drv_ad_read_inner(void);


// 描述：上电校零偏。
void drv_ad_check(void)
{
	unsigned i,j;

	#define	DRV_F_D		FILTER1_DEFAULTS
	FILTER1 iaref_filter1_in  = DRV_F_D,	ibref_filter1_in  = DRV_F_D,	icref_filter1_in  = DRV_F_D;
	FILTER1 iaref_filter1_out = DRV_F_D,	ibref_filter1_out = DRV_F_D,	icref_filter1_out = DRV_F_D;

	for(i=0;i<500;i++){				        // AD约20us读1次，读取5000个数，约10ms
		AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1; 		// 为了在下一次SOC请求时重复排序操作,必须在下一个SOC之前使用RST_SEQ1复位排序器
		AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;	  	// 启动 SEQ1 或者 Cascade
		drv_ad_read_inner();					// 校准采样
		drv_ad_read_temp(0);					// 采集全部温度量 + STACK_size
		for (j=0;j<9000;j++)	NOP1();	        // RAM跑的时候没事，FLASH跑有事??

		#define		T_S		20e-6
		#define		FC		1e3

		//LEM校零偏================================================================================
		drv_ad.iaref_in  = filter1_func(&iaref_filter1_in, 		T_S,	FC,		drv_ad.iaref1_in);
		drv_ad.ibref_in  = filter1_func(&ibref_filter1_in, 		T_S,	FC,		drv_ad.ibref1_in);
		drv_ad.icref_in  = filter1_func(&icref_filter1_in, 		T_S,	FC,		drv_ad.icref1_in);

		drv_ad.iaref_out = filter1_func(&iaref_filter1_out, 	T_S,	FC,		drv_ad.iaref1_out);
		drv_ad.ibref_out = filter1_func(&ibref_filter1_out, 	T_S,	FC,		drv_ad.ibref1_out);
		drv_ad.icref_out = filter1_func(&icref_filter1_out, 	T_S,	FC,		drv_ad.icref1_out);
	}
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ 	= 1;    // ePWM_SOCA_SEQ2来启动转换
	for (i=0;i<60000;i++)	NOP1();			    // 要等到AD的硬件配置稳定后才能采样; 一个for循环按5条指令，即6.66*5 = 30ns,总时间60000L*30ns = 1.8ms

	EXCEPT_JUDGE_SET( abs(drv_ad.iaref1_in -3072) > 350 || abs(drv_ad.ibref1_in -3072) > 350 || abs(drv_ad.icref1_in -3072) > 350, ERROR_AD);
	EXCEPT_JUDGE_SET( abs(drv_ad.iaref1_out-3072) > 350 || abs(drv_ad.ibref1_out-3072) > 350 || abs(drv_ad.icref1_out-3072) > 350, ERROR_AD);
}


//内部函数,专用于校准用的。
void drv_ad_read_inner(void)
{
	int    i = 6, j = 0;

    do{
	    while ((int)(AdcRegs.ADCASEQSR.bit.SEQ_CNTR) > i) ; // 等待相应通道转换完成, 读此通道的数据
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
