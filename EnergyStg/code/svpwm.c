#include "apply.h"
#include "svpwm.h"
#include "IQmathLib.h"
#include "svgen_dq.h"
#include "svgen_mf2.h"			//���˸Ľ�����������2���滻>>1 = x0.5

SVGENDQ svm_dq = SVGENDQ_DEFAULTS;

// ��������ȫ����TI�����˸���װ���ѡ�������������[-1 +1]
// ���ߣ���º��
// �汾��3.1
// ���ڣ�������,2011-12-28

void svpwm_dq(float u_alpha, float u_beta, float *p_ta, float *p_tb, float *p_tc)
{
	svm_dq.Ualpha = u_alpha; 	// Pass inputs to svpwm
	svm_dq.Ubeta = u_beta; 		// Pass inputs to svpwm
	SVGEN_MACRO(svm_dq); 		// Call compute macro for svpwm
	*p_ta = svm_dq.Ta; 			// Access the outputs of svpwm
	*p_tb = svm_dq.Tb; 			// Access the outputs of svpwm
	*p_tc = svm_dq.Tc; 			// Access the outputs of svpwm
}

SVGENMF svm_mf = SVGENMF_DEFAULTS;

// ��������ȫ����TI�����˸���װ���ѡ�������������[-1 +1]��������float���ڲ������޷���ʩ������f��gain���ɣ�offset�Ǹ�ֱ��ƫ����
// ���ߣ���º��
// �汾��3.1
// ���ڣ�������,2011-12-28

void svpwm_mf(float T_sample, float f, float gain, float offset, float *p_ta, float *p_tb, float *p_tc)
{
	svm_mf.FreqMax = 6.0 * 50.0 * T_sample; 
	
	svm_mf.Freq = f; 			// Pass inputs to svpwm
	svm_mf.Gain = gain; 		// Pass inputs to svpwm
	svm_mf.Offset = offset;		// Pass inputs to svpwm
	SVGENMF_MACRO(svm_mf); 		// Call compute macro for svpwm
	*p_ta = svm_mf.Ta; 			// Access the outputs of svpwm
	*p_tb = svm_mf.Tb; 			// Access the outputs of svpwm
	*p_tc = svm_mf.Tc; 			// Access the outputs of svpwm
}

