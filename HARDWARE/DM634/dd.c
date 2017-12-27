/*这段时间在用时代民芯的MXT0106做全彩LED控制，SPARC V8架构的，刷新速度还是有一些问题，不过基本显示还可以了。

主函数    */


#include "config.h"
#include "stdio.h"
#include "flashTest.h"
#include "eIntTest.h"
#include "gpiotest.h"
#include "sysTimerTest.h"
#include "advTimerTest.h"
#include "counterTest.h"
#include "adcTest.h"
#include "iicTest.h"
#include "sdramTest.h"
#include "GPIOTest.h"


#define DDR_RST (1<<2)
#define DDR_LAT (1<<3)
#define DDR_SLB (1<<4)
#define DDR_SDA (1<<5)
#define DDR_SCL (1<<6)


#define DDR_openline_BIT0   (1<<0)
#define DDR_openline_BIT1   (1<<1)
#define DDR_openline_BIT2   (1<<2)
#define DDR_openline_BIT3   (1<<3)
#define DDR_openline_BIT4   (1<<4)
#define DDR_openline_BIT5   (1<<5)
#define DDR_openline_BIT6   (1<<6)
#define DDR_openline_BIT7   (1<<7)

#define DATA_RST (1<<2)
#define DATA_LAT (1<<3)
#define DATA_SLB (1<<4)
#define DATA_SDA (1<<5)
#define DATA_SCL (1<<6)
#define DATA_openline_BIT0   (1<<0)
#define DATA_openline_BIT1   (1<<1)
#define DATA_openline_BIT2   (1<<2)
#define DATA_openline_BIT3   (1<<3)
#define DATA_openline_BIT4   (1<<4)
#define DATA_openline_BIT5   (1<<5)
#define DATA_openline_BIT6   (1<<6)
#define DATA_openline_BIT7   (1<<7)

static StructCPURegs *regs = (StructCPURegs *)0x80000000;    /*指向配置寄存器*/

unsigned char dots[2][8][8][3];
unsigned char Gamma_Value[3] = {10,63,63};
//Gamma correctly value, every LED plane is different.value range is 0~63
//[3]:RGB data, 0 for Red; 1 for green, 2 for Blue
unsigned char Page_Index = 0; // the index of buffer
unsigned char row = 0;//the value of row in LED plane, from 0~7
unsigned char column = 0;//the value of every row, from 0~7
unsigned char color = 0;//the value of every dots, 0 is Red, 1 is Green, 2 is Blue
unsigned char line = 0;

/**************************************************
define the extern data zone
**************************************************/
extern unsigned char font8_8[92][8];
extern unsigned char pic[4][8][8][3];
/***************************************************
all parts inition functions zone
***************************************************/

/*
*函数说明:GPIO初始化 
*入口参数:无
*返回值:无
*/
INT32 GPIO_init(void)
{
    gpioInit(
    3 << 8 | (100 - 1) , /* 100分频 */
    0xff,                         /*  0~31 output */
    0xff,                         /* 32~63 output */
    0xff, /* 使能滤波 */
    0, /* 使能 GPIO0中断，上升沿有效，对应GPIO7 */
    0);/*使能  GPIO4中断，上升沿有效，对应中断号GPI32 */
    //输入输出设置 
    regs->pioDir1 = regs->pioDir1|DDR_RST|DDR_LAT|DDR_SLB|DDR_SDA|DDR_SCL;
    regs->pioDir2 = regs->pioDir2|DDR_openline_BIT0|DDR_openline_BIT1|DDR_openline_BIT2|DDR_openline_BIT3|DDR_openline_BIT4|DDR_openline_BIT5|DDR_openline_BIT6|DDR_openline_BIT7;
    //数据 
    //初始置0 
    regs->pioData1 = regs->pioData1&~(DATA_RST|DATA_LAT|DATA_SLB|DATA_SDA|DATA_SCL);
    regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);    
    //regs->pioData1 = regs->pioData1|(DATA_RST|DATA_LAT|DATA_SLB|DATA_SDA|DATA_SCL);
    //regs->pioData2 |= (DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7); 
    return 1;
}
/****************************************************
the LED Hardware operate functions zone
****************************************************/
void LED_SDA(unsigned char temp)
{
  if (temp) 
    regs->pioData1|=DDR_SDA;
  else
    regs->pioData1&=~DDR_SDA;
}

void LED_SCL(unsigned char temp)
{
  if (temp) 
    regs->pioData1|=DDR_SCL;
  else
    regs->pioData1&=~DDR_SCL;
}

void LED_RST(unsigned char temp)
{
  if (temp) 
    regs->pioData1|=DDR_RST;
  else
    regs->pioData1&=~DDR_RST;
}

void LED_LAT(unsigned char temp)
{
  if (temp) 
    regs->pioData1|=DDR_LAT;
  else
    regs->pioData1&=~DDR_LAT;
}

void LED_SLB(unsigned char temp)
{
  if (temp) 
    regs->pioData1|=DDR_SLB;
  else
    regs->pioData1&=~DDR_SLB;
}
/***************************************************
the LED datas operate functions zone
***************************************************/
void SetGamma()
{
  unsigned char i = 0;
  unsigned char j = 0;
  unsigned char k = 0;
  unsigned char temp = 0;
  LED_LAT(0);
  LED_SLB(0);
  for(k=0;k<8;k++)
    for(i = 3;i > 0 ;i--)
    {
      temp = Gamma_Value[i-1]<<2;
      for(j = 0;j<6;j++)
      {
        if(temp &0x80)
          LED_SDA(1);
        else
          LED_SDA(0);
        temp = temp << 1;
        LED_SCL(0);
        LED_SCL(1);
    }
  }
  LED_SLB(1);
}
void run(unsigned char k)
{
  unsigned char i = 0;
  unsigned char j = 0;
  unsigned char p = 0;
  unsigned char temp = 0;
  LED_SLB(1);
  LED_LAT(0);
  for(i = 0;i<8;i++)
  {
    
    for(j=0;j<3;j++)
    {
      temp = dots[Page_Index][k][i][2-j];
      for(p=0;p<8;p++)
      {
         if(temp & 0x80)
           LED_SDA(1);
         else
           LED_SDA(0);
           
         temp = temp<<1;  
         LED_SCL(0);
         LED_SCL(1);
       }
     }
  }
  LED_LAT(1);
  LED_LAT(0);
}
void open_line(unsigned char x)
{
  switch (x)
  {  
    case 0 :
        {
            regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);  
            regs->pioData2 = regs->pioData2|DATA_openline_BIT0;
            break;
        }    
    case 1 :
        {
            regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);  
            regs->pioData2 = regs->pioData2|DATA_openline_BIT1;
            break;
        } 
    case 2 :
        {
            regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);  
            regs->pioData2 = regs->pioData2|DATA_openline_BIT2;
            break;
        } 
    case 3 :
        {
            regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);  
            regs->pioData2 = regs->pioData2|DATA_openline_BIT3;
            break;
        } 
    case 4 :
        {
            regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);  
            regs->pioData2 = regs->pioData2|DATA_openline_BIT4;
            break;
        } 
    case 5 :
        {
            regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);  
            regs->pioData2 = regs->pioData2|DATA_openline_BIT5;
            break;
        } 
    case 6 :
        {
            regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);  
            regs->pioData2 = regs->pioData2|DATA_openline_BIT6;
            break;
        } 
    case 7 :
        {
            regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);  
            regs->pioData2 = regs->pioData2|DATA_openline_BIT7;
            break;
        } 
    default: 
        {
            regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);  
            break;
        } 
  }
}
/******************************************
the other operate functions zone
******************************************/
void LED_Delay(INT32 i)
{
  unsigned int y;
  y = i * 10;
  while(y--);
}
/********************************************************
Name:DispShowChar
Function:Display a English latter in LED matrix
Parameter:chr :the latter want to show
          R: the value of RED.   Range:RED 0~255
          G: the value of GREEN. Range:RED 0~255
          B: the value of BLUE.  Range:RED 0~255
          bias: the bias of a letter in LED Matrix.Range -7~7
********************************************************/
void DispShowChar(char chr,unsigned char R,unsigned char G,unsigned char B,char bias)
{
  unsigned char i,j,Page_Write,temp;
  unsigned char Char;
  unsigned char chrtemp[24] = {0};
  
  if ((bias > 8) || (bias < -8))
    return;
    
  
  Char = chr - 32;
  
  if(Page_Index == 0)
    Page_Write = 1;
  if(Page_Index == 1)
    Page_Write = 0;
  j = 8 - bias;
  for(i = 0;i< 8;i++)
  {
    chrtemp[j] = font8_8[Char][i];    
    j++;
  }  
  for(i = 0;i < 8;i++)
  {
    
    temp = chrtemp[i+8];
    for(j = 0;j < 8;j++)
    {
      if(temp & 0x80)
      {
        dots[Page_Write][j][i][0] = B;
        dots[Page_Write][j][i][1] = G;
        dots[Page_Write][j][i][2] = R;
      }
      else
      {
        dots[Page_Write][j][i][0] = 0;
        dots[Page_Write][j][i][1] = 0;
        dots[Page_Write][j][i][2] = 0;
      }
      temp = temp << 1;
    }
  }
  Page_Index = Page_Write;  
}
/********************************************************
Name:DispShowColor
Function:Fill a color in LED matrix
Parameter:R: the value of RED.   Range:RED 0~255
          G: the value of GREEN. Range:RED 0~255
          B: the value of BLUE.  Range:RED 0~255
********************************************************/
void DispShowColor(unsigned char R,unsigned char G,unsigned char B)
{
  unsigned char Page_Write,i,j;
  
  if(Page_Index == 0)
    Page_Write = 1;
  if(Page_Index == 1)
    Page_Write = 0;
    
  for (i = 0;i<8;i++)
    for(j = 0;j<8;j++)
    {
      dots[Page_Write][i][j][2] = R;
      dots[Page_Write][i][j][1] = G;
      dots[Page_Write][i][j][0] = B;
    }
  
  Page_Index = Page_Write;
}
/********************************************************
Name:DispShowPic
Function:Fill a picture in LED matrix from FLASH
Parameter:Index:the index of picture in Flash.
********************************************************/
void DispShowPic(unsigned char Index)
{
  unsigned char Page_Write,i,j;
  
  if(Page_Index == 0)
    Page_Write = 1;
  if(Page_Index == 1)
    Page_Write = 0;
    
  for (i = 0;i<8;i++)
  {
    
    for(j = 0;j<8;j++)
    {
      
      dots[Page_Write][i][j][0] = pic[Index][i][j][2];
      dots[Page_Write][i][j][1] = pic[Index][i][j][1];
      dots[Page_Write][i][j][2] = pic[Index][i][j][0];
    }
  }
  Page_Index = Page_Write;
  
}

//LED初始化 
INT32 _LED_Init()
{
  LED_RST(1);
  LED_Delay(1);
  LED_RST(0);
  LED_Delay(1);
  LED_RST(1);
  LED_Delay(1);
  SetGamma();
  line = 0;
  return 1;
}
/*
*函数说明:入口函数
*入口参数:无
*返回值:无
*/
INT32 main()
{
    //关闭中断 
    IrqInit();                  /* 关闭所有一级中断 */
    Irq2Init();                 /* 关闭所有二级中断 */
    //串口初始化
    AdvTimerInit(); 
    //Serial1Init(115200,3);      /* 串口1初始化，使能发送接收 */
    //Serial2Init(115200,3);      /* 串口2初始化，使能发送接收 */
    //Serial3Init(115200,0,3);    /* 串口3初始化，使能发送接收 */
    Serial4Init(115200,0,3);    /* 串口4初始化，使能发送接收 */
	
    Printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    
    Printf("*----------------------------------------------------------*\r\n\r\n");
    Printf("*                      MXT0106 全彩LED显示                     *\r\n");
    Printf("\r\n");
    Printf("*                  Copyright (c) 2011,Edward.xu                *\r\n");
    Printf("\r\n");                                                    
    Printf("*----------------------------------------------------------*\r\n\r\n");
    //GPIO初始化 
    if(GPIO_init()==1)
    {
        Printf("GPIO config is ok \r\n");
    } 
    if(_LED_Init()==1)
    {
        Printf("_LED config is ok \r\n");
    }
    //close_all_line  
    //regs->pioData2 = regs->pioData2&~(DATA_openline_BIT0|DATA_openline_BIT1|DATA_openline_BIT2|DATA_openline_BIT3|DATA_openline_BIT4|DATA_openline_BIT5|DATA_openline_BIT6|DATA_openline_BIT7);    
    Printf("Start LED program \r\n");
    //SysTimerTest(); 
    AdvTimerTest();
    Printf("Timeintrupt config is ok \r\n");
    while(1)
    {
         //DispShowChar('M',60,52,42,0);
         DispShowPic(0);
         Delay(5);
         DispShowPic(1);
         Delay(5);
         DispShowPic(2);
         Delay(5);
         DispShowPic(3);
         Delay(5);
         DispShowPic(4);
         Delay(5);
         DispShowPic(5);
         Delay(5);
    }
    return 0;
}



