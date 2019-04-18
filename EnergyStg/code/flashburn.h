// 描述：头文件。只被包括在四个文件中：zmain.c，zisr_fs.h、cunit.h、test.h
#ifndef __FLASHBURN_H__
#define __FLASHBURN_H__

#define	 FLASHBURN	    1		//有OK
		//屏蔽该语句就是RAM运行，有了该语句就是烧写FLASH。
		//两个CMD文件和zmain用到


#define	 DEBUG	   		_DEBUG	//屏蔽OK
		//屏蔽该语句就是正常运行，有了该语句就是工厂调试。调用test_main和test_fs两个函数。
		//test、zmain和zisr_fs文件用到


//#define  SIMULATOR		1		//屏蔽OK
		//该语句就是正常运行，有了该语句就是软件仿真。调用cunit和/或zisr_fs函数。
		//cunit、zmain和zisr_fs文件用到
		//zmain:屏蔽寄存器初始化，for循环中除cunit外一切函数。zisr_fs:中断函数变成普通函数



// 宏定义说明： 
// if语句，			必须加{}，	比如 {if(a)	(a=b+2);}		能防止if else 悬挂
// 所有多条语句，	必须加{}，	比如 {a=b+2;	c=b+3;}		被if调用时，只执行一条语句
// 凡是加{}，再if调用时，if必须加大括号，否则编译出错。比如，if(){ TP();	}else

#endif  // __FLASHBURN_H__

