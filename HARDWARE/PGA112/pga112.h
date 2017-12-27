#ifndef __PGA112_H
#define __PGA112_H
#include "spi1.h"
#include "sys.h"
	
#define	 PGA112_CS  PAout(15)  		//PGA112的片选信号
/*指令表

WRITE  0X2A00 + G3G2G1G0CH3CH2CH1CH0  ---> 0X2A00 + 0x11
D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4  D3 D2  D1  D0
 0	 0	 1	 0	 1	 0	 1	0 G3 G2 G1 G0 CH3 CH2 CH1 CH0

0x11 --> 选中BINARY GAIN2、CH1
*/

/*
//CH0
#define    Gn01Chn0 0x00  //!< Gain:1. 
#define    Gn02Chn0 0x10  //!< Gain:2. 
#define    Gn04Chn0 0x20  //!< Gain:4. 
#define    Gn08Chn0 0x30  //!< Gain:8. 
#define    Gn16Chn0 0x40  //!< Gain:16.
#define    Gn32Chn0 0x50  //!< Gain:32. 
#define    Gn64Chn0 0x60  //!< Gain:64. 
#define    Gn128Chn0 0x70  //!< Gain:128. 

//CH1
#define    Gn01Chn1 0x01  //!< Gain:1. 
#define    Gn02Chn1 0x11  //!< Gain:2.//常用的是这个 
#define    Gn04Chn1 0x21  //!< Gain:4. 
#define    Gn08Chn1 0x31  //!< Gain:8. 
#define    Gn16Chn1 0x41  //!< Gain:16. 
#define    Gn32Chn1 0x51  //!< Gain:32. 
#define    Gn64Chn1 0x61  //!< Gain:64. 
#define    Gn128Chn1 0x71  //!< Gain:128. 


*/

#define PGA1112_GAIN_1 0x2a01
#define PGA1112_GAIN_2 0x2a11
#define PGA1112_GAIN_4 0X2a21
#define PGA1112_GAIN_8 0x2a31
#define PGA1112_GAIN_16 0x2a41
#define PGA1112_GAIN_32 0X2a51
#define PGA1112_GAIN_64 0x2a61
#define PGA1112_GAIN_128 0x2a71


	
void PGA112_Init(void);			 //初始化SPI1口
void PGA112_Write(u16 TxData); 
	
#endif


