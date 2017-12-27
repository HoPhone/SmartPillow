#ifndef __DM634_H
#define __DM634_H	 
#include "sys.h" 



//MCU控制DM634的三个端口定义
//IO操作函数	 
#define  DAI    PEout(4) // 串行数据输入
#define  DCK    PEout(5) //	 时钟信号输入
#define  LAT    PEout(6)  //  锁存信号输入



void DM634_Init(void);//初始化
void Send_Byte_to_DM634(u8 txd);			//IIC发送一个字节

#endif



