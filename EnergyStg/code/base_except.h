// 比较特殊的.c文件里定义了个实例

// 示例：声明对象、外部引用对象、引用函数
//    EXCEPT except=EXCEPT_DEFAULTS;
//    extern EXCEPT except;
//    except_judge_set_inner();
//    except_clear_error();
//    except_set_fname(p_fname); 	// 使用说明：请先给钩子函数赋值——函数名。p_fname必须指向这样的函数void fname(void)
//    except.shut_hook();			//当用.+函数名shut_hook，则调用外部钩子函数，一般用这个

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19


#ifndef __EXCEPT_H__
#define __EXCEPT_H__

#include "base_except_table.h"
#include "base.h"

typedef struct {
                //内部：
                CLK clk0;
                //输入：
                void (*shut_hook)(void);
                void (*reset_hook)(void);
                //输出：
				unsigned warn,warn_back;				//警告、上次警告
                unsigned error,error_back;				//故障、上次故障
                unsigned warn_old,warn_old_old;			//
                unsigned warn1, warn2, warn3, warn4;	//警告
                unsigned error1,error2,error3,error4;	//故障
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

// 声明对象指针
typedef EXCEPT *EXCEPT_HANDLE;

// 声明对象内部函数
void except_clear_error(void);
void except_judge_set_inner(unsigned bflag, unsigned n);
void except_hook_inner(void);
void except_set_fname(void (*p1_fname)(void), void (*p2_fname)(void));

extern EXCEPT except;

// 描述：警告故障处理，防止频繁调用函数增加函数开销，这里加一个壳
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19
//void 		except_judge_set(unsigned berror_flag, unsigned n)
//#define		EXCEPT_JUDGE_SET(berror_flag, n)		except_judge_set_inner(berror_flag, n)


#define		EXCEPT_JUDGE_SET(berror_flag, n)							\
{																		\
			if (  (berror_flag)||(except.warn == (n))  )				\
				except_judge_set_inner(berror_flag, n);					\
}

#endif  // __EXCEPT_H__

