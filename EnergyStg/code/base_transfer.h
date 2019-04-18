// ʾ�������������ⲿ���ö������ú���
//    FILTER1 filter1=FILTER1_DEFAULTS;
//    extern FILTER1 filter1;

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19


#ifndef __TRANSFER_H__
#define __TRANSFER_H__

//filter1=================================================

typedef struct {
                //�ڲ�������
                float y_back,	fc_T;		//��Ҫ��ʼ��
				float ky, 		ku;			//����Ҫ��ʼ��
} FILTER1;


#define FILTER1_DEFAULTS {   0.0,  -1.0        }

// ���������ڲ�����
void filter1_init(FILTER1 *p, float value);
void filter1_init_rapid(FILTER1 *p, float value, float T_sample, float fc);
float filter1_func(FILTER1 *p, float T_sample, float fc, float u);


#define FILTER1_FUNC_RAPID(P, u, out)						\
{															\
	P.y_back  = P.ky * P.y_back   +    P.ku * u;			\
	out = P.y_back;											\
}


//============================================�����˲�===========================================

typedef struct {
                //�ڲ�������
                float y_back2;
                float y_back,	fc_T;		//��Ҫ��ʼ��
				float ky, 		ku;			//����Ҫ��ʼ��
} FILTER2;

#define FILTER2_DEFAULTS { 0.0,		0.0,  -1.0          }


// ���������ڲ�����
void filter2_init(FILTER2 *p, float value);
float filter2_func(FILTER2 *p, float T_sample, float fc, float u);



//============================================�����˲�����===========================================

#define FILTER2_FUNC_RAPID(P, u, out)						\
{															\
	P.y_back  = P.ky * P.y_back   +    P.ku * u;			\
	P.y_back2 = P.ky * P.y_back2  +    P.ku * P.y_back;		\
	out = P.y_back2;										\
}

#define FILTERH2_FUNC_RAPID(P, u, out)							\
{																\
	P.y_back  = P.ky * P.y_back   +    P.ku * u;				\
	P.y_back2 = P.ky * P.y_back2  +    P.ku * P.y_back;			\
	out = 1.0*u - P.y_back2;										\
}

// ���������ڲ�����
void filter2_init_rapid(FILTER2 *p, float value, float T_sample, float fc);

#define  OFF_FC								10000.0


typedef struct {
                //�ڲ�������
				float y_back;
} INTEGRAL;

#define INTEGRAL_DEFAULTS {  0              }

// ��������ָ��
typedef INTEGRAL *INTEGRAL_HANDLE;

// ���������ڲ�����
void integral_init(INTEGRAL_HANDLE p, float value);
float integral_func(INTEGRAL_HANDLE p, float T, float u);


#endif  // __TRANSFER_H__

