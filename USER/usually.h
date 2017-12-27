#ifndef __USUALLY_H
#define __USUALLY_H

//ͷ�ļ�����
#include "stm32f10x.h"
#include "hardware.h"


//LED���Ŷ���
#define LED1 PDout(6)	   	//�����϶�Ӧ��LD1
#define LED2 PDout(12)	   	//�����϶�Ӧ��LD2
#define LED3 PDout(13)	   	//�����϶�Ӧ��LD3

//�������Ŷ���
#define KEY_USER  PCin(13) 	//�����϶�Ӧ�ڰ���USER
#define KEY_S  PEin(0)  	//�����϶�Ӧ�ڰ���JOY-SEN
#define KEY_D  PEin(1)  	//�����϶�Ӧ�ڰ���JOY-DOWN
#define KEY_L  PEin(2)  	//�����϶�Ӧ�ڰ���JOY-LEFT
#define KEY_R  PEin(3)  	//�����϶�Ӧ�ڰ���JOY-RIGHT
#define KEY_U  PEin(4)  	//�����϶�Ӧ�ڰ���JOY-UP

//����һ��ö��
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
������ɫ�궨��
***********************************************************************************/
#define   BLACK        0x0000                    // ��ɫ�� 0, 0, 0              
#define   NAVY         0x000F                    // ����ɫ�� 0, 0, 128          
#define   DGREEN       0x03E0                    // ����ɫ�� 0, 128, 0          
#define   DCYAN        0x03EF                    // ����ɫ�� 0, 128, 128        
#define   MAROON       0x7800                    // ���ɫ��128, 0, 0           
#define   PURPLE       0x780F                    // ��ɫ�� 128, 0, 128          
#define   OLIVE        0x7BE0                    // ����̣�128, 128, 0         
#define   LGRAY        0xC618                    // �Ұ�ɫ��192, 192, 192        
#define   DGRAY        0x7BEF                    // ���ɫ��128, 128, 128        
#define   BLUE         0x001F                    // ��ɫ�� 0, 0, 255            
#define   GREEN        0x07E0                 	 // ��ɫ�� 0, 255, 0             
#define   CYAN         0x07FF                    // ��ɫ�� 0, 255, 255           
#define   RED          0xF800                    // ��ɫ�� 255, 0, 0             
#define   MAGENTA      0xF81F                    // Ʒ�죺 255, 0, 255          
#define   YELLOW       0xFFE0                    // ��ɫ�� 255, 255, 0           
#define   WHITE        0xFFFF                    // ��ɫ�� 255, 255, 255         

extern void Delay_Ms(uint16_t time);  	 //�����ⲿ��ʱ����
extern void Delay_Us(uint16_t time); 	 //�����ⲿ��ʱ����

#endif
