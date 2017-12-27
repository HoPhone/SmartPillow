#include "tm1620.h"
#include "delay.h"

/*
             c
           b   a
             g
           d   f
             e db

          g f db e d c b a
        0-0 1 0  1 1 1 1 1  //0x5F
        1-0 1 0  0 0 0 0 1    0x41
        2-1 0 0  1 1 1 0 1     0x9D
        3-1 1 0  1 0 1 0 1     0xD5
        4-1 1 0  0 0 0 1 1     0xC3
        5-1 1 0  1 0 1 1 0     0xD6
        6-1 1 0  1 1 1 1 0     0xDE
        7-0 1 0  0 0 1 0 1     0x45
        8-1 1 0  1 1 1 1 1     0xDF
        9-1 1 0  1 0 1 1 1     0xD7          0xFFȫ��  0x00ȫ��
       
  	    A-1 1 0  0 1 1 1 1
        b-1 1 0  1 1 0 1 0
        c-1 0 0  1 1 0 0 0
        C-0 0 0  1 1 1 1 0
        d-1 1 0  1 1 0 0 1
        E-1 0 0  1 1 1 1 0
        F-1 0 0  0 1 1 1 0
        g-1 1 0  1 0 1 1 1
        H-1 1 0  0 1 0 1 1
        L-0 0 0  1 1 0 1 0
        n-1 1 0  0 1 0 0 0
        o-1 1 0  1 1 0 0 0
*/

//define
//#define SPI2_BASE	
#define TM1620_CMD_MODE				0x02
#define TM1620_CMD_WRITE_SINGLE		0x44
#define TM1620_CMD_READ_SINGLE		0x46
#define TM1620_CMD_WRITE_CONTINUE	0x40
#define TM1620_CMD_READ_CONTINUE	0x42
#define TM1620_CMD_ADDR_BASE		0xc0
#define TM1620_CMD_BRIGHT_BASE		0x80
#define GRID_BASE_LED				0x01
#define GRID_BASE_NUM				0x03
#define GRID_BASE_DOT				0x03
#define GRID_BASE_COLON				0x05
#define GRID_ADDR(i) ((i-0x01) << 1)
#define LED1_GRID 1
#define LED2_GRID 2
#define SEGMENT_MASK_COLON 0x20
#define GRID_COLON1		5
#define GRID_COLON2		6
#define SEGMENT_MASK_NUM 0xDF
//parameter

unsigned char TM1620_Grid_Value[4];
DISLAY_PARAMETER_SWITCH_E display_parameter_switch_flag = DISPLAY_HEART_RATE_SINGLE;
DISLAY_SWITCH_E display_switch_flag =DISPLAY_TIME;
DISLAY_TIME_SWITCH_E display_time_flag = DISPLAY_HOUR;

unsigned char display_times_s = 0;
bool display_colon_flag = false;
unsigned char display_flag;

extern volatile bool rtc_irq_flag;
extern struct OutputBatch pOutput;
extern unsigned short adc_buffer_index;


/* void TM1620_Set_Bright(uint8_t level)
 *
 * @parameter
 * level:0-8
 *
 * */
void TM1620_Set_Bright(u8 level)
{
	u8 temp;
    unsigned char Brightness_Level[9] ={0x80, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f};
	
	TM1620_CS = 0;    //ѡ��
	temp = Brightness_Level[level];
	SPI2_WriteByte(temp);
	delay_us(5);//Delay_SPI();
	TM1620_CS = 1;    //��ѡ��
}


/* void TM1620_Write_Data(uint8_t address, uint8_t data)
 *
 *
 * @parameter
 * address:0x00-0x0b
 * data:0x00-0xff
 *
 * @return NULL
 *
 * */
void TM1620_Write_Data(u8 address, u8 data)
{
	u8 temp;
	temp = TM1620_CMD_WRITE_SINGLE;
	
    TM1620_CS = 0;    //ѡ��
	//SPI2_WriteByte(SPI2_BASE);
	SPI2_WriteByte(temp);
	delay_us(5);//Delay_SPI();
	
	temp = address | TM1620_CMD_ADDR_BASE;
	SPI2_WriteByte(temp);
	delay_us(5);//Delay_SPI();
	
	temp = data;
	SPI2_WriteByte(temp);
	delay_us(5);//Delay_SPI();
	TM1620_CS = 1;    //��ѡ��
	
	delay_us(5);//Delay_SPI();
	delay_us(5);//Delay_SPI();
}


void TM1620_Init(void)
{
	SPI2_Init();
	
	uint8_t temp;
    
	temp = TM1620_CMD_MODE;
	SPI2_WriteByte(temp);
	delay_us(5);//Delay_SPI();
	
	TM1620_Grid_Value[0] = 0;
	//TM1620_Write_Data(0x04, 0);
	TM1620_Write_Data(GRID_ADDR(0 + GRID_BASE_NUM), 0); //clear the register,��������ʵ����(0x04, 0)
	                                                     //�����: 0x04 | 0xC0 = 0xC4
	TM1620_Grid_Value[1] = 0;
	//TM1620_Write_Data(0x06, 0);
	TM1620_Write_Data(GRID_ADDR(1 + GRID_BASE_NUM), 0);
	
	TM1620_Grid_Value[2] = 0;
	//TM1620_Write_Data(0x08, 0);
	TM1620_Write_Data(GRID_ADDR(2 + GRID_BASE_NUM), 0);
	
	TM1620_Grid_Value[3] = 0;
	//TM1620_Write_Data(0x0A, 0);
	TM1620_Write_Data(GRID_ADDR(3 + GRID_BASE_NUM), 0);
	
	TM1620_Set_Bright(8); //set light brightness

    
    //RTC_DRV_GetDatetime(rtcTimer1_IDX, &rtc_date);
    //TM1620_Display_Num(rtc_date.hour*100+rtc_date.minute); 
    
}



/*
 *
 * @parameter
 * address:0x00-0x0b
 * dataptr:ptr of data to be sent
 * length:length of data
 *
 *
 * */
void TM1620_Write_Continue(uint8_t address, uint8_t *dataptr, uint8_t length)
{
	u8 i = 0;
	u8 temp;
	
	TM1620_CS = 0;    //ѡ��
	temp = TM1620_CMD_WRITE_CONTINUE;
	SPI2_WriteByte(temp);
	delay_us(5);//Delay_SPI();
	
	temp = address | TM1620_CMD_ADDR_BASE;
	SPI2_WriteByte(temp);
	delay_us(5);//Delay_SPI();
	for(i = 0; i < length - 1; i++)
	{
		temp = *(dataptr + i);
		SPI2_WriteByte(temp);
		delay_us(5);//Delay_SPI();
		
		temp = 0x00;
		SPI2_WriteByte(temp);
		delay_us(5);//Delay_SPI();
	}
	i++;
	temp = *(dataptr + i);
	SPI2_WriteByte(temp);
	delay_us(5);
	TM1620_CS = 1;    //��ѡ��
	delay_us(5);//Delay_SPI();
}

/*
 *
 * @parameter
 * grid = 0-3
 * num = 0-9
 *
 * @return NULL
 *
*/
void TM1620_Display_Single(u8 grid, u8 num) //�ڼ�λ��0~3����ʾʲô���֣�0~9����grid3��Ӧ��0λ�������λ��
{
    unsigned char SegNumber[12] = {0x5f, 0x41, 0x9D, 0xD5, 0xC3, 0xD6, 0xDE, 0x45, 0xDF, 0xD7, 0xFF, 0x00};
	TM1620_Grid_Value[grid] &= ~SEGMENT_MASK_NUM;
	TM1620_Grid_Value[grid] |= SegNumber[num];
    //TM1620_Grid_Value[grid] = SegNumber[num];
	TM1620_Write_Data(GRID_ADDR(grid + GRID_BASE_NUM), TM1620_Grid_Value[grid]);
	//GRID_ADDR(grid + GRID_BASE_NUM)�����ַ����
}


void TM1620_Display_Off(void)
{
	TM1620_Display_Single(0, 11);  //����0x00 ȫ��
	TM1620_Display_Single(1, 11);
	TM1620_Display_Single(2, 11);
	TM1620_Display_Single(3, 11);
}



void TM1620_Colon_On(void)  //�������
{
	TM1620_Grid_Value[2] |= SEGMENT_MASK_COLON;
	TM1620_Write_Data(GRID_ADDR(GRID_BASE_COLON), TM1620_Grid_Value[2]);
	TM1620_Grid_Value[3] |= SEGMENT_MASK_COLON;
	TM1620_Write_Data(GRID_ADDR(GRID_BASE_COLON + 1), TM1620_Grid_Value[3]);
}

void TM1620_Colon_Off(void)  //�������
{
	TM1620_Grid_Value[2] &= ~SEGMENT_MASK_COLON;
	TM1620_Write_Data(GRID_ADDR(GRID_BASE_COLON), TM1620_Grid_Value[2]);
	TM1620_Grid_Value[3] &= ~SEGMENT_MASK_COLON;
	TM1620_Write_Data(GRID_ADDR(GRID_BASE_COLON + 1), TM1620_Grid_Value[3]);
}


void TM1620_Display_Num(u16 num)      //��ʾһ����λ��
{
	uint8_t Temp[4];
	Temp[0] = num / 1000;             //ǧλ����Ӧgrid3
	Temp[1] = ((num / 100) % 10);     //��λ����Ӧgrid4
	Temp[2] = (num / 10) % 10;
	Temp[3] = num % 10;
    
	if(Temp[0] > 0)
	{
    	TM1620_Display_Single(0,Temp[0]);
        TM1620_Display_Single(1,Temp[1]);
        TM1620_Display_Single(2,Temp[2]);
        TM1620_Display_Single(3,Temp[3]);
	}
	else if(Temp[1] > 0)
	{
    	TM1620_Display_Single(0,0);  //��һλ�����λ����ǧλ����ʾSegNumber[0]����Ϊ0��
        TM1620_Display_Single(1,Temp[1]);
        TM1620_Display_Single(2,Temp[2]);
        TM1620_Display_Single(3,Temp[3]);
	}
	else if(Temp[2] > 0)
	{
    	TM1620_Display_Single(0,0);
        TM1620_Display_Single(1,0);
        TM1620_Display_Single(2,Temp[2]);
        TM1620_Display_Single(3,Temp[3]);
	}
	else
	{
    	TM1620_Display_Single(0,0);
        TM1620_Display_Single(1,0);
        TM1620_Display_Single(2,0);
        TM1620_Display_Single(3,Temp[3]);
	}

}


void TM1620_Display_HR(uint8_t HR) //��ʾ����
{
    uint8_t Temp[3];  //һ������λ��
    TM1620_Write_Data(GRID_ADDR(0x00 + GRID_BASE_NUM), 0xCB); //���λ��grid3��Ӧ�ĵ�ַ0x04�����ռ������
    Temp[0] = ((HR / 100) % 10);                              //�ĵ�ַ��0xC4������д��0xCB��11001011������ʾΪH
    Temp[1] = (HR / 10) % 10;                                 //H����HR�����ʣ���
    Temp[2] = HR % 10;
    
    if(Temp[0] > 0)
    {
        TM1620_Display_Single(1,Temp[0]);
        TM1620_Display_Single(2,Temp[1]);
        TM1620_Display_Single(3,Temp[2]);
    }else
    {
        TM1620_Display_Single(1,11);
        TM1620_Display_Single(2,Temp[1]);
        TM1620_Display_Single(3,Temp[2]);
    }

}


void TM1620_Display_RR(uint8_t RR) //����Ƶ��
{
    uint8_t Temp[3];
    TM1620_Write_Data(GRID_ADDR(0x00 + GRID_BASE_NUM), 0xDA);  //��ʾΪb�����������breath��
    Temp[0] = ((RR / 100) % 10);
    Temp[1] = (RR / 10) % 10;
    Temp[2] = RR % 10;
    
    if(Temp[0] > 0)
    {
        TM1620_Display_Single(1, Temp[0]);
        TM1620_Display_Single(2, Temp[1]);
        TM1620_Display_Single(3, Temp[2]);
    }
    else
    {
        TM1620_Display_Single(1, 11);
        TM1620_Display_Single(2, Temp[1]);
        TM1620_Display_Single(3, Temp[2]);
    }

}


void TM1620_Display_SP(uint8_t SP)   //���Ǵ���˯�ߵĵڼ��ڡ�
{
    TM1620_Display_Single(0,5);   //��ʾ5��������S��
    TM1620_Display_Single(1,11);
    TM1620_Display_Single(2,11);
    TM1620_Display_Single(3, (SP % 10));
}


/*
gain    binary gain
  0         1
  1         2
  2         4
  3         8
  4         16
*/
void TM1620_Display_gain(uint8_t gain)
{
    TM1620_Display_Single(0,9);    //��ʾ9��������g
    TM1620_Display_Single(1,11);
    TM1620_Display_Single(2,11);
    TM1620_Display_Single(3,(1 << gain));
}


void TM1620_Display_Result(void)
{    
    if (display_times_s > 0)
    {
        return;  //ֱ�ӷ��أ�����Ĳ�ִ���ˡ�
    }
    TM1620_Colon_Off();
    switch (display_switch_flag)
    {
        case DISPLAY_HR:
        {
            TM1620_Display_HR(pOutput.HR);
            break;
        }
        case DISPLAY_RR:
        {
            TM1620_Display_RR(pOutput.RR);
            break;
        }
        case DISPLAY_HR_RR:
        {
            if (0 == display_flag)
            {
                display_flag = 4;
            }

            if (display_flag < 3)
            {
                TM1620_Display_HR(pOutput.HR);
            }
            else
            {
                TM1620_Display_RR(pOutput.RR);
            }

            break;
        }
        case DISPLAY_SlEEP:
        {
            TM1620_Display_SP(pOutput.sleepStage);
            break;
        }
        default:
        {
            break;
        }
    }

}

void TM1620_Display_save_on(void)     //��ʾ���ǣ�5 on��������S on
{
    TM1620_Write_Data(GRID_ADDR(0x00 + GRID_BASE_NUM), 0xD6);
    TM1620_Write_Data(GRID_ADDR(0x01 + GRID_BASE_NUM), 0x00);
    TM1620_Write_Data(GRID_ADDR(0x02 + GRID_BASE_NUM), 0xD8);
    TM1620_Write_Data(GRID_ADDR(0x03 + GRID_BASE_NUM), 0xC8);

}
void TM1620_Display_save_off(void)   // ��ʾ����5oFF,������SoFF
{
    TM1620_Write_Data(GRID_ADDR(0x00 + GRID_BASE_NUM), 0xD6);
    TM1620_Write_Data(GRID_ADDR(0x01 + GRID_BASE_NUM), 0xD8);
    TM1620_Write_Data(GRID_ADDR(0x02 + GRID_BASE_NUM), 0x8E);
    TM1620_Write_Data(GRID_ADDR(0x03 + GRID_BASE_NUM), 0x8E);

}

void TM1620_Display_clear(void)    //��ʾ���ǣ�cL  ��
{
    TM1620_Write_Data(GRID_ADDR(0x00 + GRID_BASE_NUM), 0x98);
    TM1620_Write_Data(GRID_ADDR(0x01 + GRID_BASE_NUM), 0x1A);
    TM1620_Write_Data(GRID_ADDR(0x02 + GRID_BASE_NUM), 0x00);
    TM1620_Write_Data(GRID_ADDR(0x03 + GRID_BASE_NUM), 0x00);
}


/*��ת*/
void TM1620_Colon_Toggle(void)
{
	TM1620_Grid_Value[2] ^= SEGMENT_MASK_COLON;  //^��λ���
	TM1620_Write_Data(GRID_ADDR(GRID_BASE_COLON), TM1620_Grid_Value[2]);
	TM1620_Grid_Value[3] ^= SEGMENT_MASK_COLON;
	TM1620_Write_Data(GRID_ADDR(GRID_BASE_COLON + 1), TM1620_Grid_Value[3]);
}

void display_time(void)
{

    RTC_TimeTypeDef RTC_TimeStruct;
//	RTC_DateTypeDef RTC_DateStruct;
    My_RTC_Init();		 		//��ʼ��RTC
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//����WAKE UP�ж�,1�����ж�һ��.���õ�����ʱ��
    
    
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	TM1620_Display_Num(RTC_TimeStruct.RTC_Hours * 100 + RTC_TimeStruct.RTC_Minutes);
			//RTC_DRV_GetDatetime(rtcTimer1_IDX, &rtc_date);
            //TM1620_Display_Num_(rtc_date.hour*100+rtc_date.minute); 

}



void display_result(void)
{
    if (DISPLAY_TIME != display_switch_flag)
    {
       TM1620_Display_Result();
    }  

}





