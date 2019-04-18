// 示例：声明对象、外部引用对象、引用函数
//    PROTECT protect=PROTECT_DEFAULTS;
//    protect_init();

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19


#ifndef __PROTECT_H__
#define __PROTECT_H__


// 声明对象内部函数
void protect_init(void);
void protect_over(void);
void protect_fs(void);

extern unsigned protect_k;
extern unsigned protect_close_dis;
extern float    protect_uouta;



#endif  // __PROTECT_H__

