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

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	IIC2_Init();//IIC��ʼ��
}


//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC2_Start();  
	if(EE_TYPE > AT24C16)
	{
		IIC2_Send_Byte(0XA0);	   //����д����
		IIC2_Wait_Ack();
		IIC2_Send_Byte(ReadAddr >> 8);//���͸ߵ�ַ	    
	}
	else 
		IIC2_Send_Byte(0XA0 + ((ReadAddr / 256) << 1));   //����������ַ0XA0,д���� 	 
	
	IIC2_Wait_Ack(); 
    IIC2_Send_Byte(ReadAddr % 256);   //���͵͵�ַ
	IIC2_Wait_Ack();	    
	IIC2_Start();  	 	   
	IIC2_Send_Byte(0XA1);           //�������ģʽ			   
	IIC2_Wait_Ack();	 
    temp = IIC2_Read_Byte(0);		   
    IIC2_Stop();//����һ��ֹͣ����
	
	return temp;
}


//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC2_Start();  
	if(EE_TYPE > AT24C16)
	{
		IIC2_Send_Byte(0XA0);	    //����д����
		IIC2_Wait_Ack();
		IIC2_Send_Byte(WriteAddr >> 8);//���͸ߵ�ַ	  
	}
	else
		IIC2_Send_Byte(0XA0 + ((WriteAddr / 256) << 1));   //����������ַ0XA0,д���� 	 

	IIC2_Wait_Ack();	   
    IIC2_Send_Byte(WriteAddr % 256);   //���͵͵�ַ
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC2_Wait_Ack();  		    	   
    IIC2_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}


//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{  	
	u8 t;
	for(t = 0; t < Len; t++)
	{
		AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
	}												    
}


//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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


//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX	
	
	if(temp == 0X55)
		return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255, 0X55);
	    temp = AT24CXX_ReadOneByte(255);	  
		if(temp == 0X55)
			return 0;
	}
	return 1;											  
}


//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);	//-->   *(pBuffer++)
		NumToRead--;
	}
}  


//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
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
		
		/*�������ݣ�����ʱ��*/
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
            //send_sign_data_to_host(&sign_parameter_);   /*����޸��˱�������Ľṹ�壬ע���޸ķ������ݵĸ���*/
             //��ʱ�Ȳ�����
		} 
    }
    else if (true == request_send_intervention_flag)
    {
        for (i=0; i<save_addr_index; i++)
        {
			u8 buf5[50]={0};
		    memcpy(buf5, &sign_parameter_, sizeof(sign_parameter_)); 
            AT24CXX_Read(i * length_sign_parameter + 4, buf5, length_sign_parameter);
            //send_sign_data_to_intervention(&sign_parameter_);   /*����޸��˱�������Ľṹ�壬ע���޸ķ������ݵĸ���*/
            //��ʱ�Ȳ�����
		} 
    }

}

void clear_parameter(void)
{
    unsigned char clear[4] = {0};
    unsigned char delay_num = 0;
    
    if (true == clear_parameter_flag)
    {
        if (true != request_send_flag)         /* �ȶ��������֮������� */
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
        if (ARRAY_NUM == array_index)    //ARRAY_NUM:��ʾ�洢���ݵ�ʱ��������λ��
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







