#ifndef __IIC1_H
#define __IIC1_H
#include "sys.h" 

   	   		   
//IO��������
//#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
//#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ

//IO��������
#define SDA1_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB7����ģʽ
#define SDA1_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB7���ģʽ


//IO��������	 
#define IIC1_SCL    PBout(8) //SCL
#define IIC1_SDA    PBout(7) //SDA	 
#define READ1_SDA   PBin(7)  //����SDA 


//IIC���в�������
void IIC1_Init(void);                //��ʼ��IIC��IO��				 
void IIC1_Start(void);				//����IIC��ʼ�ź�
void IIC1_Stop(void);	  			//����IICֹͣ�ź�
void IIC1_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC1_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC1_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC1_Ack(void);					//IIC����ACK�ź�
void IIC1_NAck(void);				//IIC������ACK�ź�

//void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	

#endif


