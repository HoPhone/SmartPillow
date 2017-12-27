#include "key.h"
#include "delay.h" 
	 

//按键初始化函数
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOA,GPIOB时钟
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //KEY0 KEY1 对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB0,1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // KEY2对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
	 
    
 
} 


//按键处理函数
//返回按键值
/*mode:0,不支持连续按;1,支持连续按;
  当mode为0的时候，KEY_Scan函数将不支持连续按，扫描某个按键，该按键按下之后必
须要松开，才能第二次触发，否则不会再响应这个按键，这样的好处就是可以防止按一次多次
触发，而坏处就是在需要长按的时候比较不合适。

  当mode为1的时候，KEY_Scan函数将支持连续按，如果某个按键一直按下，则会一直返
回这个按键的键值，这样可以方便的实现长按检测。

*/
/*
返回值：
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 

//注意此函数有响应优先级,KEY0>KEY1>KEY2!!  */
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up = 1;//按键松开的标志
	
	if(mode)
		key_up = 1;  //支持连按	
	
	if(key_up && (KEY0==0 || KEY1==0 || KEY2==0))
	{
		delay_ms(10);//去抖动 
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
	
 	return 0;// 无按键按下
}

