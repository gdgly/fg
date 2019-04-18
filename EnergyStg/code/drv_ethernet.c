#include "drv.h"
#include "drv_ethernet.h"

struct udp_packet udp_data;

union packet_eop 		drv_ethernet_eop;
union packet_sop		drv_ethernet_sop;
union packet_seg		drv_ethernet_seg;

static unsigned long check_sum_head;


// 描述：
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-6-19

void drv_ethernet_init1(void)
{
	unsigned i;
	unsigned long check_sum_IP;
//	unsigned test_reg;

	DRV_IO_RESET_ETHERNET(0);
	NOP3();NOP3();NOP3();NOP3();NOP3();NOP3();NOP3();NOP3();NOP3();NOP3();NOP3();NOP3();
	DRV_IO_RESET_ETHERNET(1);

	udp_data.destination_addr[0]=0xffff;
	udp_data.destination_addr[1]=0xffff;
	udp_data.destination_addr[2]=0xffff;

	udp_data.source_addr[0]=0x00e0;
	udp_data.source_addr[1]=0x4c68;
	udp_data.source_addr[2]=0x0899;

	udp_data.tpye_e[0]=0x0800;
	udp_data.IP[0]=0x4500;
	udp_data.IP_length[0]=(28+DATA_ALL_N);//28+数据总长度
	udp_data.check[0]=0;//*********
	udp_data.reserved[0]=0x0;
	udp_data.type[0]=0x4011;

	udp_data.IP_source[0]=0xa9fe;
	udp_data.IP_source[1]=0xe005;
	udp_data.IP_destinat[0]=0xffff;
	udp_data.IP_destinat[1]=0xffff;

	udp_data.udp_head1[0]=0xf074;
	udp_data.udp_head2[0]=0xf075;

	udp_data.udp_length1[0]=(8+DATA_ALL_N);//根据数据包修改8+

	DRV_ETHERNET_REG_WRITE(0,0x00,0x8000); // 以太网芯片软件复位	
	drv_ethernet_sop.bit.packet_length = PACKET_LENGTH;   	    
	drv_ethernet_sop.bit.rsvd = 0;        			
	drv_ethernet_sop.bit.manual_dequeue = 0;   	    
	drv_ethernet_sop.bit.tx_int = 0;      			
	drv_ethernet_sop.bit.cphi = 0;      			    
	drv_ethernet_sop.bit.dicf = 1;      			    
	drv_ethernet_sop.bit.packet_length_reverse = 0x7ff- drv_ethernet_sop.bit.packet_length;      
	drv_ethernet_sop.bit.sequence_number = 0;         

	drv_ethernet_seg.bit.segment_length = PACKET_LENGTH;   	    
	drv_ethernet_seg.bit.segment_number = 0;  	    
	drv_ethernet_seg.bit.ls = 1;      				
	drv_ethernet_seg.bit.fs = 1;      				
	drv_ethernet_seg.bit.segment_length_reverse = 0x7ff-drv_ethernet_seg.bit.segment_length;    
	drv_ethernet_seg.bit.start_offset = 0;      	    
	drv_ethernet_seg.bit.end_offset = 0;      		

	drv_ethernet_eop.bit.packet_length = PACKET_LENGTH;
	drv_ethernet_eop.bit.sequence_number = 0;
	drv_ethernet_eop.bit.packet_length_reverse = 0x7ff-drv_ethernet_eop.bit.packet_length;
	drv_ethernet_eop.bit.sequence_number_reverse = 0x1f-drv_ethernet_eop.bit.sequence_number;
//	test_reg = drv_ethernet_reg_read(0,0x02);//test_reg is 0x1234 = OK
//	test_reg = drv_ethernet_reg_read(0,0x00);//check the bus type
//	test_reg = drv_ethernet_reg_read(0,0x0E);//check pmm POWER STATUS device ready

	DRV_ETHERNET_REG_WRITE(1,0x02,1);//rpper enable RX PACKET PROCESS function
	DRV_ETHERNET_REG_WRITE(1,0x10,0);//disable the RX bridge stuffing 
	DRV_ETHERNET_REG_WRITE(3,0x02,0x0c01);//MAC5,MAC6
	DRV_ETHERNET_REG_WRITE(3,0x04,0x04cd);//MAC3,MAC4
	DRV_ETHERNET_REG_WRITE(3,0x06,0x1100);//MAC1,MAC2

	DRV_ETHERNET_REG_WRITE(0,0x04,0xC243);//Enable TX/RX ,byte swap(bit9)	
									  //bit15,TX Bridge Enable = 1
									  //bit14,RX Bridge Enable = 1
									  //bit9, ByteSwapF_EN = 1	Enable byte swap function SD bus = {SD [7:0], SD [15:8]}
									  //bit5, ALECLK_HL  = 1 	 positive edge trigger
									  //bit1, RX Drop CRC Enable = 1
									  //bit0, RX IP header aligned 32-bit = 1
	DRV_ETHERNET_REG_WRITE(2,0x0C,0x0305);//使能I_FULL,I_SPEED LCR0
	DRV_ETHERNET_REG_WRITE(2,0x0E,0x1509);//使能I_act LCR1
	DRV_ETHERNET_REG_WRITE(2,0x04,0x0003);//使能PCR
										  //bit1, Enable Auto-polling Flow control function. = 1
										  //bit0, Auto_Poll_En = 1
	DRV_ETHERNET_REG_WRITE(0,0x0E,0x031c);//以太网速度设为100M, 

	for (i=0;i<ETHERNET_DATA_N;i++){
		udp_data.data[i] = 0;
	}


	//因为IP的数据固定,这里直接给出IP_check
	check_sum_IP =	(unsigned long)udp_data.IP[0]+
					(unsigned long)udp_data.IP_length[0]+
					(unsigned long)udp_data.check[0]+
					(unsigned long)udp_data.reserved[0]+
					(unsigned long)udp_data.type[0]+
					(unsigned long)udp_data.IP_source[0]+
					(unsigned long)udp_data.IP_source[1]+
					(unsigned long)udp_data.IP_destinat[0]+
					(unsigned long)udp_data.IP_destinat[1];

	udp_data.IP_check[0] = ~((unsigned)(check_sum_IP>>16) + (unsigned)check_sum_IP);
	

	//UDP的前几个数据固定,这里直接给出check_sum_head
	check_sum_head = 	(unsigned long)(udp_data.type[0]&0x0fff) + 
						(unsigned long)udp_data.udp_length1[0]+
						(unsigned long)udp_data.IP_source[0]+(unsigned long)udp_data.IP_source[1]+
						(unsigned long)udp_data.IP_destinat[0]+(unsigned long)udp_data.IP_destinat[1]+
						(unsigned long)udp_data.udp_head1[0]+
						(unsigned long)udp_data.udp_head2[0]+
						(unsigned long)udp_data.udp_length1[0];
}



// 描述：
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-6-19

void drv_ethernet_ram_write(void)
{
	unsigned long check_sum; 
	unsigned char* pdata;

	check_sum = check_sum_head;	//UDP的前几个数据固定,这里直接给出

/*	for (i=0;i<ETHERNET_DATA_N;i++) {
		pdata = (unsigned char*)&udp_data.data[i];  //进行指针的强制转换
		check_sum  = check_sum + *(pdata) + *(pdata+1);
	}*/
#define	UDP_DATA_CHECK(i)								\
		pdata = (unsigned char*)&udp_data.data[i];		\
		check_sum  = check_sum + *(pdata) + *(pdata+1)	\

	UDP_DATA_CHECK(0);
	UDP_DATA_CHECK(1);
	UDP_DATA_CHECK(2);
	UDP_DATA_CHECK(3);
	UDP_DATA_CHECK(4);
	UDP_DATA_CHECK(5);
	UDP_DATA_CHECK(6);
	UDP_DATA_CHECK(7);
	UDP_DATA_CHECK(8);
	UDP_DATA_CHECK(9);
	UDP_DATA_CHECK(10);
	UDP_DATA_CHECK(11);
	UDP_DATA_CHECK(12);
	UDP_DATA_CHECK(13);
	UDP_DATA_CHECK(14);
	UDP_DATA_CHECK(15);
	UDP_DATA_CHECK(16);
	UDP_DATA_CHECK(17);
	UDP_DATA_CHECK(18);
	UDP_DATA_CHECK(19);
	UDP_DATA_CHECK(20);
	UDP_DATA_CHECK(21);
	UDP_DATA_CHECK(22);
	UDP_DATA_CHECK(23);
	UDP_DATA_CHECK(24);
	UDP_DATA_CHECK(25);
	UDP_DATA_CHECK(26);
	UDP_DATA_CHECK(27);
	UDP_DATA_CHECK(28);
	UDP_DATA_CHECK(29);
	check_sum += udp_data.data_end;  // 加上最后一个数据
	udp_data.udp_checksum[0] = ~((unsigned)(check_sum>>16) + (unsigned)check_sum);

	DRV_ETHERNET_REG_WRITE(0,0x12,0x4000); // 复位TX,bridge	
	DRV_ETHERNET_REG_WRITE(0,0x06,0x0300); 
	DRV_ETHERNET_REG_WRITE(0,0x04,0x8043); // FER使能TX,bridge,byte swap
	DRV_ETHERNET_REG_WRITE(0,0x12,0x0140); // 发送一个数据包
	DRV_ETHERNET_REG_WRITE(0,0x12,0x8140); // 0x0120 2表示bridge处于Error状态
	
	ETHERNET_WRITE_RAM = (unsigned)drv_ethernet_sop.all;
	ETHERNET_WRITE_RAM = (unsigned)(drv_ethernet_sop.all>>16);
	ETHERNET_WRITE_RAM = (unsigned)drv_ethernet_seg.all;
	ETHERNET_WRITE_RAM = (unsigned)(drv_ethernet_seg.all>>16);

	ETHERNET_WRITE_RAM = udp_data.destination_addr[0];
	ETHERNET_WRITE_RAM = udp_data.destination_addr[1];
	ETHERNET_WRITE_RAM = udp_data.destination_addr[2];
                         
	ETHERNET_WRITE_RAM = udp_data.source_addr[0];
	ETHERNET_WRITE_RAM = udp_data.source_addr[1];
	ETHERNET_WRITE_RAM = udp_data.source_addr[2];
                         
	ETHERNET_WRITE_RAM = udp_data.tpye_e[0];
	ETHERNET_WRITE_RAM = udp_data.IP[0];
	ETHERNET_WRITE_RAM = udp_data.IP_length[0];
                         
	ETHERNET_WRITE_RAM = udp_data.check[0];
	ETHERNET_WRITE_RAM = udp_data.reserved[0];
                         
	ETHERNET_WRITE_RAM = udp_data.type[0];
	ETHERNET_WRITE_RAM = udp_data.IP_check[0];
                         
	ETHERNET_WRITE_RAM = udp_data.IP_source[0];
	ETHERNET_WRITE_RAM = udp_data.IP_source[1];
	ETHERNET_WRITE_RAM = udp_data.IP_destinat[0];
	ETHERNET_WRITE_RAM = udp_data.IP_destinat[1];
                         
	ETHERNET_WRITE_RAM = udp_data.udp_head1[0];
	ETHERNET_WRITE_RAM = udp_data.udp_head2[0];
                         
	ETHERNET_WRITE_RAM = udp_data.udp_length1[0];
	ETHERNET_WRITE_RAM = udp_data.udp_checksum[0];

/*	for(i=0;i<ETHERNET_DATA_N;i++) {
		pdata = (unsigned char*)&udp_data.data[i];  //进行指针的强制转换
		ETHERNET_WRITE_RAM = *(pdata+1);
		ETHERNET_WRITE_RAM = *(pdata);
	}*/
#define	UDP_DATA_SEND(i)							\
		pdata = (unsigned char*)&udp_data.data[i];	\
		ETHERNET_WRITE_RAM = *(pdata+1);			\
		ETHERNET_WRITE_RAM = *(pdata)				\

	UDP_DATA_SEND(0);
	UDP_DATA_SEND(1);
	UDP_DATA_SEND(2);
	UDP_DATA_SEND(3);
	UDP_DATA_SEND(4);
	UDP_DATA_SEND(5);
	UDP_DATA_SEND(6);
	UDP_DATA_SEND(7);
	UDP_DATA_SEND(8);
	UDP_DATA_SEND(9);
	UDP_DATA_SEND(10);
	UDP_DATA_SEND(11);
	UDP_DATA_SEND(12);
	UDP_DATA_SEND(13);
	UDP_DATA_SEND(14);
	UDP_DATA_SEND(15);
	UDP_DATA_SEND(16);
	UDP_DATA_SEND(17);
	UDP_DATA_SEND(18);
	UDP_DATA_SEND(19);
	UDP_DATA_SEND(20);
	UDP_DATA_SEND(21);
	UDP_DATA_SEND(22);
	UDP_DATA_SEND(23);
	UDP_DATA_SEND(24);
	UDP_DATA_SEND(25);
	UDP_DATA_SEND(26);
	UDP_DATA_SEND(27);
	UDP_DATA_SEND(28);
	UDP_DATA_SEND(29);
	ETHERNET_WRITE_RAM = udp_data.data_end; // 发最后一个数据
	ETHERNET_WRITE_RAM = (unsigned)drv_ethernet_eop.all;
	ETHERNET_WRITE_RAM = (unsigned)(drv_ethernet_eop.all>>16);
}


