#ifndef __ADC1_H
#define __ADC1_H
#include "sys.h"	 



void DMA2_Init(void);
void ADC1_Init(void); 				//ADC通道初始化

//u16 Get_Adc(u8 ch) ;
uint16_t Get_Adc(u8 ch);
void  Adc3_Init(void);
u16 Get_Adc3(u8 ch);





#endif




