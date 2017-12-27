#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"

uint32_t GetSector(uint32_t Address);
void flash_config(void);


#endif