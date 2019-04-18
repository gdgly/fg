
#ifndef __CHARGEBMS_H__
#define __CHARGEBMS_H__


#define  HAND_SHAKE     0 //握手阶段
#define  PARAM_CONFIG   1 //参数配置
#define  CHARGEING      2 //充电阶段
#define  CHARGE_END     3 //充电结束阶段
#define  CHARGE_OVER    4 //充电完成
#define  CHARGE_ERR     5 //充电失败


#define  CHARGE_STATE_TIME     1000 //充电结束阶段

#define  HANK_OVER_TIME        1000 //握手超时：5s
#define  PARAM_OVER_TIME       1000 //参数配置报文超时：5s
#define  CHARGE_OVER_TIME      1000 //充电状态帽ㄎ某保?5s

#define BufMAX 8


////////////////////////////////////////////////////////////////////////////////////////


typedef struct
{

  unsigned long   ExtId;
  unsigned char   DLC;
  unsigned char   Data[8];

} CanRxMsg;

//6.6地址分配
#define BMS         0xA9       //电池管理系统
#define TES         0xA6       //检测



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct BMSERROR_BITS{
        unsigned	other					:1;      // bit0 其他故障
	    unsigned	Curr_High_DisCharge		:1;      // bit1 放电过流
	    unsigned	Curr_High_Charge		:1;      // bit2 充电过流
	    unsigned	Temp_Low				:1;      // bit3 温度过低
	    unsigned	Temp_Over				:1;      // bit4 温度过高
	    unsigned	Cell_Vlot_Unblance		:1;      // bit5 压差过大
	    unsigned	Cell_Vlot_low			:1;      // bit6 单体欠压
	    unsigned	Cell_Vlot_High			:1;      // bit7 单体过压
	    unsigned	rsvd1					:1;      // bit8--bit15
	};
union   BMSERROR_REG{
        unsigned int all;
        struct BMSERROR_BITS bits;
};

typedef struct{
        union  BMSERROR_REG		BmsState2	;
}BMS_E;



typedef struct{
        float 		N_VoltMaxCharge			;		//  允许最大充电电压
        float 		N_currentMaxCharge		;		//  允许最大充电电流
		unsigned 	EnableCharge			;		//  充电使能 0-禁止充电 1-允许充电 相当于开机命令

}BMS_1;

typedef struct{
        float 		N_VoltMinDischarge	    ;		//  允许最大放电电压
        float 		N_currentMaxDischarge	;		//  允许最大放电电流
		unsigned 	EnableDisCharge			;		//  放电使能 0-禁止放电 1-允许放电 相当于开机命令

}BMS_2;

struct BMSMODE_BITS{
        unsigned	other					:1;      // bit0 其他故障
	    unsigned	Curr_High_DisCharge		:1;      // bit1 放电过流
	    unsigned	rsvd1					:1;      // bit8--bit15
	};
union   BMSMODE_REG{
        unsigned int all;
        struct BMSMODE_BITS bits;
};



typedef struct{
        float   			N_systemVoltage	; //  母线总电压
        float   			N_systemCurrent	; //  母线总电流
        unsigned int		N_SOC			; //  剩余电量SOC
        unsigned int		WorkMode		; //  当前工作模式 0-停机模式 1-充电模式 2-加热模式 3-放电模式 4-故障模式
		union BMSMODE_REG	PermitWorkMode	; //  当前允许工作模式

}BMS_3;




typedef struct{
		unsigned	BatRunState	;           // 电池系统运行控制

}PCS_1;


typedef struct
{
  unsigned long   ExtId;  
  unsigned char   DLC;    
  unsigned char   Data[8]; 
                  
} CanTxMsg;


////////////////////////////////////////////////////////////////////////////////////
extern  BMS_E					bmsA_error;
extern  BMS_1                	bmsA_1;
extern  BMS_2                	bmsA_2;
extern  BMS_3                	bmsA_3;
extern  unsigned				error_canA;
extern  PCS_1                   PcsA_1;
extern  CanTxMsg 				CAN_EV_TxBox; 


void chargebms_init1(void);
void EV_CAN_RxA();
void BMS_CAN_Tx();


#endif  // __CHARGEBMS_H__

//===========================================================================
// End of file.
//===========================================================================
