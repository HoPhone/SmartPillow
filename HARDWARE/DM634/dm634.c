#include "dm634.h"
#include "delay.h"


//��ʼ��DM634
void IIC2_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOBʱ��

	//GPIOB6,B7��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;//Ԥ�����õģ���LED�ơ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	
	LAT = 0;

}


void Send_Data_to_DM634(u16 txd)
{                        
    u16 t;       
    DCK = 0;//����ʱ�ӿ�ʼ���ݴ���
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







