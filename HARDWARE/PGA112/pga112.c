#include "pga112.h"
#include "delay.h"
#include "usart1.h"	
// #include "led_indication.h"
//u8 amplifier_gain_value =Gn01Chn1 ;    

//u8 amplifier_gain_value = Gn02Chn1;  //����������adx�Դ���2200��ʩ��������ʮ��~4093�� ��4093�ָ����ȶ�ֵ�ܿ졣

//u8 amplifier_gain_value =Gn16Chn1 ;  



//u8 amplifier_gain_value =Gn64Chn1 ;//����������adxֵ���ȶ���ʩ��������ʮ��~4093����4093�ָ����ȶ�ֵ������






u16 TxData;

void PGA112_Init(void)   //�����PGA112����,���ó�дģʽ���������档
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	
	//GPIOA15��NSS��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	
	PGA112_CS = 1;			//SPI PGA��ѡ��
	//SPI1_Init();		   			//��ʼ��SPI
	
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_2);		//����Ϊ42Mʱ��,����ģʽ
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_4);		//����Ϊ21Mʱ��	
      // SPI1_SetSpeed(SPI_BaudRatePrescaler_64);   //1.3125M
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_128);      //656.25K
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_8);     //10.5M
	 //SPI1_SetSpeed(SPI_BaudRatePrescaler_256);   //328.125K
	 
	//printf("\r\n SPI1 init ok!\r\n");
	
	
   // TxData = 0x2a00 + amplifier_gain_value;
	
        delay_ms(10);
		
	    PGA112_Write(PGA1112_GAIN_1);

        printf("\r\nPGA112���ú���\r\n");	
        //delay_ms(1000);		
	
	
}



void PGA112_Write(u16 TxData)   
{   
	 PGA112_CS = 0;  
     delay_us(10);
	
	 SPI1_WriteBytes(TxData);               //ͨ��SPIд��PGA�������� 
	 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);       //�жϷ������,������Ҫ����TXE����־���У���������
                                                                         //ʾ������ʱ��ͼ��ʾ֮ǰ����Ϊ	ʱ���źŻ�û�з����꣬Ƭѡ�������ˡ�����
	 delay_us(10);

	 PGA112_CS = 1;                            //ȡ��Ƭѡ  
}







