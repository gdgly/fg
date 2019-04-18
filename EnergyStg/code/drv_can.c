#include "drv.h"
#include "drv_can.h"


 void drv_cana_init1(void)                                                                                                                                                                
 {                                                                                                                                                                                             
 	struct ECAN_REGS ECanaShadow;                                                                                                                                                              
                                                                                                                                                                                               
     InitECanaGpio();            // 来自TI，初始化ECana的GPIO                                                                                                                                  
 	InitECana();                // 来自TI，初始化ECana寄存器                                                                                                                                   
                                                                                                                                                                                               
     // 禁止所有邮箱		写入输ID号之前必须将邮箱屏蔽使用                                                                                                                                   
     ECanaRegs.CANME.all                =  0;                                                                                                                                                  
                                                                                                                                                                                               
     EALLOW;                                                                                                                                                                                   
     // 广播接收设置屏蔽位,屏蔽第0位...第11位,则标误符是的第12位...第28位是真正的标误符                                                                                                        
     ECanaLAMRegs.LAM1.all              =  0xFFFFFFFF;//0xFFFF0000;                                                                                                                            
                                                                                                                                                                                               
     ECanaRegs.CANMIM.all    = 0;   // 禁止所有中断                                                                                                                                            
     EDIS;                                                                                                                                                                                     
                                                                                                                                                                                               
     // 邮箱ID    扩展帧 PU                                                                                                                                                                    
     // 邮箱0：点对点，接收                                                                                                                                                                    
     //ECanaMboxes.MBOX0.MSGID.all      =  0x9555AAAA;                                                                                                                                         
     // 邮箱1：广播，接收  // 30位, 使能屏蔽,即C, 屏蔽的第0位...第11位用cmm_id来填充,这样各板的标误符就不相同了                                                                                
 //    ECanaMboxes.MBOX1.MSGID.all        =  0xC000A6A9;                                                                                                                                       
     // 邮箱4：点对点，发送                                                                                                                                                                    
     //ECanaMboxes.MBOX4.MSGID.all      =  0x9555AAAA;                                                                                                                                         
     // 邮箱5：广播，发送 // 发送不用使能屏蔽,即8, 屏蔽的第0位...第11位用cmm_id来填充,这样各板的标误符就不相同了                                                                               
     //ECanaMboxes.MBOX5.MSGID.all        =  0x85550000 + (unsigned long)(unsigned)( (drv_io.cmm_id<<12) + (drv_io.cmm_id<<8) + (drv_io.cmm_id<<4) + (drv_io.cmm_id) );                        
     ECanaMboxes.MBOX1.MSGID.all        =  0xC000A6A9;                                                                                                                                         
                                                                                                                                                                                               
     // 数据长度:8byte                                                                                                                                                                         
     ECanaMboxes.MBOX0.MSGCTRL.bit.DLC  =  8;                                                                                                                                                  
     ECanaMboxes.MBOX1.MSGCTRL.bit.DLC  =  8;                                                                                                                                                  
     ECanaMboxes.MBOX4.MSGCTRL.bit.DLC  =  8;                                                                                                                                                  
    //  ECanaMboxes.MBOX5.MSGCTRL.bit.DLC  =  8;                                                                                                                                               
                                                                                                                                                                                               
     // 邮箱方向     1100   0011    1接受  0 发送                                                                                                                                              
     ECanaRegs.CANMD.all                =  0xFFFFFFC3;                                                                                                                                         
                                                                                                                                                                                               
     EALLOW;                                                                                                                                                                                   
     ECanaShadow.CANMC.all              =  ECanaRegs.CANMC.all;                                                                                                                                
     // ECanaShadow.CANMC.bit.STM       =  1;  // 测试模式                                                                                                                                     
     ECanaRegs.CANMC.all                =  ECanaShadow.CANMC.all;                                                                                                                              
     EDIS;                                                                                                                                                                                     
                                                                                                                                                                                               
     // 使能邮箱0，1，4，5                                                                                                                                                                     
     ECanaRegs.CANME.all                =  0x00000033;                                                                                                                                         
 }                                                                                                                                                                                             
                                                                                                                                              
void drv_canb_init1(void)
{
	struct ECAN_REGS ECanbShadow;
    
    InitECanbGpio();            // 来自TI，初始化ECANB的GPIO
	InitECanb();                // 来自TI，初始化ECANB寄存器

    // 禁止所有邮箱		写入输ID号之前必须将邮箱屏蔽使用
    ECanbRegs.CANME.all                =  0;  

    EALLOW; 
    // 广播接收设置屏蔽位,屏蔽第0位...第11位,则标误符是的第12位...第28位是真正的标误符
    ECanbLAMRegs.LAM1.all              =  0xFFFFFFFF;//0xFFFF0000;

    ECanbRegs.CANMIM.all    = 0;   // 禁止所有中断
    EDIS; 

    // 邮箱ID    扩展帧 PU
    // 邮箱0：点对点，接收
    //ECanbMboxes.MBOX0.MSGID.all      =  0x9555AAAA;  
    // 邮箱1：广播，接收  // 30位, 使能屏蔽,即C, 屏蔽的第0位...第11位用cmm_id来填充,这样各板的标误符就不相同了
//    ECanbMboxes.MBOX1.MSGID.all        =  0xC000A6A9;
    // 邮箱4：点对点，发送
    //ECanbMboxes.MBOX4.MSGID.all      =  0x9555AAAA; 
    // 邮箱5：广播，发送 // 发送不用使能屏蔽,即8, 屏蔽的第0位...第11位用cmm_id来填充,这样各板的标误符就不相同了
    //ECanbMboxes.MBOX5.MSGID.all        =  0x85550000 + (unsigned long)(unsigned)( (drv_io.cmm_id<<12) + (drv_io.cmm_id<<8) + (drv_io.cmm_id<<4) + (drv_io.cmm_id) );  
    ECanbMboxes.MBOX1.MSGID.all        =  0xC000A6A9;

    // 数据长度:8byte
    ECanbMboxes.MBOX0.MSGCTRL.bit.DLC  =  8;
    ECanbMboxes.MBOX1.MSGCTRL.bit.DLC  =  8; 
    ECanbMboxes.MBOX4.MSGCTRL.bit.DLC  =  8; 
   //  ECanbMboxes.MBOX5.MSGCTRL.bit.DLC  =  8; 
    
    // 邮箱方向     1100   0011    1接受  0 发送
    ECanbRegs.CANMD.all                =  0xFFFFFFC3; 

    EALLOW; 
    ECanbShadow.CANMC.all              =  ECanbRegs.CANMC.all; 
    // ECanbShadow.CANMC.bit.STM       =  1;  // 测试模式   
    ECanbRegs.CANMC.all                =  ECanbShadow.CANMC.all; 
    EDIS; 

    // 使能邮箱0，1，4，5
    ECanbRegs.CANME.all                =  0x00000033;     
}
            
// 描述：广播发送消息函数                                                                                                         
///////////////////////////////////////////////////////////////////////////////////////                                           
                                                                                                                                  
void drv_cana_wrmsg (unsigned long  Identifier, unsigned char dlc, unsigned char data[8])                                         
                                                                                                                                  
{                                                                                                                                 
    struct ECAN_REGS ECanaShadow;                                                                                                 
                                                                                                                                  
	                                                                                                                              
	// 如果邮箱消息没发完，就返回。  成功发送后TA5位被置位； 当邮箱n准备发送时，CPU会将所有的TRS[n]位设置为1,启动发送             
    if (ECanaRegs.CANTRS.bit.TRS5) 		return;                                                                                   
                                                                                                                                  
	ECanaShadow.CANME.all 	  = ECanaRegs.CANME.all;		// 发送前必须禁止该邮箱                                               
	ECanaShadow.CANME.bit.ME5 = 0;                                                                                                
	ECanaRegs.CANME.all       = ECanaShadow.CANME.all;                                                                            
                                                                                                                                  
	ECanaMboxes.MBOX5.MSGID.all =  Identifier + 0x80000000;                                                                       
	ECanaMboxes.MBOX5.MSGCTRL.bit.DLC  =  dlc ;                                                                                   
                                                                                                                                  
	ECanaMboxes.MBOX5.MDL.byte.BYTE0 = data[3];	   // 低32位                                                                      
    ECanaMboxes.MBOX5.MDL.byte.BYTE1 = data[2];                                                                                   
	ECanaMboxes.MBOX5.MDL.byte.BYTE2 = data[1];                                                                                   
	ECanaMboxes.MBOX5.MDL.byte.BYTE3 = data[0];                                                                                   
                                                                                                                                  
    ECanaMboxes.MBOX5.MDH.byte.BYTE4 = data[7];	    // 高32位                                                                     
   	ECanaMboxes.MBOX5.MDH.byte.BYTE5 = data[6];                                                                                   
	ECanaMboxes.MBOX5.MDH.byte.BYTE6 = data[5];                                                                                   
	ECanaMboxes.MBOX5.MDH.byte.BYTE7 = data[4];                                                                                   
                                                                                                                                  
	                                                                                                                              
                                                                                                                                  
	ECanaShadow.CANME.all  		= ECanaRegs.CANME.all;		// 使能该邮箱                                                         
	ECanaShadow.CANME.bit.ME5 	= 1;                                                                                              
	ECanaRegs.CANME.all       	= ECanaShadow.CANME.all;                                                                          
	                                                                                                                              
	ECanaShadow.CANTRS.all      = ECanaRegs.CANTRS.all;                                                                           
	ECanaShadow.CANTRS.bit.TRS5 = 1; // 置发送请求                                                                                
	ECanaRegs.CANTRS.all        = ECanaShadow.CANTRS.all;                                                                         
}                                                                                                                                 






// 描述：广播发送消息函数
///////////////////////////////////////////////////////////////////////////////////////

void drv_canb_wrmsg (unsigned long  Identifier, unsigned char dlc, unsigned char data[8])  

{
    struct ECAN_REGS ECanbShadow;

	
	// 如果邮箱消息没发完，就返回。  成功发送后TA5位被置位； 当邮箱n准备发送时，CPU会将所有的TRS[n]位设置1,启动发送
    if (ECanbRegs.CANTRS.bit.TRS5) 		return;

	ECanbShadow.CANME.all 	  = ECanbRegs.CANME.all;		// 发送前必须禁止该邮箱
	ECanbShadow.CANME.bit.ME5 = 0;
	ECanbRegs.CANME.all       = ECanbShadow.CANME.all;

	ECanbMboxes.MBOX5.MSGID.all =  Identifier + 0x80000000;
	ECanbMboxes.MBOX5.MSGCTRL.bit.DLC  =  dlc ;

	ECanbMboxes.MBOX5.MDL.byte.BYTE0 = data[3];	   // 低32位
    ECanbMboxes.MBOX5.MDL.byte.BYTE1 = data[2];
	ECanbMboxes.MBOX5.MDL.byte.BYTE2 = data[1];
	ECanbMboxes.MBOX5.MDL.byte.BYTE3 = data[0];

    ECanbMboxes.MBOX5.MDH.byte.BYTE4 = data[7];	    // 高32位
   	ECanbMboxes.MBOX5.MDH.byte.BYTE5 = data[6];
	ECanbMboxes.MBOX5.MDH.byte.BYTE6 = data[5];
	ECanbMboxes.MBOX5.MDH.byte.BYTE7 = data[4];

	

	ECanbShadow.CANME.all  		= ECanbRegs.CANME.all;		// 使能该邮箱
	ECanbShadow.CANME.bit.ME5 	= 1;     
	ECanbRegs.CANME.all       	= ECanbShadow.CANME.all;
	
	ECanbShadow.CANTRS.all      = ECanbRegs.CANTRS.all;
	ECanbShadow.CANTRS.bit.TRS5 = 1; // 置发送请求
	ECanbRegs.CANTRS.all        = ECanbShadow.CANTRS.all;
}


 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////      
 // 描述：Cana接收(查询接收)                                                                                                            
                                                                                                                                        
 unsigned  drv_cana_rdmsg (unsigned long* pIdentifier, unsigned char pdata[8])                                                          
 {                                                                                                                                      
     struct ECAN_REGS ECanaShadow;                                                                                                      
                                                                                                                                        
     // CAN邮箱1接收判断  // 邮箱1：广播，接收                                                                                          
     if (!ECanaRegs.CANRMP.bit.RMP1)  	return 0;                                                                                       
                                                                                                                                        
 	ECanaRegs.CANRMP.bit.RMP1 = 1;   							 // 清零邮箱1接收完成标志                                               
 	     	                                                                                                                            
    * pIdentifier = ECanaMboxes.MBOX1.MSGID.all & 0x1FFFFFFF;                                                                           
                                                                                                                                        
 	//真正读取数据代码                                                                                                                  
                                                                                                                                        
 //	*pdata = ECanaMboxes.MBOX1.MDL.all + ECanaMboxes.MBOX1.MDH.all <<32;                                                                  
                                                                                                                                          
    pdata[3] = ECanaMboxes.MBOX1.MDL.byte.BYTE0;	                                                                                      
    pdata[2] = ECanaMboxes.MBOX1.MDL.byte.BYTE1;                                                                                      
 	pdata[1] = ECanaMboxes.MBOX1.MDL.byte.BYTE2;                                                                                          
 	pdata[0] = ECanaMboxes.MBOX1.MDL.byte.BYTE3;                                                                                        
                                                                                                                                        
 	pdata[7] = ECanaMboxes.MBOX1.MDH.byte.BYTE4;	                                                                                    
 	pdata[6] = ECanaMboxes.MBOX1.MDH.byte.BYTE5;                                                                                        
 	pdata[5] = ECanaMboxes.MBOX1.MDH.byte.BYTE6;                                                                                        
 	pdata[4] = ECanaMboxes.MBOX1.MDH.byte.BYTE7;                                                                                        
                                                                                                                                        
 	ECanaShadow.CANRMP.all     	= ECanaRegs.CANRMP.all;		      // 使能该邮箱                                                         
 	ECanaShadow.CANRMP.bit.RMP1	= 1;                                                                                                    
 	ECanaRegs.CANRMP.all       	= ECanaShadow.CANRMP.all;                                                                               
                                                                                                                                        
 	return 1;                                                                                                                             
 }                                                                                                                                        


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 描述：canb接收(查询接收)

unsigned  drv_canb_rdmsg (unsigned long* pIdentifier, unsigned char pdata[8])
{
    struct ECAN_REGS ECanbShadow;

    // CAN邮箱1接收判断  // 邮箱1：广播，接收 
    if (!ECanbRegs.CANRMP.bit.RMP1)  	return 0;
  
	ECanbRegs.CANRMP.bit.RMP1 = 1;   							 // 清零邮箱1接收完成标志 
	     	
   * pIdentifier = ECanbMboxes.MBOX1.MSGID.all & 0x1FFFFFFF;
    
	//真正读取数据代码

//	*pdata = ECanbMboxes.MBOX1.MDL.all + ECanbMboxes.MBOX1.MDH.all <<32;

    pdata[3] = ECanbMboxes.MBOX1.MDL.byte.BYTE0;	
   	pdata[2] = ECanbMboxes.MBOX1.MDL.byte.BYTE1;
	pdata[1] = ECanbMboxes.MBOX1.MDL.byte.BYTE2;
	pdata[0] = ECanbMboxes.MBOX1.MDL.byte.BYTE3;

	pdata[7] = ECanbMboxes.MBOX1.MDH.byte.BYTE4;	
	pdata[6] = ECanbMboxes.MBOX1.MDH.byte.BYTE5;
	pdata[5] = ECanbMboxes.MBOX1.MDH.byte.BYTE6;
	pdata[4] = ECanbMboxes.MBOX1.MDH.byte.BYTE7;

	ECanbShadow.CANRMP.all     	= ECanbRegs.CANRMP.all;		      // 使能该邮箱
	ECanbShadow.CANRMP.bit.RMP1	= 1;     
	ECanbRegs.CANRMP.all       	= ECanbShadow.CANRMP.all;

	return 1;
}





