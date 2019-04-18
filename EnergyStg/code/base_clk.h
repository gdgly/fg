// �Ƚ������.c�ļ��ﶨ���˸�ʵ��

// ʾ�������������ⲿ���ö������ú���
// �ϸ���˵��ģ���������ģ�飺bellģ���clkģ�飬��˶�bellģ���������Ҫclk��ַ
//    CLK clk0 = CLK_DEFAULTS;
//    clk_click(nkHZ);
//	  clk_Bjudge1(&clk0, ON, 20);
//	  clk_Bjudgen(&clk0, OFF, 20);


//	3����ʱִ��һ�Σ�����ѭ���ȴ�����������Ƚ��ټ�
//	����timer0�Ǿֲ���������
//	while( !clk_Bjudgen(&clk0, 1, 20) ) ;				//��ѭ���ȴ�20ms
//	clk_Bjudgen(&clk0, 0, 20);							//��ʼ��
//	......ִ���жϳɹ���ĳ���;

//	1����ʱִ��һ�Σ���PLC��ʱ�����
//	����һ����̬��ȫ�ֱ���clk0��
//	z=clk_Bjudge1(&clk0, 1, 20);

//	2����ʱִ�������
//	����һ����̬��ȫ�ֱ���clk0��
//	if (!clk_Bjudgen(&clk0, 1, 20))	 	return;
//	......ִ���жϳɹ���Ķ�ʱ����;



// ������ʱ��ģ��ͷ�ļ�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

#ifndef __CLK_H__
#define __CLK_H__

typedef struct {
	unsigned long  bell;				// ����
	unsigned       Bstate;				// ���뿪�ر�ʶ,0=�ܽ�����������(��)�����ܽ��������жϣ�1=���ܽ�����������(��)���ܽ��������жϣ�
	unsigned int   Bout;				// Bout��clk_Bclk1()�õ������״̬���ڴ�һ���塣
} CLK;
#define CLK_DEFAULTS {	0,0,0}


typedef struct {
	// ���
	//unsigned long  ms;				// ʱ�ӻ�������, 1msһ��
	unsigned long  ms0_1;				// ʱ�ӻ�������, 0.1msһ��
} CLK_INNER;

#define CLK_INNER_DEFAULTS {0}		// ����1

// ��������ָ��
typedef CLK *CLK_HANDLE;

// ���������ڲ�����
//void clk_click(unsigned nkhz);
unsigned int clk_Bjudge1(CLK  *pclk, unsigned Bon,	unsigned long  interval);
unsigned int clk_Bjudgen(CLK  *pclk, unsigned Bon,	unsigned long  interval);

extern CLK clk;
extern CLK_INNER clk_inner;


// ������ʱ�Ӽ�ʱ
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19
//void 	clk_click(unsigned value)
#define	CLK_CLICK_FS(value)		CLK_CLICK(value)
#define	CLK_CLICK(value)		(clk_inner.ms0_1 = value)			//clk_click()����д��һ����
#define	CLK_CLICK_TEST()		(clk_inner.ms0_1++)					//�����ڲ��ԣ�����cunit.c��


#endif  // __CLK_H__
