#include "apply.h"
#include "led.h"

//LED2	���ϵƣ�		����û��������		���������У���			����˸�����ϣ�


//===================�����Ǿֲ���̬����==============


//�ڲ��õĽṹ�塢�ꡢ��������
#define 	LED_OFF		0
#define 	LED_ON		1
#define 	LED_BLINK	2


typedef struct {
	unsigned bon;				// �ƵĿ���״̬
	CLK 	 clk0;        		// ����
	char 	 n;					// ͨ����û�г�ʼֵ�������ַ�n��ȷ�������ַ�Χ����㣬�����á�
} LED;


static LED ledA = {0,  CLK_DEFAULTS, 'A'};
static LED led1 = {0,  CLK_DEFAULTS, '1'};
static LED led2 = {0,  CLK_DEFAULTS, '2'}; 			
static LED led3 = {0,  CLK_DEFAULTS, '3'}; 			

static void led_on_blink(LED *p, unsigned bin, unsigned long ms);		//�ֲ���̬��������



// ��ʼ��
//void led_init1(void)
//{
//}





// ������������
void led_main(void)
{
    //LED2  ���ϵ�
	if (except.error)						{led_on_blink(&led2, LED_BLINK, 400);}
	else if (burntest.bwork)				    {led_on_blink(&led2, LED_ON,    400);}//fsm_inv_bwork
	else									{led_on_blink(&led2, LED_OFF,   400);}

}








//���ܵ��������ֲ���̬����
static void led_on_blink(LED *p, unsigned bin, unsigned long ms)
{
	 if(bin <= LED_ON){												//bin = LED_OFF��LED_ON����״̬����ʱʱ��msû��

	 	p->bon = bin;												//�˴���bon���ȿ��Ա��浱ǰ״ֵ̬����������������������ͳһ���á�
	 	
	 }else{															//bin = LED_BLINK��˸״̬

		if (clk_Bjudgen(&p->clk0, 1, ms)){
			p->bon = 1 - p->bon;
		}

	}

// 	drv_io_led_on(p->n, p->bon);

	if (p->n == '2'){
		if (p->bon){
//			drv_cpld_do_cab_lamp(1);
		}else{
//			drv_cpld_do_cab_lamp(0);
		}
	}

}

