// ʾ�������������ⲿ���ö������ú���
//    POWER_init();
//    POWER_func();

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19


#ifndef __POWER_H__
#define __POWER_H__

typedef struct {
                //�ڲ�������
                FILTER1 filter1_p_ct, 		filter1_q_ct;
                FILTER1 filter1_p_sys, 		filter1_q_sys;
				
				//�����
				float p_ct,  	q_ct,  		s_ct, 	pf_ct;
				float p_sys,	q_sys,  	s_sys, 	pf_sys;
} POWER;

#define POWER_DEFAULTS {		FILTER1_DEFAULTS,	FILTER1_DEFAULTS,	\
								FILTER1_DEFAULTS,	FILTER1_DEFAULTS	}

// ���������ڲ�����
void power_init1(void);
void power_func(void);


extern POWER power;

#endif  // __POWER_H__

