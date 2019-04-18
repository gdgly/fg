#include "drv.h"
#include "drv_sci.h"


// CRC ��λ�ֽ�ֵ��
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

// CRC ��λ�ֽ�ֵ��
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

// ������uint16_t CRC16(uint8_t * pMsg, uint16_t Len)
// ���ܣ�����������ݵ�CRC16ֵ�������ء�
// ������pMsg ����ָ�룬Len ���ݳ��ȡ�

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



// ������scic�Ĵ�����ʼ��;ֻ��һ��,ͣ��ʱ�������³�ʼ��
// ���ߣ��γ�
// �汾��4.0
// ���ڣ����ڶ�,2013-1-22

void drv_scib_init1(void)
{
	InitScibGpio();									  // ����TI����ʼ��Scib�Ĵ���


    ScibRegs.SCICCR.all                  = 0x0007;    //  ������չ������
    // ScibRegs.SCICCR.bit.STOPBITS      = 0;		  //  1��ֹͣλ
    // ScibRegs.SCICCR.bit.PARITYENA   	 = 0;		  //  ����żУ��
    // ScibRegs.SCICCR.bit.LOOPBKENA     = 0;         //  �Բ�ģʽ��ֹ,���һ�����ظ�, �ݱ���
    // ScibRegs.SCICCR.bit.ADDRIDLE_MODE = 0;		  //  ������ģʽ
    // ScibRegs.SCICCR.bit.SCICHAR 		 = 7;		  //  8������λ,

    //  ʹ��TX,  ʹ��RX,  internal SCICLK,
    //  ��ֹ���մ����ж�, ��ֹ����,  ���ͻ���δʹ��
    ScibRegs.SCICTL1.all               =  0x0003;

    ScibRegs.SCICTL2.bit.TXINTENA      =  0;      	  // �ط����ж�
    ScibRegs.SCICTL2.bit.RXBKINTENA    =  0;      	  // �ؽ����ж�

//	ScibRegs.SCIHBAUD                  =  0x00;		  // ��������Ϊ115200,BRR =  = 0x
//	ScibRegs.SCILBAUD                  =  0xa2;
	ScibRegs.SCIHBAUD                  =  0x07;		  // ��������Ϊ9600,BRR =  = 0x
	ScibRegs.SCILBAUD                  =  0xa0;


    ScibRegs.SCIFFTX.all                  =  0xC000;  // 0xE121; ������չ������
    // ScibRegs.SCIFFTX.bit.SCIRST        = 1;        // SCI��λ
    // ScibRegs.SCIFFTX.bit.SCIFFENA      = 1;        // SCI FIFOʹ��
    // ScibRegs.SCIFFTX.bit.TXFIFOXRESET  = 0;        // ����FIFO��λλ; 0:��λFIFOָ��Ϊ0,�����ָ�λ״̬
    // ScibRegs.SCIFFTX.bit.TXFFST        = 0;        // 5λ:����FIFOΪ��,ֻ��λ
    // ScibRegs.SCIFFTX.bit.TXFFINT       = 0;        // ����FIFO��λ��0:TXFIFO�ж�û�з���,ֻ��λ
    // ScibRegs.SCIFFTX.bit.TXFFINTCLR    = 0;        // ����FIFO�ж����λ; 0:��TXFFINTд��0��Ч
	// ScibRegs.SCIFFTX.bit.TXFFIENA      = 0;        // ����FIFO���ʹ�?
	// ScibRegs.SCIFFTX.bit.TXFFIL        = 0;        // 5?: ����FIFO�жϼ���Ϊ1

    ScibRegs.SCIFFRX.all                  =  0x0001;  // 0x2021;������չ������
    // ScibRegs.SCIFFRX.bit.RXFFOVF       = 0; 		  // ����FIFOδ�������,ֻ��λ
    // ScibRegs.SCIFFRX.bit.RXFFOVRCLR    = 0; 		  // RXFFOVF���λ; 0:д��0��RXFFOVF�־λ��Ч,��ȡ��λΪ0
    // ScibRegs.SCIFFRX.bit.RXFIFORESET   = 0; 		  // ����FIFO��λλ; 0:��λFIFOָ��Ϊ0,�����ָ�λ״̬
    // ScibRegs.SCIFFRX.bit.RXFFST        = 0; 		  // 5λ: ����FIFOΪ��,ֻ��λ
    // ScibRegs.SCIFFRX.bit.RXFFINT       = 0; 		  // ����FIFO�ж�λ��0:RXFIFO�ж�û�з���,ֻ��λ
    // ScibRegs.SCIFFRX.bit.RXFFINTCLR    = 0; 		  // ����FIFO�ж����λ 0:��RXFFINTд��0��Ч
    // ScibRegs.SCIFFRX.bit.RXFFIENA      = 0; 		  // ����FIFO�жϽ�ֹ
    // ScibRegs.SCIFFRX.bit.RXFFIL        = 1; 		  // 5λ:����FIFO�жϵȼ�Ϊ1

    ScibRegs.SCIFFCT.all               =  0x01;    // 0x0f; FIFO������ʱ

	ScibRegs.SCICTL1.bit.SWRESET	   = 1;		   // SCI�����λ

    ScibRegs.SCIPRI.bit.FREE           = 0;        // ����ʱ����ֹͣ
    ScibRegs.SCIFFTX.bit.TXFIFOXRESET  = 1;		   // ����FIFO��λ
    ScibRegs.SCIFFRX.bit.RXFIFORESET   = 1;		   // ����FIFO��λ
}






// ���������Ǹ�ͨ�÷��ͣ����巢�ͱ��˵�ַ���ο�ͷ�ļ����������⣬��ֱ�����á�
// ���ߣ��γ�
// �汾��4.0
// ���ڣ����ڶ�,2013-1-22

#define		LEN				5+2		// ����	��2���ֽڵ�CRC
#define		RD				0x82	// д�����洢������,�������ݵ���
#define		WR				0x83	// �������洢������,��Ҫ������������
#define		SOI_H			0xAA	// ֡ͷ���ֽ�
#define		SOI_L			0x55	// ֡ͷ���ֽ�

void drv_scib_write(unsigned addr,unsigned data)
{
	unsigned crc = 0;
	unsigned scib_send[5];

	ScibRegs.SCITXBUF = SOI_H;		ScibRegs.SCITXBUF = SOI_L;			// ֡ͷ
	ScibRegs.SCITXBUF = LEN;		ScibRegs.SCITXBUF = RD;				// ���Ⱥ�����
	ScibRegs.SCITXBUF = addr>>8;	ScibRegs.SCITXBUF = addr;			// ��ַ
	ScibRegs.SCITXBUF = data>>8;	ScibRegs.SCITXBUF = data;			// ����

	scib_send[0] = RD;		scib_send[1] = addr>>8;		scib_send[2] = addr&0x00FF;	
	scib_send[3] = data>>8;	scib_send[4] = data&0x00FF;	
	crc = CRC16(scib_send, 5 );
	ScibRegs.SCITXBUF = crc>>8;		ScibRegs.SCITXBUF = crc;			// 
}

// ������scib���Ͷ������� //û�н�����һ֡���������ݶ�����0,�ɹ�����һ֡���ݷ���1
// ���ߣ��γ�
// �汾��4.0
// ���ڣ����ڶ�,2013-1-22

#define		RESET_RXFIFO()	drv_scib_init1();ScibRegs.SCICTL1.bit.SWRESET	   = 0;		   /*SCI�����λ*/		\
							ScibRegs.SCIFFTX.bit.TXFIFOXRESET  = 0;		   /*����FIFO��λ*/		\
							ScibRegs.SCIFFRX.bit.RXFIFORESET   = 0;	/*����FIFO��λ*/			\
							NOP3();																\
							ScibRegs.SCIFFRX.bit.RXFIFORESET   = 1;	/*�ٴ�ʹܽ���FIFO*/		\
							ScibRegs.SCIFFTX.bit.TXFIFOXRESET  = 1;		   /*����FIFO��λ*/		\
							ScibRegs.SCICTL1.bit.SWRESET	   = 1;		   /*SCI�����λ*/

#define		READ_FIFO()		ScibRegs.SCIRXBUF.bit.RXDT		// �ն�2�ν���FIFO

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
		ScibRegs.SCITXBUF = SOI_H;		ScibRegs.SCITXBUF = SOI_L;			// ֡ͷ
		ScibRegs.SCITXBUF = LEN-1;		ScibRegs.SCITXBUF = WR;				// ���Ⱥ�����
		ScibRegs.SCITXBUF = addr>>8;	ScibRegs.SCITXBUF = addr;			// ��ַ
		ScibRegs.SCITXBUF = 1;												// ����

		scib_send[0] = WR;		scib_send[1] = addr>>8;		scib_send[2] = addr&0x00FF;	
		scib_send[3] = 1;	
		crc = CRC16(scib_send, 4 );
		ScibRegs.SCITXBUF = crc>>8;		ScibRegs.SCITXBUF = crc;			// 

		bflag = 1;
		return 0;
	}else {			// scib��������Ӧ����Ϣ

		if (ScibRegs.SCIFFRX.bit.RXFFST != 11){								// ���ص�һ֡����Ӧ����9��byre����
			n++;
			if (n > 5)	{RESET_RXFIFO();	n = 0;	bflag = 0;}		
			return 0;
		}else{
			n = 0;		bflag = 0;
		}
		if (READ_FIFO() != SOI_H	||	READ_FIFO() != SOI_L /*|| 	READ_FIFO() != LEN*/)	{ RESET_RXFIFO();	return 0;}
		READ_FIFO();	command = READ_FIFO();	//	�ն�2�ν���FIFO
		
		addr_readh = READ_FIFO();	addr_readl = READ_FIFO();
		if (addr_readh != (addr>>8) || addr_readl != (addr & 0x00FF))						{ RESET_RXFIFO();	return 0;}
		bytes = READ_FIFO();					//	�ն�1�ν���FIFO

		data   =	(READ_FIFO()<<8);
		data  +=	READ_FIFO();		//	������
		*pdata =	data;
		
		crc_read   =	(READ_FIFO()<<8);
		crc_read  +=	READ_FIFO();			//	��CRC
		scib_receive[0] = command;	scib_receive[1] = addr_readh;	scib_receive[2] = addr_readl;	
		scib_receive[3] = bytes;	scib_receive[4] = data>>8;		scib_receive[5] = data&0x00FF;	
		crc_calu = CRC16(scib_receive, 6 );
		if (crc_calu != crc_read)	return 0;	// CRCУ�����

		return 1;
	}
}













// ������scic�Ĵ�����ʼ��;ֻ��һ��,ͣ��ʱ�������³�ʼ��
// ���ߣ��γ�
// �汾��4.0
// ���ڣ����ڶ�,2013-1-22

void drv_scic_init1(void)
{
	InitScicGpio();									  // ����TI����ʼ��Scic�Ĵ���


    ScicRegs.SCICCR.all                  = 0x0007;    //  ������չ������
    // ScicRegs.SCICCR.bit.STOPBITS      = 0;		  //  1��ֹͣλ
    // ScicRegs.SCICCR.bit.PARITYENA   	 = 0;		  //  ����żУ��
    // ScicRegs.SCICCR.bit.LOOPBKENA     = 0;         //  �Բ�ģʽ��ֹ,���һ�����ظ�, �ݱ���
    // ScicRegs.SCICCR.bit.ADDRIDLE_MODE = 0;		  //  ������ģʽ
    // ScicRegs.SCICCR.bit.SCICHAR 		 = 7;		  //  8������λ,

    //  ʹ��TX,  ʹ��RX,  internal SCICLK,
    //  ��ֹ���մ����ж�, ��ֹ����,  ���ͻ���δʹ��
    ScicRegs.SCICTL1.all               =  0x0003;

    ScicRegs.SCICTL2.bit.TXINTENA      =  0;      	  // �ط����ж�
    ScicRegs.SCICTL2.bit.RXBKINTENA    =  0;      	  // �ؽ����ж�

//	ScicRegs.SCIHBAUD                  =  0x00;		  // ��������Ϊ115200,BRR =  = 0x
//	ScicRegs.SCILBAUD                  =  0xa2;
    ScicRegs.SCIHBAUD                  =  0x07;		  // ��������Ϊ9600,BRR =  = 0x
    ScicRegs.SCILBAUD                  =  0xa0;


    ScicRegs.SCIFFTX.all                  =  0xC000;  // 0xE121; ������չ������
    // ScicRegs.SCIFFTX.bit.SCIRST        = 1;        // SCI��λ
    // ScicRegs.SCIFFTX.bit.SCIFFENA      = 1;        // SCI FIFOʹ��
    // ScicRegs.SCIFFTX.bit.TXFIFOXRESET  = 0;        // ����FIFO��λλ; 0:��λFIFOָ��Ϊ0,�����ָ�λ״̬
    // ScicRegs.SCIFFTX.bit.TXFFST        = 0;        // 5λ:����FIFOΪ��,ֻ��λ
    // ScicRegs.SCIFFTX.bit.TXFFINT       = 0;        // ����FIFO��λ��0:TXFIFO�ж�û�з���,ֻ��λ
    // ScicRegs.SCIFFTX.bit.TXFFINTCLR    = 0;        // ����FIFO�ж����λ; 0:��TXFFINTд��0��Ч
	// ScicRegs.SCIFFTX.bit.TXFFIENA      = 0;        // ����FIFO���ʹ�?
	// ScicRegs.SCIFFTX.bit.TXFFIL        = 0;        // 5?: ����FIFO�жϼ���Ϊ1

    ScicRegs.SCIFFRX.all                  =  0x0001;  // 0x2021;������չ������
    // ScicRegs.SCIFFRX.bit.RXFFOVF       = 0; 		  // ����FIFOδ�������,ֻ��λ
    // ScicRegs.SCIFFRX.bit.RXFFOVRCLR    = 0; 		  // RXFFOVF���λ; 0:д��0��RXFFOVF�־λ��Ч,��ȡ��λΪ0
    // ScicRegs.SCIFFRX.bit.RXFIFORESET   = 0; 		  // ����FIFO��λλ; 0:��λFIFOָ��Ϊ0,�����ָ�λ״̬
    // ScicRegs.SCIFFRX.bit.RXFFST        = 0; 		  // 5λ: ����FIFOΪ��,ֻ��λ
    // ScicRegs.SCIFFRX.bit.RXFFINT       = 0; 		  // ����FIFO�ж�λ��0:RXFIFO�ж�û�з���,ֻ��λ
    // ScicRegs.SCIFFRX.bit.RXFFINTCLR    = 0; 		  // ����FIFO�ж����λ 0:��RXFFINTд��0��Ч
    // ScicRegs.SCIFFRX.bit.RXFFIENA      = 0; 		  // ����FIFO�жϽ�ֹ
    // ScicRegs.SCIFFRX.bit.RXFFIL        = 1; 		  // 5λ:����FIFO�жϵȼ�Ϊ1

    ScicRegs.SCIFFCT.all               =  0x01;    // 0x0f; FIFO������ʱ

	ScicRegs.SCICTL1.bit.SWRESET	   = 1;		   // SCI�����λ

    ScicRegs.SCIPRI.bit.FREE           = 0;        // ����ʱ����ֹͣ
    ScicRegs.SCIFFTX.bit.TXFIFOXRESET  = 1;		   // ����FIFO��λ
    ScicRegs.SCIFFRX.bit.RXFIFORESET   = 1;		   // ����FIFO��λ
}


// ���������Ǹ�ͨ�÷��͡�
// ���ߣ��γ�
// �汾��4.0
// ���ڣ����ڶ�,2014-3-12

#define		CANOPEN_SOI				0x7E	// CANOPEN��֡ͷ
#define		CANOPEN_SPE				0x11	// CANOPEN�������

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


// ������scic���Ͷ�XGate-COP10���������� //û�н�����һ֡���������ݶ�����0,�ɹ�����һ֡���ݷ���1
// ���ߣ��γ�
// �汾��4.0
// ���ڣ����ڶ�,2014-3-12

#define		RESET_SCIC_RXFIFO()		ScicRegs.SCICTL1.bit.SWRESET	   = 0;		   /*SCI�����λ*/		\
									ScicRegs.SCIFFTX.bit.TXFIFOXRESET  = 0;		   /*����FIFO��λ*/		\
									ScicRegs.SCIFFRX.bit.RXFIFORESET   = 0;		/*����FIFO��λ*/			\
									NOP3();																\
									ScicRegs.SCIFFRX.bit.RXFIFORESET   = 1;		/*�ٴ�ʹ�ܽ���FIFO*/		\
									ScicRegs.SCIFFTX.bit.TXFIFOXRESET  = 1;		   /*����FIFO��λ*/		\
									ScicRegs.SCICTL1.bit.SWRESET	   = 1;		   /*SCI�����λ*/

#define		READ_SCIC_FIFO()		ScicRegs.SCIRXBUF.bit.RXDT		// �ն�����FIFO

unsigned drv_scic_read_buf(unsigned offset, unsigned *pdata)
{
	static unsigned n	  = 0;
	static unsigned bflag = 0;
	unsigned data;
	unsigned soi = 0, cmd = 0, len = 0, spe = 0, mod = 0, crc = 0, crc_cal;
	unsigned i;
	unsigned datah, datal;

	if (!bflag){
		data = 0x08;	// һ�ζ�8���ֽ�
		drv_scic_write(0x11, 0x02, offset, &data);

		bflag = 1;
		return 0;
	}else {			// scic��������Ӧ����Ϣ

		if (ScicRegs.SCIFFRX.bit.RXFFST != 14){								// ���ص�һ֡����Ӧ����14��byre����
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
			datal = READ_SCIC_FIFO();	//�ȶ����ǵͰ�λ��
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

// ������scic����дXGate-COP10����,��������Ӧ //û�н�����һ֡���������ݶ�����0,�ɹ�����һ֡���ݷ���1
// ���ߣ��γ�
// �汾��4.0
// ���ڣ����ڶ�,2014-3-12

unsigned drv_scic_write_buf(unsigned offset, unsigned *pdata)
{
	static unsigned n	  = 0;
	static unsigned bflag = 0;
	unsigned soi = 0, cmd = 0, len = 0, spe = 0, mod = 0, crc = 0, crc_cal;
	unsigned data_byte[8];	//Ҫ���͵��������֣�����ֽڴ����⣬�ٷ���
	unsigned i;

	if (!bflag){
		for (i=0;i<4;i++){
			data_byte[2*i] = pdata[i]&0x00FF;	data_byte[2*i + 1] = pdata[i]>>8;//�ȷ��Ͱ�λ���ٷ��߰�λ
		}
		drv_scic_write(0x10, 0x09, offset, data_byte);

		bflag = 1;
		return 0;
	}else {			// scic��������Ӧ����Ϣ

		if (ScicRegs.SCIFFRX.bit.RXFFST != 6){								// ���ص�һ֡����Ӧ����9��byre����
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

// ������scic����дXGate-COP10����,��������Ӧ //û�н�����һ֡���������ݶ�����0,�ɹ�����һ֡���ݷ���1
// ���ߣ��γ�
// �汾��4.0
// ���ڣ����ڶ�,2014-3-13

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
	}else {			// scic��������Ӧ����Ϣ

		if (ScicRegs.SCIFFRX.bit.RXFFST != (ack_len + 5)){								// ���ص�һ֡����Ӧ����9��byre����
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

