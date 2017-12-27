#include "beep.h" 


void BEEP_Init(void)
{   
	GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOFʱ��
  
    //��ʼ����������Ӧ����GPIOE9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIO
	
    GPIO_ResetBits(GPIOE,GPIO_Pin_9);  //��������Ӧ����GPIOE9���ͣ� 
}

void BEEP_ON(void)
{
	BEEP = 1;

}

void BEEP_OFF(void)
{
	BEEP = 0;

}

