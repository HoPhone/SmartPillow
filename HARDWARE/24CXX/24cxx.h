#ifndef __24CXX_H
#define __24CXX_H
#include "iic2.h" 
#include <stdbool.h>


#define AT24C01		127
#define AT24C02		255 //�õ����
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  

//STM32���ذ�ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
#define EE_TYPE AT24C02
#define EEPROM_MEMORY_SPACE (255)
#define ARRAY_NUM (300)   //��ʾ�洢���ݵ�ʱ��������λ��





typedef struct sign_parameter_t_
{
    unsigned short rtc_year;    
    unsigned char rtc_month;
    unsigned char rtc_day;
    unsigned char rtc_hour;
    unsigned char rtc_minute;
    unsigned char rtc_second;
    
    //unsigned char move_flag;    //�嶯���
    //unsigned char err_flag;     //������
    unsigned char sleep_stage;  //˯�߽׶�
    
    unsigned short mvCount_;    //�嶯���޴���
    unsigned short adjustCount_;//�嶯У������   
    
    float mean_HR_;             //���ʾ�ֵ
    float mean_RR_;             //�����ʾ�ֵ
    float stdHR_;               //���ʱ�׼��
    float range_precent_;       //�����ٷֱ�
    
}SIGN_PARAMETER_T_;






u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�������ֽڵ�����
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ�������ֽڵ�����
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC


void save_parameter_(void);
void read_parameter_(void);
void clear_parameter(void);
void save_parameter(void);
void read_parameter(void);

#endif




