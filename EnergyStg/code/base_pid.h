// ʾ�������������ⲿ���ö������ú���
// ͳһ���±�̷����ʵ�ڲ����õ���TI��PID��Ҳû��̫��
//    PID pid=PID_DEFAULTS;
//    extern PID pid;
//	  pid_init(p, kp, ki, out_reset, max, min)
//    pid_init(&pid, kp, ki, kd, kc, max, min);
//    pid_func(&pid, ref, feed);

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19


#ifndef __PID_H__
#define __PID_H__

//#include "IQmathLib.h"
//#include "pid_reg3.h"

/*typedef struct {

} PID;*/

typedef struct {
			    //�ڲ�������
				  float  err;				// Variable: Error
				  float  up;				// Variable: Proportional output
				  float  ui;				// Variable: Integral output
//				  float  OutPreSat; 		// Variable: Pre-saturated output
				  float  T_sample;

                //���룺
				  float  kp;				// Parameter: Proportional gain
				  float  KI;			    // �˴�����ϵ���������ǲ���ʱ�䣬������ʱ���������
				  float  ki;			    // Parameter: Integral gain		�˴�����ϵ�������ǲ���ʱ��
				  float  max;			    // Parameter: Maximum output
				  float  min;	    		// Parameter: Minimum output

                //�����

		 	 	} PID;

#define PID_DEFAULTS { 0.0, 			\
                       0.0, 			\
                       0.0, 			\
                       0.001, 			\
                       					\
                       0.0, 			\
                       0.0, 			\
                       0.0, 			\
                       0.0	 			\
              		}

// ���������ڲ�����
void pid_init_rapid(PID *p, float kp, float ki, float reset, float max, float min, float T_sample);
void pid_init(PID *p, float kp, float ki, float reset, float max, float min);
void pid_kpki(PID *p, float kp, float ki);
void pid_reset(PID *p, float reset);
void pid_limit(PID *p, float max, float min);
float pid_func(PID *p, float T_sample, float ref, float feed);



#define PID_FUNC_RAPID(P, ref, feed, out)		\
{												\
	P.err = ref - feed; 						\
	P.up= P.kp * P.err;							\
	P.ui= P.ui + P.ki * P.up;					\
	out= P.up + P.ui;							\
	SAT_VOID(P.ui, P.max, P.min);				\
	SAT_VOID(out, P.max, P.min);				\
}

			
#endif  // __PID_H__

