#ifndef __ADC1_H
#define __ADC1_H
#include "sys.h"	 

//<<< Use Configuration Wizard in Context Menu >>>
 
//  <o> ���ò���buffer�Ĵ�С
#define SAMPLE_SIZE			128

//<<< end of configuration section >>>
//����������


void DMA2_Init(void);
void ADC1_Init(void); 				//ADCͨ����ʼ��


//u16 Get_Adc(u8 ch) ;
u16 Get_Adc(u8 ch);
void  Adc_Init(uint32_t Frequency);






#endif




