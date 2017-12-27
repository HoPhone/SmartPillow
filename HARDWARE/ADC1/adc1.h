#ifndef __ADC1_H
#define __ADC1_H
#include "sys.h"	 

//<<< Use Configuration Wizard in Context Menu >>>
 
//  <o> 设置采样buffer的大小
#define SAMPLE_SIZE			128

//<<< end of configuration section >>>
//采样率设置


void DMA2_Init(void);
void ADC1_Init(void); 				//ADC通道初始化


//u16 Get_Adc(u8 ch) ;
u16 Get_Adc(u8 ch);
void  Adc_Init(uint32_t Frequency);






#endif




