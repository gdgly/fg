#include "base.h"
#include "base_except.h"
#include "drv_pwm.h"

EXCEPT except=EXCEPT_DEFAULTS;

#define SET		SET16_BIT
#define CLR		CLR16_BIT
#define SETCLR(ACT, n, var1, var2, var3, var4)	\
{												\
	if (n<=16*1)                				\
		ACT(var1, n-(1     )); 				    \
	else if (n<=16*2)           				\
		ACT(var2, n-(1+16*1));      			\
	else if (n<=16*3)           				\
		ACT(var3, n-(1+16*2));      			\
	else if (n<=16*4)           				\
		ACT(var4, n-(1+16*3));      			\
}

/*
#define CHKCLR(n, var1, var2, var3, var4)							\
{																	\
	if (n<=16*1)                									\
		if (BCHECK16_BIT(var1, n-1-16*0))	CLR(var1, n-1-16*0);    \
	else if (n<=16*2)           									\
		if (BCHECK16_BIT(var1, n-1-16*1))	CLR(var1, n-1-16*1);    \
	else if (n<=16*3)           									\
		if (BCHECK16_BIT(var1, n-1-16*2))	CLR(var1, n-1-16*2);    \
	else if (n<=16*4)           									\
		if (BCHECK16_BIT(var1, n-1-16*3))	CLR(var1, n-1-16*3);    \
}
*/

// �������������
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

void except_clear_error(void)
{
	if (except.error >= 1000) return;		//������2000��ʵ����1000��
	except.error_back = except.error;
	except.error = 0;
	except.error1 = except.error2 = except.error3 = except.error4 =0;
	except.reset_hook();
}



// �������жϡ����þ�����ϣ����������һ��
// 		 ����Ҫ��ʱ���,��ʱ200ms
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

void except_judge_set_inner(unsigned berror_flag, unsigned n)
{

	if (!berror_flag){		//�������
		if (except.warn==n && clk_Bjudge1(&except.clk0, 1, 200L)){
			except.warn = 0;
			SETCLR(CLR, except.warn, except.warn1, except.warn2, except.warn3, except.warn4);
		}
		
	}else{

		if (n<1000){		//�þ���
			(void)clk_Bjudge1(&except.clk0, 0, 200L);		//��ʼ��
			except.warn = n;								//warn = 0-999
			SETCLR(SET, except.warn, except.warn1, except.warn2, except.warn3, except.warn4);
		}else{				//�ù���
			except.shut_hook();
			if(except.error)	return;
			except.error = n - 1000;						//error��ֵǰ�ȼ�ȥ1000�����error = 0-999
			SETCLR(SET, except.error, except.error1, except.error2, except.error3, except.error4);
		}

	}
}




// ��������ʼ�����Ӻ�����
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

void except_set_fname(void (*p1_fname)(void), void (*p2_fname)(void))
{
	except.shut_hook = p1_fname;					//p_fname�Ǹ�����ָ�룬�õ��µĺ������ƺ󣬸�except.shut_hook
	except.reset_hook = p2_fname;
}


// �������ڲ����Ӻ���
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

void except_hook_inner(void)
{
	//ʲôҲ���������ã�������ǳ�ʼ����������ָ����ָ
}

