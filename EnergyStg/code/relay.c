#include "apply.h"
#include "relay.h"

void relay_init(RELAY_HANDLE p, void (*p1_fname)(unsigned) )
{
	p->frun_hook = p1_fname;
	p->bflag1 	   = 0;			p->bflag2      = 0;
	p->step        = 0;
}

void relay_hook_inner(unsigned bflag)
{
	//ʲôҲ���������ã�������ǳ�ʼ����������ָ����ָ
}

//bonoff = 1��բ��0��բ
//bflag1��¼��ǰ��ִ�е�˲ʱ���
//bflag2��¼����ִ�����״̬��
//ʱ�����������Ϊ����ִ�С���һ��ʱ��ûʵ�֣��Ե�һ������ִ�б���ִ���꣬��ִ�еڶ�����
//��Ҫ��֤�߼��������֣�5Сʱ��50ms��һ�ε��������ִ��
unsigned relay_level(RELAY_HANDLE p, unsigned bonoff)
{
	p->frun_hook(bonoff);
	p->bflag1 = bonoff;			

	if(p->bflag1 == p->bflag2)		
		return 1;
	else
		return 0;
}


// Ӧ�þ�����
//relay_pulse_ready();	//�ȴ򿪣����ٴδ򿪡�	���ùܹرգ��Զ��ر�
//relay_pulse(p, 1);
void relay_pulse_ready(RELAY_HANDLE p)
{
	p->step = 0;
}

//bonoff_back��¼�ϴ�bonoff���
//step��¼ִ����������Ĳ��裬��2����0��ʼ��������1�����һ����ƽ��2����ڶ�����ƽ��������̬����
//����һ�����壬�Զ��ر�
unsigned relay_pulse(RELAY_HANDLE p, unsigned bonoff)
{
	if(p->step==2)		return 1;
		
	switch (p->step){
		case 0:	if (relay_level(p, bonoff))									p->step++;	break;
		case 1:	if (relay_level(p, !bonoff))								p->step++;	break;
	}
	return 0;
}



// ��������բ��բ��Ȧ����ʱ����������������������������������ʵ�ִ˹��ܡ�
// ������bjudge1������bjudgen����Ϊ�ܶ�ʱ�����Զ����㡣    
// ��������������庯���У�case 0 ��1��case 1��0����case 0�������ִ�� case 1��û�л���ִ��reset����ʱ
void relay_reset(RELAY_HANDLE p, unsigned long ms)
{
	if (clk_Bjudgen(&p->clk,  p->bflag1 != p->bflag2, ms)){		
		p->bflag2 = p->bflag1;									
	}
}
