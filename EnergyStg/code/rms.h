// 示例：声明对象、引用函数
//       RMS rms=RMS_DEFAULTS;


#ifndef __RMS_H__
#define __RMS_H__

extern float rms_ua_in, 	rms_ub_in, 		rms_uc_in;
extern float rms_ia_in, 	rms_ib_in, 		rms_ic_in;
extern float rms_ua_out, 	rms_ub_out, 	rms_uc_out;
extern float rms_ia_out, 	rms_ib_out, 	rms_ic_out;
extern float rms_ie;
extern float rms_uac,       rms_uacm;
extern float rms_uac1,      rms_uacm1;      // 滤波后的有效值
extern float rms_uac_max,   rms_uac_min;

void rms_init1(void);
void rms_input_fs(void);
void rms_calc(void);

#endif  // __RMS_H__
