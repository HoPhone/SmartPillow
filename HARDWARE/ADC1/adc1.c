#include "adc1.h"
#include "delay.h"	
#include "usart1.h"	
#include "arm_math.h"
#include "stm32f4xx.h"
#include <stdlib.h>
#include <string.h>
#include "mic_sample.h"
#include <stdio.h> 
#include "snore_detection.h"

//版本控制器测试

__IO u16 ADC_ConvertedValue[SAMPLE_SIZE];   //__IO  u16				//buffer0
__IO u16 ADC_ConvertedValue2[SAMPLE_SIZE];										//buffer1
u8 adc_sample = 0;	
short i , j;

#define MIC_SAMPLE_BUFF_SIZE      (128*2)

float adc_dma_data[128]={0};
float adc_dma_data2[128]={0};

extern int event_flag2;


/*								DMA buffer采集								*/
//by Phone

void  Adc_Init(uint32_t Frequency)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef       ADC_InitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
     
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//??GPIOB??
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //??ADC2??
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//??DMA2
     
     
    /* DMA2 Stream3 channel1 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//0x0000FFFF
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue;		//buffer0
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = SAMPLE_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_DoubleBufferModeConfig(DMA2_Stream0, (uint32_t)ADC_ConvertedValue2, DMA_Memory_0);
  DMA_DoubleBufferModeCmd(DMA2_Stream0, ENABLE);
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);  //DMA2??????
     
  DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF3);      //???
  DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF3);      //???     
     
     
  DMA_Cmd(DMA2_Stream0, ENABLE);
     
     
 
  //????ADC2?? IO?
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//Pb0 ??8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//????
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//?????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???  
  
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);   //ADC2??
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);    //????   
  
     
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//????
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//???????????5???
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA??
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//???4???ADCCLK=PCLK2/4=84/4=21Mhz,ADC????????36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//???
     
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12???
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//?????  
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//??????
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;//??????,??????
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//???   
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1????????? ??????????1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC???
     
     
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_144Cycles );    //ADC2,ADC??,144???,?????????????,???? 134K
 
/* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
 
/* Enable ADC2 DMA */
  ADC_DMACmd(ADC1, ENABLE);
     
  
  ADC_Cmd(ADC1, ENABLE);//??AD??? 
     
     
     /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
     
     
	//<<< Use Configuration Wizard in Context Menu >>>
 
	//  <o> 设置TIMER Period
	TIM_TimeBaseStructure.TIM_Period = 124; // SystemCoreClock / 2 / 10000000 - 1;    //PWM ??   10us  50K
	//  <o> 设置TIMER Prescaler
  TIM_TimeBaseStructure.TIM_Prescaler = 39; //10000000 / 16000 - 1;    //?????    ????  84M/84=1M		16ksps 12-21

	//<<< end of configuration section >>>   
    /* Time base configuration */
  
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
     
    /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_Pulse = 0x04;
 
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
 
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
     
  TIM_ARRPreloadConfig(TIM2, ENABLE);
     
     /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
	

 
}                 


void DMA2_Stream0_IRQHandler(void)
{
	int t=0;
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		DMA_ClearFlag(DMA2_Stream0,DMA_FLAG_TCIF0); 

			if (DMA_GetCurrentMemoryTarget(DMA2_Stream0))
			{
				//访问buffer1
				PBout(14) = 1;
				for(t = 0; t < 128; t++)
				{
					adc_dma_data[t]=ADC_ConvertedValue[t];//取dma buffer0的数据
					adc_dma_data2[t]=adc_dma_data[t]/32768.0F;
				}
				event_flag2=SnoreDetect_int_proc(adc_dma_data2);
			}
			else
			{
				//访问buffer0
				PBout(14) = 0;
				for(t = 0; t < 128; t++)
				{
					adc_dma_data[t]=ADC_ConvertedValue2[t];//取dma buffer1的数据
					adc_dma_data2[t]=adc_dma_data[t]/32768.0F;					
				}
				event_flag2=SnoreDetect_int_proc(adc_dma_data2);
			}
		}

	
}

//??ADC?
//ch: @ref ADC_channels 
//??? 0~16?????:ADC_Channel_0~ADC_Channel_16
//???:????
u16 Get_Adc(u8 ch)   
{
	  	//????ADC??????,????,????
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC??,480???,?????????????			    
  
	ADC_SoftwareStartConv(ADC1);		//?????ADC1?????????	
	 
	printf("\r\n??Get_Adc\r\n");
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//??????

	return ADC_GetConversionValue(ADC1);	//??????ADC1????????
}


