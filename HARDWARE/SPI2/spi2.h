#ifndef __SPI2_H
#define __SPI2_H
#include "sys.h"
	
#define	 TM1620_CS  PBout(9)  		//TM1620��Ƭѡ�ź�

 	  	    													  
void SPI2_Init(void);			 //��ʼ��SPI2��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
void SPI2_WriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�
		 
#endif


