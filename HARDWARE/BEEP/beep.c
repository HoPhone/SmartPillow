#include "beep.h" 


void BEEP_Init(void)
{   
	GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOF时钟
  
    //初始化蜂鸣器对应引脚GPIOE9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
	
    GPIO_ResetBits(GPIOE,GPIO_Pin_9);  //蜂鸣器对应引脚GPIOE9拉低， 
}

void BEEP_ON(void)
{
	BEEP = 1;

}

void BEEP_OFF(void)
{
	BEEP = 0;

}

