#include "lsens.h"
#include "delay.h"
#include "adc.h" 
  
//初始化光敏传感器
void Lsens_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
	
    //先初始化ADC3通道11IO口
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PC1 通道11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
	Adc3_Init();
}



//读取Light Sens的值
//0~100：0,最暗;100,最亮 
u8 Lsens_Get_Val(void)
{
	u32 temp_val = 0;
	u8 t;
	
	for(t = 0; t < LSENS_READ_TIMES; t++)
	{
		temp_val += Get_Adc3(ADC_Channel_11);	//读取ADC值
		delay_ms(5);
	}
	temp_val /= LSENS_READ_TIMES;//得到平均值 
	
	if(temp_val > 4000)
		temp_val = 4000;
	
	return 100 - (temp_val / 40);
}

//u8 lsens;
	//lsens=Lsens_Get_Val()*0.7;











