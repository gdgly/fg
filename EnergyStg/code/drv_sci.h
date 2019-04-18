//    驱动层sci模块头文件
//	  scib是高速通信:
//    drv_scib_init();  scib寄存器初始化;只能一次,停机时不能重新初始化
//	  drv_chk_scib();  scib通信错误检测,用户循环检测sci的通信错误,有错误则将sci软件复位一次,错误次数太多,可告警sci通信错误.
//	  drv_scib_send();    放在pwm中断中定时发送
//	  drv_scib_receive(); 放在pwm中断中查询接收
//
//	  scic是低速通信:用法同scib,波特率设的低,是9600
//	  

#ifndef __DRV_SCI_H__
#define __DRV_SCI_H__
/*

typedef struct {
                // 内部变量：

                // 输入：

                // 输出：
} DRV_SCI;

#define DRV_SCI_DEFAULTS {              }

// 声明对象指针
typedef DRV_SCI *DRV_SCI_HANDLE;
*/

//----------------------- 函数声明区--------------------------------//

void drv_scib_init1(void);
void drv_scib_write(unsigned addr,unsigned data);
unsigned drv_scib_read(unsigned addr, unsigned *pdata);

void drv_scic_init1(void);
void drv_scic_write(unsigned cmd, unsigned len, unsigned offset, unsigned *data);
unsigned drv_scic_read_buf(unsigned offset, unsigned *pdata);
unsigned drv_scic_write_buf(unsigned offset, unsigned *pdata);
unsigned drv_scic_xgate_uart(unsigned cmd, unsigned len, unsigned mod_offset, unsigned *data,  unsigned *ackdata, unsigned ack_len);


#endif  // __DRV_SCI_H__

