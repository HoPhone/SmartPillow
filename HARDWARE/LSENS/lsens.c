#include "lsens.h"
#include "delay.h"
#include "adc.h" 
  
//��ʼ������������
void Lsens_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
	
    //�ȳ�ʼ��ADC3ͨ��11IO��
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PC1 ͨ��11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
	Adc3_Init();
}



//��ȡLight Sens��ֵ
//0~100��0,�;100,���� 
u8 Lsens_Get_Val(void)
{
	u32 temp_val = 0;
	u8 t;
	
	for(t = 0; t < LSENS_READ_TIMES; t++)
	{
		temp_val += Get_Adc3(ADC_Channel_11);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val /= LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	
	if(temp_val > 4000)
		temp_val = 4000;
	
	return 100 - (temp_val / 40);
}

//u8 lsens;
	//lsens=Lsens_Get_Val()*0.7;











