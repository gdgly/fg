// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:46:38 $
//###########################################################################
//
// FILE:	DSP2833x_PieVect.c
//
// TITLE:	DSP2833x Devices PIE Vector Table Initialization Functions.
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.10 $
// $Release Date: February 15, 2008 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


const struct PIE_VECT_TABLE PieVectTableInit = {

      PIE_RESERVED,  // 0  Reserved space
      PIE_RESERVED,  // 1  Reserved space 
      PIE_RESERVED,  // 2  Reserved space 
      PIE_RESERVED,  // 3  Reserved space 
      PIE_RESERVED,  // 4  Reserved space 
      PIE_RESERVED,  // 5  Reserved space 
      PIE_RESERVED,  // 6  Reserved space 
      PIE_RESERVED,  // 7  Reserved space 
      PIE_RESERVED,  // 8  Reserved space 
      PIE_RESERVED,  // 9  Reserved space 
      PIE_RESERVED,  // 10 Reserved space 
      PIE_RESERVED,  // 11 Reserved space 
      PIE_RESERVED,  // 12 Reserved space 


// Non-Peripheral Interrupts
      PIE_RESERVED,	//INT13_ISR,     // XINT13 or CPU-Timer 1
      PIE_RESERVED,	//INT14_ISR,     // CPU-Timer2
      PIE_RESERVED,	//DATALOG_ISR,   // Datalogging interrupt
      PIE_RESERVED,	//RTOSINT_ISR,   // RTOS interrupt
      PIE_RESERVED,	//EMUINT_ISR,    // Emulation interrupt
      PIE_RESERVED,	//NMI_ISR,       // Non-maskable interrupt
      PIE_RESERVED,	//ILLEGAL_ISR,   // Illegal operation TRAP
      PIE_RESERVED,	//USER1_ISR,     // User Defined trap 1
      PIE_RESERVED,	//USER2_ISR,     // User Defined trap 2
      PIE_RESERVED,	//USER3_ISR,     // User Defined trap 3
      PIE_RESERVED,	//USER4_ISR,     // User Defined trap 4
      PIE_RESERVED,	//USER5_ISR,     // User Defined trap 5
      PIE_RESERVED,	//USER6_ISR,     // User Defined trap 6
      PIE_RESERVED,	//USER7_ISR,     // User Defined trap 7
      PIE_RESERVED,	//USER8_ISR,     // User Defined trap 8
      PIE_RESERVED,	//USER9_ISR,     // User Defined trap 9
      PIE_RESERVED,	//USER10_ISR,    // User Defined trap 10
      PIE_RESERVED,	//USER11_ISR,    // User Defined trap 11
      PIE_RESERVED,	//USER12_ISR,    // User Defined trap 12

// Group 1 PIE Vectors
      PIE_RESERVED,	//SEQ1INT_ISR,     // 1.1 ADC  
      PIE_RESERVED,	//SEQ2INT_ISR,     // 1.2 ADC  
      PIE_RESERVED,	//rsvd_ISR,        // 1.3
      PIE_RESERVED,	//XINT1_ISR,       // 1.4     
      PIE_RESERVED,	//XINT2_ISR,       // 1.5
      PIE_RESERVED,	//ADCINT_ISR,      // 1.6 ADC
      PIE_RESERVED,	//TINT0_ISR,       // 1.7 Timer 0
      PIE_RESERVED,	//WAKEINT_ISR,     // 1.8 WD, Low Power

// Group 2 PIE Vectors
      PIE_RESERVED,	//EPWM1_TZINT_ISR, // 2.1 EPWM-1 Trip Zone
      PIE_RESERVED,	//EPWM2_TZINT_ISR, // 2.2 EPWM-2 Trip Zone
      PIE_RESERVED,	//EPWM3_TZINT_ISR, // 2.3 EPWM-3 Trip Zone
      PIE_RESERVED,	//EPWM4_TZINT_ISR, // 2.4 EPWM-4 Trip Zone
      PIE_RESERVED,	//EPWM5_TZINT_ISR, // 2.5 EPWM-5 Trip Zone
      PIE_RESERVED,	//EPWM6_TZINT_ISR, // 2.6 EPWM-6 Trip Zone                        
      PIE_RESERVED,	//rsvd_ISR,        // 2.7
      PIE_RESERVED,	//rsvd_ISR,        // 2.8
      
// Group 3 PIE Vectors
      PIE_RESERVED,	//EPWM1_INT_ISR,   // 3.1 EPWM-1 Interrupt
      PIE_RESERVED,	//EPWM2_INT_ISR,   // 3.2 EPWM-2 Interrupt
      PIE_RESERVED,	//EPWM3_INT_ISR,   // 3.3 EPWM-3 Interrupt
      PIE_RESERVED,	//EPWM4_INT_ISR,   // 3.4 EPWM-4 Interrupt
      PIE_RESERVED,	//EPWM5_INT_ISR,   // 3.5 EPWM-5 Interrupt
      PIE_RESERVED,	//EPWM6_INT_ISR,   // 3.6 EPWM-6 Interrupt                        
      PIE_RESERVED,	//rsvd_ISR,        // 3.7
      PIE_RESERVED,	//rsvd_ISR,        // 3.8
      
// Group 4 PIE Vectors
      PIE_RESERVED,	//ECAP1_INT_ISR,   // 4.1 ECAP-1
      PIE_RESERVED,	//ECAP2_INT_ISR,   // 4.2 ECAP-2
      PIE_RESERVED,	//ECAP3_INT_ISR,   // 4.3 ECAP-3
      PIE_RESERVED,	//ECAP4_INT_ISR,   // 4.4 ECAP-4            
      PIE_RESERVED,	//ECAP5_INT_ISR,   // 4.5 ECAP-5     
      PIE_RESERVED,	//ECAP6_INT_ISR,   // 4.6 ECAP-6          
      PIE_RESERVED,	//rsvd_ISR,        // 4.7      
      PIE_RESERVED,	//rsvd_ISR,        // 4.8      
      
// Group 5 PIE Vectors
      PIE_RESERVED,//EQEP1_INT_ISR,   // 5.1 EQEP-1
      PIE_RESERVED,//EQEP2_INT_ISR,   // 5.2 EQEP-2
      PIE_RESERVED,//rsvd_ISR,        // 5.3      
      PIE_RESERVED,//rsvd_ISR,        // 5.4           
      PIE_RESERVED,//rsvd_ISR,        // 5.5      
      PIE_RESERVED,//rsvd_ISR,        // 5.6      
      PIE_RESERVED,//rsvd_ISR,        // 5.7      
      PIE_RESERVED,//rsvd_ISR,        // 5.8   


// Group 6 PIE Vectors
      PIE_RESERVED,	//SSPIRXINTA_ISR,   // 6.1 SPI-A
      PIE_RESERVED,	//SSPITXINTA_ISR,   // 6.2 SPI-A
      PIE_RESERVED,	//SMRINTA_ISR,      // 6.3 McBSP-A
      PIE_RESERVED,	//SMXINTA_ISR,      // 6.4 McBSP-A
      PIE_RESERVED,	//SMRINTB_ISR,      // 6.5 McBSP-B
      PIE_RESERVED,	//SMXINTB_ISR,      // 6.6 McBSP-B
      PIE_RESERVED,	//Srsvd_ISR,        // 6.7 
      PIE_RESERVED,	//Srsvd_ISR,        // 6.8                  

      
// Group 7 PIE Vectors
      PIE_RESERVED,	//DINTCH1_ISR,     // 7.1  DMA channel 1    
      PIE_RESERVED,	//DINTCH2_ISR,     // 7.2  DMA channel 2
      PIE_RESERVED,	//DINTCH3_ISR,     // 7.3  DMA channel 3      
      PIE_RESERVED,	//DINTCH4_ISR,     // 7.4  DMA channel 4           
      PIE_RESERVED,	//DINTCH5_ISR,     // 7.5  DMA channel 5      
      PIE_RESERVED,	//DINTCH6_ISR,     // 7.6  DMA channel 6      
      PIE_RESERVED,	//rsvd_ISR,        // 7.7      
      PIE_RESERVED,	//rsvd_ISR,        // 7.8   

// Group 8 PIE Vectors
      PIE_RESERVED,	//I2CINT1A_ISR,    // 8.1  I2C    
      PIE_RESERVED,	//I2CINT2A_ISR,    // 8.2  I2C
      PIE_RESERVED,	//rsvd_ISR,        // 8.3      
      PIE_RESERVED,	//rsvd_ISR,        // 8.4           
      PIE_RESERVED,	//SCIRXINTC_ISR,   // 8.5  SCI-C
      PIE_RESERVED,	//SCITXINTC_ISR,   // 8.6  SCI-C      
      PIE_RESERVED,	//rsvd_ISR,        // 8.7      
      PIE_RESERVED,	//rsvd_ISR,        // 8.8    
      
// Group 9 PIE Vectors     
      PIE_RESERVED,	//SCIRXINTA_ISR,   // 9.1 SCI-A  
      PIE_RESERVED,	//SCITXINTA_ISR,   // 9.2 SCI-A  
      PIE_RESERVED,	//SCIRXINTB_ISR,   // 9.3 SCI-B
      PIE_RESERVED,	//SCITXINTB_ISR,   // 9.4 SCI-B
      PIE_RESERVED,	//ECAN0INTA_ISR,   // 9.5 eCAN-A
      PIE_RESERVED,	//ECAN1INTA_ISR,   // 9.6 eCAN-A
      PIE_RESERVED,	//ECAN0INTB_ISR,   // 9.7 eCAN-B
      PIE_RESERVED,	//ECAN1INTB_ISR,   // 9.8 eCAN-B
      
// Group 10 PIE Vectors
      PIE_RESERVED,	//rsvd_ISR,        // 10.1      
      PIE_RESERVED,	//rsvd_ISR,        // 10.2
      PIE_RESERVED,	//rsvd_ISR,        // 10.3      
      PIE_RESERVED,	//rsvd_ISR,        // 10.4           
      PIE_RESERVED,	//rsvd_ISR,        // 10.5      
      PIE_RESERVED,	//rsvd_ISR,        // 10.6      
      PIE_RESERVED,	//rsvd_ISR,        // 10.7      
      PIE_RESERVED,	//rsvd_ISR,        // 10.8    
            
// Group 11 PIE Vectors
      PIE_RESERVED,	//rsvd_ISR,        // 11.1      
      PIE_RESERVED,	//rsvd_ISR,        // 11.2
      PIE_RESERVED,	//rsvd_ISR,        // 11.3      
      PIE_RESERVED,	//rsvd_ISR,        // 11.4           
      PIE_RESERVED,	//rsvd_ISR,        // 11.5      
      PIE_RESERVED,	//rsvd_ISR,        // 11.6      
      PIE_RESERVED,	//rsvd_ISR,        // 11.7      
      PIE_RESERVED,	//rsvd_ISR,        // 11.8
        
// Group 12 PIE Vectors
      PIE_RESERVED,	//XINT3_ISR,       // 12.1      
      PIE_RESERVED,	//XINT4_ISR,       // 12.2
      PIE_RESERVED,	//XINT5_ISR,       // 12.3      
      PIE_RESERVED,	//XINT6_ISR,       // 12.4           
      PIE_RESERVED,	//XINT7_ISR,       // 12.5      
      PIE_RESERVED,	//rsvd_ISR,        // 12.6      
      PIE_RESERVED,	//LVF_ISR,         // 12.7      
      PIE_RESERVED,	//LUF_ISR,         // 12.8   
};


//---------------------------------------------------------------------------
// InitPieVectTable: 
//---------------------------------------------------------------------------
// This function initializes the PIE vector table to a known state.
// This function must be executed after boot time.
//

void InitPieVectTable(void)
{
	Uint16	i;
	Uint32 *Source = (void *) &PieVectTableInit;
	Uint32 *Dest = (void *) &PieVectTable;
		
	EALLOW;	
	for(i=0; i < 128; i++)
		*Dest++ = *Source++;	
	EDIS;

	// Enable the PIE Vector Table
	//PieCtrlRegs.PIECTRL.bit.ENPIE = 1;		//фа╠н
			
}

//===========================================================================
// End of file.
//===========================================================================
