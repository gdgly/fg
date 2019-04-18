// 示例：声明对象、外部引用对象、引用函数
//    SVPWM svpwm=SVPWM_DEFAULTS;
//    extern SVPWM svpwm;
//    svpwm_func();

// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-06-19


#ifndef __SVPWM_H__
#define __SVPWM_H__

/*typedef struct {
                //内部变量：

                //输入：

                //输出：
} SVPWM;
*/

// 声明对象指针
//typedef SVPWM *SVPWM_HANDLE;

// 声明对象内部函数
void svpwm_dq(float u_alpha, float u_beta, float *p_ta, float *p_tb, float *p_tc);
void svpwm_mf(float T_sample, float f, float gain, float offset, float *p_ta, float *p_tb, float *p_tc);

#endif  // __SVPWM_H__

