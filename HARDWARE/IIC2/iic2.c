#include "iic2.h"
#include "delay.h"


//��ʼ��IIC2
void IIC2_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

	//GPIOB6,B7��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	IIC2_SCL = 1;
	IIC2_SDA = 1;
}


//����IIC2��ʼ�ź�
void IIC2_Start(void)
{
	SDA2_OUT();     //sda�����
	IIC2_SDA = 1;	  	  
	IIC2_SCL = 1;
	delay_us(4);
 	IIC2_SDA = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC2_SCL = 0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IIC2ֹͣ�ź�
void IIC2_Stop(void)
{
	SDA2_OUT();//sda�����
	IIC2_SCL = 0;
	IIC2_SDA = 0;//STOP:when CLK is high, DATA change form low to high
 	delay_us(4);
	IIC2_SCL = 1; 
	IIC2_SDA = 1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC2_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA2_IN();      //SDA����Ϊ����  
	IIC2_SDA = 1;delay_us(1);	   
	IIC2_SCL = 1;delay_us(1);	 
	while(READ2_SDA)
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			IIC2_Stop();
			return 1;
		}
	}
	IIC2_SCL = 0;//ʱ�����0 	   
	return 0;  
} 

//����ACKӦ��
void IIC2_Ack(void)
{
	IIC2_SCL = 0;
	SDA2_OUT();
	IIC2_SDA = 0;
	delay_us(2);
	IIC2_SCL = 1;
	delay_us(2);
	IIC2_SCL = 0;
}

//������ACKӦ��		    
void IIC2_NAck(void)
{       
	IIC2_SCL = 0;
	SDA2_OUT();
	IIC2_SDA = 1;
	delay_us(2);
	IIC2_SCL = 1;
	delay_us(2);
	IIC2_SCL = 0;
}		

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��//0����Ӧ��
//txd��8λ��16���Ƶ�ַ
void IIC2_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA2_OUT(); 	    
    IIC2_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
	
    for(t = 0; t < 8; t++)
    {              
        IIC2_SDA = (txd & 0x80) >> 7;
        txd <<= 1; 	
		/*if(txd & 0x80)
			IIC2_SDA = 1;
		else
			IIC2_SDA = 0;*/	
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC2_SCL = 1;
		delay_us(2); 
		IIC2_SCL = 0;	
		delay_us(2);
    }	 
} 	   

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC2_Read_Byte(unsigned char ack)
{
	unsigned char i,receive2 = 0;
	SDA2_IN();//SDA����Ϊ����
	
    for(i = 0; i < 8; i++ )
	{
        IIC2_SCL= 0; 
        delay_us(2);
		IIC2_SCL=1;
        receive2 <<= 1;
        if(READ2_SDA)
			receive2++;   
		delay_us(1); 
    }
	
    if (!ack)
        IIC2_NAck();//����nACK
    else
        IIC2_Ack(); //����ACK   
	
    return receive2;
}
