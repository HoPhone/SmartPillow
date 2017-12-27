#include "adc1.h"
#include "delay.h"	
#include "usart1.h"	


__IO u16 ADC_ConvertedValue = 0;   //__IO  u16
 
//��ʼ��ADC			

void  DMA2_Init(void)
{    
	
	
	//NVIC_InitTypeDef   NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	
   
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DMA2ʱ��ʹ�� 
	
	
	DMA_DeInit(DMA2_Stream0);
	while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
	{
	}//�ȴ�DMA2_Stream0������ 
		
    /* ���� DMA Stream */
    DMA_InitStructure.DMA_Channel            = DMA_Channel_0;  //ͨ��0 ADC1ͨ�� 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&ADC1 -> DR);//�����ַΪ:(u32)&ADC1->DR
    DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)&ADC_ConvertedValue;//DMA ��ַ
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;//���赽�洢��(�ڴ�)
    DMA_InitStructure.DMA_BufferSize         = 1;//���ݴ����� 
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;//���������ģʽ
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;//�洢������ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݳ���:16λ
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;//�洢�����ݳ��ȣ�16λ 
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;// ʹ��ѭ��ģʽ,ѭ��ģʽ���ڴ�����һ������֮�������´��䣬//���ַ�ʽ���ʺ� ADC ���ϲɼ����ݵĳ��ϡ�
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;//�����ȼ�
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable; //��ʹ��FIFOģʽ        
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;//����ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;//�洢��ͻ�����δ���
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);//��ʼ��DMA Stream
	
	
   
    DMA_Cmd(DMA2_Stream0, ENABLE);
    
	printf("\r\n DMA���ú���\r\n");
}	


void  ADC1_Init(void)
{	
	
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	
	//�ȳ�ʼ��ADC1ͨ��14 IO��
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//PC4 ͨ��14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
 
	printf("\r\n ADC_MDA��GPIO�ڳ�ʼ������\r\n");
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
    ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;//����ģʽ
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//����˫ADCģʽ�µĽ������ʱ�������ã����������׶�֮����ӳ�5��ʱ��
    ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled; //���ADCģʽ�£�DMAʧ��
    ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
    ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
    ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;//12λ�ֱ���
    ADC_InitStructure.ADC_ScanConvMode         = DISABLE;//��ɨ��ģʽ	
    ADC_InitStructure.ADC_ContinuousConvMode   = ENABLE;//��������ת��
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
    ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;//�Ҷ���	
    ADC_InitStructure.ADC_NbrOfConversion      = 1;//˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	printf("\r\n ADC_Init��ʼ����,\r\n");
	
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,����ʱ��Ϊ56����                                                  //ADC_SampleTime_480Cycles 
	printf("\r\n ADC_RegularChannelConfig��\r\n");
	
    ADC_DMACmd(ADC1, ENABLE);  
	printf("\r\n ADC_DMACmd��\r\n");
	
	ADC_Cmd(ADC1, ENABLE);//����ADת����
	printf("\r\n ADC_Cmd��\r\n");
	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	
	
}	

      //DMA�����жϣ�ADCת����֮��ֱ�Ӿ�����DMA����������ת�Ƶ��ڴ�������ˡ�


//void ADC_DMARequestAfterLastTransferCmd(ADC_TypeDef* ADCx, FunctionalState NewState);//DDSλ	


//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
uint16_t Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

/*
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 
 //�ر���������������������ͨ��forѭ��������Get_Adc,���ƴ�ADC�������ܡ�

*/






