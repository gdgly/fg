

#ifndef __DRV_CAN_H__
#define __DRV_CAN_H__




//----------------------- º¯ÊýÉùÃ÷Çø--------------------------------//

void drv_cana_init1 (void); 
void drv_canb_init1 (void); 
void drv_cana_wrmsg (unsigned long  Identifier, unsigned char dlc, unsigned char data[8]);  
void drv_canb_wrmsg (unsigned long  Identifier, unsigned char dlc, unsigned char data[8]);  
unsigned  drv_cana_rdmsg(unsigned long * pIdentifier, unsigned char pdata[8]);
unsigned  drv_canb_rdmsg(unsigned long * pIdentifier, unsigned char pdata[8]);

#endif  // __DRV_CAN_H__



