#include "timer.h"
#include <stdbool.h>
#include "usart1.h"	


#define NUMBER_SAMPLE_POINT (2000)
//extern short adc_value_signed;
//extern unsigned int seperation_buffer_index;
//extern unsigned short interval_set;
// short adc_sample_buffer[NUMBER_SAMPLE_POINT + 200];

//extern unsigned int seperation_buffer_index;
// unsigned short adc_buffer_index = 0;
//extern bool sample_data_seperation_start;
extern volatile u16 ADC_ConvertedValue;

//u8 a = 0;
//int cnt = 0;
//int i;
//signed short sample_data[40000];
//u8 flag = 0;
//signed int average_amp; 
//signed int data_amp[2500];
//signed int sum_data_amp;

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!



