#include "sys.h"
#include "usart1.h"	
#include "stdint.h"
#include "usually.h"
#include "stdio.h"
////////////////////////////////////////////////////////////////////////////////// 	 
////如果使用ucos,则包括下面的头文件即可.
//#if SYSTEM_SUPPORT_OS
//#include "includes.h"					//ucos 使用	  
//#endif


//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
//#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART3 , (unsigned char)ch);
	while(!(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == SET));//循环发送,直到发送完毕   
	USART3->DR = (u8) ch;      
	return ch;
}
#endif
 
/*
//重定向C库函数printf到USART1
int fputc(int ch, FILE *f)
{  
   USART_SendData(USART1, (u8) ch);
   while(!(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET))
   {
   }
   return ch;
}
//重定向C库函数scanf到USART1
int fgetc(FILE *f)
{
   while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET))
   {
   }  
   return (USART_ReceiveData(USART1));
}
*/


#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA = 0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void uart3_init(u32 bound)
{
   //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10复用为USART1
	
	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
    USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器

#endif
	
}


void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntEnter();    
//#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		
		if((USART3_RX_STA & 0x8000) == 0)//接收未完成
		{
			if(USART3_RX_STA & 0x4000)//接收到了0x0d
			{
				if(Res != 0x0a)
					USART3_RX_STA = 0;//接收错误,重新开始
				else 
					USART3_RX_STA |= 0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res == 0x0d)
					USART3_RX_STA |= 0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = Res ;
					USART3_RX_STA++;
					
					if(USART3_RX_STA > (USART3_REC_LEN - 1))
						USART3_RX_STA = 0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	  OSIntExit();  											 
//#endif
} 

void USART_SendNData( USART_TypeDef* USARTx, uint8_t *Data, uint32_t len)
{
	uint32_t i;
	
	for(i = 0; i < len; i++)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, Data[i]); 
	}
}






#endif	

 



