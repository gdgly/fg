// 示例：声明对象、外部引用对象、引用函数
//    DRV_CPLD drv_cpld = DRV_CPLD_DEFAULTS;
//    extern DRV_CPLD drv_cpld;
//    drv_cpld_init();
//    drv_cpld_read();

// 描述：头文件。
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

#ifndef __DRV_CPLD_H__
#define __DRV_CPLD_H__


typedef struct {
                // 内部变量：
				unsigned di;
				unsigned error;		

                // 输入：

                // 输出：
				unsigned version;		//16进制
				unsigned id;
				unsigned bMCB_on;
				unsigned bMCB_off;
				unsigned bdischarge, bprecharge;
} DRV_CPLD;

#define DRV_CPLD_DEFAULTS {0, 0, 0, 0, 	0, 0, 0, 0}


// 声明对象内部函数
void drv_cpld_init1(void);
void drv_cpld_read_judge_error(void);
void drv_cpld_reset_error(void);

// 写
void drv_cpld_do_cab_fan(unsigned bflag);
void drv_cpld_do_yellow(unsigned bflag);
void drv_cpld_do_blue(unsigned bflag);
void drv_cpld_do_red(unsigned bflag);
void drv_cpld_do_MCB_grid(unsigned bflag);
void drv_cpld_do_prechg_grid(unsigned bflag);
void drv_cpld_do_MCB_bat(unsigned bflag);
void drv_cpld_do_dis(unsigned bflag);
void drv_cpld_do_MCB_gridCAB(unsigned bflag);
void drv_cpld_do_POWER(unsigned bflag);
void drv_cpld_do_prechg_bat(unsigned bflag);
unsigned drv_cpld_laidian(void);

//----------------------------2、地址定义----------------------------
#define XINF6					0x100000		// 目前用的是XINF6，即10 0000h-20 0000h，共1M*16bit

// 读(输入)
#define CPLD_TABLE1				(XINF6)

#define CPLD_BUS_TEST1			( *((volatile unsigned int *)(CPLD_TABLE1 + 90)) )		// 测试总线，读出来应该是AA55
#define CPLD_BUS_TEST2			( *((volatile unsigned int *)(CPLD_TABLE1 + 165)) )		// 测试总线，读出来应该是55AA
#define CPLD_VERSION			( *((volatile unsigned int *)(CPLD_TABLE1 + 2)) )		// CPLD版本，16进制：0110代表01.10
#define CPLD_DI					( *((volatile unsigned int *)(CPLD_TABLE1 + 3)) )		// DIN,新SVG板是10是DIN
#define CPLD_ERROR				( *((volatile unsigned int *)(CPLD_TABLE1 + 4)) )		// 故障标志。二进制位代表一个故障标志（15-0）


// 写(输出)                                                                              
#define CPLD_TABLE2				(30 + XINF6)

#define CPLD_RELAY1				( *((volatile unsigned int *)(CPLD_TABLE2 + 1)) )		// J1_DO--1  风机
#define CPLD_RELAY2				( *((volatile unsigned int *)(CPLD_TABLE2 + 2)) )		// J1_DO--2  黄灯
#define CPLD_RELAY3				( *((volatile unsigned int *)(CPLD_TABLE2 + 3)) )		// J1_DO--3  蓝灯
#define CPLD_RELAY4				( *((volatile unsigned int *)(CPLD_TABLE2 + 4)) )		// J1_DO--4  红灯
#define CPLD_RELAY5				( *((volatile unsigned int *)(CPLD_TABLE2 + 5)) )		// J1_DO--5  模块网侧输入主开关合闸
#define CPLD_RELAY6				( *((volatile unsigned int *)(CPLD_TABLE2 + 6)) )		// J1_DO--6  模块网侧输入预充电
#define CPLD_RELAY7				( *((volatile unsigned int *)(CPLD_TABLE2 + 7)) )		// J1_DO--7  模块电池侧主开关合闸
#define CPLD_RELAY8				( *((volatile unsigned int *)(CPLD_TABLE2 + 8)) )		// J1_DO--8  NC


#define CPLD_MUX				( *((volatile unsigned int *)(CPLD_TABLE2 + 11)))		// 关闭启动MUX
#define CPLD_PWM				( *((volatile unsigned int *)(CPLD_TABLE2 + 13)))		// 关闭启动PWM。FFB0:关闭，FFB1启动
#define CPLD_RESET_ERROR		( *((volatile unsigned int *)(CPLD_TABLE2 + 14)))		// 清除CPLD故障信息FF50+0000
#define CPLD_PWM_GRID			( *((volatile unsigned int *)(CPLD_TABLE2 + 15)))		// 关闭启动网侧PWM。FCA0:关闭，FCA1启动
#define CPLD_DIS    			( *((volatile unsigned int *)(CPLD_TABLE2 + 17)))		// 
#define CPLD_RELAY9				( *((volatile unsigned int *)(CPLD_TABLE2 + 18)) )		// J2_DO--9  机柜主接触器contact
#define CPLD_RELAY10			( *((volatile unsigned int *)(CPLD_TABLE2 + 19)) )		// J2_DO--10  电源继电器relay
#define CPLD_RELAY11			( *((volatile unsigned int *)(CPLD_TABLE2 + 20)) )		// J2_DO--  电池侧预充电

//----------------------------3、宏函数定义----------------------------

// 外部用到的宏函数定义

// 读
#define DRV_CPLD_DI_POW()		(	BCHECK16_BIT(drv_cpld.di, 0)	)				//	J1_DI--1  开机按钮
#define DRV_CPLD_DI_MPS()		(	BCHECK16_BIT(drv_cpld.di, 1)	)				//	J1_DI--2  带电指示

	// drv_cpld.error各位定义
	// 0位：A相驱动故障，1代表故障
	// 1位：B相驱动故障，1代表故障
	// 2位：C相驱动故障，1代表故障
	// 3位：电网侧UVW驱动故障，1代表故障
	// 
	// 4位：A相过流故障，1代表故障
	// 5位：B相过流故障，1代表故障
	// 6位：C相过流故障，1代表故障
	// 7位：电网侧UVW过流故障，1代表故障
	// 
	// 8位： 
	// 9位： 
	// 10位：
	// 11位：
	// 
	// 12位：直流过压1故障，1代表故障
	// 13位：CPLD看门狗，1代表看门狗溢出。如DSP不踢CPLD看门狗(io.h)，CPLD封PWM。CPLD40KHZ检测1个点的速度，30个点没检测到，就封锁了。目前DSP没有处理
	// 14位：保留
	// 15位：保留
#define DRV_CPLD_A_DRV_ERROR()	(drv_cpld.error & 0x0001)	// 驱动故障 bit0
#define DRV_CPLD_B_DRV_ERROR()	(drv_cpld.error & 0x0002)	// 驱动故障 bit1
#define DRV_CPLD_C_DRV_ERROR()	(drv_cpld.error & 0x0004)	// 驱动故障 bit2
#define DRV_CPLD_D_DRV_ERROR()	(drv_cpld.error & 0x0008)	// 驱动故障 bit3
#define DRV_CPLD_A_OUT_OI()		(drv_cpld.error & 0x0010)	// 过流故障 bit4
#define DRV_CPLD_B_OUT_OI()		(drv_cpld.error & 0x0020)	// 过流故障 bit5
#define DRV_CPLD_C_OUT_OI()		(drv_cpld.error & 0x0040)	// 过流故障 bit6
#define DRV_CPLD_D_OUT_OI()		(drv_cpld.error & 0x0080)	// 过流故障 bit7
#define DRV_CPLD_A_OT()		    (drv_cpld.error & 0x0100)	// 过温故障 bit8
#define DRV_CPLD_B_OT()		    (drv_cpld.error & 0x0200)	// 过温故障 bit9
#define DRV_CPLD_C_OT()		    (drv_cpld.error & 0x0400)	// 过温故障 bit10
#define DRV_CPLD_D_OT()		    (drv_cpld.error & 0x0800)	// 过温故障 bit11
#define DRV_CPLD_ODC()		    (drv_cpld.error & 0x1000)	// 直流故障 bit12
#define DRV_CPLD_DSP()    		(drv_cpld.error & 0x2000)	// 直流故障 bit13

// 写                                   
#define DRV_CPLD_DO_START_PWM() 		(CPLD_PWM=0xFFB1		)	// 启动电池侧PWM
#define DRV_CPLD_DO_STOP_PWM()			(CPLD_PWM=0xFFB0		)	// 关闭电池侧PWM
                                                              
#define DRV_CPLD_DO_START_GRID_PWM()	(CPLD_PWM_GRID=0xFCA1	)	// 启动网侧的PWM
#define DRV_CPLD_DO_STOP_GRID_PWM()		(CPLD_PWM_GRID=0xFCA0	)	// 关闭网侧的PWM


// 以下三个是AD的选择开关,DRV_IO_MUX_AD1是低位.		在IO驱动文件的初始化时，IO12、IO13、IO14、IO15,初始化必须把为0。
#define	DRV_CPLD_MUX(data)				(CPLD_MUX = data		)


extern DRV_CPLD drv_cpld;

#endif  // __DRV_CPLD_H__

