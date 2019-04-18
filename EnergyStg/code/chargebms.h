
#ifndef __CHARGEBMS_H__
#define __CHARGEBMS_H__


#define  HAND_SHAKE     0 //���ֽ׶�
#define  PARAM_CONFIG   1 //��������
#define  CHARGEING      2 //���׶�
#define  CHARGE_END     3 //�������׶�
#define  CHARGE_OVER    4 //������
#define  CHARGE_ERR     5 //���ʧ��


#define  CHARGE_STATE_TIME     1000 //�������׶�

#define  HANK_OVER_TIME        1000 //���ֳ�ʱ��5s
#define  PARAM_OVER_TIME       1000 //�������ñ��ĳ�ʱ��5s
#define  CHARGE_OVER_TIME      1000 //���״̬ñ��ĳ�ʱ�?5s

#define BufMAX 8


////////////////////////////////////////////////////////////////////////////////////////


typedef struct
{

  unsigned long   ExtId;
  unsigned char   DLC;
  unsigned char   Data[8];

} CanRxMsg;

//6.6��ַ����
#define BMS         0xA9       //��ع���ϵͳ
#define TES         0xA6       //���



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct BMSERROR_BITS{
        unsigned	other					:1;      // bit0 ��������
	    unsigned	Curr_High_DisCharge		:1;      // bit1 �ŵ����
	    unsigned	Curr_High_Charge		:1;      // bit2 ������
	    unsigned	Temp_Low				:1;      // bit3 �¶ȹ���
	    unsigned	Temp_Over				:1;      // bit4 �¶ȹ���
	    unsigned	Cell_Vlot_Unblance		:1;      // bit5 ѹ�����
	    unsigned	Cell_Vlot_low			:1;      // bit6 ����Ƿѹ
	    unsigned	Cell_Vlot_High			:1;      // bit7 �����ѹ
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
        float 		N_VoltMaxCharge			;		//  ����������ѹ
        float 		N_currentMaxCharge		;		//  ������������
		unsigned 	EnableCharge			;		//  ���ʹ�� 0-��ֹ��� 1-������ �൱�ڿ�������

}BMS_1;

typedef struct{
        float 		N_VoltMinDischarge	    ;		//  �������ŵ��ѹ
        float 		N_currentMaxDischarge	;		//  �������ŵ����
		unsigned 	EnableDisCharge			;		//  �ŵ�ʹ�� 0-��ֹ�ŵ� 1-����ŵ� �൱�ڿ�������

}BMS_2;

struct BMSMODE_BITS{
        unsigned	other					:1;      // bit0 ��������
	    unsigned	Curr_High_DisCharge		:1;      // bit1 �ŵ����
	    unsigned	rsvd1					:1;      // bit8--bit15
	};
union   BMSMODE_REG{
        unsigned int all;
        struct BMSMODE_BITS bits;
};



typedef struct{
        float   			N_systemVoltage	; //  ĸ���ܵ�ѹ
        float   			N_systemCurrent	; //  ĸ���ܵ���
        unsigned int		N_SOC			; //  ʣ�����SOC
        unsigned int		WorkMode		; //  ��ǰ����ģʽ 0-ͣ��ģʽ 1-���ģʽ 2-����ģʽ 3-�ŵ�ģʽ 4-����ģʽ
		union BMSMODE_REG	PermitWorkMode	; //  ��ǰ������ģʽ

}BMS_3;




typedef struct{
		unsigned	BatRunState	;           // ���ϵͳ���п���

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
