// ʾ�������������ⲿ���ö������ú���
//    VC_SSC vc_ssc=VC_SSC_DEFAULTS;
//    extern VC_SSC vc_ssc;
//    vc_ssc_init();
//    vc_ssc_func();

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19


#ifndef __VC_SSC_H__
#define __VC_SSC_H__

typedef struct {
                //�ڲ�������

                //SPEED
                SPEED 		speed_v;
				unsigned 	bwork;
				float 		vmax, v;
				unsigned	samlpe_num;		//һ����Ƶ���ڵĲ�������,50Hzʱ��60��,60Hzʱ��50�㡣

				//SVPWM
				float 		aduty, bduty, cduty;
} VC_SSC;


#define VC_SSC_DEFAULTS {  	SPEED_DEFAULTS, 0, 0, 0, 0,		0,0,0}

// ��������ָ��
typedef VC_SSC *VC_SSC_HANDLE;

// ���������ڲ�����
void vc_ssc_init1(void);
void vc_ssc_init(void);
void vc_ssc_func(void);

extern VC_SSC vc_ssc;
extern float vc_Uamp_ref;
extern float vc_Uamp;
extern float vc_Uamp1;
extern float vc_Uamp2;


#endif  // __VC_SSC_H__

