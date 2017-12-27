#ifndef __I2S3_H
#define __I2S3_H
#include "sys.h"    									



extern void (*i2s_tx_callback)(void);		//IIS TX回调函数指针  

void I2S3_Init(u16 I2S_Standard ,u16 I2S_Mode,u16 I2S_Clock_Polarity,u16 I2S_DataFormat);  
u8 I2S3_SampleRate_Set(u32 samplerate);
void I2S3_TX_DMA_Init(u8* buf0,u8 *buf1,u16 num); 
void I2S_Play_Start(void); 
void I2S_Play_Stop(void);  

#endif





















