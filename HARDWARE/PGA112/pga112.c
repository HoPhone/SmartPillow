#include "pga112.h"
#include "delay.h"
#include "usart1.h"	
// #include "led_indication.h"
//u8 amplifier_gain_value =Gn01Chn1 ;    

//u8 amplifier_gain_value = Gn02Chn1;  //不加外力，adx略大于2200。施加外力，十几~4093。 从4093恢复到稳定值很快。

//u8 amplifier_gain_value =Gn16Chn1 ;  



//u8 amplifier_gain_value =Gn64Chn1 ;//不加外力，adx值不稳定。施加外力，十几~4093。从4093恢复到稳定值很慢。






u16 TxData;

void PGA112_Init(void)   //完成了PGA112配置,设置成写模式，还有增益。
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	
	//GPIOA15：NSS初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	
	PGA112_CS = 1;			//SPI PGA不选中
	//SPI1_Init();		   			//初始化SPI
	
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_2);		//设置为42M时钟,高速模式
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_4);		//设置为21M时钟	
      // SPI1_SetSpeed(SPI_BaudRatePrescaler_64);   //1.3125M
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_128);      //656.25K
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_8);     //10.5M
	 //SPI1_SetSpeed(SPI_BaudRatePrescaler_256);   //328.125K
	 
	//printf("\r\n SPI1 init ok!\r\n");
	
	
   // TxData = 0x2a00 + amplifier_gain_value;
	
        delay_ms(10);
		
	    PGA112_Write(PGA1112_GAIN_1);

        printf("\r\nPGA112配置好了\r\n");	
        //delay_ms(1000);		
	
	
}



void PGA112_Write(u16 TxData)   
{   
	 PGA112_CS = 0;  
     delay_us(10);
	
	 SPI1_WriteBytes(TxData);               //通过SPI写入PGA配置命令 
	 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);       //判断发送完成,这句很重要，用TXE做标志不行！！！！！
                                                                         //示波器的时序图显示之前是因为	时钟信号还没有发送完，片选就拉高了。。。
	 delay_us(10);

	 PGA112_CS = 1;                            //取消片选  
}







