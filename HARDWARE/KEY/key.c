#include "key.h"
#include "delay.h" 
	 

//������ʼ������
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOA,GPIOBʱ��
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //KEY0 KEY1 ��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB0,1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // KEY2��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
	 
    
 
} 


//����������
//���ذ���ֵ
/*mode:0,��֧��������;1,֧��������;
  ��modeΪ0��ʱ��KEY_Scan��������֧����������ɨ��ĳ���������ð�������֮���
��Ҫ�ɿ������ܵڶ��δ��������򲻻�����Ӧ��������������ĺô����ǿ��Է�ֹ��һ�ζ��
��������������������Ҫ������ʱ��Ƚϲ����ʡ�

  ��modeΪ1��ʱ��KEY_Scan������֧�������������ĳ������һֱ���£����һֱ��
����������ļ�ֵ���������Է����ʵ�ֳ�����⡣

*/
/*
����ֵ��
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 

//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2!!  */
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up = 1;//�����ɿ��ı�־
	
	if(mode)
		key_up = 1;  //֧������	
	
	if(key_up && (KEY0==0 || KEY1==0 || KEY2==0))
	{
		delay_ms(10);//ȥ���� 
		key_up = 0;
		
		if(KEY0 == 0)
			return 1;
		else if(KEY1 == 0)
			return 2;
		else if(KEY2 == 0)
			return 3;
		
	}
	else if(KEY0 == 1 && KEY1 == 1 && KEY2 == 1)
		key_up = 1;
	
 	return 0;// �ް�������
}

