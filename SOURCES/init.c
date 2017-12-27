#include "init.h"
#include "sys.h"
#include "usart1.h"	




void sleep_init(void)
{
	//Init 24C02
//	AT24CXX_Init();
//	printf("\r\n 24C02 init ok!\r\n");
	 printf("\r\n 进入sleep_init函数!\r\n");
	
    // Init amplifier
	PGA112_Init();
	printf("\r\n PGA112 Amplifier init ok!\r\n");
	
	
	
	
//	DMA2_Init();
//	ADC1_Init();
//	printf("\r\n ADC1_DMA2 init ok!\r\n");
//    

    // Init key 先空着。
    //Button_Init();
    
	//Lsens_Init();
	
	
    // Init TM1620
   // TM1620_Init();
	//printf("\r\n TM1620 init ok!\r\n");
	
	
	
	

    //DealMainReset();
    //sleepStagingReset();
    
    

}





