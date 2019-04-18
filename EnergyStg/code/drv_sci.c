#include "drv.h"
#include "drv_sci.h"


// CRC 高位字节值表
static unsigned auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;

// CRC 低位字节值表
static unsigned auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;

// 函数：uint16_t CRC16(uint8_t * pMsg, uint16_t Len)
// 功能：计算给定数据的CRC16值，并返回。
// 参数：pMsg 数据指针，Len 数据长度。

unsigned CRC16(unsigned * pMsg, unsigned Len)
{
	unsigned i; 
	unsigned uchCRCHi,uchCRCLo;	    
	
	uchCRCHi = 0xFF;     
	uchCRCLo = 0xFF;     	
	while (Len--)         
	{
		i = uchCRCHi ^ (*pMsg++);   
		uchCRCHi = uchCRCLo ^ auchCRCHi[i];
		uchCRCLo = auchCRCLo[i];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}



// 描述：scic寄存器初始化;只能一次,停机时不能重新初始化
// 作者：何超
// 版本：4.0
// 日期：星期二,2013-1-22

void drv_scib_init1(void)
{
	InitScibGpio();									  // 来自TI，初始化Scib寄存器


    ScibRegs.SCICCR.all                  = 0x0007;    //  以下是展开解释
    // ScibRegs.SCICCR.bit.STOPBITS      = 0;		  //  1个停止位
    // ScibRegs.SCICCR.bit.PARITYENA   	 = 0;		  //  无奇偶校验
    // ScibRegs.SCICCR.bit.LOOPBKENA     = 0;         //  自测模式禁止,与第一句有重复, 暂保留
    // ScibRegs.SCICCR.bit.ADDRIDLE_MODE = 0;		  //  空闲线模式
    // ScibRegs.SCICCR.bit.SCICHAR 		 = 7;		  //  8个数据位,

    //  使能TX,  使能RX,  internal SCICLK,
    //  禁止接收错误中断, 禁止休眠,  发送唤醒未使能
    ScibRegs.SCICTL1.all               =  0x0003;

    ScibRegs.SCICTL2.bit.TXINTENA      =  0;      	  // 关发送中断
    ScibRegs.SCICTL2.bit.RXBKINTENA    =  0;      	  // 关接收中断

//	ScibRegs.SCIHBAUD                  =  0x00;		  // 波特率设为115200,BRR =  = 0x
//	ScibRegs.SCILBAUD                  =  0xa2;
	ScibRegs.SCIHBAUD                  =  0x07;		  // 波特率设为9600,BRR =  = 0x
	ScibRegs.SCILBAUD                  =  0xa0;


    ScibRegs.SCIFFTX.all                  =  0xC000;  // 0xE121; 以下是展开解释
    // ScibRegs.SCIFFTX.bit.SCIRST        = 1;        // SCI复位
    // ScibRegs.SCIFFTX.bit.SCIFFENA      = 1;        // SCI FIFO使能
    // ScibRegs.SCIFFTX.bit.TXFIFOXRESET  = 0;        // 发送FIFO复位位; 0:复位FIFO指针为0,并保持复位状态
    // ScibRegs.SCIFFTX.bit.TXFFST        = 0;        // 5位:发送FIFO为空,只读位
    // ScibRegs.SCIFFTX.bit.TXFFINT       = 0;        // 发送FIFO断位；0:TXFIFO中断没有发生,只读位
    // ScibRegs.SCIFFTX.bit.TXFFINTCLR    = 0;        // 发送FIFO中断清除位; 0:对TXFFINT写入0无效
	// ScibRegs.SCIFFTX.bit.TXFFIENA      = 0;        // 发送FIFO中鲜鼓?
	// ScibRegs.SCIFFTX.bit.TXFFIL        = 0;        // 5?: 发送FIFO中断级别为1

    ScibRegs.SCIFFRX.all                  =  0x0001;  // 0x2021;以下是展开解释
    // ScibRegs.SCIFFRX.bit.RXFFOVF       = 0; 		  // 接收FIFO未发生溢出,只读位
    // ScibRegs.SCIFFRX.bit.RXFFOVRCLR    = 0; 		  // RXFFOVF清除位; 0:写入0对RXFFOVF瓯志位无效,读取此位为0
    // ScibRegs.SCIFFRX.bit.RXFIFORESET   = 0; 		  // 接收FIFO复位位; 0:复位FIFO指针为0,并保持复位状态
    // ScibRegs.SCIFFRX.bit.RXFFST        = 0; 		  // 5位: 接收FIFO为空,只读位
    // ScibRegs.SCIFFRX.bit.RXFFINT       = 0; 		  // 接收FIFO中断位；0:RXFIFO中断没有发生,只读位
    // ScibRegs.SCIFFRX.bit.RXFFINTCLR    = 0; 		  // 接收FIFO中断清除位 0:对RXFFINT写入0无效
    // ScibRegs.SCIFFRX.bit.RXFFIENA      = 0; 		  // 接收FIFO中断禁止
    // ScibRegs.SCIFFRX.bit.RXFFIL        = 1; 		  // 5位:接收FIFO中断等级为1

    ScibRegs.SCIFFCT.all               =  0x01;    // 0x0f; FIFO传送延时

	ScibRegs.SCICTL1.bit.SWRESET	   = 1;		   // SCI软件复位

    ScibRegs.SCIPRI.bit.FREE           = 0;        // 挂起时立即停止
    ScibRegs.SCIFFTX.bit.TXFIFOXRESET  = 1;		   // 发送FIFO复位
    ScibRegs.SCIFFRX.bit.RXFIFORESET   = 1;		   // 接收FIFO复位
}






// 描述：这是个通用发送，具体发送便宜地址，参考头文件。基本对外，不直接引用。
// 作者：何超
// 版本：4.0
// 日期：星期二,2013-1-22

#define		LEN				5+2		// 长度	加2个字节的CRC
#define		RD				0x82	// 写变量存储器命令,即发数据到屏
#define		WR				0x83	// 读变量存储器命令,即要求屏发来数据
#define		SOI_H			0xAA	// 帧头高字节
#define		SOI_L			0x55	// 帧头低字节

void drv_scib_write(unsigned addr,unsigned data)
{
	unsigned crc = 0;
	unsigned scib_send[5];

	ScibRegs.SCITXBUF = SOI_H;		ScibRegs.SCITXBUF = SOI_L;			// 帧头
	ScibRegs.SCITXBUF = LEN;		ScibRegs.SCITXBUF = RD;				// 长度和命令
	ScibRegs.SCITXBUF = addr>>8;	ScibRegs.SCITXBUF = addr;			// 地址
	ScibRegs.SCITXBUF = data>>8;	ScibRegs.SCITXBUF = data;			// 数据

	scib_send[0] = RD;		scib_send[1] = addr>>8;		scib_send[2] = addr&0x00FF;	
	scib_send[3] = data>>8;	scib_send[4] = data&0x00FF;	
	crc = CRC16(scib_send, 5 );
	ScibRegs.SCITXBUF = crc>>8;		ScibRegs.SCITXBUF = crc;			// 
}

// 描述：scib发送读屏命令 //没有接收完一帧完整的数据都返回0,成功接收一帧数据返回1
// 作者：何超
// 版本：4.0
// 日期：星期二,2013-1-22

#define		RESET_RXFIFO()	drv_scib_init1();ScibRegs.SCICTL1.bit.SWRESET	   = 0;		   /*SCI软件复位*/		\
							ScibRegs.SCIFFTX.bit.TXFIFOXRESET  = 0;		   /*发送FIFO复位*/		\
							ScibRegs.SCIFFRX.bit.RXFIFORESET   = 0;	/*接收FIFO复位*/			\
							NOP3();																\
							ScibRegs.SCIFFRX.bit.RXFIFORESET   = 1;	/*再次使芙邮誇IFO*/		\
							ScibRegs.SCIFFTX.bit.TXFIFOXRESET  = 1;		   /*发送FIFO复位*/		\
							ScibRegs.SCICTL1.bit.SWRESET	   = 1;		   /*SCI软件复位*/

#define		READ_FIFO()		ScibRegs.SCIRXBUF.bit.RXDT		// 空读2次接收FIFO

unsigned drv_scib_read(unsigned addr, unsigned *pdata)
{
	static unsigned n	  = 0;
	static unsigned bflag = 0;
	unsigned data;
	unsigned crc_calu = 0, crc_read = 0;
	unsigned crc = 0;
	unsigned scib_send[4];
	unsigned scib_receive[6];
	unsigned command = 0, addr_readh = 0, addr_readl = 0, bytes = 0;


	if (!bflag){
		ScibRegs.SCITXBUF = SOI_H;		ScibRegs.SCITXBUF = SOI_L;			// 帧头
		ScibRegs.SCITXBUF = LEN-1;		ScibRegs.SCITXBUF = WR;				// 长度和命令
		ScibRegs.SCITXBUF = addr>>8;	ScibRegs.SCITXBUF = addr;			// 地址
		ScibRegs.SCITXBUF = 1;												// 长度

		scib_send[0] = WR;		scib_send[1] = addr>>8;		scib_send[2] = addr&0x00FF;	
		scib_send[3] = 1;	
		crc = CRC16(scib_send, 4 );
		ScibRegs.SCITXBUF = crc>>8;		ScibRegs.SCITXBUF = crc;			// 

		bflag = 1;
		return 0;
	}else {			// scib接收屏回应的信息

		if (ScibRegs.SCIFFRX.bit.RXFFST != 11){								// 返回的一帧数据应该有9个byre数据
			n++;
			if (n > 5)	{RESET_RXFIFO();	n = 0;	bflag = 0;}		
			return 0;
		}else{
			n = 0;		bflag = 0;
		}
		if (READ_FIFO() != SOI_H	||	READ_FIFO() != SOI_L /*|| 	READ_FIFO() != LEN*/)	{ RESET_RXFIFO();	return 0;}
		READ_FIFO();	command = READ_FIFO();	//	空读2次接收FIFO
		
		addr_readh = READ_FIFO();	addr_readl = READ_FIFO();
		if (addr_readh != (addr>>8) || addr_readl != (addr & 0x00FF))						{ RESET_RXFIFO();	return 0;}
		bytes = READ_FIFO();					//	空读1次接收FIFO

		data   =	(READ_FIFO()<<8);
		data  +=	READ_FIFO();		//	读数据
		*pdata =	data;
		
		crc_read   =	(READ_FIFO()<<8);
		crc_read  +=	READ_FIFO();			//	读CRC
		scib_receive[0] = command;	scib_receive[1] = addr_readh;	scib_receive[2] = addr_readl;	
		scib_receive[3] = bytes;	scib_receive[4] = data>>8;		scib_receive[5] = data&0x00FF;	
		crc_calu = CRC16(scib_receive, 6 );
		if (crc_calu != crc_read)	return 0;	// CRC校验错误

		return 1;
	}
}













// 描述：scic寄存器初始化;只能一次,停机时不能重新初始化
// 作者：何超
// 版本：4.0
// 日期：星期二,2013-1-22

void drv_scic_init1(void)
{
	InitScicGpio();									  // 来自TI，初始化Scic寄存器


    ScicRegs.SCICCR.all                  = 0x0007;    //  以下是展开解释
    // ScicRegs.SCICCR.bit.STOPBITS      = 0;		  //  1个停止位
    // ScicRegs.SCICCR.bit.PARITYENA   	 = 0;		  //  无奇偶校验
    // ScicRegs.SCICCR.bit.LOOPBKENA     = 0;         //  自测模式禁止,与第一句有重复, 暂保留
    // ScicRegs.SCICCR.bit.ADDRIDLE_MODE = 0;		  //  空闲线模式
    // ScicRegs.SCICCR.bit.SCICHAR 		 = 7;		  //  8个数据位,

    //  使能TX,  使能RX,  internal SCICLK,
    //  禁止接收错误中断, 禁止休眠,  发送唤醒未使能
    ScicRegs.SCICTL1.all               =  0x0003;

    ScicRegs.SCICTL2.bit.TXINTENA      =  0;      	  // 关发送中断
    ScicRegs.SCICTL2.bit.RXBKINTENA    =  0;      	  // 关接收中断

//	ScicRegs.SCIHBAUD                  =  0x00;		  // 波特率设为115200,BRR =  = 0x
//	ScicRegs.SCILBAUD                  =  0xa2;
    ScicRegs.SCIHBAUD                  =  0x07;		  // 波特率设为9600,BRR =  = 0x
    ScicRegs.SCILBAUD                  =  0xa0;


    ScicRegs.SCIFFTX.all                  =  0xC000;  // 0xE121; 以下是展开解释
    // ScicRegs.SCIFFTX.bit.SCIRST        = 1;        // SCI复位
    // ScicRegs.SCIFFTX.bit.SCIFFENA      = 1;        // SCI FIFO使能
    // ScicRegs.SCIFFTX.bit.TXFIFOXRESET  = 0;        // 发送FIFO复位位; 0:复位FIFO指针为0,并保持复位状态
    // ScicRegs.SCIFFTX.bit.TXFFST        = 0;        // 5位:发送FIFO为空,只读位
    // ScicRegs.SCIFFTX.bit.TXFFINT       = 0;        // 发送FIFO断位；0:TXFIFO中断没有发生,只读位
    // ScicRegs.SCIFFTX.bit.TXFFINTCLR    = 0;        // 发送FIFO中断清除位; 0:对TXFFINT写入0无效
	// ScicRegs.SCIFFTX.bit.TXFFIENA      = 0;        // 发送FIFO中鲜鼓?
	// ScicRegs.SCIFFTX.bit.TXFFIL        = 0;        // 5?: 发送FIFO中断级别为1

    ScicRegs.SCIFFRX.all                  =  0x0001;  // 0x2021;以下是展开解释
    // ScicRegs.SCIFFRX.bit.RXFFOVF       = 0; 		  // 接收FIFO未发生溢出,只读位
    // ScicRegs.SCIFFRX.bit.RXFFOVRCLR    = 0; 		  // RXFFOVF清除位; 0:写入0对RXFFOVF瓯志位无效,读取此位为0
    // ScicRegs.SCIFFRX.bit.RXFIFORESET   = 0; 		  // 接收FIFO复位位; 0:复位FIFO指针为0,并保持复位状态
    // ScicRegs.SCIFFRX.bit.RXFFST        = 0; 		  // 5位: 接收FIFO为空,只读位
    // ScicRegs.SCIFFRX.bit.RXFFINT       = 0; 		  // 接收FIFO中断位；0:RXFIFO中断没有发生,只读位
    // ScicRegs.SCIFFRX.bit.RXFFINTCLR    = 0; 		  // 接收FIFO中断清除位 0:对RXFFINT写入0无效
    // ScicRegs.SCIFFRX.bit.RXFFIENA      = 0; 		  // 接收FIFO中断禁止
    // ScicRegs.SCIFFRX.bit.RXFFIL        = 1; 		  // 5位:接收FIFO中断等级为1

    ScicRegs.SCIFFCT.all               =  0x01;    // 0x0f; FIFO传送延时

	ScicRegs.SCICTL1.bit.SWRESET	   = 1;		   // SCI软件复位

    ScicRegs.SCIPRI.bit.FREE           = 0;        // 挂起时立即停止
    ScicRegs.SCIFFTX.bit.TXFIFOXRESET  = 1;		   // 发送FIFO复位
    ScicRegs.SCIFFRX.bit.RXFIFORESET   = 1;		   // 接收FIFO复位
}


// 描述：这是个通用发送。
// 作者：何超
// 版本：4.0
// 日期：星期二,2014-3-12

#define		CANOPEN_SOI				0x7E	// CANOPEN的帧头
#define		CANOPEN_SPE				0x11	// CANOPEN特殊参数

void drv_scic_write(unsigned cmd, unsigned len, unsigned offset, unsigned *data)
{
	unsigned i;
	unsigned crc;

	ScicRegs.SCITXBUF = CANOPEN_SOI;		ScicRegs.SCITXBUF = cmd;
	ScicRegs.SCITXBUF = len;				ScicRegs.SCITXBUF = CANOPEN_SPE;
	ScicRegs.SCITXBUF = offset;	

	crc = CANOPEN_SOI^cmd^len^CANOPEN_SPE^offset;

	for (i=0;i<len-1;i++){
		ScicRegs.SCITXBUF = data[i];
		crc = crc^data[i];
	}
	ScicRegs.SCITXBUF = crc;
}


// 描述：scic发送读XGate-COP10缓冲区命令 //没有接收完一帧完整的数据都返回0,成功接收一帧数据返回1
// 作者：何超
// 版本：4.0
// 日期：星期二,2014-3-12

#define		RESET_SCIC_RXFIFO()		ScicRegs.SCICTL1.bit.SWRESET	   = 0;		   /*SCI软件复位*/		\
									ScicRegs.SCIFFTX.bit.TXFIFOXRESET  = 0;		   /*发送FIFO复位*/		\
									ScicRegs.SCIFFRX.bit.RXFIFORESET   = 0;		/*接收FIFO复位*/			\
									NOP3();																\
									ScicRegs.SCIFFRX.bit.RXFIFORESET   = 1;		/*再次使能接收FIFO*/		\
									ScicRegs.SCIFFTX.bit.TXFIFOXRESET  = 1;		   /*发送FIFO复位*/		\
									ScicRegs.SCICTL1.bit.SWRESET	   = 1;		   /*SCI软件复位*/

#define		READ_SCIC_FIFO()		ScicRegs.SCIRXBUF.bit.RXDT		// 空读接收FIFO

unsigned drv_scic_read_buf(unsigned offset, unsigned *pdata)
{
	static unsigned n	  = 0;
	static unsigned bflag = 0;
	unsigned data;
	unsigned soi = 0, cmd = 0, len = 0, spe = 0, mod = 0, crc = 0, crc_cal;
	unsigned i;
	unsigned datah, datal;

	if (!bflag){
		data = 0x08;	// 一次读8个字节
		drv_scic_write(0x11, 0x02, offset, &data);

		bflag = 1;
		return 0;
	}else {			// scic接收屏回应的信息

		if (ScicRegs.SCIFFRX.bit.RXFFST != 14){								// 返回的一帧数据应该有14个byre数据
			n++;
			if (n > 5)	{RESET_SCIC_RXFIFO();	n = 0;	bflag = 0;}		
			return 0;
		}else{
			n = 0;		bflag = 0;
		}
		soi = READ_SCIC_FIFO(); cmd = READ_SCIC_FIFO(); len = READ_SCIC_FIFO(); spe = READ_SCIC_FIFO(); mod = READ_SCIC_FIFO();
		if (soi != CANOPEN_SOI	||	cmd != 0x11)	{ RESET_SCIC_RXFIFO();	return 0;}

		crc_cal = CANOPEN_SOI^cmd^len^spe^mod;
		for (i=0;i<((len-1)>>1);i++){
			datal = READ_SCIC_FIFO();	//先读的是低八位，
			crc_cal = crc_cal^datal;
			datah = READ_SCIC_FIFO();
			crc_cal = crc_cal^datah;
			pdata[i] = 	(datah<<8) + datal;
		}
		crc = READ_SCIC_FIFO();
		if (crc_cal != crc)		{ RESET_SCIC_RXFIFO();	return 0;}

		return 1;
	}
}

// 描述：scic发送写XGate-COP10命令,并读出回应 //没有接收完一帧完整的数据都返回0,成功接收一帧数据返回1
// 作者：何超
// 版本：4.0
// 日期：星期二,2014-3-12

unsigned drv_scic_write_buf(unsigned offset, unsigned *pdata)
{
	static unsigned n	  = 0;
	static unsigned bflag = 0;
	unsigned soi = 0, cmd = 0, len = 0, spe = 0, mod = 0, crc = 0, crc_cal;
	unsigned data_byte[8];	//要发送的数据是字，拆成字节存在这，再发它
	unsigned i;

	if (!bflag){
		for (i=0;i<4;i++){
			data_byte[2*i] = pdata[i]&0x00FF;	data_byte[2*i + 1] = pdata[i]>>8;//先发低八位，再发高八位
		}
		drv_scic_write(0x10, 0x09, offset, data_byte);

		bflag = 1;
		return 0;
	}else {			// scic接收屏回应的信息

		if (ScicRegs.SCIFFRX.bit.RXFFST != 6){								// 返回的一帧数据应该有9个byre数据
			n++;
			if (n > 5)	{RESET_SCIC_RXFIFO();	n = 0;	bflag = 0;}		
			return 0;
		}else{
			n = 0;		bflag = 0;
		}
		soi = READ_SCIC_FIFO(); cmd = READ_SCIC_FIFO(); len = READ_SCIC_FIFO(); spe = READ_SCIC_FIFO(); mod = READ_SCIC_FIFO();
		if (soi != CANOPEN_SOI	||	cmd != 0x10)	{ RESET_SCIC_RXFIFO();	return 0;}
		
		crc_cal = CANOPEN_SOI^cmd^len^spe^mod;
		crc = READ_SCIC_FIFO();
		if (crc_cal != crc)		{ RESET_SCIC_RXFIFO();	return 0;}

		return 1;
	}
}

// 描述：scic发送写XGate-COP10命令,并读出回应 //没有接收完一帧完整的数据都返回0,成功接收一帧数据返回1
// 作者：何超
// 版本：4.0
// 日期：星期二,2014-3-13

unsigned drv_scic_xgate_uart(unsigned cmd, unsigned len, unsigned mod_offset, unsigned *data,  unsigned *ackdata, unsigned ack_len)
{
	static unsigned n	  = 0;
	static unsigned bflag = 0;
	unsigned soi = 0, rcmd = 0, rlen = 0, spe = 0, mod = 0, crc = 0, crc_cal;
	unsigned i;

	if (!bflag){
		drv_scic_write(cmd, len, mod_offset, data);

		bflag = 1;
		return 0;
	}else {			// scic接收屏回应的信息

		if (ScicRegs.SCIFFRX.bit.RXFFST != (ack_len + 5)){								// 返回的一帧数据应该有9个byre数据
			n++;
			if (n > 5)	{RESET_SCIC_RXFIFO();	n = 0;	bflag = 0;}		
			return 0;
		}else{
			n = 0;		bflag = 0;
		}
		soi = READ_SCIC_FIFO(); rcmd = READ_SCIC_FIFO(); rlen = READ_SCIC_FIFO(); spe = READ_SCIC_FIFO(); mod = READ_SCIC_FIFO();
		if (soi != CANOPEN_SOI	||	rcmd != cmd)	{ RESET_SCIC_RXFIFO();	return 0;}
		if (rlen != ack_len)	{ RESET_SCIC_RXFIFO();	return 0;}
		
		crc_cal = CANOPEN_SOI^rcmd^rlen^spe^mod;
		for (i=0;i<rlen-1;i++){
			ackdata[i] = READ_SCIC_FIFO();
			crc_cal = crc_cal^ackdata[i];
		}
		crc = READ_SCIC_FIFO();
		if (crc_cal != crc)		{ RESET_SCIC_RXFIFO();	return 0;}

		return 1;
	}
}

