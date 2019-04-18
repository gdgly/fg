// ʾ�������������ⲿ���ö������ú���
//    SPEED speed=SPEED_DEFAULTS;
//    extern SPEED speed;


// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19


#ifndef __SPEED_H__
#define __SPEED_H__


typedef struct {
                //�ڲ�������
				float up, dn;
				float up1s, dn1s;
				float max, min;
				float out;
  				//����
                //�����
} SPEED;

#define SPEED_DEFAULTS 	{	0,	0,		\
							0,	0,		\
							0,	0,		\
							0			}

// ���������ڲ�����
void speed_set(SPEED *p, float timeup, float timedn, float max, float min);
void speed_reset(SPEED *p, float u);
float speed_updn(SPEED *p, float T_sample, float in);

void speed_set_rapid(SPEED *p, float timeup, float timedn, float max, float min, float T_sample);

#define SPEED_UPDN_RAPID(P, in, result)			\
{												\
	if(in == P.out){                          	\
		/*�������޷�*/     					    \
	}else if(in > P.out){                      	\
		P.out += P.up;							\
		if(P.out > in)		P.out = in;         \
		SAT_VOID(P.out, P.max, P.min);			\
	}else{                                      \
		P.out -= P.dn;                         	\
		if(P.out < in)		P.out = in;         \
		SAT_VOID(P.out, P.max, P.min);			\
	}											\
	/*SAT_VOID(P.out, P.max, P.min);*/			\
	result = P.out;                             \
}

#endif  // __SPEED_H__
