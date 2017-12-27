#ifndef __TM1620_H
#define __TM1620_H
#include "sys.h"
#include "spi2.h"
#include "rtc.h"
#include <stdbool.h>

#include "DealMain.h"

 							   
typedef enum display_e
{
    DISPLAY_HEART_RATE = 0,
    DISPLAY_RESPIRATORY_RATE,

}DISPLAY_E;

typedef enum display_parameter_switch_e
{
    DISPLAY_HEART_RATE_SINGLE = 0,
    DISPLAY_RESPIRATORY_RATE_SINGLE,
    DISPLAY_HEART_RESPIRTORY_SWITCH,
    DISPLAY_BODY_MOVE_SINGLE,
}DISLAY_PARAMETER_SWITCH_E;

typedef enum display_switch_e
{
    DISPLAY_TIME = 0,
    DISPLAY_HR,
    DISPLAY_RR,
    DISPLAY_HR_RR,
    DISPLAY_SlEEP,
    
    //DISPLAY_CLOSE,
}DISLAY_SWITCH_E;

#define  DISLAY_SWITCH_E_NUM  5

typedef enum display_time_switch_e
{
    DISPLAY_HOUR = 0,
    DISPLAY_DATE,
}DISLAY_TIME_SWITCH_E;


//parameter

//function
void TM1620_Init(void);

void TM1620_Set_Bright(uint8_t bright);

void TM1620_Write_Data(u8 address, u8 data); //Write Data to TM1620
void TM1620_Write_Continue(uint8_t address, uint8_t *dataptr, uint8_t length);

void TM1620_Display_Single(uint8_t grid, uint8_t num);

void TM1620_Display_Num(uint16_t num);

void TM1620_Display_Off(void);

//void TM1620_Led_On(uint8_t lednum);

//void TM1620_Led_Off(uint8_t lednum);

void TM1620_Colon_On(void);

void TM1620_Colon_Off(void);


void TM1620_Display_HR(uint8_t HR);
void TM1620_Display_RR(uint8_t RR);
void TM1620_Display_SP(uint8_t SP);
void TM1620_Display_gain(uint8_t gain);
void TM1620_Display_Result(void);
void TM1620_Display_save_on(void); 
void TM1620_Display_save_off(void);
void TM1620_Display_clear(void);
void TM1620_Colon_Toggle(void);

void display_time(void);
void display_result(void);

#endif

