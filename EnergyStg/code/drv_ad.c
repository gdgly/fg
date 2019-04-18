#include "drv.h"
#include "base.h"
#include "drv_ad.h"
#include "pu.h"
#include "test.h"

DRV_AD drv_ad = DRV_AD_DEFAULTS;
#define	DRV_F_D		FILTER1_DEFAULTS

static 	FILTER1 stacka_temp_filter1 = DRV_F_D,	stackb_temp_filter1 = DRV_F_D,	stackc_temp_filter1 = DRV_F_D,	stackn_temp_filter1 = DRV_F_D;


void drv_ad_check(void);

// ������ad�Ĵ�����ʼ��,ֻ��һ��,ͣ��ʱ�������³�ʼ��
void drv_ad_init1(void)
{
	unsigned long i;

	InitAdc();        							 // ����TI����ʼ��ad
	//AdcRegs.ADCTRL1.bit.RESET = 1;             // ��λADC

    AdcRegs.ADCTRL3.bit.SMODE_SEL   = 1;       	// ѡ��ͬ������ģʽ
	// ad�������ʵķ���: ���Ŀ���Ƶ����10KHz(100us),����10%(10us)���Ҹ�ad����
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 11;   		// ADCģ��ʱ�� = HSPCLK/(ADC_CKPS+1) = 150MHz/(11+1) = 12.5MHz(80ns)
    AdcRegs.ADCTRL1.bit.ACQ_PS   = 3;  			// ����������Ϊ2��ADC����
                        					   	// ͬ������ģʽ��ʽ: �������� = ADC clock MHz/(2+ACQ_PS)
                        					   	// �������� = 12.5MHz/(2+2) = 3.125MHz
                        					   	// ����������12.5MSPS�����ADCʱ��25MHz,������ߵĲ�������6.25MSPS��ADCʱ��12.5MHz,
	AdcRegs.ADCTRL1.bit.SUSMOD		= 0;		// �������ʱӲ������ֹͣ
    AdcRegs.ADCTRL1.bit.SEQ_CASC    = 1;       	// SEQ���ü���ģʽ
    AdcRegs.ADCTRL1.bit.CONT_RUN    = 0;       	// ��Ϊ����_ֹͣ����ģʽ
    AdcRegs.ADCTRL1.bit.SEQ_OVRD    = 1;       	// ʹ��-������MAX_CONV1���õ����ͨ����, ������ģ��ͨ����ѭ��ת��
//	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 	= 1;	  	// ePWM_SOCA_SEQ1������ת��
//	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 	= 1;	  	// ePWM_SOCA_SEQ2������ת��
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ 	= 1;	  	// ePWM_SOCA_SEQ2������ת��

    // AD�ο���Դ��ѡ��,���ⲿ��׼
	AdcRegs.ADCREFSEL.bit.REF_SEL = 1;

    // AD������
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0x7;  	// ���ת��ͨ����Ϊ7,��ֵΪ1��(MAX_CONV1+1)֮���ת����������ͨ�����ʵ�ֲ���ת��
    AdcRegs.ADCCHSELSEQ1.bit.CONV00  = 0x0;  	// A0, B0:
    AdcRegs.ADCCHSELSEQ1.bit.CONV01  = 0x1;  	// A1, B1:
    AdcRegs.ADCCHSELSEQ1.bit.CONV02  = 0x2;  	// A2, B2:
    AdcRegs.ADCCHSELSEQ1.bit.CONV03  = 0x3;  	// A3, B3:
    AdcRegs.ADCCHSELSEQ2.bit.CONV04  = 0x4;  	// A4, B4:
    AdcRegs.ADCCHSELSEQ2.bit.CONV05  = 0x5;  	// A5, B5:
    AdcRegs.ADCCHSELSEQ2.bit.CONV06  = 0x6;  	// A6, B6:
    AdcRegs.ADCCHSELSEQ2.bit.CONV07  = 0x7;  	// A7, B7:

    //��λ SEQ1 ���� Cascade�����ж�
    AdcRegs.ADCTRL2.bit.RST_SEQ1     = 1;
	AdcRegs.ADCST.bit.INT_SEQ1_CLR   = 1;
	AdcRegs.ADCST.bit.INT_SEQ2_CLR   = 1;

	//��ƫֵ
//	drv_ad.iaref_in 	 = 	drv_ad.ibref_in 	 = 	drv_ad.icref_in 	 = 2048;
//	drv_ad.iaref_out 	 = 	drv_ad.ibref_out 	 = 	drv_ad.icref_out 	 = 2048;

    filter1_init(&stacka_temp_filter1, 0);	filter1_init(&stackb_temp_filter1, 0);	filter1_init(&stackc_temp_filter1, 0);	filter1_init(&stackn_temp_filter1, 0);

    for (i=0;i<100000L;i++)	NOP1();			// Ҫ�ȵ�AD��Ӳ�������ȶ�����ܲ���; һ��forѭ����5��ָ���6.66*5 = 30ns,��ʱ��100000L*30ns = 3ms
   	drv_ad_check();							// stack���ʵȼ���У׼
}


//�ߵ�ѹto���ѹ
#define  line2phase(ab, bc, ca, 	a, b, c)		a = (ab - ca) * C_1_3;	b = (bc - ab) * C_1_3; 	c = (ca -bc) * C_1_3

#define	GET_ZERO(a, a_old, a_zero)					\
{													\
	if (a * a_old < 0) a_zero = 1; else a_zero = 0;	\
	a_old = a;										\
}
// 2048�Ǹ����ֵ-187.5A; 3072��0A; 4096�������ֵ187.5A;
#define GET_LEM(AD, ref)   (((float)AD - ref + 1024.0)* (2*187.5/2048.0) - 187.5)*(1.0/IACPU)   
#define GET_OUT(AD, ref)   (((float)AD - ref + 1024.0)* (2*187.5/2048.0) - 187.5)*(1.0/IDCPU)   

// ��������ad���ݿ�;����pwm�жϳ����ж�ʵʱ����
// ���ߣ��γ�
// �汾��4.0
// ���ڣ�������,2018-8-22
// LEM��װ������Ϊ�������������Ϊ��,����LEM��������һ��-1
void drv_ad_read(void)
{
	int    i = 6, j = 0;
	//float  uab, ubc, uca,		ua, ub, uc,		udc;

    do{
	    while ((int)(AdcRegs.ADCASEQSR.bit.SEQ_CNTR) > i) ; // �ȴ���Ӧͨ��ת�����, ����ͨ��������
		switch (j){

	    	case 0:	drv_ad.ic_in = GET_LEM(AdcMirror.ADCRESULT0 , drv_ad.icref_in);	        //A0:I_INW ��������W�����

	    			drv_ad.ua_out = (AdcMirror.ADCRESULT1 - 2048.0) * (1/UOUT_PU);	        //B0:A���ѹ, �ӵ�� ֱ��
	    			break;

	    	case 1:	drv_ad.ib_in = GET_LEM(AdcMirror.ADCRESULT2 , drv_ad.ibref_in);	        //A1:I_INV ��������V�����

	    			drv_ad.ub_out = (AdcMirror.ADCRESULT3 - 2048.0) * (1/UOUT_PU);	        //B1:B���ѹ, �ӵ�� ֱ��
	    			break;

	    	case 2:	drv_ad.ia_in = GET_LEM(AdcMirror.ADCRESULT4 , drv_ad.iaref_in);	        //A2: I_INU ��������U�����

    				drv_ad.uc_out = (AdcMirror.ADCRESULT5 - 2048.0) * (1/UOUT_PU);	        //B2: C���ѹ, �ӵ�� ֱ��
	    			break;

	    	case 3:	//drv_ad.ie = ((float)AdcMirror.ADCRESULT6 - drv_ad.ie_ref)* (1/IEX_PU);//A3: I_OUTLC ���ŵ���

	    			drv_ad.udc = (AdcMirror.ADCRESULT7 - 2048.0) * (1/UDC_PU);	        	//B3: U_UDC ֱ����ѹ
					break;

	    	case 4:	//drv_ad.iz = GET_LEM(AdcMirror.ADCRESULT8 , drv_ad.iz_ref);	        //A4: I_OUTZD �ƶ�����

	    			drv_ad.uca_in = (AdcMirror.ADCRESULT9 - 2048.0) * (1/UIN_PU);		    //B4: U_IN_UV �����ѹUV

	    			break;

	    	case 5:	drv_ad.ic_out = -GET_OUT(AdcMirror.ADCRESULT10 , drv_ad.icref_out);      //A5: I_OUTC ������C�����

	    			drv_ad.ubc_in = (AdcMirror.ADCRESULT11 - 2048.0) * (1/UIN_PU);			//B5: U_IN_VW �����ѹVW
	    			break;

	    	case 6:	drv_ad.ib_out = -GET_OUT(AdcMirror.ADCRESULT12 , drv_ad.ibref_out);	    //A6: I_OUTB ������B�����

	    			drv_ad.uab_in = (AdcMirror.ADCRESULT13 - 2048.0) * (1/UIN_PU);			//B6: U_IN_WU �����ѹWU
	    			break;

			case 7:	drv_ad.ia_out = -GET_OUT(AdcMirror.ADCRESULT14 , drv_ad.iaref_out);	    //A7: I_OUTA ������A�����

	    			drv_ad.admux = AdcMirror.ADCRESULT15;		                            //B7: MUX ��·ѡ��ģ����

	    			drv_ad.iabc = (drv_ad.ia_out + drv_ad.ib_out + drv_ad.ic_out) + 0.056;//0.035*3;
					//drv_ad_calibrate();
	    			break;
	    			

    		default: break;
	    }

		j++;
		if (i>0)   i--;

    }while(j<8);


	// �������ߵ�ѹ�Ĺ����,��ua��ub��uc���ཻ��ĵ�
	GET_ZERO(drv_ad.uab_in, drv_ad.uab_old, drv_ad.buab_zero);	// ua-ub�����
	GET_ZERO(drv_ad.ubc_in, drv_ad.ubc_old, drv_ad.bubc_zero);	// ub-uc�����
	GET_ZERO(drv_ad.uca_in, drv_ad.uca_old, drv_ad.buca_zero);	// uc-ua�����

	// �����ߵ�ѹת���ѹ
	line2phase(drv_ad.uab_in, drv_ad.ubc_in, drv_ad.uca_in,   drv_ad.ua_in, drv_ad.ub_in, drv_ad.uc_in);
	TRANSFORM_CLARK(drv_ad.ua_in, drv_ad.ub_in, drv_ad.uc_in, drv_ad.ualpha, drv_ad.ubeta);


	// ���������ѹ�Ĺ����,��ua��ub��uc�Ĺ����
	GET_ZERO(drv_ad.ua_in, drv_ad.ua_old, drv_ad.bua_zero);	        // a������
	GET_ZERO(drv_ad.ub_in, drv_ad.ub_old, drv_ad.bub_zero);	        // b������
	GET_ZERO(drv_ad.uc_in, drv_ad.uc_old, drv_ad.buc_zero);	        // c������
}


float drv_ad_ntc(unsigned REG, FILTER1 *p, float T_sample);

//STACK SIZE
#define GET_STACK_SIZE(REG)						 ((10.0/4096.0) * REG- 5.0)

// ������ad���¶ȣ������������С�				
void drv_ad_read_temp(unsigned bnormal)
{						//	0	1	2	3	4	5	6	7	8	9
	static unsigned sel[10]={ 	0,	1,	2,	3,	4,	5,	6,	7,	8,	9};
	static CLK clk0 = CLK_DEFAULTS;

	static unsigned channel = 0;
	unsigned        channel_max;

	#define	 T_ms  		1						// 1ms
	#define	 T_s  		(T_ms*9*0.001)			//��Ϊ����9�Σ���ִ��1�Σ������˲�����ʱ��9��

	if ( bnormal && !clk_Bjudgen(&clk0, 1, T_ms) )	 	return;

	switch (channel) {
		case 0:	drv_ad.stackn_temp      = drv_ad_ntc(drv_ad.admux, &stackn_temp_filter1, T_s);		break;
		case 1:	drv_ad.stackn_size1     = GET_STACK_SIZE(drv_ad.admux);								break;

		case 2:	drv_ad.stackc_temp      = drv_ad_ntc(drv_ad.admux, &stackc_temp_filter1, T_s);		break;
		case 3:	drv_ad.stackc_size1     = GET_STACK_SIZE(drv_ad.admux);								break;

		case 4:	drv_ad.stackb_temp      = drv_ad_ntc(drv_ad.admux, &stackb_temp_filter1, T_s);		break;
		case 5:	drv_ad.stackb_size1		= GET_STACK_SIZE(drv_ad.admux);								break;

		case 6:	drv_ad.stacka_temp      = drv_ad_ntc(drv_ad.admux, &stacka_temp_filter1, T_s);		break;
		case 7:	drv_ad.stacka_size1		= GET_STACK_SIZE(drv_ad.admux);								break;

		default: 	break;
	}

	channel_max = 7;
	if (channel >= channel_max)		channel = 0;	else	channel++;

	DRV_CPLD_MUX(sel[channel]);

}



//======================================================�ڲ�����============================================================

// ������NTC���������,�����ֵ�ǲ�����ĵ�ѹֵ
// ���ߣ��γ�

float drv_ad_ntc(unsigned REG, FILTER1 *p, float T_sample)
{
#define 	N_NTC_U						40							//��ѹ�ֳ�N_NTC_U��
#define 	INTERVAL					((10.0 - 0.5)/N_NTC_U)		//��ѹ�ļ��

	static float ntc_tab[N_NTC_U+1]=	{150.52, 129.80, 115.85, 105.39, 97.03, 90.08, 84.12, 78.90, 74.24, 70.02,
										66.16, 62.59, 59.26, 56.14, 53.19, 50.38, 47.70, 45.12, 42.64, 40.24,
										37.90, 35.62, 33.39, 31.20, 29.04, 26.91, 24.79, 22.69, 20.59, 18.49,
										16.37, 14.24, 12.08, 9.89, 7.65, 5.35, 2.98, 0.52, -2.05, -4.76,
										-7.63, };
	float temp, temp_out;				//�¶�
	unsigned u_int;
	float u, u1;

	u = ( (10.0/4.7) * ( (10.0/4096.0) * (float)REG - 5.0 ) );
	if(u < 0.5)		return ntc_tab[0];

	u_int 	= (unsigned)( (u-0.5) * (1.0/INTERVAL) );
	u1 		= u_int * INTERVAL + 0.5;
	temp 	= (u - u1) * ( ntc_tab[u_int + 1] - ntc_tab[u_int] ) * (1.0/INTERVAL) + ntc_tab[u_int];		//���Բ�ֵ

	temp_out = filter1_func(p, T_sample, FC_STACK_TEMP_SW, temp);

	return temp_out;
}


// ��ز�ĵ���У׼: У׼ʱ, �ȹص����У׼����, ��x[]�����趨����ֵ, ��ǯ���������ʵֵ,��������y[]�м��ɡ�
void drv_ad_calibrate(void)
{
#define 	N_DUAN				10  							//�ֳ�N_DUAN��
#define     ORIGINAL            (-100.0)
#define     LAST                ( 100.0)
#define 	IABC_INTERVAL		((LAST - ORIGINAL)/N_DUAN)		//�����10A ����ȼ��,��Ȼ��Ҫ����x�Ķ�λ

	//x �趨ֵ,Ҳ�ǲ���ֵ 
//	float x[N_DUAN]   = {-100,-80,-60,-40,-20,0,20,40,60,80,100}; 
    //y ����ֵ,������ʵֵ  // �ŵ� �����Ǹ�ֵ  // ��� ��������ֵ
	float m[N_DUAN+1] = {-101,-81,-61,-41,-21,0,22,42,62,82,102};
	unsigned i;    // �ڼ���
	float x,y, x0;

	x = drv_ad.iabc*(IDCPU);

	i  = (unsigned)( (x - ORIGINAL) * (1.0/IABC_INTERVAL) );
	if (i > (N_DUAN-1)) i = (N_DUAN-1);
	if (x < ORIGINAL)   i = 0;            // iҪ�޶�,��ֹ�ڷ�Χ֮��,����������Խ��
	x0 = i * IABC_INTERVAL + ORIGINAL;
	y  = (x - x0) * ( m[i+1] - m[i] ) * (1.0/IABC_INTERVAL) + m[i];	 //���Բ�ֵ

	drv_ad.iabc = y*(1.0/IDCPU);
}
