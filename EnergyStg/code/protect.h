// ʾ�������������ⲿ���ö������ú���
//    PROTECT protect=PROTECT_DEFAULTS;
//    protect_init();

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19


#ifndef __PROTECT_H__
#define __PROTECT_H__


// ���������ڲ�����
void protect_init(void);
void protect_over(void);
void protect_fs(void);

extern unsigned protect_k;
extern unsigned protect_close_dis;
extern float    protect_uouta;



#endif  // __PROTECT_H__

