#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h" 

//BEEP�˿ڶ���
#define BEEP PEout(9)	// ����������IO 

void BEEP_Init(void);//��ʼ��
void BEEP_ON(void);
void BEEP_OFF(void);


#endif

