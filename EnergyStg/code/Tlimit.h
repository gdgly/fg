// ʾ�������������ⲿ���ö������ú���
//    Tlimit Tlimit1 = Tlimit_DEFAULTS;
//    extern Tlimit Tlimit1;
//    �ο�����

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19


#ifndef __Tlimit_H__
#define __Tlimit_H__

typedef struct {
                //�ڲ�������
				unsigned bresult;
				float sum;
				CLK clk0, clk1;
                //���룺

                //�����
} Tlimit;

#define Tlimit_DEFAULTS { 0, 0, CLK_DEFAULTS, CLK_DEFAULTS  }

// ��������ָ��
typedef Tlimit *Tlimit_HANDLE;

// ���������ڲ�����
void Tlimit_init(Tlimit_HANDLE p);
unsigned Tlimit_fix(Tlimit_HANDLE p, float in, float threshold, unsigned long interval);
unsigned Tlimit_anti(Tlimit_HANDLE p, float in, float threshold, float limit1, unsigned long interval1);


// ��������ʱ��Ƿ����
#define	Tlimit_fixn(p, in, limit1, interval)	Tlimit_fix(p, -in, -limit1, interval)



#endif  // __Tlimit_H__

