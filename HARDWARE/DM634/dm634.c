#include "dm634.h"
#include "delay.h"


//初始化DM634
void IIC2_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOB时钟

	//GPIOB6,B7初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;//预留备用的，接LED灯。
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	
	LAT = 0;

}


void Send_Data_to_DM634(u16 txd)
{                        
    u16 t;       
    DCK = 0;//拉低时钟开始数据传输
	LAT = 0;
	//delay_us(2);
	
    for(t = 0; t < 16; t++)
    {              
        
		DAI = (txd & 0x8000) >> 15;
        txd <<= 1; 			
		delay_us(2);
		
		DCK = 1;	
		delay_us(2);
		DCK = 0;	
		
    }
	delay_us(2);
	LAT = 0;
} 	







