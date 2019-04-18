#include "base.h"
#include "base_clk.h"

CLK_INNER clk_inner = CLK_INNER_DEFAULTS;		//�Ƚ������.c�ļ��ﶨ���˸�ʵ��

// ������ʱ�Ӽ�ʱ
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

//void clk_click(unsigned value)	
//{
//	clk_inner.ms0_1 = value;		//�ڲ�ʱ������Ժ�����ٶȣ����Ϊ2^32/1000/3600s/24h/30d=1.65��
//}


// ����������ʱ��ʱ�䣬ֻ�����һ��
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19

//void clk_start_bell(CLK  *pclk)
//{
//	pclk->bell=clk_inner.ms;
//}
#define clk_start_bell(pclk)   {pclk->bell = clk_inner.ms0_1;}


// ������ʱ���ж�
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19
//unsigned int clk_Bjudge_bell(const CLK  *pclk, unsigned long  interval)
//{
//	if ( (clk_inner.ms - pclk->bell) >= interval){
//		return 1;
//	}else{
//		return 0;
//	}
//}
											//�ڴ˴�����10,��Ϊ��0.1ms����
#define clk_Bjudge_bell(pclk, interval)		(  ( (clk_inner.ms0_1 - pclk->bell) >= interval*10 )?	1:	0  )




// ��Ϊÿ��clk_Bjudge1����������OFF��ʹ�ܳ�ʼ�������Բ���ר�ű�д��ʼ��������


// ������һ�ζ�ʱ��Bon=OFF�ܳ�ʼ��,Bon=ON������
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19
unsigned int clk_Bjudge1(CLK  *pclk, unsigned Bon,	unsigned long  interval)
{
	if (!Bon)								{							pclk->Bout=0;	pclk->Bstate=0;	return 0;}	// ��ʼ��; ��ʱ������,���漸��������
	if (pclk->Bout)							{															return 1;}	// ��˾�
	if (!pclk->Bstate)						{clk_start_bell(pclk);						pclk->Bstate=1;	return 0;}	// ���ӿ�ʼ��ʱ;��¼��ǰʱ��,���ر�ʶ�ù�
	if (!clk_Bjudge_bell(pclk,interval))	{															return 0;}	// ����û��ʱ��,����0,�´λ��ܽ���	
																		pclk->Bout=1;	pclk->Bstate=0;	return 1;	// ���ӵ�ʱ��,����1,���Ӷ�ʱ���
	
//	if (Bon==0)															{							pclk->Bout=0;	pclk->Bstate=0;	return pclk->Bout;}
//	if (Bon==1 && pclk->Bout==1)										{							pclk->Bout=1;	pclk->Bstate=0;	return pclk->Bout;}
//	if (Bon==1 && pclk->Bout==0 && pclk->Bstate==0)						{clk_start_bell(pclk);	pclk->Bout=0;	pclk->Bstate=1;	return pclk->Bout;}
//	
//	if (Bon==1 && pclk->Bout==0 && pclk->Bstate==1 && clk_Bjudge_bell(pclk,interval)==0)	{	pclk->Bout=0;	pclk->Bstate=1;	return pclk->Bout;}
//	
//	if (Bon==1 && pclk->Bout==0 && pclk->Bstate==1 && clk_Bjudge_bell(pclk,interval)==1)	{	pclk->Bout=1;	pclk->Bstate=0;	return pclk->Bout;}
}



// �������ظ���ʱ��Bon=0�ܳ�ʼ��,Bon=1������
// ���ߣ���º��
// �汾��4.0
// ���ڣ�������,2013-6-19
unsigned int clk_Bjudgen(CLK  *pclk, unsigned Bon,	unsigned long  interval)
{
	if (!Bon)								{							pclk->Bstate=0;	return 0;}
	if (!pclk->Bstate)						{clk_start_bell(pclk);		pclk->Bstate=1;	return 0;}	
	if (!clk_Bjudge_bell(pclk,interval))	{											return 0;}	
																		pclk->Bstate=0;	return 1;
	
//	if (Bon==0)																	{							pclk->Bstate=0;	return 0;}
//	if (Bon==1 && pclk->Bstate==0)												{clk_start_bell(pclk);	pclk->Bstate=1;	return 0;}	
//	if (Bon==1 && pclk->Bstate==1 && clk_Bjudge_bell(pclk,interval)==0)			{							pclk->Bstate=1;	return 0;}	
//	if (Bon==1 && pclk->Bstate==1 && clk_Bjudge_bell(pclk,interval)==1)			{							pclk->Bstate=0;	return 1;}
}

