#include "adc1.h"
#include "delay.h"	
#include "usart1.h"	


__IO u16 ADC_ConvertedValue = 0;   //__IO  u16
 
//初始化ADC			

void  DMA2_Init(void)
{    
	
	
	//NVIC_InitTypeDef   NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	
   
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DMA2时钟使能 
	
	
	DMA_DeInit(DMA2_Stream0);
	while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
	{
	}//等待DMA2_Stream0可配置 
		
    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel            = DMA_Channel_0;  //通道0 ADC1通道 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&ADC1 -> DR);//外设地址为:(u32)&ADC1->DR
    DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)&ADC_ConvertedValue;//DMA 地址
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;//外设到存储器(内存)
    DMA_InitStructure.DMA_BufferSize         = 1;//数据传输量 
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;//外设非增量模式
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;//存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度:16位
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;//存储器数据长度：16位 
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;// 使用循环模式,循环模式即在传输完一轮数据之后再重新传输，//这种方式很适合 ADC 不断采集数据的场合。
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;//高优先级
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable; //不使用FIFO模式        
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;//外设突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;//存储器突发单次传输
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);//初始化DMA Stream
	
	
   
    DMA_Cmd(DMA2_Stream0, ENABLE);
    
	printf("\r\n DMA配置好了\r\n");
}	


void  ADC1_Init(void)
{	
	
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	
	//先初始化ADC1通道14 IO口
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//PC4 通道14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
 
	printf("\r\n ADC_MDA的GPIO口初始化好了\r\n");
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
    ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//这是双ADC模式下的交错采样时间间隔设置，两个采样阶段之间的延迟5个时钟
    ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled; //多个ADC模式下，DMA失能
    ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
    ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
    ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;//12位分辨率
    ADC_InitStructure.ADC_ScanConvMode         = DISABLE;//非扫描模式	
    ADC_InitStructure.ADC_ContinuousConvMode   = ENABLE;//开启连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;//右对齐	
    ADC_InitStructure.ADC_NbrOfConversion      = 1;//顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
	printf("\r\n ADC_Init初始化了,\r\n");
	
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,采样时间为56周期                                                  //ADC_SampleTime_480Cycles 
	printf("\r\n ADC_RegularChannelConfig了\r\n");
	
    ADC_DMACmd(ADC1, ENABLE);  
	printf("\r\n ADC_DMACmd了\r\n");
	
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器
	printf("\r\n ADC_Cmd了\r\n");
	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	
	
}	

      //DMA无需中断，ADC转化完之后，直接就引发DMA，把数字量转移到内存变量中了。


//void ADC_DMARequestAfterLastTransferCmd(ADC_TypeDef* ADCx, FunctionalState NewState);//DDS位	


//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
uint16_t Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

/*
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
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
	 
 //关闭了连续采样，所以作者通过for循环，调用Get_Adc,控制打开ADC采样功能。

*/






