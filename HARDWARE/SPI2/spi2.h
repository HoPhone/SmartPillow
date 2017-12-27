#ifndef __SPI2_H
#define __SPI2_H
#include "sys.h"
	
#define	 TM1620_CS  PBout(9)  		//TM1620的片选信号

 	  	    													  
void SPI2_Init(void);			 //初始化SPI2口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI1速度   
void SPI2_WriteByte(u8 TxData);//SPI1总线读写一个字节
		 
#endif


