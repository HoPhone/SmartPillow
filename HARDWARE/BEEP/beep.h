#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h" 

//BEEP端口定义
#define BEEP PEout(9)	// 蜂鸣器控制IO 

void BEEP_Init(void);//初始化
void BEEP_ON(void);
void BEEP_OFF(void);


#endif

