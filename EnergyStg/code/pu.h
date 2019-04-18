// 描述：PU头文件，方便外部引用
// 作者：王潞钢
// 版本：4.0
// 日期：星期三,2013-6-19

#ifndef __PU_H__
#define __PU_H__


//------------------------- 标幺化额定值---------------------------------
#define  ULRMSPU 		80.0							// 线电压有效值	380
#define  UACRMSPU 		(ULRMSPU / C_SQRT3)				// 相电压有效值	220
#define  IACRMSPU 		(50.0)							// 相电流有效值
#define  IDCPU	 		(100.0)							// 电池充放电电流

#define  UACPU 			(UACRMSPU * C_SQRT2)			// 相电压峰值	65.3
#define  IACPU 			(IACRMSPU * C_SQRT2)			// 相电流峰值	

#define  SPU 			(UACRMSPU * C_SQRT3 * IACRMSPU * C_SQRT3/1000.0) 	// 视在功率峰值，单位KVA	=33KW

#define  UDCPU    		100.0							// 额定直流母线电压



#endif  // __PU_H__

