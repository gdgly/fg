// ʾ�������������ⲿ���ö������ú���
//    RELAY relay=RELAY_DEFAULTS;
//    extern RELAY relay;

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��3.1
// ���ڣ�������,2011-12-28


#ifndef __RELAY_H__
#define __RELAY_H__

typedef struct {
                //�ڲ�������
				CLK clk;
				void (*frun_hook)(unsigned);
				unsigned bflag1, bflag2;
				unsigned step;
} RELAY;

#define RELAY_DEFAULTS {	CLK_DEFAULTS,				\
							relay_hook_inner,			\
							0, 0,		                \
							0			                }

// ��������ָ��
typedef RELAY *RELAY_HANDLE;

// ���������ڲ�����
void relay_init(RELAY_HANDLE p, void (*p1_fname)(unsigned));
void relay_hook_inner(unsigned);
void relay_reset(RELAY_HANDLE p, unsigned long ms);
unsigned relay_level(RELAY_HANDLE p, unsigned bonoff);

void relay_pulse_ready(RELAY_HANDLE p);
unsigned relay_pulse(RELAY_HANDLE p, unsigned bonoff);


#endif  // __RELAY_H__

