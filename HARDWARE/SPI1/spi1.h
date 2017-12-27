#ifndef __SPI1_H
#define __SPI1_H
#include "sys.h"
	
#define	 PGA112_CS  PAout(15)  		//PGA112的片选信号

 	  	    													  
void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
void SPI1_WriteBytes(u16 TxData);//
 //void SPI1_WriteBytes(u8 TxData);//

#endif


