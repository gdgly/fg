// 示例：声明对象、外部引用对象、引用函数
//    DRV_CPLD drv_cpld = DRV_CPLD_DEFAULTS;
//    extern DRV_CPLD drv_cpld;
//    drv_ethernet_init();
//    drv_ethernet_read();

// 描述：头文件。
// 作者：何超
// 版本：4.0
// 日期：星期三,2013-12-26

#ifndef __DRV_ETHERNET_H__
#define __DRV_ETHERNET_H__


#define ETHERNET_DATA_N					30
#define DATA_ALL_N						(ETHERNET_DATA_N*2*2+2)
#define PACKET_LENGTH					(42+DATA_ALL_N)


//----------------------------地址定义----------------------------
#define XINF0							0x4000		// 目前用的是XINF0，即00 4000h-00 5000h，共4K*16bit

#define ETHERNET_BASE_ADDRESS			(XINF0)		// 
#define ETHERNET_PAGE_SELECT			( *((volatile unsigned int *)ETHERNET_BASE_ADDRESS) )		// 测试总线，读出来应该是AA55

// 写(输出)                                                                              
#define ETHERNET_WRITE_ADDRESS			(0x0020 + XINF0)	// 以太网高位连在地址A5
#define ETHERNET_WRITE_reg				(0x14  + XINF0)	// 
#define ETHERNET_WRITE_RAM    			( *((volatile unsigned int *)ETHERNET_WRITE_ADDRESS) )

#define DRV_ETHERNET_VAR_READ(n, u,v,w,x,y,z) 											\
	{udp_data.data[n-1] = u;	udp_data.data[n+(1-1)] = v;	udp_data.data[n+(2-1)] = w;	\
	udp_data.data[n+(3-1)] = x;	udp_data.data[n+(4-1)] = y;	udp_data.data[n+(5-1)] = z;}

// 声明对象内部函数
void drv_ethernet_init1(void);
void drv_ethernet_ram_write(void);

// 选寄存器页
#define DRV_ETHERNET_PAGE_SELECT(page)		( ETHERNET_PAGE_SELECT = ((page)|0x8000) )

// 写寄存器
#define DRV_ETHERNET_REG_WRITE(page, addr, data)												\
{																								\
	DRV_ETHERNET_PAGE_SELECT(page);																\
	( *((volatile unsigned int *)(ETHERNET_BASE_ADDRESS + (unsigned long)(addr))) ) = (data);	\
}



struct  packet_sop_bits {     // bits  description
   unsigned packet_length:11;   		// 10:0     
   unsigned rsvd:1;        				// 11  reserved
   unsigned manual_dequeue:1;   		// 12     
   unsigned tx_int:1;      				// 13     
   unsigned cphi:1;      				// 14     
   unsigned dicf:1;      				// 15     
   unsigned packet_length_reverse:11;   // 26:16  
   unsigned sequence_number:5;      	// 31:27     
};

union packet_sop {
   unsigned long			all;
   struct packet_sop_bits  	bit;
};



struct  packet_eop_bits {     // bits  description
   unsigned packet_length:11;   		// 10:0     
   unsigned sequence_number:5;      	// 15:11     
   unsigned packet_length_reverse:11;   // 26:16     
   unsigned sequence_number_reverse:5;  // 31:27     
};

union packet_eop {
   unsigned long			all;
   struct packet_eop_bits  	bit;
};



struct  packet_seg_bits {     // bits  description
   unsigned segment_length:11;   		// 10:0     
   unsigned segment_number:3;   		// 13:11     
   unsigned ls:1;      					// 14     
   unsigned fs:1;      					// 15     
   unsigned segment_length_reverse:11;  // 26:16  
   unsigned start_offset:3;      		// 29:27     
   unsigned end_offset:2;      			// 31:30     
};

union packet_seg {
   unsigned long			all;
   struct packet_seg_bits  	bit;
};

struct udp_packet {
	unsigned destination_addr[3];
	unsigned source_addr[3];
	unsigned tpye_e[1];
	unsigned IP[1];
    unsigned IP_length[1];
	unsigned check[1];
	unsigned reserved[1];
	unsigned type[1];
	unsigned IP_check[1];
	unsigned IP_source[2];
	unsigned IP_destinat[2];
	unsigned udp_head1[1];
	unsigned udp_head2[1];
	unsigned udp_length1[1];
	unsigned udp_checksum[1];
	float 	 data[ETHERNET_DATA_N];
	unsigned data_end;
};//96 bytes

extern struct udp_packet udp_data;


#endif  // __DRV_ETHERNET_H__



