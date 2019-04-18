#ifndef __DRV_AD_H__
#define __DRV_AD_H__

//------------------------------------------------------------ 宏定义区-----------------------------------------------------------------------------------

#define 	FC_UAC_SW_ON			0			// 1表示开启滤波，0表示关闭滤波，因为条件编译只认整数
#define 	FC_IAC_SW_ON			0
#define 	FC_ICT_SW_ON			0
#define 	FC_UDC_SW_ON			0
#define 	FC_AI_SW_ON				0
#define 	FC_PT100_SW_ON			1
#define 	FC_STACK_TEMP_SW_ON		1
#define 	FC_STACK_SIZE_SW_ON		1

#define 	FC_UAC_SW				(FC_UAC_SW_ON 			* 300.0)		// 截止频率HZ，下同
#define 	FC_IAC_SW				(FC_IAC_SW_ON 			* 300.0)
#define 	FC_ICT_SW				(FC_ICT_SW_ON 			* 300.0)
#define 	FC_UDC_SW				(FC_UDC_SW_ON 			* 300.0)
#define 	FC_AI_SW				(FC_AI_SW_ON 			* 300.0)
#define 	FC_PT100_SW				(FC_PT100_SW_ON 		* 300.0)
#define 	FC_STACK_TEMP_SW		(FC_STACK_TEMP_SW_ON 	* 30.0)
#define 	FC_STACK_SIZE_SW		(FC_STACK_SIZE_SW_ON 	* 10.0)


//======================================================UAC、IAC、UDC的系数=====================================================================================

// 采样的比例定义 硬件决定

#define 	UIN_K 					(68.0/2400.0)					// 
#define 	UOUT_K 					(100.0/2400.0)					// 
#define 	IAC_K 					(5.0/187.5)						// 
#define 	IEX_K 					(2.0/20.0)					    // ACS712ELCTR-20A-T:100mV/A 0A：对应2.5V
#define 	IZD_K 					(2.0/20.0)						// 
#define 	UDC_K 					(68.0/2400.0)		            // 

// 以交流电压ua为例,推导归一化公式
// ua*k + uref = uad	(1)
// ua是实际的强电电压值(如690V),k是比例值,uref是固定的1.5V,实际是1.5V附近波动,uad是DSP的AD口的电压(最大3V)
// 归一化: 是对UACPU的归一化,即ua等于UACPU时,程序中运算后的最后结果为1.0
// 对式(1)除UACPU		ua*k/UACPU + uref/UACPU = uad/UACPU		最后要的结果就是ua/UACPU
// ua/UACPU = (uad - uref)/(k*UACPU)	(2)
// 由DSP的ad转换关系可知	AD - ADref = (uad - uref)*0xFFF/3.0		(3)  	AD是DSP的ad转换后的值(最大0xFFF),ADref是uref经DSP的ad转换后的值(0x7FF左右)
// (3)代入(2)可得:	ua/UACPU = (AD - ADref)*3.0/(k*UACPU*0xFFF)			(4)
// 归一化系数; 归一化系数应用时作为除数,定义UAC_PU = (k*UACPU*0xFFF)/3.0		ad.c中应用时就用式(4)就可以直接算出归一化的值

#define 	UIN_PU 					((UIN_K *4096.0*UACPU)/10.0)		// 转换输入交流电压的归一化
#define 	UOUT_PU 				((UOUT_K*4096.0*UDCPU)/10.0)		// 转换输出直流电压的归一化 接电池的直流
#define 	IAC_PU 					((IAC_K *4096.0*IACPU)/10.0)		// 转换交流电流的归一化
#define 	IEX_PU 					((IEX_K *4096.0*IEXPU)/10.0)		// 转换励磁电流的归一化
#define 	IZD_PU 					((IZD_K *4096.0*IACPU)/10.0)		// 转换直流电压的归一化
#define 	UDC_PU 					((UDC_K *4096.0*UDCPU)/10.0)		// 转换直流电压的归一化


//======================================================PT100温度的系数=====================================================================================

#define 	PT100_K_U2T 			(50.0192)        	// PT100斜率
#define 	PT100_B_U2T 			(-47.3682)        	// PT100截距



typedef struct {
                // 内部变量：
                // 输入：

                // 输出：

				// 输入信号
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

              	// 输出信号
				float		ua_out,	    	ub_out, 			uc_out;
				float		ia_out,			ib_out,			    ic_out;
				float       iabc;

              	float 	    iaref_out, 		ibref_out, 			icref_out;
              	float 	    iaref1_out,		ibref1_out,			icref1_out;

              	float	    udc;

				float  	    admux;								   					// 多路选择输入
				float       stacka_temp,    stackb_temp,        stackc_temp,        stackn_temp;
				float       stacka_size1,   stackb_size1,       stackc_size1,       stackn_size1;
				
				float	bias[2][13],	k[2][13];									// 分段的校准数据


} DRV_AD;


#define DRV_AD_DEFAULTS {       0		  }

// 声明对象指针
typedef DRV_AD *DRV_AD_HANDLE;



//--------------------------- 函数定义区--------------------------------//

void drv_ad_init1(void);
void drv_ad_read(void);
void drv_ad_read_temp(unsigned bstack);
void drv_ad_calibrate(void);

//------------------------- 全局变量定义区------------------------------//

extern DRV_AD drv_ad;




#endif  // __DRV_AD_H__

