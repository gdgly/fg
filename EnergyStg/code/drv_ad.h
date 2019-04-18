#ifndef __DRV_AD_H__
#define __DRV_AD_H__

//------------------------------------------------------------ �궨����-----------------------------------------------------------------------------------

#define 	FC_UAC_SW_ON			0			// 1��ʾ�����˲���0��ʾ�ر��˲�����Ϊ��������ֻ������
#define 	FC_IAC_SW_ON			0
#define 	FC_ICT_SW_ON			0
#define 	FC_UDC_SW_ON			0
#define 	FC_AI_SW_ON				0
#define 	FC_PT100_SW_ON			1
#define 	FC_STACK_TEMP_SW_ON		1
#define 	FC_STACK_SIZE_SW_ON		1

#define 	FC_UAC_SW				(FC_UAC_SW_ON 			* 300.0)		// ��ֹƵ��HZ����ͬ
#define 	FC_IAC_SW				(FC_IAC_SW_ON 			* 300.0)
#define 	FC_ICT_SW				(FC_ICT_SW_ON 			* 300.0)
#define 	FC_UDC_SW				(FC_UDC_SW_ON 			* 300.0)
#define 	FC_AI_SW				(FC_AI_SW_ON 			* 300.0)
#define 	FC_PT100_SW				(FC_PT100_SW_ON 		* 300.0)
#define 	FC_STACK_TEMP_SW		(FC_STACK_TEMP_SW_ON 	* 30.0)
#define 	FC_STACK_SIZE_SW		(FC_STACK_SIZE_SW_ON 	* 10.0)


//======================================================UAC��IAC��UDC��ϵ��=====================================================================================

// �����ı������� Ӳ������

#define 	UIN_K 					(68.0/2400.0)					// 
#define 	UOUT_K 					(100.0/2400.0)					// 
#define 	IAC_K 					(5.0/187.5)						// 
#define 	IEX_K 					(2.0/20.0)					    // ACS712ELCTR-20A-T:100mV/A 0A����Ӧ2.5V
#define 	IZD_K 					(2.0/20.0)						// 
#define 	UDC_K 					(68.0/2400.0)		            // 

// �Խ�����ѹuaΪ��,�Ƶ���һ����ʽ
// ua*k + uref = uad	(1)
// ua��ʵ�ʵ�ǿ���ѹֵ(��690V),k�Ǳ���ֵ,uref�ǹ̶���1.5V,ʵ����1.5V��������,uad��DSP��AD�ڵĵ�ѹ(���3V)
// ��һ��: �Ƕ�UACPU�Ĺ�һ��,��ua����UACPUʱ,�����������������Ϊ1.0
// ��ʽ(1)��UACPU		ua*k/UACPU + uref/UACPU = uad/UACPU		���Ҫ�Ľ������ua/UACPU
// ua/UACPU = (uad - uref)/(k*UACPU)	(2)
// ��DSP��adת����ϵ��֪	AD - ADref = (uad - uref)*0xFFF/3.0		(3)  	AD��DSP��adת�����ֵ(���0xFFF),ADref��uref��DSP��adת�����ֵ(0x7FF����)��
// (3)����(2)�ɵ�:	ua/UACPU = (AD - ADref)*3.0/(k*UACPU*0xFFF)			(4)
// ��һ��ϵ��; ��һ��ϵ��Ӧ��ʱ��Ϊ����,����UAC_PU = (k*UACPU*0xFFF)/3.0		ad.c��Ӧ��ʱ����ʽ(4)�Ϳ���ֱ�������һ����ֵ

#define 	UIN_PU 					((UIN_K *4096.0*UACPU)/10.0)		// ת�����뽻����ѹ�Ĺ�һ��
#define 	UOUT_PU 				((UOUT_K*4096.0*UDCPU)/10.0)		// ת�����ֱ����ѹ�Ĺ�һ�� �ӵ�ص�ֱ��
#define 	IAC_PU 					((IAC_K *4096.0*IACPU)/10.0)		// ת�����������Ĺ�һ��
#define 	IEX_PU 					((IEX_K *4096.0*IEXPU)/10.0)		// ת�����ŵ����Ĺ�һ��
#define 	IZD_PU 					((IZD_K *4096.0*IACPU)/10.0)		// ת��ֱ����ѹ�Ĺ�һ��
#define 	UDC_PU 					((UDC_K *4096.0*UDCPU)/10.0)		// ת��ֱ����ѹ�Ĺ�һ��


//======================================================PT100�¶ȵ�ϵ��=====================================================================================

#define 	PT100_K_U2T 			(50.0192)        	// PT100��б��
#define 	PT100_B_U2T 			(-47.3682)        	// PT100���ؾ�



typedef struct {
                // �ڲ�������
                // ���룺

                // �����

				// �����ź�
				float		uab_in,			ubc_in,				uca_in;
				float		ua_in,			ub_in,				uc_in;
				float		ia_in,			ib_in,				ic_in;

              	float 	    iaref_in, 		ibref_in, 			icref_in;
              	float 	    iaref1_in,		ibref1_in,			icref1_in;
              	
              	float 	    ualpha,         ubeta;
              	
				float		ua_old,			ub_old,				uc_old;
				unsigned	bua_zero,		bub_zero,			buc_zero;
				float		uab_old,		ubc_old,			uca_old;
				unsigned	buab_zero,		bubc_zero,			buca_zero;

              	// ����ź�
				float		ua_out,	    	ub_out, 			uc_out;
				float		ia_out,			ib_out,			    ic_out;
				float       iabc;

              	float 	    iaref_out, 		ibref_out, 			icref_out;
              	float 	    iaref1_out,		ibref1_out,			icref1_out;

              	float	    udc;

				float  	    admux;								   					// ��·ѡ������
				float       stacka_temp,    stackb_temp,        stackc_temp,        stackn_temp;
				float       stacka_size1,   stackb_size1,       stackc_size1,       stackn_size1;
				
				float	bias[2][13],	k[2][13];									// �ֶε�У׼����


} DRV_AD;


#define DRV_AD_DEFAULTS {       0		  }

// ��������ָ��
typedef DRV_AD *DRV_AD_HANDLE;



//--------------------------- ����������--------------------------------//

void drv_ad_init1(void);
void drv_ad_read(void);
void drv_ad_read_temp(unsigned bstack);
void drv_ad_calibrate(void);

//------------------------- ȫ�ֱ���������------------------------------//

extern DRV_AD drv_ad;




#endif  // __DRV_AD_H__

