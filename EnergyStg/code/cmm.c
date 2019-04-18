#include "apply.h"
#include "cmm.h"
#include "pu.h"

extern CMMPARA  cmm_w[CMM_READ_N];
extern CMMPARA  cmm_r[CMM_WRITE_N];

CMM cmm = CMM_DEFAULTS;
unsigned cmm_delay = 0;

#define		F2U(a)		((a>0)?	(unsigned)(a) :	(unsigned)(int)(a))		//a= [ -32768  65535]
void cmm_read_reset1(void *p);

// ��������ʼ��
void cmm_init(void)
{
	unsigned i;
	float tmp;
	unsigned data,data2;

	// R���ʼ��
   	for (i=0; i<CMM_READ_N;	i++){
   		if      (cmm_r[i].ptype=='U')	*(unsigned *)cmm_r[i].p = (unsigned)(cmm_r[i].factory / cmm_r[i].pu);
   		else if (cmm_r[i].ptype=='I')	*(int *     )cmm_r[i].p = (int     )(cmm_r[i].factory / cmm_r[i].pu);
   		else							*(float *   )cmm_r[i].p = (float   )(cmm_r[i].factory / cmm_r[i].pu);

   		//����Ļ�����ò�������ʼ�� R��
		tmp = cmm_r[i].factory/cmm_r[i].resolution;
   		drv_scib_write(cmm_r[i].addr, F2U(tmp));
		DELAY_US(20000);							//���˸�������ʱ��
	}

	// W���ʼ��
   	for (i=0; i<CMM_WRITE_N;	i++){
   		if      (cmm_w[i].ptype=='U')	*(unsigned *)cmm_w[i].p = (unsigned)(cmm_w[i].factory / cmm_w[i].pu);
   		else if (cmm_w[i].ptype=='I')	*(int *     )cmm_w[i].p = (int     )(cmm_w[i].factory / cmm_w[i].pu);
   		else							*(float *   )cmm_w[i].p = (float   )(cmm_w[i].factory / cmm_w[i].pu);
	}

	cmm.dis_on = 1;

	cmm.di_local = 1;							// Ĭ���Ǵ���������   0:���ؿ��� remote 1:���������� Local
}


// �������ٲ��������������ֻ�ڴ�ģ�����õ����������
void cmm_arb(void)
{
	if(cmm.bclear_error && !PcsA_1.BatRunState)	    except_clear_error();	// ���ϸ�λ

    charge_speedA();    // ��������ʱ������ø���

}


//���
unsigned  cmm_data2real( unsigned rw_table, unsigned bwork, unsigned i, unsigned data,  float *preal)
{
	float tmp, tmp2;

	if(!rw_table) {			// rw_table=0:������������
		if(i >= CMM_READ_N)		return 0;
		
		if      (cmm_r[i].ptype=='U')	tmp = (unsigned)data;
		else if (cmm_r[i].ptype=='I')	tmp = (int     )data;
		else							tmp = (int     )data;
				
		tmp2 = tmp * cmm_r[i].resolution;
			
		if (!(bwork && cmm_r[i].rsw=='S') && tmp2>=cmm_r[i].low && tmp2<=cmm_r[i].upper){
			*preal = tmp2 / cmm_r[i].pu;
			return 1;
		}
		return 0;
		
	}else{			// rw_table=1:������������

		if(i >= CMM_WRITE_N)	return 0;
			
		if      (cmm_w[i].ptype=='U')	tmp = (unsigned)data;
		else if (cmm_w[i].ptype=='I')	tmp = (int     )data;
		else							tmp = (int     )data;
			
		tmp2 = tmp * cmm_w[i].resolution;

		if (tmp2>=cmm_w[i].low && tmp2<=cmm_w[i].upper){
			*preal = tmp2 / cmm_w[i].pu;
			return 1;
		}
		return 0;
	}
}


//��д
//void cmm_real2data(unsigned rw_table, unsigned id,  unsigned i, unsigned *pdata)
void cmm_real2data(unsigned rw_table, unsigned i, unsigned offset, unsigned *pdata)
{
	float tmp, tmp2;
	
	if(!rw_table) {				// rw_table=0:������������
		if(i >= CMM_READ_N)		return;
		
		if      (cmm_r[i].ptype=='U')	tmp = *(unsigned *   )cmm_r[i].p;
		else if (cmm_r[i].ptype=='I')	tmp = *(int *  		 )cmm_r[i].p;
		else							tmp = *(float *    	 )cmm_r[i].p;
		
		tmp2 = tmp  *  cmm_r[i].pu   /  cmm_r[i].resolution;
		*pdata = F2U(tmp2);		
	}else{						// rw_table=1:������������

		if(i >= CMM_WRITE_N)	return;

		if      (cmm_w[i].ptype=='U')	tmp = *((unsigned * )cmm_w[i].p + offset);
		else if (cmm_w[i].ptype=='I')	tmp = *((int *  	)cmm_w[i].p + offset);
		else							tmp = *((float *    )cmm_w[i].p + offset);
		
		tmp2 = tmp  *  cmm_w[i].pu   /  cmm_w[i].resolution;
		*pdata = F2U(tmp2);		 
	}
}


void cmm_real_handle(unsigned rw_table, unsigned id, unsigned i, float real)
{
	if(!rw_table) {			// rw_table=0:������������
		if      (cmm_r[i].ptype=='U')	*(unsigned *)cmm_r[i].p = (unsigned)real;
		else if (cmm_r[i].ptype=='I')	*(int *     )cmm_r[i].p = (int     )real;
		else							*(float *   )cmm_r[i].p = (float   )real;
	}else if(rw_table){		// rw_table=1:������������
		if      (cmm_w[i].ptype=='U')	*(unsigned *)cmm_w[i].p = (unsigned)real;
		else if (cmm_w[i].ptype=='I')	*(int *     )cmm_w[i].p = (int     )real;
		else							*(float *   )cmm_w[i].p = (float   )real;
	}
}


//���-->��DSP	R��
void cmm_read_pnl(unsigned bwork)
{
	static unsigned i=0;
	unsigned data;
	float real;
	static unsigned di_local_old = 1;
	static unsigned cnt_cmm_err = 0;							// ��Ļͨ�Ź��ϼ�����

#define CMM_IN_START	6										// �����Զ�̿���,�������˼��������������ú���ʾ
#define CMM_IN_END		10

	if(!cmm.dis_on)										return;	// ����������������ĳ���,ȥ����

	if (cmm.di_local != di_local_old){							// ��/Զ�̿����л�
		if (cmm.di_local)	i = 0;	else	i = CMM_IN_START;	// 0:���ؿ��� remote 1:���������� Local
		di_local_old = cmm.di_local;
	}

	//������壬������ֵ
	if (!drv_scib_read(cmm_r[i].addr, &data)){
		if (cnt_cmm_err < 10){
			cnt_cmm_err++;	
		}else{
//bltest			EXCEPT_JUDGE_SET(1, ERROR_COMM);					// ��Ļͨ�Ź���
			cnt_cmm_err = 0;
		}
		return;
	}

	if (cmm_data2real(CMM_RW_TABLE_R, bwork, i, data,  &real))	cmm_real_handle(CMM_RW_TABLE_R, 0, i, real);

	if (i < (CMM_READ_N-1))	i++;	else 	i=0;			// ���������ƣ���ַ��0��CMM_READ_N-1
}



// ��DSP --> ���	W��
void cmm_write_pnl(void)
{
	static unsigned i=0;
	unsigned data;

	if (!cmm.dis_on)								return;		// �������� �� û���� ����

	cmm_real2data(CMM_RW_TABLE_W, i, 0,	&data);					// rw_table=1:������������

	drv_scib_write(cmm_w[i].addr, data);

	if (i < (CMM_WRITE_N-1))		i++;	else 	i=0;
}


// ��������д����
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19
void cmm_reset_parameter(void)
{
	static CLK clk_clear = CLK_DEFAULTS;
	static unsigned turn_flag = 0;
	unsigned index = 0;
	static unsigned reset_index = 0;
	static unsigned array[5] = {0,0,0,0,0};

	if (!turn_flag) {

		if ((except.error)&&(cmm.bready)) 					array[index++] = 1;  // ��дbready
		if ((except.error)&&(cmm.bstart_chg)) 	 			array[index++] = 2;  // ��дbstart_lsc
		if ((except.error)&&(cmm.bstart_dchg))	 			array[index++] = 3;  // ��дbstart_ssc
		if (clk_Bjudge1(&clk_clear, cmm.bclear_error, 2000L)) {
			except_clear_error();							array[index++] = 4;} // ��дbclear_error
	//	if ( ) array[index++] = ; // ��д
		turn_flag = 1;
	}
	if (array[reset_index]) {
		switch (array[reset_index]) {
			case 1:		cmm_read_reset1((void *)(&cmm.bready));			array[reset_index++] = 0;	break;    // ��дbready            
			case 2:		cmm_read_reset1((void *)(&cmm.bstart_chg));		array[reset_index++] = 0;	break;    // ��дbstart_LSC
			case 3:		cmm_read_reset1((void *)(&cmm.bstart_dchg));	array[reset_index++] = 0;	break;    // ��дbstart_GSC
			case 4:		cmm_read_reset1((void *)(&cmm.bclear_error));	array[reset_index++] = 0;	break;    // ��дbclear_error      
			//case  :	cmm_read_reset1( );								array[reset_index++] = 0;	break;    // ��?                  
			default:	break;
		}
	}else{ // ���ֽ���
		reset_index = 0;
		turn_flag = 0;
	}
}


// ��������ĳ��ȡ�Ĳ�������λ�ɹ���ֵ��cmm_read_reset((void *)cmm.bready);
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19
void cmm_read_reset1(void *p)
{
	unsigned i;
	float tmp;
	
	for (i=0; i<CMM_READ_N;	i++){
   		if (p == cmm_r[i].p){
   			
   			if      (cmm_r[i].ptype=='U')	*(unsigned *)cmm_r[i].p = (unsigned)(cmm_r[i].factory / cmm_r[i].pu);
	   		else if (cmm_r[i].ptype=='I')	*(int *     )cmm_r[i].p = (int     )(cmm_r[i].factory / cmm_r[i].pu);
   			else							*(float *   )cmm_r[i].p = (float   )(cmm_r[i].factory / cmm_r[i].pu);

			tmp = cmm_r[i].factory / cmm_r[i].resolution;
   			drv_scib_write(cmm_r[i].addr, F2U(tmp));

			return;		//break;
		}
	}
}
		
void cmm_lemp_fan(void)
{
	static CLK clk0 = CLK_DEFAULTS;

	if (!clk_Bjudgen(&clk0, 1, 100L)) 	return;			// 1sִ��һ�εĳ�ʼ��

	if (fsm_chg_bwork){
		drv_cpld_do_yellow(1);		// ���ָʾ��
	}else{
		drv_cpld_do_yellow(0);
	}
	if (fsm_dchg_bwork){
		drv_cpld_do_blue(1);		// �ŵ�ָʾ��
		drv_cpld_do_red(0);         // ���Ƿŵ���һ�����·
	}else{
		drv_cpld_do_blue(0);		// 
		drv_cpld_do_red(1);
	}
}
