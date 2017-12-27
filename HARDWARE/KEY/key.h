#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define  KEY0 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) //PB0
#define  KEY1 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)	//PB1 
#define  KEY2 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) //PA0



/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//P32

*/


#define  KEY0_PRES 	1
#define  KEY1_PRES	2
#define  KEY2_PRES	3


void KEY_Init(void);	//IO��ʼ��
u8 KEY_Scan(u8);  		//����ɨ�躯��	

#endif
