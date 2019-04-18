// 描述：except异常的常数定义的头文件。包括警告和故障2部分
//		 warn和error共用一个表：0-999是警告，1000-1999是故障，2000以上不可复位故障。
//		 实际变量值，warn直接赋值，error需减去1000后才赋值。		 warn = 0-999， error = 000-999， 1000以上error不可复位。
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

#ifndef __EXCEPT_TABLE_H__
#define __EXCEPT_TABLE_H__

#define E2W(n)		(n-1000)		//共用ERROR和WARN表格，这为转换函数

// error常数
enum {						        // 此枚举名称无意义
	ERROR_NO = 1000,				// 1000开始，错误信号不能定义为1000。
	
//交流输入电压
	ERROR_UA_IN_OVER,				// 暂态过压
	ERROR_UB_IN_OVER,
	ERROR_UC_IN_OVER,

//交流输入电流		
	ERROR_IA_IN_OVER,				// 暂态过流
	ERROR_IB_IN_OVER,
	ERROR_IC_IN_OVER,

//交流输出电压
	ERROR_UA_OUT_OVER,				// 暂态过压
	ERROR_UB_OUT_OVER,
	ERROR_UC_OUT_OVER,
	ERROR_U_IMBALANCE,				// 暂态电压检测不平衡
	ERROR_UIN_OVER,
	
	ERROR_U1A_OVER,					// 稳态电压过压
	ERROR_U1B_OVER,
	ERROR_U1C_OVER,
	ERROR_U1A_UNDER,				// 稳态电压欠压
	ERROR_U1B_UNDER,
	ERROR_U1C_UNDER,
	ERROR_U1_IMBALANCE,				// 稳态电压不平衡-定时
		
	ERROR_UF_OVER,					// 稳态频率过频-定时
	ERROR_UF_UNDER,					// 稳态频率欠频-定时
	
//交流输出电流		
	ERROR_IA_OUT_OVER,				// 暂态过流
	ERROR_IB_OUT_OVER,
	ERROR_IC_OUT_OVER,
	ERROR_IN_OVER,
	ERROR_I_IMBALANCE,				// 暂态电流检测不平衡
	
	ERROR_I1A_OVER,					// 稳态交流过流-定时
	ERROR_I1B_OVER,
	ERROR_I1C_OVER,
	ERROR_I1N_OVER,
	ERROR_I1_IMBALANCE,				// 稳态电流检测不平衡
	
	ERROR_I1A_OUT_OVER,				// 稳态交流过流-定时
	ERROR_I1B_OUT_OVER,
	ERROR_I1C_OUT_OVER,
		
//直流电压
	ERROR_UDC_OVER,					// 暂态直流母线过压
	ERROR_UDC_UNDER,				// 暂态直流母线欠压
	ERROR_UDC_RIPPLE_OVER,			// 暂态直流母线波动-定时
	ERROR_UDC_LOW_OVER,				// 暂态直流母线过压
	
//STACK+IGBT
	ERROR_STACK_SIZE,				// STACK功率等级错误
	ERROR_DRV_CPLD_A_STACK_DRV,		// STACKA驱动故障
	ERROR_DRV_CPLD_B_STACK_DRV,		// STACKB驱动故障
	ERROR_DRV_CPLD_C_STACK_DRV,		// STACKC驱动故障
	ERROR_DRV_CPLD_D_STACK_DRV,		// STACKD驱动故障
	ERROR_DRV_CPLD_A_OUT_OI,		// STACKA过流故障
	ERROR_DRV_CPLD_B_OUT_OI,		// STACKB过流故障
	ERROR_DRV_CPLD_C_OUT_OI,		// STACKC过流故障
	ERROR_DRV_CPLD_D_OUT_OI,		// STACKD过流故障
	ERROR_DRV_CPLD_A_OT,			// STACKA过温故障
	ERROR_DRV_CPLD_B_OT,			// STACKB过温故障
	ERROR_DRV_CPLD_C_OT,			// STACKC过温故障
	ERROR_DRV_CPLD_D_OT,			// STACKD过温故障
	ERROR_DRV_CPLD_ODC,			    // STACK直流过压故障
	ERROR_DRV_CPLD_OUT_OI,			// 输入电流过载位，不区分UVW

//CPLD
	ERROR_DRV_CPLD_BUS_TEST,		// 测试故障，来自CPLD
	ERROR_DRV_CPLD_MCB_trip,
	ERROR_DRV_CPLD_EMERGENCY,		//急停

//通信
	ERROR_COMM,						// 通讯
	ERROR_COMM_ID,					// 通讯ID冲突
	ERROR_COMM_INNER_IDLE,			// 内部通信scib长时间无信息,7个通信周期
	ERROR_EEPROM,					// EEPROM故障
	ERROR_PNL,						// 面板参数输入出错，实则是个警告
	
//其它温度
	ERROR_REACTOR_TEMP_OVER,		// 过热：电感
	ERROR_CABINET_TEMP_OVER,		// 过热：柜体
	ERROR_HEATER_OVERHEAT,			// 过热：逆变器散热片
	ERROR_TRANSFORMER_OVERHEAT,		// 过热：变压器

	ERROR_BATAA_REVERSE,			// 电池反接
	ERROR_BATAB_REVERSE,
	ERROR_BATAC_REVERSE,

	ERROR_CHG_OVER,					// 过充
	ERROR_DCHG_OVER,				// 过放
	ERROR_DISI_OVER,				// 放电电流过大
	
//其它
	ERROR_PREGRID,					// 网侧预充电
	ERROR_PREBAT,					// 电池侧预充电
	ERROR_PWM,						// PWM占空比超过1
	ERROR_MCB,						// 大开关故障
	ERROR_PHASE_LOSS,				// 充放电
	ERROR_QSUM_OVER,

	ERROR_BAT_OTHER,              
	ERROR_BAT_CURR_HIGH_DISCHARGE,
	ERROR_BAT_CURR_HIGH_CHARGE,   
	ERROR_BAT_TEMP_LOW,           
	ERROR_BAT_TEMP_OVER,          
	ERROR_BAT_CELL_VLOT_UNBLANCE, 
	ERROR_BAT_CELL_VLOT_LOW,      
	ERROR_BAT_CELL_VLOT_HIGH,     

	ERROR_CANA_NOTCON,
	
	ERROR_WARN,						// 所有error信号都会自动产生此警告。接受复位信号后才清除
	ERROR_OTHER,					// 其它：请联系厂家

	ERROR_UACB=2000,				// 稳态电压相序反相，只能重启。不可复位的故障
	ERROR_AD,						// AD采样精度有问题

	ERROR_END						// 结束用，最后一个不用逗号。这样，上面定义的都统一用逗号。
};

#endif  // __EXCEPT_TABLE_H__
