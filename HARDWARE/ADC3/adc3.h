#ifndef __ADC3_H
#define __ADC3_H	
#include "sys.h" 
 
 							   
void Adc3_Init(void); 				//ADCͨ����ʼ��
u16  Get_Adc3(u8 ch); 				//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
#endif 















