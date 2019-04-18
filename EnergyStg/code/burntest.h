// ʾ�������������ⲿ���ö������ú���
//    BURNTEST burntest=BURNTEST_DEFAULTS;
//    extern BURNTEST burntest;
//    burntest_init();
//    burntest_func();

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19


#ifndef __BURNTEST_H__
#define __BURNTEST_H__

typedef struct {
                //�ڲ�������

                //SPEED
                SPEED speed_v;
				unsigned bwork;
				float vmax, v;

				//SVPWM
				float aduty, bduty, cduty;
} BURNTEST;


#define BURNTEST_DEFAULTS {  	SPEED_DEFAULTS, 0, 0}

// ��������ָ��
typedef BURNTEST *BURNTEST_HANDLE;

// ���������ڲ�����
void burntest_init(void);
void burntest_func(void);
void burntest_break(void);

extern BURNTEST burntest;


#endif  // __BURNTEST_H__

