#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"  
#include "usart1.h"
#include "timer.h"
#include "rtc.h"

 #include "pga112.h"
 #include "adc1.h"
//#include "malloc.h" 

#include <stdbool.h>
//#include "sleep_manage.h"

#include "malloc.h"

#include "iwdg.h"
#include "stdlib.h"

#include "flash.h"

#include "core_cm4.h"             /* Cortex-M4 processor and core peripherals */
#include "system_stm32f4xx.h"
#include <stdint.h>
#include <math.h>

#define pi		3.14159265


//void TIM3_Int_Init(u16 arr, u16 psc);
//void TIM3_IRQHandler(void);
 void NVIC_Config(void);
 void GPIO_Configuration(void);

 extern u8 a;
 extern __IO u16 ADC_ConvertedValue[SAMPLE_SIZE];
 u32 adcx;
 extern u8 adc_sample;
 u8 a = 0;
 int cnt = 0;
 u8 flag = 1;
 int state = 0;
 int indicator = 0;
 int end_flag = 0;
 u8 snore_process_flag = 0;
 //short int sample_data[6400];
 int event_flag2;

	
int main(void)
{   
int i = 0;

	int addr = 0;
	char g = 0;
	

  SystemInit();
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init(168);  //��ʼ����ʱ����
	uart3_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	/*************************      ��ʱ�����ڶ���        *****************************************/	
//	TIM3_Int_Init(125-1, 84-1);//��ʱ��ʱ��84M����Ƶϵ��84������84M/84=1Mhz�ļ���Ƶ�ʣ�����125��Ϊ125us //�Ķ���8000������
//	TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3
	
	
//	RTC_TimeTypeDef RTC_TimeStruct;
//	RTC_DateTypeDef RTC_DateStruct;
    My_RTC_Init();		 		//��ʼ��RTC
//	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits, 0);		//����WAKE UP�ж�,1�����ж�һ��.���õ�����ʱ��
  

	
	delay_ms(10);

  delay_ms(10);
  //DMA2_Init();
	Adc_Init(SAMPLE_SIZE);
	NVIC_Config();
	ADC_SoftwareStartConv(ADC1);
	GPIO_Configuration();
	//ADC1_Init();
	printf("\r\n ADC1_DMA2 init ok!\r\n");
	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 	
	printf("\r\n�ڴ��!\r\n");
	
	//flash_config();
	flag = 0;
	
	
	delay_ms(10);
	
    while(1)
	{
		if (event_flag2 == 1)
		{
			PBout(12) = 0;
		}
		else
		{
			PBout(12) = 1;
		}
		
		
   }
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
     
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 
  /* Enable the DMA Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);   
}   

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;                                  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                              //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                             //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                          //50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                               //����
  GPIO_Init(GPIOB, &GPIO_InitStructure);				   	                                         //���˿�GPIOB���г�ʼ������
	PBout(12) = 1;
	PBout(14) = 1;	
}
		


//void TIM3_Int_Init(u16 arr, u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
//	
//    TIM_TimeBaseInitStructure.TIM_Period        = arr; 	//�Զ���װ��ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler     = psc;  //��ʱ����Ƶ
//	TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up; //���ϼ���ģʽ
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);//��ʼ��TIM3
//	
//	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
//	//TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3
//	
//	NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn; //��ʱ��3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //��ռ���ȼ�0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x03; //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//}


////��ʱ��3�жϷ�����
///*
//��ADC_ConvertedValue��ֵ������ת��֮���ֵ�������������Լ�������ʲ��ϸ��µģ�����Ƶ��ΪADC1�Ĳ���Ƶ�ʣ�
// ÿ5ms��һ��ֵ��adc_sample_buffer�����С�
//*/
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //����ж�
//	{
///*************************      �ɼ�һ������        *****************************************/
//				if (flag == 0)
//		{
//			sample_data[cnt] = ADC_ConvertedValue;//Get_Adc(ADC_Channel_14);
//			cnt++;
//			if (cnt >= 8000)
//			{
//				cnt = 0;
//				flag = 1;
//			}
//		}
//		
//		
//		
//	}
//	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //����жϱ�־λ
//}