#ifndef __SPI1_H
#define __SPI1_H
#include "sys.h"
	
#define	 PGA112_CS  PAout(15)  		//PGA112��Ƭѡ�ź�

 	  	    													  
void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
void SPI1_WriteBytes(u16 TxData);//
 //void SPI1_WriteBytes(u8 TxData);//

#endif


