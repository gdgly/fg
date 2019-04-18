#include "drv.h"
#include "drv_can.h"


 void drv_cana_init1(void)                                                                                                                                                                
 {                                                                                                                                                                                             
 	struct ECAN_REGS ECanaShadow;                                                                                                                                                              
                                                                                                                                                                                               
     InitECanaGpio();            // ����TI����ʼ��ECana��GPIO                                                                                                                                  
 	InitECana();                // ����TI����ʼ��ECana�Ĵ���                                                                                                                                   
                                                                                                                                                                                               
     // ��ֹ��������		д����ID��֮ǰ���뽫��������ʹ��                                                                                                                                   
     ECanaRegs.CANME.all                =  0;                                                                                                                                                  
                                                                                                                                                                                               
     EALLOW;                                                                                                                                                                                   
     // �㲥������������λ,���ε�0λ...��11λ,�������ǵĵ�12λ...��28λ�������ı����                                                                                                        
     ECanaLAMRegs.LAM1.all              =  0xFFFFFFFF;//0xFFFF0000;                                                                                                                            
                                                                                                                                                                                               
     ECanaRegs.CANMIM.all    = 0;   // ��ֹ�����ж�                                                                                                                                            
     EDIS;                                                                                                                                                                                     
                                                                                                                                                                                               
     // ����ID    ��չ֡ PU                                                                                                                                                                    
     // ����0����Ե㣬����                                                                                                                                                                    
     //ECanaMboxes.MBOX0.MSGID.all      =  0x9555AAAA;                                                                                                                                         
     // ����1���㲥������  // 30λ, ʹ������,��C, ���εĵ�0λ...��11λ��cmm_id�����,��������ı�����Ͳ���ͬ��                                                                                
 //    ECanaMboxes.MBOX1.MSGID.all        =  0xC000A6A9;                                                                                                                                       
     // ����4����Ե㣬����                                                                                                                                                                    
     //ECanaMboxes.MBOX4.MSGID.all      =  0x9555AAAA;                                                                                                                                         
     // ����5���㲥������ // ���Ͳ���ʹ������,��8, ���εĵ�0λ...��11λ��cmm_id�����,��������ı�����Ͳ���ͬ��                                                                               
     //ECanaMboxes.MBOX5.MSGID.all        =  0x85550000 + (unsigned long)(unsigned)( (drv_io.cmm_id<<12) + (drv_io.cmm_id<<8) + (drv_io.cmm_id<<4) + (drv_io.cmm_id) );                        
     ECanaMboxes.MBOX1.MSGID.all        =  0xC000A6A9;                                                                                                                                         
                                                                                                                                                                                               
     // ���ݳ���:8byte                                                                                                                                                                         
     ECanaMboxes.MBOX0.MSGCTRL.bit.DLC  =  8;                                                                                                                                                  
     ECanaMboxes.MBOX1.MSGCTRL.bit.DLC  =  8;                                                                                                                                                  
     ECanaMboxes.MBOX4.MSGCTRL.bit.DLC  =  8;                                                                                                                                                  
    //  ECanaMboxes.MBOX5.MSGCTRL.bit.DLC  =  8;                                                                                                                                               
                                                                                                                                                                                               
     // ���䷽��     1100   0011    1����  0 ����                                                                                                                                              
     ECanaRegs.CANMD.all                =  0xFFFFFFC3;                                                                                                                                         
                                                                                                                                                                                               
     EALLOW;                                                                                                                                                                                   
     ECanaShadow.CANMC.all              =  ECanaRegs.CANMC.all;                                                                                                                                
     // ECanaShadow.CANMC.bit.STM       =  1;  // ����ģʽ                                                                                                                                     
     ECanaRegs.CANMC.all                =  ECanaShadow.CANMC.all;                                                                                                                              
     EDIS;                                                                                                                                                                                     
                                                                                                                                                                                               
     // ʹ������0��1��4��5                                                                                                                                                                     
     ECanaRegs.CANME.all                =  0x00000033;                                                                                                                                         
 }                                                                                                                                                                                             
                                                                                                                                              
void drv_canb_init1(void)
{
	struct ECAN_REGS ECanbShadow;
    
    InitECanbGpio();            // ����TI����ʼ��ECANB��GPIO
	InitECanb();                // ����TI����ʼ��ECANB�Ĵ���

    // ��ֹ��������		д����ID��֮ǰ���뽫��������ʹ��
    ECanbRegs.CANME.all                =  0;  

    EALLOW; 
    // �㲥������������λ,���ε�0λ...��11λ,�������ǵĵ�12λ...��28λ�������ı����
    ECanbLAMRegs.LAM1.all              =  0xFFFFFFFF;//0xFFFF0000;

    ECanbRegs.CANMIM.all    = 0;   // ��ֹ�����ж�
    EDIS; 

    // ����ID    ��չ֡ PU
    // ����0����Ե㣬����
    //ECanbMboxes.MBOX0.MSGID.all      =  0x9555AAAA;  
    // ����1���㲥������  // 30λ, ʹ������,��C, ���εĵ�0λ...��11λ��cmm_id�����,��������ı�����Ͳ���ͬ��
//    ECanbMboxes.MBOX1.MSGID.all        =  0xC000A6A9;
    // ����4����Ե㣬����
    //ECanbMboxes.MBOX4.MSGID.all      =  0x9555AAAA; 
    // ����5���㲥������ // ���Ͳ���ʹ������,��8, ���εĵ�0λ...��11λ��cmm_id�����,��������ı�����Ͳ���ͬ��
    //ECanbMboxes.MBOX5.MSGID.all        =  0x85550000 + (unsigned long)(unsigned)( (drv_io.cmm_id<<12) + (drv_io.cmm_id<<8) + (drv_io.cmm_id<<4) + (drv_io.cmm_id) );  
    ECanbMboxes.MBOX1.MSGID.all        =  0xC000A6A9;

    // ���ݳ���:8byte
    ECanbMboxes.MBOX0.MSGCTRL.bit.DLC  =  8;
    ECanbMboxes.MBOX1.MSGCTRL.bit.DLC  =  8; 
    ECanbMboxes.MBOX4.MSGCTRL.bit.DLC  =  8; 
   //  ECanbMboxes.MBOX5.MSGCTRL.bit.DLC  =  8; 
    
    // ���䷽��     1100   0011    1����  0 ����
    ECanbRegs.CANMD.all                =  0xFFFFFFC3; 

    EALLOW; 
    ECanbShadow.CANMC.all              =  ECanbRegs.CANMC.all; 
    // ECanbShadow.CANMC.bit.STM       =  1;  // ����ģʽ   
    ECanbRegs.CANMC.all                =  ECanbShadow.CANMC.all; 
    EDIS; 

    // ʹ������0��1��4��5
    ECanbRegs.CANME.all                =  0x00000033;     
}
            
// �������㲥������Ϣ����                                                                                                         
///////////////////////////////////////////////////////////////////////////////////////                                           
                                                                                                                                  
void drv_cana_wrmsg (unsigned long  Identifier, unsigned char dlc, unsigned char data[8])                                         
                                                                                                                                  
{                                                                                                                                 
    struct ECAN_REGS ECanaShadow;                                                                                                 
                                                                                                                                  
	                                                                                                                              
	// ���������Ϣû���꣬�ͷ��ء�  �ɹ����ͺ�TA5λ����λ�� ������n׼������ʱ��CPU�Ὣ���е�TRS[n]λ����Ϊ1,��������             
    if (ECanaRegs.CANTRS.bit.TRS5) 		return;                                                                                   
                                                                                                                                  
	ECanaShadow.CANME.all 	  = ECanaRegs.CANME.all;		// ����ǰ�����ֹ������                                               
	ECanaShadow.CANME.bit.ME5 = 0;                                                                                                
	ECanaRegs.CANME.all       = ECanaShadow.CANME.all;                                                                            
                                                                                                                                  
	ECanaMboxes.MBOX5.MSGID.all =  Identifier + 0x80000000;                                                                       
	ECanaMboxes.MBOX5.MSGCTRL.bit.DLC  =  dlc ;                                                                                   
                                                                                                                                  
	ECanaMboxes.MBOX5.MDL.byte.BYTE0 = data[3];	   // ��32λ                                                                      
    ECanaMboxes.MBOX5.MDL.byte.BYTE1 = data[2];                                                                                   
	ECanaMboxes.MBOX5.MDL.byte.BYTE2 = data[1];                                                                                   
	ECanaMboxes.MBOX5.MDL.byte.BYTE3 = data[0];                                                                                   
                                                                                                                                  
    ECanaMboxes.MBOX5.MDH.byte.BYTE4 = data[7];	    // ��32λ                                                                     
   	ECanaMboxes.MBOX5.MDH.byte.BYTE5 = data[6];                                                                                   
	ECanaMboxes.MBOX5.MDH.byte.BYTE6 = data[5];                                                                                   
	ECanaMboxes.MBOX5.MDH.byte.BYTE7 = data[4];                                                                                   
                                                                                                                                  
	                                                                                                                              
                                                                                                                                  
	ECanaShadow.CANME.all  		= ECanaRegs.CANME.all;		// ʹ�ܸ�����                                                         
	ECanaShadow.CANME.bit.ME5 	= 1;                                                                                              
	ECanaRegs.CANME.all       	= ECanaShadow.CANME.all;                                                                          
	                                                                                                                              
	ECanaShadow.CANTRS.all      = ECanaRegs.CANTRS.all;                                                                           
	ECanaShadow.CANTRS.bit.TRS5 = 1; // �÷�������                                                                                
	ECanaRegs.CANTRS.all        = ECanaShadow.CANTRS.all;                                                                         
}                                                                                                                                 






// �������㲥������Ϣ����
///////////////////////////////////////////////////////////////////////////////////////

void drv_canb_wrmsg (unsigned long  Identifier, unsigned char dlc, unsigned char data[8])  

{
    struct ECAN_REGS ECanbShadow;

	
	// ���������Ϣû���꣬�ͷ��ء�  �ɹ����ͺ�TA5λ����λ�� ������n׼������ʱ��CPU�Ὣ���е�TRS[n]λ����1,��������
    if (ECanbRegs.CANTRS.bit.TRS5) 		return;

	ECanbShadow.CANME.all 	  = ECanbRegs.CANME.all;		// ����ǰ�����ֹ������
	ECanbShadow.CANME.bit.ME5 = 0;
	ECanbRegs.CANME.all       = ECanbShadow.CANME.all;

	ECanbMboxes.MBOX5.MSGID.all =  Identifier + 0x80000000;
	ECanbMboxes.MBOX5.MSGCTRL.bit.DLC  =  dlc ;

	ECanbMboxes.MBOX5.MDL.byte.BYTE0 = data[3];	   // ��32λ
    ECanbMboxes.MBOX5.MDL.byte.BYTE1 = data[2];
	ECanbMboxes.MBOX5.MDL.byte.BYTE2 = data[1];
	ECanbMboxes.MBOX5.MDL.byte.BYTE3 = data[0];

    ECanbMboxes.MBOX5.MDH.byte.BYTE4 = data[7];	    // ��32λ
   	ECanbMboxes.MBOX5.MDH.byte.BYTE5 = data[6];
	ECanbMboxes.MBOX5.MDH.byte.BYTE6 = data[5];
	ECanbMboxes.MBOX5.MDH.byte.BYTE7 = data[4];

	

	ECanbShadow.CANME.all  		= ECanbRegs.CANME.all;		// ʹ�ܸ�����
	ECanbShadow.CANME.bit.ME5 	= 1;     
	ECanbRegs.CANME.all       	= ECanbShadow.CANME.all;
	
	ECanbShadow.CANTRS.all      = ECanbRegs.CANTRS.all;
	ECanbShadow.CANTRS.bit.TRS5 = 1; // �÷�������
	ECanbRegs.CANTRS.all        = ECanbShadow.CANTRS.all;
}


 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////      
 // ������Cana����(��ѯ����)                                                                                                            
                                                                                                                                        
 unsigned  drv_cana_rdmsg (unsigned long* pIdentifier, unsigned char pdata[8])                                                          
 {                                                                                                                                      
     struct ECAN_REGS ECanaShadow;                                                                                                      
                                                                                                                                        
     // CAN����1�����ж�  // ����1���㲥������                                                                                          
     if (!ECanaRegs.CANRMP.bit.RMP1)  	return 0;                                                                                       
                                                                                                                                        
 	ECanaRegs.CANRMP.bit.RMP1 = 1;   							 // ��������1������ɱ�־                                               
 	     	                                                                                                                            
    * pIdentifier = ECanaMboxes.MBOX1.MSGID.all & 0x1FFFFFFF;                                                                           
                                                                                                                                        
 	//������ȡ���ݴ���                                                                                                                  
                                                                                                                                        
 //	*pdata = ECanaMboxes.MBOX1.MDL.all + ECanaMboxes.MBOX1.MDH.all <<32;                                                                  
                                                                                                                                          
    pdata[3] = ECanaMboxes.MBOX1.MDL.byte.BYTE0;	                                                                                      
    pdata[2] = ECanaMboxes.MBOX1.MDL.byte.BYTE1;                                                                                      
 	pdata[1] = ECanaMboxes.MBOX1.MDL.byte.BYTE2;                                                                                          
 	pdata[0] = ECanaMboxes.MBOX1.MDL.byte.BYTE3;                                                                                        
                                                                                                                                        
 	pdata[7] = ECanaMboxes.MBOX1.MDH.byte.BYTE4;	                                                                                    
 	pdata[6] = ECanaMboxes.MBOX1.MDH.byte.BYTE5;                                                                                        
 	pdata[5] = ECanaMboxes.MBOX1.MDH.byte.BYTE6;                                                                                        
 	pdata[4] = ECanaMboxes.MBOX1.MDH.byte.BYTE7;                                                                                        
                                                                                                                                        
 	ECanaShadow.CANRMP.all     	= ECanaRegs.CANRMP.all;		      // ʹ�ܸ�����                                                         
 	ECanaShadow.CANRMP.bit.RMP1	= 1;                                                                                                    
 	ECanaRegs.CANRMP.all       	= ECanaShadow.CANRMP.all;                                                                               
                                                                                                                                        
 	return 1;                                                                                                                             
 }                                                                                                                                        


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������canb����(��ѯ����)

unsigned  drv_canb_rdmsg (unsigned long* pIdentifier, unsigned char pdata[8])
{
    struct ECAN_REGS ECanbShadow;

    // CAN����1�����ж�  // ����1���㲥������ 
    if (!ECanbRegs.CANRMP.bit.RMP1)  	return 0;
  
	ECanbRegs.CANRMP.bit.RMP1 = 1;   							 // ��������1������ɱ�־ 
	     	
   * pIdentifier = ECanbMboxes.MBOX1.MSGID.all & 0x1FFFFFFF;
    
	//������ȡ���ݴ���

//	*pdata = ECanbMboxes.MBOX1.MDL.all + ECanbMboxes.MBOX1.MDH.all <<32;

    pdata[3] = ECanbMboxes.MBOX1.MDL.byte.BYTE0;	
   	pdata[2] = ECanbMboxes.MBOX1.MDL.byte.BYTE1;
	pdata[1] = ECanbMboxes.MBOX1.MDL.byte.BYTE2;
	pdata[0] = ECanbMboxes.MBOX1.MDL.byte.BYTE3;

	pdata[7] = ECanbMboxes.MBOX1.MDH.byte.BYTE4;	
	pdata[6] = ECanbMboxes.MBOX1.MDH.byte.BYTE5;
	pdata[5] = ECanbMboxes.MBOX1.MDH.byte.BYTE6;
	pdata[4] = ECanbMboxes.MBOX1.MDH.byte.BYTE7;

	ECanbShadow.CANRMP.all     	= ECanbRegs.CANRMP.all;		      // ʹ�ܸ�����
	ECanbShadow.CANRMP.bit.RMP1	= 1;     
	ECanbRegs.CANRMP.all       	= ECanbShadow.CANRMP.all;

	return 1;
}





