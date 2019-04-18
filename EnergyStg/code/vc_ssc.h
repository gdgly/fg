// 示例：声明对象、外部引用对象、引用函数
//    VC_SSC vc_ssc=VC_SSC_DEFAULTS;
//    extern VC_SSC vc_ssc;
//    vc_ssc_init();
//    vc_ssc_func();

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19


#ifndef __VC_SSC_H__
#define __VC_SSC_H__

typedef struct {
                //内部变量：

                //SPEED
                SPEED 		speed_v;
				unsigned 	bwork;
				float 		vmax, v;
				unsigned	samlpe_num;		//一个工频周期的采样点数,50Hz时是60点,60Hz时是50点。

				//SVPWM
				float 		aduty, bduty, cduty;
} VC_SSC;


#define VC_SSC_DEFAULTS {  	SPEED_DEFAULTS, 0, 0, 0, 0,		0,0,0}

// 声明对象指针
typedef VC_SSC *VC_SSC_HANDLE;

// 声明对象内部函数
void vc_ssc_init1(void);
void vc_ssc_init(void);
void vc_ssc_func(void);

extern VC_SSC vc_ssc;
extern float vc_Uamp_ref;
extern float vc_Uamp;
extern float vc_Uamp1;
extern float vc_Uamp2;


#endif  // __VC_SSC_H__

