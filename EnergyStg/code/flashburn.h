// ������ͷ�ļ���ֻ���������ĸ��ļ��У�zmain.c��zisr_fs.h��cunit.h��test.h
#ifndef __FLASHBURN_H__
#define __FLASHBURN_H__

#define	 FLASHBURN	    1		//��OK
		//���θ�������RAM���У����˸���������дFLASH��
		//����CMD�ļ���zmain�õ�


#define	 DEBUG	   		_DEBUG	//����OK
		//���θ��������������У����˸������ǹ������ԡ�����test_main��test_fs����������
		//test��zmain��zisr_fs�ļ��õ�


//#define  SIMULATOR		1		//����OK
		//���������������У����˸�������������档����cunit��/��zisr_fs������
		//cunit��zmain��zisr_fs�ļ��õ�
		//zmain:���μĴ�����ʼ����forѭ���г�cunit��һ�к�����zisr_fs:�жϺ��������ͨ����



// �궨��˵���� 
// if��䣬			�����{}��	���� {if(a)	(a=b+2);}		�ܷ�ֹif else ����
// ���ж�����䣬	�����{}��	���� {a=b+2;	c=b+3;}		��if����ʱ��ִֻ��һ�����
// ���Ǽ�{}����if����ʱ��if����Ӵ����ţ��������������磬if(){ TP();	}else

#endif  // __FLASHBURN_H__

