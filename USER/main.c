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
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init(168);  //初始化延时函数
	uart3_init(115200);		//初始化串口波特率为115200
	
	/*************************      定时器周期定义        *****************************************/	
//	TIM3_Int_Init(125-1, 84-1);//定时器时钟84M，分频系数84，所以84M/84=1Mhz的计数频率，计数125次为125us //改动：8000采样率
//	TIM_Cmd(TIM3, ENABLE); //使能定时器3
	
	
//	RTC_TimeTypeDef RTC_TimeStruct;
//	RTC_DateTypeDef RTC_DateStruct;
    My_RTC_Init();		 		//初始化RTC
//	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits, 0);		//配置WAKE UP中断,1秒钟中断一次.配置的是秒时钟
  

	
	delay_ms(10);

  delay_ms(10);
  //DMA2_Init();
	Adc_Init(SAMPLE_SIZE);
	NVIC_Config();
	ADC_SoftwareStartConv(ADC1);
	GPIO_Configuration();
	//ADC1_Init();
	printf("\r\n ADC1_DMA2 init ok!\r\n");
	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池 	
	printf("\r\n内存池!\r\n");
	
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                              //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                             //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                          //50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                               //上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);				   	                                         //将端口GPIOB进行初始化配置
	PBout(12) = 1;
	PBout(14) = 1;	
}
		


//void TIM3_Int_Init(u16 arr, u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
//	
//    TIM_TimeBaseInitStructure.TIM_Period        = arr; 	//自动重装载值
//	TIM_TimeBaseInitStructure.TIM_Prescaler     = psc;  //定时器分频
//	TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up; //向上计数模式
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);//初始化TIM3
//	
//	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE); //允许定时器3更新中断
//	//TIM_Cmd(TIM3, ENABLE); //使能定时器3
//	
//	NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn; //定时器3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x03; //子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//}


////定时器3中断服务函数
///*
//将ADC_ConvertedValue的值，即是转化之后的值，（它本身是以极快的速率不断更新的，更新频率为ADC1的采样频率）
// 每5ms传一次值到adc_sample_buffer数组中。
//*/
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
//	{
///*************************      采集一秒数据        *****************************************/
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
//	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除中断标志位
//}