#include "drv.h"
#include "base.h"
#include "drv_ad.h"
#include "pu.h"
#include "test.h"

DRV_AD drv_ad = DRV_AD_DEFAULTS;
#define	DRV_F_D		FILTER1_DEFAULTS

static 	FILTER1 stacka_temp_filter1 = DRV_F_D,	stackb_temp_filter1 = DRV_F_D,	stackc_temp_filter1 = DRV_F_D,	stackn_temp_filter1 = DRV_F_D;


void drv_ad_check(void);

// ÃèÊö£ºad¼Ä´æÆ÷³õÊ¼»¯,Ö»ÄÜÒ»´Î,Í£»úÊ±²»ÄÜÖØÐÂ³õÊ¼»¯
void drv_ad_init1(void)
{
	unsigned long i;

	InitAdc();        							 // À´×ÔTI£¬³õÊ¼»¯ad
	//AdcRegs.ADCTRL1.bit.RESET = 1;             // ¸´Î»ADC

    AdcRegs.ADCTRL3.bit.SMODE_SEL   = 1;       	// Ñ¡ÔñÍ¬²½²ÉÑùÄ£Ê½
	// ad²ÉÑùËÙÂÊµÄ·ÖÅä: ×î¿ìµÄ¿ª¹ØÆµÂÊÊÇ10KHz(100us),·ÖÅä10%(10us)×óÓÒ¸øad²ÉÑù
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 11;   		// ADCÄ£¿éÊ±ÖÓ = HSPCLK/(ADC_CKPS+1) = 150MHz/(11+1) = 12.5MHz(80ns)
    AdcRegs.ADCTRL1.bit.ACQ_PS   = 3;  			// ²ÉÑù´°¿ÚÉèÎª2¸öADCÖÜÆÚ
                        					   	// Í¬²½²ÉÑùÄ£Ê½¹«Ê½: ²ÉÑùËÙÂÊ = ADC clock MHz/(2+ACQ_PS)
                        					   	// ²ÉÑùËÙÂÊ = 12.5MHz/(2+2) = 3.125MHz
                        					   	// ×î´ó²ÉÑùËÙÂÊ12.5MSPS£¬×î´óADCÊ±ÖÓ25MHz,¾«¶È×î¸ßµÄ²ÉÑùËÙÂÊ6.25MSPS£¬ADCÊ±ÖÓ12.5MHz,
	AdcRegs.ADCTRL1.bit.SUSMOD		= 0;		// ·ÂÕæ¹ÒÆðÊ±Ó²¼þÁ¢¼´Í£Ö¹
    AdcRegs.ADCTRL1.bit.SEQ_CASC    = 1;       	// SEQ²ÉÓÃ¼¶ÁªÄ£Ê½
    AdcRegs.ADCTRL1.bit.CONT_RUN    = 0;       	// ÉèÎªÆô¶¯_Í£Ö¹ÔËÐÐÄ£Ê½
    AdcRegs.ADCTRL1.bit.SEQ_OVRD    = 1;       	// Ê¹ÄÜ-¸²¸ÇÓÉMAX_CONV1ÉèÖÃµÄ×î´óÍ¨µÀÊý, ÔÚÕû¸öÄ£¿éÍ¨µÀÄÚÑ­»·×ª»»
//	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 	= 1;	  	// ePWM_SOCA_SEQ1À´Æô¶¯×ª»»
//	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 	= 1;	  	// ePWM_SOCA_SEQ2À´Æô¶¯×ª»»
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ 	= 1;	  	// ePWM_SOCA_SEQ2À´Æô¶¯×ª»»

    // AD²Î¿¼µçÔ´µÄÑ¡Ôñ,ÓÃÍâ²¿»ù×¼
	AdcRegs.ADCREFSEL.bit.REF_SEL = 1;

    // ADµÄÅÅÐò
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0x7;  	// ×î´ó×ª»»Í¨µÀÉèÎª7,ÊýÖµÎª1µ½(MAX_CONV1+1)Ö®¼äµÄ×ª»»Æ÷¶¼¿ÉÒÔÍ¨¹ý±à³ÌÊµÏÖ²ÉÑù×ª»»
    AdcRegs.ADCCHSELSEQ1.bit.CONV00  = 0x0;  	// A0, B0:
    AdcRegs.ADCCHSELSEQ1.bit.CONV01  = 0x1;  	// A1, B1:
    AdcRegs.ADCCHSELSEQ1.bit.CONV02  = 0x2;  	// A2, B2:
    AdcRegs.ADCCHSELSEQ1.bit.CONV03  = 0x3;  	// A3, B3:
    AdcRegs.ADCCHSELSEQ2.bit.CONV04  = 0x4;  	// A4, B4:
    AdcRegs.ADCCHSELSEQ2.bit.CONV05  = 0x5;  	// A5, B5:
    AdcRegs.ADCCHSELSEQ2.bit.CONV06  = 0x6;  	// A6, B6:
    AdcRegs.ADCCHSELSEQ2.bit.CONV07  = 0x7;  	// A7, B7:

    //¸´Î» SEQ1 »òÕß Cascade£¬ÇåÖÐ¶Ï
    AdcRegs.ADCTRL2.bit.RST_SEQ1     = 1;
	AdcRegs.ADCST.bit.INT_SEQ1_CLR   = 1;
	AdcRegs.ADCST.bit.INT_SEQ2_CLR   = 1;

	//ÁãÆ«Öµ
//	drv_ad.iaref_in 	 = 	drv_ad.ibref_in 	 = 	drv_ad.icref_in 	 = 2048;
//	drv_ad.iaref_out 	 = 	drv_ad.ibref_out 	 = 	drv_ad.icref_out 	 = 2048;

    filter1_init(&stacka_temp_filter1, 0);	filter1_init(&stackb_temp_filter1, 0);	filter1_init(&stackc_temp_filter1, 0);	filter1_init(&stackn_temp_filter1, 0);

    for (i=0;i<100000L;i++)	NOP1();			// ÒªµÈµ½ADµÄÓ²¼þÅäÖÃÎÈ¶¨ºó²ÅÄÜ²ÉÑù; Ò»¸öforÑ­»·°´5ÌõÖ¸Áî£¬¼´6.66*5 = 30ns,×ÜÊ±¼ä100000L*30ns = 3ms
   	drv_ad_check();							// stack¹¦ÂÊµÈ¼¶¼°Ð£×¼
}


//ÏßµçÑ¹toÏàµçÑ¹
#define  line2phase(ab, bc, ca, 	a, b, c)		a = (ab - ca) * C_1_3;	b = (bc - ab) * C_1_3; 	c = (ca -bc) * C_1_3

#define	GET_ZERO(a, a_old, a_zero)					\
{													\
	if (a * a_old < 0) a_zero = 1; else a_zero = 0;	\
	a_old = a;										\
}
// 2048ÊÇ¸º×î´óÖµ-187.5A; 3072ÊÇ0A; 4096ÊÇÕý×î´óÖµ187.5A;
#define GET_LEM(AD, ref)   (((float)AD - ref + 1024.0)* (2*187.5/2048.0) - 187.5)*(1.0/IACPU)   
#define GET_OUT(AD, ref)   (((float)AD - ref + 1024.0)* (2*187.5/2048.0) - 187.5)*(1.0/IDCPU)   

// ÃèÊö£º¶ÁadÊý¾Ý¿ì;·ÅÔÚpwmÖÐ¶Ï³ÌÐòÖÐ¶ÁÊµÊ±Êý¾Ý
// ×÷Õß£ººÎ³¬
// °æ±¾£º4.0
// ÈÕÆÚ£ºÐÇÆÚÈý,2018-8-22
// LEM°²×°ÒÔÊäÈëÎªÕý£¬³ÌÐòÒÔÊä³öÎªÕý,ËùÓÐLEMµçÁ÷³ËÁËÒ»¸ö-1
void drv_ad_read(void)
{
	int    i = 6, j = 0;
	//float  uab, ubc, uca,		ua, ub, uc,		udc;

    do{
	    while ((int)(AdcRegs.ADCASEQSR.bit.SEQ_CNTR) > i) ; // µÈ´ýÏàÓ¦Í¨µÀ×ª»»Íê³É, ¶Á´ËÍ¨µÀµÄÊý¾Ý
		switch (j){

	    	case 0:	drv_ad.ic_in = GET_LEM(AdcMirror.ADCRESULT0 , drv_ad.icref_in);	        //A0:I_INW ÕûÁ÷ÊäÈëWÏàµçÁ÷

	    			drv_ad.ua_out = (AdcMirror.ADCRESULT1 - 2048.0) * (1/UOUT_PU);	        //B0:AÏàµçÑ¹, ½Óµç³Ø Ö±Á÷
	    			break;

	    	case 1:	drv_ad.ib_in = GET_LEM(AdcMirror.ADCRESULT2 , drv_ad.ibref_in);	        //A1:I_INV ÕûÁ÷ÊäÈëVÏàµçÁ÷

	    			drv_ad.ub_out = (AdcMirror.ADCRESULT3 - 2048.0) * (1/UOUT_PU);	        //B1:BÏàµçÑ¹, ½Óµç³Ø Ö±Á÷
	    			break;

	    	case 2:	drv_ad.ia_in = GET_LEM(AdcMirror.ADCRESULT4 , drv_ad.iaref_in);	        //A2: I_INU ÕûÁ÷ÊäÈëUÏàµçÁ÷

    				drv_ad.uc_out = (AdcMirror.ADCRESULT5 - 2048.0) * (1/UOUT_PU);	        //B2: CÏàµçÑ¹, ½Óµç³Ø Ö±Á÷
	    			break;

	    	case 3:	//drv_ad.ie = ((float)AdcMirror.ADCRESULT6 - drv_ad.ie_ref)* (1/IEX_PU);//A3: I_OUTLC Àø´ÅµçÁ÷

	    			drv_ad.udc = (AdcMirror.ADCRESULT7 - 2048.0) * (1/UDC_PU);	        	//B3: U_UDC Ö±Á÷µçÑ¹
					break;

	    	case 4:	//drv_ad.iz = GET_LEM(AdcMirror.ADCRESULT8 , drv_ad.iz_ref);	        //A4: I_OUTZD ÖÆ¶¯µçÁ÷

	    			drv_ad.uca_in = (AdcMirror.ADCRESULT9 - 2048.0) * (1/UIN_PU);		    //B4: U_IN_UV ÊäÈëµçÑ¹UV

	    			break;

	    	case 5:	drv_ad.ic_out = -GET_OUT(AdcMirror.ADCRESULT10 , drv_ad.icref_out);      //A5: I_OUTC Äæ±äÊä³öCÏàµçÁ÷

	    			drv_ad.ubc_in = (AdcMirror.ADCRESULT11 - 2048.0) * (1/UIN_PU);			//B5: U_IN_VW ÊäÈëµçÑ¹VW
	    			break;

	    	case 6:	drv_ad.ib_out = -GET_OUT(AdcMirror.ADCRESULT12 , drv_ad.ibref_out);	    //A6: I_OUTB Äæ±äÊä³öBÏàµçÁ÷

	    			drv_ad.uab_in = (AdcMirror.ADCRESULT13 - 2048.0) * (1/UIN_PU);			//B6: U_IN_WU ÊäÈëµçÑ¹WU
	    			break;

			case 7:	drv_ad.ia_out = -GET_OUT(AdcMirror.ADCRESULT14 , drv_ad.iaref_out);	    //A7: I_OUTA Äæ±äÊä³öAÏàµçÁ÷

	    			drv_ad.admux = AdcMirror.ADCRESULT15;		                            //B7: MUX ¶àÂ·Ñ¡ÔñÄ£ÄâÁ¿

	    			drv_ad.iabc = (drv_ad.ia_out + drv_ad.ib_out + drv_ad.ic_out) + 0.056;//0.035*3;
					//drv_ad_calibrate();
	    			break;
	    			

    		default: break;
	    }

		j++;
		if (i>0)   i--;

    }while(j<8);


	// ÇóÊäÈëÏßµçÑ¹µÄ¹ýÁãµã,¼´ua¡¢ub¡¢uc»¥Ïà½»²æµÄµã
	GET_ZERO(drv_ad.uab_in, drv_ad.uab_old, drv_ad.buab_zero);	// ua-ub¹ýÁãµã
	GET_ZERO(drv_ad.ubc_in, drv_ad.ubc_old, drv_ad.bubc_zero);	// ub-uc¹ýÁãµã
	GET_ZERO(drv_ad.uca_in, drv_ad.uca_old, drv_ad.buca_zero);	// uc-ua¹ýÁãµã

	// ÊäÈëÏßµçÑ¹×ªÏàµçÑ¹
	line2phase(drv_ad.uab_in, drv_ad.ubc_in, drv_ad.uca_in,   drv_ad.ua_in, drv_ad.ub_in, drv_ad.uc_in);
	TRANSFORM_CLARK(drv_ad.ua_in, drv_ad.ub_in, drv_ad.uc_in, drv_ad.ualpha, drv_ad.ubeta);


	// ÇóÊäÈëÏàµçÑ¹µÄ¹ýÁãµã,¼´ua¡¢ub¡¢ucµÄ¹ýÁãµã
	GET_ZERO(drv_ad.ua_in, drv_ad.ua_old, drv_ad.bua_zero);	        // aÏà¹ýÁãµã
	GET_ZERO(drv_ad.ub_in, drv_ad.ub_old, drv_ad.bub_zero);	        // bÏà¹ýÁãµã
	GET_ZERO(drv_ad.uc_in, drv_ad.uc_old, drv_ad.buc_zero);	        // cÏà¹ýÁãµã
}


float drv_ad_ntc(unsigned REG, FILTER1 *p, float T_sample);

//STACK SIZE
#define GET_STACK_SIZE(REG)						 ((10.0/4096.0) * REG- 5.0)

// ÃèÊö£ºad²ÉÎÂ¶È£¬·ÅÔÚÖ÷º¯ÊýÖÐ¡£				
void drv_ad_read_temp(unsigned bnormal)
{						//	0	1	2	3	4	5	6	7	8	9
	static unsigned sel[10]={ 	0,	1,	2,	3,	4,	5,	6,	7,	8,	9};
	static CLK clk0 = CLK_DEFAULTS;

	static unsigned channel = 0;
	unsigned        channel_max;

	#define	 T_ms  		1						// 1ms
	#define	 T_s  		(T_ms*9*0.001)			//ÒòÎª½øÀ´9´Î£¬²ÅÖ´ÐÐ1´Î£¬ËùÒÔÂË²¨²ÉÑùÊ±¼ä9´Î

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



//======================================================ÄÚ²¿º¯Êý============================================================

// ÃèÊö£ºNTC²é±í²âÁ¿µçè,ÊäÈëµÄÖµÊÇ²ÉÑùµãµÄµçÑ¹Öµ
// ×÷Õß£ººÎ³¬

float drv_ad_ntc(unsigned REG, FILTER1 *p, float T_sample)
{
#define 	N_NTC_U						40							//µçÑ¹·Ö³ÉN_NTC_U¶Î
#define 	INTERVAL					((10.0 - 0.5)/N_NTC_U)		//µçÑ¹µÄ¼ä¸ô

	static float ntc_tab[N_NTC_U+1]=	{150.52, 129.80, 115.85, 105.39, 97.03, 90.08, 84.12, 78.90, 74.24, 70.02,
										66.16, 62.59, 59.26, 56.14, 53.19, 50.38, 47.70, 45.12, 42.64, 40.24,
										37.90, 35.62, 33.39, 31.20, 29.04, 26.91, 24.79, 22.69, 20.59, 18.49,
										16.37, 14.24, 12.08, 9.89, 7.65, 5.35, 2.98, 0.52, -2.05, -4.76,
										-7.63, };
	float temp, temp_out;				//ÎÂ¶È
	unsigned u_int;
	float u, u1;

	u = ( (10.0/4.7) * ( (10.0/4096.0) * (float)REG - 5.0 ) );
	if(u < 0.5)		return ntc_tab[0];

	u_int 	= (unsigned)( (u-0.5) * (1.0/INTERVAL) );
	u1 		= u_int * INTERVAL + 0.5;
	temp 	= (u - u1) * ( ntc_tab[u_int + 1] - ntc_tab[u_int] ) * (1.0/INTERVAL) + ntc_tab[u_int];		//ÏßÐÔ²åÖµ

	temp_out = filter1_func(p, T_sample, FC_STACK_TEMP_SW, temp);

	return temp_out;
}


// µç³Ø²àµÄµçÁ÷Ð£×¼: Ð£×¼Ê±, ÏÈ¹ØµôÕâ¶ÎÐ£×¼´úÂë, °´x[]ÒÀ´ÎÉè¶¨µçÁ÷Öµ, ÓÃÇ¯Á÷±í¶Á³öÕæÊµÖµ,ÒÀ´ÎÌîÈëy[]ÖÐ¼´¿É¡£
void drv_ad_calibrate(void)
{
#define 	N_DUAN				10  							//·Ö³ÉN_DUAN¶Î
#define     ORIGINAL            (-100.0)
#define     LAST                ( 100.0)
#define 	IABC_INTERVAL		((LAST - ORIGINAL)/N_DUAN)		//¼ä¸ôÊÇ10A ±ØÐëµÈ¼ä¸ô,²»È»ÐèÒª²éÕÒxµÄ¶¨Î»

	//x Éè¶¨Öµ,Ò²ÊÇ²ÉÑùÖµ 
//	float x[N_DUAN]   = {-100,-80,-60,-40,-20,0,20,40,60,80,100}; 
    //y ²âÁ¿Öµ,¾ÍÊÇÕæÊµÖµ  // ·Åµç µçÁ÷ÊÇ¸ºÖµ  // ³äµç µçÁ÷ÊÇÕýÖµ
	float m[N_DUAN+1] = {-101,-81,-61,-41,-21,0,22,42,62,82,102};
	unsigned i;    // µÚ¼¸¶Î
	float x,y, x0;

	x = drv_ad.iabc*(IDCPU);

	i  = (unsigned)( (x - ORIGINAL) * (1.0/IABC_INTERVAL) );
	if (i > (N_DUAN-1)) i = (N_DUAN-1);
	if (x < ORIGINAL)   i = 0;            // iÒªÏÞ¶¨,·ÀÖ¹ÔÚ·¶Î§Ö®Íâ,»áÒýÆðÊý×éÔ½½ç
	x0 = i * IABC_INTERVAL + ORIGINAL;
	y  = (x - x0) * ( m[i+1] - m[i] ) * (1.0/IABC_INTERVAL) + m[i];	 //ÏßÐÔ²åÖµ

	drv_ad.iabc = y*(1.0/IDCPU);
}
