#include "apply.h"
#include "led.h"

//LED2	故障灯：		灯灭（没开机）、		灯亮（运行）、			灯闪烁（故障）


//===================以下是局部静态变量==============


//内部用的结构体、宏、函数声明
#define 	LED_OFF		0
#define 	LED_ON		1
#define 	LED_BLINK	2


typedef struct {
	unsigned bon;				// 灯的开关状态
	CLK 	 clk0;        		// 闹钟
	char 	 n;					// 通道，没有初始值。采用字符n，确保比数字范围更宽广，更好用。
} LED;


static LED ledA = {0,  CLK_DEFAULTS, 'A'};
static LED led1 = {0,  CLK_DEFAULTS, '1'};
static LED led2 = {0,  CLK_DEFAULTS, '2'}; 			
static LED led3 = {0,  CLK_DEFAULTS, '3'}; 			

static void led_on_blink(LED *p, unsigned bin, unsigned long ms);		//局部静态函数声明



// 初始化
//void led_init1(void)
//{
//}





// 放在主程序里
void led_main(void)
{
    //LED2  故障灯
	if (except.error)						{led_on_blink(&led2, LED_BLINK, 400);}
	else if (burntest.bwork)				    {led_on_blink(&led2, LED_ON,    400);}//fsm_inv_bwork
	else									{led_on_blink(&led2, LED_OFF,   400);}

}








//万能灯驱动，局部静态函数
static void led_on_blink(LED *p, unsigned bin, unsigned long ms)
{
	 if(bin <= LED_ON){												//bin = LED_OFF和LED_ON两个状态，此时时间ms没用

	 	p->bon = bin;												//此处用bon，既可以保存当前状态值，更可以让下面驱动函数统一调用。
	 	
	 }else{															//bin = LED_BLINK闪烁状态

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

