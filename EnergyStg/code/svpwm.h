// ʾ�������������ⲿ���ö������ú���
//    SVPWM svpwm=SVPWM_DEFAULTS;
//    extern SVPWM svpwm;
//    svpwm_func();

// ������ͷ�ļ������Ӧ�ã��ṹ����ȶ��ڴ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-06-19


#ifndef __SVPWM_H__
#define __SVPWM_H__

/*typedef struct {
                //�ڲ�������

                //���룺

                //�����
} SVPWM;
*/

// ��������ָ��
//typedef SVPWM *SVPWM_HANDLE;

// ���������ڲ�����
void svpwm_dq(float u_alpha, float u_beta, float *p_ta, float *p_tb, float *p_tc);
void svpwm_mf(float T_sample, float f, float gain, float offset, float *p_ta, float *p_tb, float *p_tc);

#endif  // __SVPWM_H__

