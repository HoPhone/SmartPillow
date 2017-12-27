
#include "iic1.h"
#include "delay.h"


//��ʼ��IIC
void IIC1_Init(void)
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
	
	IIC1_SCL = 1;
	IIC1_SDA = 1;
}


//����IIC��ʼ�ź�
void IIC1_Start(void)
{
	SDA1_OUT();     //sda�����
	IIC1_SDA = 1;	  	  
	IIC1_SCL = 1;
	delay_us(4);
 	IIC1_SDA = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC1_SCL = 0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC1_Stop(void)
{
	SDA1_OUT();//sda�����
	IIC1_SCL = 0;
	IIC1_SDA = 0;//STOP:when CLK is high, DATA change form low to high
 	delay_us(4);
	IIC1_SCL = 1; 
	IIC1_SDA = 1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC1_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA1_IN();      //SDA����Ϊ����  
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
	IIC1_SCL = 0;//ʱ�����0 	   
	return 0;  
} 

//����ACKӦ��
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

//������ACKӦ��		    
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

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��//0����Ӧ��
//txd��8λ��16���Ƶ�ַ
void IIC1_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA1_OUT(); 	    
    IIC1_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
	
    for(t = 0; t < 8; t++)
    {              
        IIC1_SDA = (txd & 0x80) >> 7;
        txd <<= 1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC1_SCL = 1;
		delay_us(2); 
		IIC1_SCL = 0;	
		delay_us(2);
    }	 
} 	   

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive1 = 0;
	SDA1_IN();//SDA����Ϊ����
	
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
        IIC1_NAck();//����nACK
    else
        IIC1_Ack(); //����ACK   
	
    return receive1;
}



