// �Ƚ������.c�ļ��ﶨ���˸�ʵ��

// ʾ�������������ⲿ���ö������ú���
//    EXCEPT except=EXCEPT_DEFAULTS;
//    extern EXCEPT except;
//    except_judge_set_inner();
//    except_clear_error();
//    except_set_fname(p_fname); 	// ʹ��˵�������ȸ����Ӻ�����ֵ������������p_fname����ָ�������ĺ���void fname(void)
//    except.shut_hook();			//����.+������shut_hook��������ⲿ���Ӻ�����һ�������

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19


#ifndef __EXCEPT_H__
#define __EXCEPT_H__

#include "base_except_table.h"
#include "base.h"

typedef struct {
                //�ڲ���
                CLK clk0;
                //���룺
                void (*shut_hook)(void);
                void (*reset_hook)(void);
                //�����
				unsigned warn,warn_back;				//���桢�ϴξ���
                unsigned error,error_back;				//���ϡ��ϴι���
                unsigned warn_old,warn_old_old;			//
                unsigned warn1, warn2, warn3, warn4;	//����
                unsigned error1,error2,error3,error4;	//����
                unsigned error_old,error_old_old;		//

} EXCEPT;

#define EXCEPT_DEFAULTS {	CLK_DEFAULTS,				\
							except_hook_inner,			\
							except_hook_inner,			\
											0,0,		\
											0,0,		\
											0,0,0,0,	\
											0,0,0,0,	\
											0,0,0,0		}

// ��������ָ��
typedef EXCEPT *EXCEPT_HANDLE;

// ���������ڲ�����
void except_clear_error(void);
void except_judge_set_inner(unsigned bflag, unsigned n);
void except_hook_inner(void);
void except_set_fname(void (*p1_fname)(void), void (*p2_fname)(void));

extern EXCEPT except;

// ������������ϴ�����ֹƵ�����ú������Ӻ��������������һ����
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19
//void 		except_judge_set(unsigned berror_flag, unsigned n)
//#define		EXCEPT_JUDGE_SET(berror_flag, n)		except_judge_set_inner(berror_flag, n)


#define		EXCEPT_JUDGE_SET(berror_flag, n)							\
{																		\
			if (  (berror_flag)||(except.warn == (n))  )				\
				except_judge_set_inner(berror_flag, n);					\
}

#endif  // __EXCEPT_H__

