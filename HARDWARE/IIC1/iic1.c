
#include "iic1.h"
#include "delay.h"


//初始化IIC
void IIC1_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

	//GPIOB6,B7初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	IIC1_SCL = 1;
	IIC1_SDA = 1;
}


//产生IIC起始信号
void IIC1_Start(void)
{
	SDA1_OUT();     //sda线输出
	IIC1_SDA = 1;	  	  
	IIC1_SCL = 1;
	delay_us(4);
 	IIC1_SDA = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC1_SCL = 0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC1_Stop(void)
{
	SDA1_OUT();//sda线输出
	IIC1_SCL = 0;
	IIC1_SDA = 0;//STOP:when CLK is high, DATA change form low to high
 	delay_us(4);
	IIC1_SCL = 1; 
	IIC1_SDA = 1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC1_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA1_IN();      //SDA设置为输入  
	IIC1_SDA = 1;delay_us(1);	   
	IIC1_SCL = 1;delay_us(1);	 
	while(READ1_SDA)
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			IIC1_Stop();
			return 1;
		}
	}
	IIC1_SCL = 0;//时钟输出0 	   
	return 0;  
} 

//产生ACK应答
void IIC1_Ack(void)
{
	IIC1_SCL = 0;
	SDA1_OUT();
	IIC1_SDA = 0;
	delay_us(2);
	IIC1_SCL = 1;
	delay_us(2);
	IIC1_SCL = 0;
}

//不产生ACK应答		    
void IIC1_NAck(void)
{
	IIC1_SCL = 0;
	SDA1_OUT();
	IIC1_SDA = 1;
	delay_us(2);
	IIC1_SCL = 1;
	delay_us(2);
	IIC1_SCL = 0;
}		

//IIC发送一个字节
//返回从机有无应答
//1，有应答//0，无应答
//txd：8位的16进制地址
void IIC1_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA1_OUT(); 	    
    IIC1_SCL = 0;//拉低时钟开始数据传输
	
    for(t = 0; t < 8; t++)
    {              
        IIC1_SDA = (txd & 0x80) >> 7;
        txd <<= 1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC1_SCL = 1;
		delay_us(2); 
		IIC1_SCL = 0;	
		delay_us(2);
    }	 
} 	   

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive1 = 0;
	SDA1_IN();//SDA设置为输入
	
    for(i = 0; i < 8; i++ )
	{
        IIC1_SCL= 0; 
        delay_us(2);
		IIC1_SCL=1;
        receive1 <<= 1;
        if(READ1_SDA)
			receive1++;   
		delay_us(1); 
    }
	
    if (!ack)
        IIC1_NAck();//发送nACK
    else
        IIC1_Ack(); //发送ACK   
	
    return receive1;
}




