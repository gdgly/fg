// 示例：声明对象、外部引用对象、引用函数
//    FSM fsm=FSM_DEFAULTS;
//    extern FSM fsm;


// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19


#ifndef __FSM_H__
#define __FSM_H__

#define FSM_N	5

enum FSM_S{			//主状态，相当于常数定义，放在前面因为函数声明中用到这个类型
	FSM_S0,
	FSM_S1,
	FSM_S2,
	FSM_S3,
	FSM_S4,
	FSM_S_END
};

enum FSM_CHILD{		//子状态，相当于常数定义
	FSM_IN,
	FSM_DO,
	FSM_OUT
};



typedef struct {
                //内部变量：
				enum FSM_S	state,	state_pre;

  				//输入
  				void (*pf[FSM_N][3])(void);

                //输出：
} FSM;

#define FSM_DEFAULTS 	{FSM_S0, FSM_S0}

// 声明对象指针
typedef FSM *FSM_HANDLE;

// 声明对象内部函数
void fsm_init(FSM_HANDLE p);
void fsm_func(FSM_HANDLE p);
void fsm_judge_go(FSM_HANDLE p, unsigned bflag, enum FSM_S state);
void fsm_go(FSM_HANDLE p, enum FSM_S state);
void fsm_set_fname(FSM_HANDLE p, enum FSM_S state, enum FSM_CHILD child, void (*p_fname)(void));
void fsm_hook_inner(void);

enum FSM_S fsm_read(FSM_HANDLE p);



#endif  // __FSM_H__


/*	  应用举例:

	
	void fa(void);	void fb(void);	void fc(void);	unsigned i=9, u=0;	unsigned i=9, u=0;

	#include "fsm.h"	

	//假设有5个状态函数
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


void fsm_f1_in(void)	{fa();     									}	//不用转移函数
void fsm_f1(void)		{fa();fb();		fsm_judge_go(i>0, FSM_S2);	}
void fsm_f1_out(void)	{fc();     									}	//不用转移函数
void fsm_f2(void)		{fa();fb();		fsm_judge_go(i>0, FSM_S3);	}
void fsm_f3(void)		{fb();fc();    	fsm_judge_go(i>0, FSM_S2);	
  	    	  							if (u>0)	fsm_go(FSM_S1);	}

//		二维表格 = 状态的转移
//  	   		S1		S2		S3		S4		S5
//		S1: 			1V
//		S2: 					V
//		S3:		V		V
//		S3:	
//		S4:	
//		S5:	

*/

