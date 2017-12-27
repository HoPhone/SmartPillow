#ifndef __24CXX_H
#define __24CXX_H
#include "iic2.h" 
#include <stdbool.h>


#define AT24C01		127
#define AT24C02		255 //用的这个
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  

//STM32主控板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C02
#define EEPROM_MEMORY_SPACE (255)
#define ARRAY_NUM (300)   //表示存储数据的时间间隔，单位秒





typedef struct sign_parameter_t_
{
    unsigned short rtc_year;    
    unsigned char rtc_month;
    unsigned char rtc_day;
    unsigned char rtc_hour;
    unsigned char rtc_minute;
    unsigned char rtc_second;
    
    //unsigned char move_flag;    //体动标记
    //unsigned char err_flag;     //错误标记
    unsigned char sleep_stage;  //睡眠阶段
    
    unsigned short mvCount_;    //体动超限次数
    unsigned short adjustCount_;//体动校正次数   
    
    float mean_HR_;             //心率均值
    float mean_RR_;             //呼吸率均值
    float stdHR_;               //心率标准差
    float range_precent_;       //波动百分比
    
}SIGN_PARAMETER_T_;






u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度字节的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度字节的数据
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC


void save_parameter_(void);
void read_parameter_(void);
void clear_parameter(void);
void save_parameter(void);
void read_parameter(void);

#endif




