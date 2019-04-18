/***********************************************
文件名：Progload28335_CANLib.h
功能: 	CAN总线下载程序头文件
************************************************/
#ifndef _H_Progload28335_CANLib
#define _H_Progload28335_CANLib


#define CPU_RATE    6.667L   // for a 150MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE    7.143L   // for a 140MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE    8.333L   // for a 120MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   10.000L   // for a 100MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   13.330L   // for a 75MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   20.000L   // for a 50MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   33.333L   // for a 30MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   41.667L   // for a 24MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   50.000L   // for a 20MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   66.667L   // for a 15MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE  100.000L   // for a 10MHz CPU clock speed  (SYSCLKOUT)
#define SCALE_FACTOR  1048576.0L*( (200L/CPU_RATE) )  // IQ20  
extern Uint32 Flash_CPUScaleFactor;

extern void ProgramLoadInitCANA(unsigned int RMailID,unsigned int TMailID);
extern void ProgramLoadInitCANB(unsigned int RMailID,unsigned int TMailID);
extern void ProgramLoadEntryCANA();
extern void ProgramLoadEntryCANB();
extern unsigned int Progload28335_CAN_LoadStart;
extern unsigned int Progload28335_CAN_LoadEnd;
extern unsigned int Progload28335_CAN_RunStart; 

/*Lib库中变量声明*/
extern unsigned int Flash28_API_LoadStart;
extern unsigned int Flash28_API_LoadEnd;
extern unsigned int Flash28_API_RunStart; 

#endif

