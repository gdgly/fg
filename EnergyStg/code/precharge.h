// ʾ�������������ⲿ���ö������ú���
//    PRECHARGE precharge=PRECHARGE_DEFAULTS;
//    extern PRECHARGE precharge;
//    precharge_init(&precharge);
//    precharge_func(&precharge);

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19


#ifndef __PRECHARGE_H__
#define __PRECHARGE_H__

// ���������ڲ�����
void precharge_init1(void);
void precharge_ready_chg(void);
void precharge_ready_dchg(void);
unsigned precharge_chg_func(void);
unsigned precharge_dchg_func(void);
unsigned precharge_release(void);
void precharge_reset(void);
unsigned precharge_MCB_onoff_func(unsigned bonoff);

#endif  // __PRECHARGE_H__
