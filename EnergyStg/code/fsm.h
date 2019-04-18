// ʾ�������������ⲿ���ö������ú���
//    FSM fsm=FSM_DEFAULTS;
//    extern FSM fsm;


// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19


#ifndef __FSM_H__
#define __FSM_H__

#define FSM_N	5

enum FSM_S{			//��״̬���൱�ڳ������壬����ǰ����Ϊ�����������õ��������
	FSM_S0,
	FSM_S1,
	FSM_S2,
	FSM_S3,
	FSM_S4,
	FSM_S_END
};

enum FSM_CHILD{		//��״̬���൱�ڳ�������
	FSM_IN,
	FSM_DO,
	FSM_OUT
};



typedef struct {
                //�ڲ�������
				enum FSM_S	state,	state_pre;

  				//����
  				void (*pf[FSM_N][3])(void);

                //�����
} FSM;

#define FSM_DEFAULTS 	{FSM_S0, FSM_S0}

// ��������ָ��
typedef FSM *FSM_HANDLE;

// ���������ڲ�����
void fsm_init(FSM_HANDLE p);
void fsm_func(FSM_HANDLE p);
void fsm_judge_go(FSM_HANDLE p, unsigned bflag, enum FSM_S state);
void fsm_go(FSM_HANDLE p, enum FSM_S state);
void fsm_set_fname(FSM_HANDLE p, enum FSM_S state, enum FSM_CHILD child, void (*p_fname)(void));
void fsm_hook_inner(void);

enum FSM_S fsm_read(FSM_HANDLE p);



#endif  // __FSM_H__


/*	  Ӧ�þ���:

	
	void fa(void);	void fb(void);	void fc(void);	unsigned i=9, u=0;	unsigned i=9, u=0;

	#include "fsm.h"	

	//������5��״̬����
	void fsm_f1_in(void);
	void fsm_f1(void);
	void fsm_f1_out(void);
	void fsm_f2(void);
	void fsm_f3(void);

main()
{
	fsm_set_fname(FSM_S1, FSM_DO,  fsm_f1);		fsm_set_fname(FSM_S1, FSM_IN,  fsm_f1_in);
												fsm_set_fname(FSM_S1, FSM_OUT, fsm_f1_out);
	fsm_set_fname(FSM_S2, FSM_DO,  fsm_f2);
	fsm_set_fname(FSM_S3, FSM_DO,  fsm_f3);

    fsm_init();

   	for(;;){
   		fsm_func();
	}
}


void fsm_f1_in(void)	{fa();     									}	//����ת�ƺ���
void fsm_f1(void)		{fa();fb();		fsm_judge_go(i>0, FSM_S2);	}
void fsm_f1_out(void)	{fc();     									}	//����ת�ƺ���
void fsm_f2(void)		{fa();fb();		fsm_judge_go(i>0, FSM_S3);	}
void fsm_f3(void)		{fb();fc();    	fsm_judge_go(i>0, FSM_S2);	
  	    	  							if (u>0)	fsm_go(FSM_S1);	}

//		��ά��� = ״̬��ת��
//  	   		S1		S2		S3		S4		S5
//		S1: 			1V
//		S2: 					V
//		S3:		V		V
//		S3:	
//		S4:	
//		S5:	

*/

