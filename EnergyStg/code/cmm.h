// ʾ�������������ⲿ���ö������ú���
//    CMM cmm=CMM_DEFAULTS;
//    extern CMM cmm;
//    cmm_init();

#ifndef __CMM_H__
#define __CMM_H__

//�����ж��ٸ�����
#define CMM_READ_N		(21)	//
#define CMM_WRITE_N		(11)			//

#define	CMM_RW_TABLE_R	0				// rw_table=0:������������
#define	CMM_RW_TABLE_W	1				// rw_table=1:������������

typedef struct {
				//״̬��===================================================================
				unsigned	bready							;
				unsigned	dis_on								;
				unsigned 	di_local							;	// ��Զ�˿��ƣ�	0:���ؿ��� remote 1:���������� Local

				//����===================================================================
				unsigned	bstart_1,bstart					;
				unsigned	bstart_chg, bstart_dchg         ;
				unsigned	bclear_error_1,bclear_error		;
				
				unsigned	IA_ref_T                        ;
				float       IA_ref                          ;
				unsigned    TimeDownI                       ;
				float		Max_Cell_VoltageA_BAT           ;
				float		Low_Cell_VoltageA_BAT           ;
				float		ChongVolt_end_A                 ;
                signed int  ChongTemp_end_A                 ;
                float		ChongCellV_end_A                ;
                signed int  ChongCellT_end_A	            ;
                unsigned    ChargeFastSlow                  ;
                                            				
				int 		in1, 	in2, 	in3, 	in4, 	in5, 	in6, 	in7, 	in8;
				int 		in9, 	in10, 	in11, 	in12, 	in13, 	in14, 	in15;
		
				//���===================================================================
				unsigned	dspversion						;
				 	        										                 	    											 	        						
				int 		out1, 	out2, 	out3, 	out4, 	out5, 	out6, 	out7, 	out8;
} CMM;

#define CMM_DEFAULTS 		{0, 0, 0, 0, 0}




// ���������ڲ�����
void cmm_init(void);
void cmm_arb(void);
unsigned cmm_data2real( unsigned rw_table, unsigned bwork, unsigned i, unsigned data,  float *preal);
void cmm_real2data(unsigned rw_table, unsigned i, unsigned offset, unsigned *pdata);
void cmm_real_handle(unsigned rw_table, unsigned id, unsigned i, float real);
void cmm_read_pnl(unsigned bwork);
void cmm_reset_parameter(void);
void cmm_write_pnl(void);
void cmm_lemp_fan(void);

typedef struct {
//	��ַ   			�ֱ���				����		����ֵ  		����			��/��ϵͳ����/д,	��׼ֵ		��������	������      �������鳤��
	unsigned addr;	float resolution;	float low;	float factory;	float upper;	char rsw;			float pu;	char ptype;	void *p; 	unsigned length;
} CMMPARA;	//��������Ĵ�ṹ������

extern CMM cmm;



#endif  // __CMM_H__

