// 描述：头文件。如何应用，结构定义等都在此
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19


#ifndef __CHARGE_H__
#define __CHARGE_H__


extern unsigned charge_bworkA;
extern unsigned charge_bworkA;
extern unsigned charge_chg_start;
extern unsigned charge_dchg_start;
extern unsigned charge_dir;
extern unsigned charge_laidian;
extern float charge_udc_ref;
extern float charge_udc_sef;
extern float charge_iabc;



extern unsigned charge_close_dchg_ok;
extern unsigned charge_close_dchg_s;
extern unsigned charge_close_chg_ok;
extern unsigned charge_close_chg_s;

void charge_init1(void);
void charge_initA(void);
void charge_speedA(void);
void charge_loopA(void);

void chage_power_key(void);
void chage_error(void);
unsigned charge_close_dchg(void);
unsigned charge_close_chg(unsigned time);
void charge_chg_setI(void);
void charge_chg_downI(void);
unsigned charge_chg_ok(void);
unsigned charge_dchg_ok(void);
void charge_close_chg_clear_s(void);
void charge_close_dchg_clear_s(void);
unsigned charge_power_relay(unsigned bflag);



#endif  // __CHARGE_H__

