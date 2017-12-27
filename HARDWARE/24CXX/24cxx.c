#include "24cxx.h" 
#include "delay.h"
#include <string.h>
#include "DealMain.h"

bool clear_parameter_flag = false;
bool eeprom_busy_flag = false;

bool save_parameter_flag = false;
bool request_send_flag = false;
bool request_send_intervention_flag = false;

bool request_send_original_data_flag = false;

unsigned short read_eeprom_begin_index = 0;
unsigned short read_eeprom_end_index = 0;

extern struct OutputBatch pOutput;

//初始化IIC接口
void AT24CXX_Init(void)
{
	IIC2_Init();//IIC初始化
}


//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC2_Start();  
	if(EE_TYPE > AT24C16)
	{
		IIC2_Send_Byte(0XA0);	   //发送写命令
		IIC2_Wait_Ack();
		IIC2_Send_Byte(ReadAddr >> 8);//发送高地址	    
	}
	else 
		IIC2_Send_Byte(0XA0 + ((ReadAddr / 256) << 1));   //发送器件地址0XA0,写数据 	 
	
	IIC2_Wait_Ack(); 
    IIC2_Send_Byte(ReadAddr % 256);   //发送低地址
	IIC2_Wait_Ack();	    
	IIC2_Start();  	 	   
	IIC2_Send_Byte(0XA1);           //进入接收模式			   
	IIC2_Wait_Ack();	 
    temp = IIC2_Read_Byte(0);		   
    IIC2_Stop();//产生一个停止条件
	
	return temp;
}


//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC2_Start();  
	if(EE_TYPE > AT24C16)
	{
		IIC2_Send_Byte(0XA0);	    //发送写命令
		IIC2_Wait_Ack();
		IIC2_Send_Byte(WriteAddr >> 8);//发送高地址	  
	}
	else
		IIC2_Send_Byte(0XA0 + ((WriteAddr / 256) << 1));   //发送器件地址0XA0,写数据 	 

	IIC2_Wait_Ack();	   
    IIC2_Send_Byte(WriteAddr % 256);   //发送低地址
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //发送字节							   
	IIC2_Wait_Ack();  		    	   
    IIC2_Stop();//产生一个停止条件 
	delay_ms(10);	 
}


//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{  	
	u8 t;
	for(t = 0; t < Len; t++)
	{
		AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
	}												    
}


//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp = 0;
	for(t = 0; t < Len; t++)
	{
		temp <<= 8;
		temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1); 	 				   
	}
	return temp;												    
}


//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX	
	
	if(temp == 0X55)
		return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255, 0X55);
	    temp = AT24CXX_ReadOneByte(255);	  
		if(temp == 0X55)
			return 0;
	}
	return 1;											  
}


//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);	//-->   *(pBuffer++)
		NumToRead--;
	}
}  


//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}





void save_parameter_(void)
{
    RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
    unsigned char check[4] = {0};
    unsigned short save_addr_index = 0;
    
    SIGN_PARAMETER_T_ sign_parameter_ = {0};
    unsigned char length_sign_parameter = sizeof(sign_parameter_);
    unsigned char buffer_write;
    //volatile unsigned char buffer_read[16] = {0};
    //unsigned char temp_index[2] = {0};

    AT24CXX_Read(0, check, 4);
	
    if (('s' == check[0]) && ('p' == check[1]))
    {
        save_addr_index = check[2] + (check[3] << 8);
    }
    else
    {
        buffer_write = 's';
        AT24CXX_WriteOneByte(0, buffer_write);
		
        buffer_write = 'p';
        AT24CXX_WriteOneByte(1, buffer_write);
        
		save_addr_index = 0;
		u8 buf1[2]={0};
		memcpy(buf1, &save_addr_index, sizeof(save_addr_index)); 
		
        AT24CXX_Write(2, (u8*)buf1, 2);
    }
    if (save_addr_index < (EEPROM_MEMORY_SPACE - 4) / length_sign_parameter)
    {
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);

        sign_parameter_.mvCount_         = pOutput.mvCount;
        sign_parameter_.mean_HR_         = pOutput.meanHR;
        sign_parameter_.mean_RR_         = pOutput.meanRR;
        sign_parameter_.adjustCount_     = pOutput.adjustCount;
        sign_parameter_.range_precent_   = pOutput.rangePercent;
        sign_parameter_.stdHR_           = pOutput.stdHR;
        sign_parameter_.sleep_stage      = pOutput.sleepStage;
        //sign_parameter_.move_flag        = pOutput.moveFlag;
        //sign_parameter_.err_flag         = pOutput.errFlag;

        sign_parameter_.rtc_year   = RTC_DateStruct.RTC_Year;
        sign_parameter_.rtc_month  = RTC_DateStruct.RTC_Month;
        sign_parameter_.rtc_day    = RTC_DateStruct.RTC_Date;
        sign_parameter_.rtc_hour   = RTC_TimeStruct.RTC_Hours;
        sign_parameter_.rtc_minute = RTC_TimeStruct.RTC_Minutes;
        sign_parameter_.rtc_second = RTC_TimeStruct.RTC_Seconds;

		
		u8 buf2[50]={0};
		memcpy(buf2, &sign_parameter_, sizeof(sign_parameter_));  
		
		
        AT24CXX_Write(save_addr_index * length_sign_parameter + 4, buf2, length_sign_parameter);
		
		/*读出数据，调试时用*/
		//#if 0
		//        eeprom_sequential_read(save_addr_index*length_sign_parameter+4, &sign_parameter_, length_sign_parameter);
		//        uart_send(&sign_parameter_, length_sign_parameter, false);
		//#endif
		
        save_addr_index++;

		u8 buf3[2]={0};
		memcpy(buf3, &save_addr_index, sizeof(save_addr_index)); 
        AT24CXX_Write(2, buf3, 2);
        /*
        eeprom_sequential_read(2, temp_index, 2);
        uart_send(temp_index+1, 1, false);
        uart_send(temp_index, 1, false);
        */
    }    
    else
    {
        save_parameter_flag = false;
    }
}

void read_parameter_(void)
{
    int i;
    unsigned char check[4] = {0};
    unsigned short save_addr_index = 0;
    SIGN_PARAMETER_T_ sign_parameter_ = {0};
    unsigned char length_sign_parameter = sizeof(sign_parameter_);

    AT24CXX_Read(0, check, 4);
    if (('s' == check[0]) && ('p' == check[1]))
    {
        save_addr_index = check[2] + (check[3] << 8);
    }
    else
    {
        return;
    }

    if ((0 == read_eeprom_begin_index) || (read_eeprom_end_index < read_eeprom_begin_index) || (read_eeprom_end_index > save_addr_index) || (read_eeprom_begin_index > save_addr_index))
    {
        return;
    }
        
    if (true == request_send_flag)
    {
        for (i=read_eeprom_begin_index-1; i<read_eeprom_end_index; i++)
        {
			u8 buf4[50]={0};
		    memcpy(buf4, &sign_parameter_, sizeof(sign_parameter_)); 
            AT24CXX_Read(i * length_sign_parameter + 4, buf4, length_sign_parameter);
            //send_sign_data_to_host(&sign_parameter_);   /*如果修改了保存参数的结构体，注意修改发送数据的个数*/
             //暂时先不发送
		} 
    }
    else if (true == request_send_intervention_flag)
    {
        for (i=0; i<save_addr_index; i++)
        {
			u8 buf5[50]={0};
		    memcpy(buf5, &sign_parameter_, sizeof(sign_parameter_)); 
            AT24CXX_Read(i * length_sign_parameter + 4, buf5, length_sign_parameter);
            //send_sign_data_to_intervention(&sign_parameter_);   /*如果修改了保存参数的结构体，注意修改发送数据的个数*/
            //暂时先不发送
		} 
    }

}

void clear_parameter(void)
{
    unsigned char clear[4] = {0};
    unsigned char delay_num = 0;
    
    if (true == clear_parameter_flag)
    {
        if (true != request_send_flag)         /* 等读操作完成之后，再清除 */
        {
            
            while (true == eeprom_busy_flag)
            {
                delay_ms(10);
                if ((delay_num++) > 50)
                {
                    return;
                }
            }
            eeprom_busy_flag = true;
            AT24CXX_Write(0, clear, 4);
            eeprom_busy_flag = false;
       
            clear_parameter_flag = false;
        }
    }
}

void save_parameter(void)
{
//    unsigned char check[4] = {0};
//    unsigned char num[2] = {0};
//    unsigned short save_addr_index;
    static unsigned short array_index = 0;
    unsigned char delay_num = 0;
    
    if (true == save_parameter_flag)
    {
        
        array_index++;
        if (ARRAY_NUM == array_index)    //ARRAY_NUM:表示存储数据的时间间隔，单位秒
        {
            while (true == eeprom_busy_flag)
            {
               delay_ms(10);
                if ((delay_num++) > 50)
                {
                    return;
                }
            }
            eeprom_busy_flag = true;
			
            save_parameter_();
			
            eeprom_busy_flag = false;
            array_index = 0;
        }
    }


}

void read_parameter(void)
{
    unsigned char delay_num = 0;

    if ((true == request_send_flag) || (true == request_send_intervention_flag))
    {
        while (true == eeprom_busy_flag)
        {
           delay_ms(10);
            if ((delay_num++) > 50)
            {
                return;
            }
        }
        eeprom_busy_flag = true;
		
        read_parameter_();
		
        eeprom_busy_flag = false;
        request_send_flag = false;
        request_send_intervention_flag = false;
    }
}

void read_data_num(unsigned char *num)
{
    unsigned char check[4] = {0};
    unsigned char delay_num = 0;
    
    while (true == eeprom_busy_flag)
    {
        delay_ms(10);
        
        if ((delay_num++) > 50)
        {
            num[0] = 0;
            num[1] = 0;
            return;
        }
    }
    eeprom_busy_flag = true;
    AT24CXX_Read(0, check, 4);
    eeprom_busy_flag = false;
	
    if (('s' == check[0]) && ('p' == check[1]))
    {
        num[0] = check[3];
        num[1] = check[2];
    }
    else
    {
        num[0] = 0;
        num[1] = 0;
    }

}







