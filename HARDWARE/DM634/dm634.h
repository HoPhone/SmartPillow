#ifndef __DM634_H
#define __DM634_H	 
#include "sys.h" 



//MCU����DM634�������˿ڶ���
//IO��������	 
#define  DAI    PEout(4) // ������������
#define  DCK    PEout(5) //	 ʱ���ź�����
#define  LAT    PEout(6)  //  �����ź�����



void DM634_Init(void);//��ʼ��
void Send_Byte_to_DM634(u8 txd);			//IIC����һ���ֽ�

#endif



