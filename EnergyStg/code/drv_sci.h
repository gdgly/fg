//    ������sciģ��ͷ�ļ�
//	  scib�Ǹ���ͨ��:
//    drv_scib_init();  scib�Ĵ�����ʼ��;ֻ��һ��,ͣ��ʱ�������³�ʼ��
//	  drv_chk_scib();  scibͨ�Ŵ�����,�û�ѭ�����sci��ͨ�Ŵ���,�д�����sci�����λһ��,�������̫��,�ɸ澯sciͨ�Ŵ���.
//	  drv_scib_send();    ����pwm�ж��ж�ʱ����
//	  drv_scib_receive(); ����pwm�ж��в�ѯ����
//
//	  scic�ǵ���ͨ��:�÷�ͬscib,��������ĵ�,��9600
//	  

#ifndef __DRV_SCI_H__
#define __DRV_SCI_H__
/*

typedef struct {
                // �ڲ�������

                // ���룺

                // �����
} DRV_SCI;

#define DRV_SCI_DEFAULTS {              }

// ��������ָ��
typedef DRV_SCI *DRV_SCI_HANDLE;
*/

//----------------------- ����������--------------------------------//

void drv_scib_init1(void);
void drv_scib_write(unsigned addr,unsigned data);
unsigned drv_scib_read(unsigned addr, unsigned *pdata);

void drv_scic_init1(void);
void drv_scic_write(unsigned cmd, unsigned len, unsigned offset, unsigned *data);
unsigned drv_scic_read_buf(unsigned offset, unsigned *pdata);
unsigned drv_scic_write_buf(unsigned offset, unsigned *pdata);
unsigned drv_scic_xgate_uart(unsigned cmd, unsigned len, unsigned mod_offset, unsigned *data,  unsigned *ackdata, unsigned ack_len);


#endif  // __DRV_SCI_H__

