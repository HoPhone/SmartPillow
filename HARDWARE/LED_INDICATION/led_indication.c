#include "led_indication.h"
#include "delay.h"

void LED_INDICATION_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOFʱ��

    //GPIOE0,1,2,3��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//LED0��LED1��ӦIO��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
	
	DO_LED1 = 0;
	DO_LED2 = 0;
	DO_LED3 = 0;
    DO_LED4 = 0;
}

void DO_LED1_ON(void)
{
	DO_LED1 = 1;
}

void DO_LED2_ON(void)
{
	DO_LED2 = 1;
}

void DO_LED3_ON(void)
{
	DO_LED3 = 1;
}

void DO_LED4_ON(void)
{
	DO_LED4 = 1;
}

void DO_LED1_OFF(void)
{
	DO_LED1 = 0;
}

void DO_LED2_OFF(void)
{
	DO_LED2 = 0;
}

void DO_LED3_OFF(void)
{
	DO_LED3 = 0;
}

void DO_LED4_OFF(void)
{
	DO_LED4 = 0;
}

void DO_LED_ON_ALL(void)
{
	DO_LED1_ON();
	DO_LED2_ON();
	DO_LED3_ON();
	DO_LED4_ON();
}

void DO_LED_OFF_ALL(void)
{
	DO_LED1_OFF();
	DO_LED2_OFF();
	DO_LED3_OFF();
	DO_LED4_OFF();
}


