#include "chargebms.h"
#include "apply.h"

//A通道BMS参数
BMS_E					bmsA_error;
BMS_1                	bmsA_1;
BMS_2                	bmsA_2;
BMS_3                	bmsA_3;
unsigned				error_canA;
PCS_1                   PcsA_1;
CanTxMsg 				CAN_EV_TxBox; 
 

///////////////////////////////////////////初始化/////////////////////////////////////
void chargebms_init1(void)
{
	//A通道参数初始化
	bmsA_error.BmsState2.all			= 0;

	bmsA_1.N_VoltMaxCharge				= 0;
	bmsA_1.N_currentMaxCharge			= 0;
	bmsA_1.EnableCharge					= 0;

	bmsA_2.N_VoltMinDischarge			= 0;
	bmsA_2.N_currentMaxDischarge		= 0;
	bmsA_2.EnableDisCharge				= 0;

	bmsA_3.N_systemVoltage   			= 0;
	bmsA_3.N_systemCurrent   			= 0;
	bmsA_3.N_SOC             			= 0;
	bmsA_3.WorkMode         			= 0;
	bmsA_3.PermitWorkMode.all      		= 0;
	bmsA_error.BmsState2.all 			= 0;

	PcsA_1.BatRunState					= 0;
}


///////////////////////////////////////////接收/////////////////////////////////////
void EV_CAN_RxA()
{
     static CLK clkRxA       = CLK_DEFAULTS;
     static CLK clkRxAclr    = CLK_DEFAULTS;
     CanRxMsg RxMessage;

	 if(drv_cana_rdmsg( &RxMessage.ExtId, RxMessage.Data))
	 {
		switch(RxMessage.ExtId)
		{
			 case 0x18A0E5F4 :  // 最大允许充电电压、最大允许充电电流

				bmsA_1.N_VoltMaxCharge 		 =   (RxMessage.Data[0] + (unsigned)(RxMessage.Data[1]<<8))*0.1;
				bmsA_1.N_currentMaxCharge    =   (RxMessage.Data[2] + (unsigned)(RxMessage.Data[3]<<8))*0.1;
				bmsA_1.EnableCharge			 =   (RxMessage.Data[4]);
		     break;

			 case 0x18A1E5F4 :  // 最大允许放电电压、最大允许放电电流

				bmsA_2.N_VoltMinDischarge	 =   (RxMessage.Data[0] + (unsigned)(RxMessage.Data[1]<<8))*0.1;
				bmsA_2.N_currentMaxDischarge =   (RxMessage.Data[2] + (unsigned)(RxMessage.Data[3]<<8))*0.1;
				bmsA_2.EnableDisCharge		 =   (RxMessage.Data[4]);
		     break;

			 case 0x18A2E5F4 :  //  总电压 总电流 SOC(低在前 高在后)  工作模式 故障信息 故障信息

	            bmsA_3.N_systemVoltage   =    (RxMessage.Data[0] + (unsigned)(RxMessage.Data[1]<<8))*0.1;
	            bmsA_3.N_systemCurrent   =    ((signed)RxMessage.Data[2] + (signed)(RxMessage.Data[3]<<8))*0.1;
	            bmsA_3.N_SOC             =    (RxMessage.Data[4]);	// 1%/bit
				bmsA_3.WorkMode			 = 	  (RxMessage.Data[5]);
				bmsA_3.PermitWorkMode.all= 	  (RxMessage.Data[6]);
				bmsA_error.BmsState2.all =    (unsigned)(RxMessage.Data[7]); // 
			 break;

	         default :
			 break;

         }

		 clk_Bjudge1(&clkRxA, 0, 1000L);				//收到数据对计时清零
		 if (clk_Bjudgen(&clkRxAclr, 1, 5000L)) 		error_canA = 0;
    }else{
		if (clk_Bjudge1(&clkRxA, 1, 1000L)) {			// 1s判断一次CANA通信
		 	error_canA = 1;
		 	EXCEPT_JUDGE_SET((fsm_chg_bwork || fsm_dchg_bwork) && error_canA, ERROR_CANA_NOTCON);
		}
    }
}






////////////////////////////发送///////////////////////////////////////////////////////

void BMS_CAN_Tx(void)
{
    static CLK clk1       = CLK_DEFAULTS;

	if (!clk_Bjudgen(&clk1, 1, 100L))   return;			// 100ms发一次

	CAN_EV_TxBox.ExtId = 0x18F0F4E5;
	CAN_EV_TxBox.DLC   = 8;

	CAN_EV_TxBox.Data[3] = 0;
	CAN_EV_TxBox.Data[2] = 0;
	CAN_EV_TxBox.Data[1] = 0;    
	CAN_EV_TxBox.Data[0] = PcsA_1.BatRunState;

	CAN_EV_TxBox.Data[7] = 0;
	CAN_EV_TxBox.Data[6] = 0;
	CAN_EV_TxBox.Data[5] = 0;
	CAN_EV_TxBox.Data[4] = 0;

	drv_cana_wrmsg(CAN_EV_TxBox.ExtId, CAN_EV_TxBox.DLC,  CAN_EV_TxBox.Data);  

}
