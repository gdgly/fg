#include "apply.h"
#include "cunit.h"
#include "test.h"

#ifdef SIMULATOR	//�е�Ԫ����ʱ




void cunit(void)
{
	static unsigned bfirst = 0;

	//��Ҫ��ʼ������������ڿ�����ʼ��ʱ���ᱻ����һ�Ρ�
	if (!bfirst){
		bfirst = 1;
		drv_pwm.ts = 1.0/10000;	//����������ʼ��ʱ�������ʼ��Ӳ���㣬���Ա��������
		fsm_inv_bwork = 0;
		return;
	}

	//��������
	test_ad();	//���������ѹ�͵�������

	// cmm.bstaie = 1;	// Ҳ���ֶ���1���ƿ��ػ�
	excita_loop();

	// fsm_inv_bwork = 1;	// Ҳ���ֶ���1���ƿ��ػ�
	vc_ssc_func();	

	CLK_CLICK_TEST();
}



#endif
/*
	static FILTER2	filter2_id;
	static FILTER2	filter2_iq;
	static FILTER2	filter2_idr = FILTER2_DEFAULTS;
	static FILTER2	filter2_iqr = FILTER2_DEFAULTS;

		#define FW_BD	5.0
		filter2_init_rapid(&filter2_id,  0.0, drv_pwm.ts, FW_BD);
		filter2_init_rapid(&filter2_iq,  0.0, drv_pwm.ts, FW_BD);
		filter2_init_rapid(&filter2_idr,  0.0, drv_pwm.ts, FW_BD);
		filter2_init_rapid(&filter2_iqr,  0.0, drv_pwm.ts, FW_BD);

//	FILTER2_FUNC_RAPID(filter2_id, id, id_f);
//	FILTER2_FUNC_RAPID(filter2_iq, iq, iq_f);
//	FILTER2_FUNC_RAPID(filter2_idr, idr, idr_f);
//	FILTER2_FUNC_RAPID(filter2_iqr, iqr, iqr_f);

*/

//	scope���� ����ʽ
//	�����˳��		=>		����vertical	=>		����horizon
//	1	2	3				6	4	2				6	3
//	4	5	6				5	3	1				5	2
//													4	1


