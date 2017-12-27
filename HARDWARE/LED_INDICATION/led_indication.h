#ifndef __LED_INDICATION_H
#define __LED_INDICATION_H
#include "sys.h" 


#define  DO_LED1  PEout(0)
#define  DO_LED2  PEout(1)
#define  DO_LED3  PEout(2)
#define  DO_LED4  PEout(3)
//DO_LED1 Running Status Indication
//DO_LED2 Wireless communication Indication
//DO_LED3 Sleep Cycle Indication  3/4两个灯不一定用。
//DO_LED4 Sleep Cycle Indication

#define  DI_SW1  PBout(0)
#define  DI_SW2  PBout(1)
#define  DI_SW3  PCout(0)
//#define  DO_LED4  PEout(3)

//GPIO_TypeDef *GPIOS[4] = {GPIOE,GPIOE,GPIOE,GPIOE};
//u16 PORT[4] = {GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3};

void LED_INDICATION_Init(void);//初始化
void DO_LED1_ON(void);
void DO_LED2_ON(void);
void DO_LED3_ON(void);
void DO_LED4_ON(void);
void DO_LED1_OFF(void);
void DO_LED2_OFF(void);
void DO_LED3_OFF(void);
void DO_LED4_OFF(void);
void DO_LED_ON_ALL(void);
void DO_LED_OFF_ALL(void);
//void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);



//unsigned int led_gpio_pins[4] = {DO_LED1, DO_LED2, DO_LED3, DO_LED4};





#endif

