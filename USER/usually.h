#ifndef __USUALLY_H
#define __USUALLY_H

//头文件调用
#include "stm32f10x.h"
#include "hardware.h"


//LED引脚定义
#define LED1 PDout(6)	   	//板子上对应于LD1
#define LED2 PDout(12)	   	//板子上对应于LD2
#define LED3 PDout(13)	   	//板子上对应于LD3

//按键引脚定义
#define KEY_USER  PCin(13) 	//板子上对应于按键USER
#define KEY_S  PEin(0)  	//板子上对应于按键JOY-SEN
#define KEY_D  PEin(1)  	//板子上对应于按键JOY-DOWN
#define KEY_L  PEin(2)  	//板子上对应于按键JOY-LEFT
#define KEY_R  PEin(3)  	//板子上对应于按键JOY-RIGHT
#define KEY_U  PEin(4)  	//板子上对应于按键JOY-UP

//声明一个枚举
enum  						
{							
	 USER=1,				//1
	 SEN,					//2
	 DOWN,					//3
	 LEFT,					//4
	 RIGHT,					//5
	 UP						//6
};

/***********************************************************************************
设置颜色宏定义
***********************************************************************************/
#define   BLACK        0x0000                    // 黑色： 0, 0, 0              
#define   NAVY         0x000F                    // 深蓝色： 0, 0, 128          
#define   DGREEN       0x03E0                    // 深绿色： 0, 128, 0          
#define   DCYAN        0x03EF                    // 深青色： 0, 128, 128        
#define   MAROON       0x7800                    // 深红色：128, 0, 0           
#define   PURPLE       0x780F                    // 紫色： 128, 0, 128          
#define   OLIVE        0x7BE0                    // 橄榄绿：128, 128, 0         
#define   LGRAY        0xC618                    // 灰白色：192, 192, 192        
#define   DGRAY        0x7BEF                    // 深灰色：128, 128, 128        
#define   BLUE         0x001F                    // 蓝色： 0, 0, 255            
#define   GREEN        0x07E0                 	 // 绿色： 0, 255, 0             
#define   CYAN         0x07FF                    // 青色： 0, 255, 255           
#define   RED          0xF800                    // 红色： 255, 0, 0             
#define   MAGENTA      0xF81F                    // 品红： 255, 0, 255          
#define   YELLOW       0xFFE0                    // 黄色： 255, 255, 0           
#define   WHITE        0xFFFF                    // 白色： 255, 255, 255         

extern void Delay_Ms(uint16_t time);  	 //声明外部延时函数
extern void Delay_Us(uint16_t time); 	 //声明外部延时函数

#endif
