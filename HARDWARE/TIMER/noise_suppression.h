#ifndef __NOISE_H
#define __NOISE_H
#include "spi1.h"
#include "sys.h"
#include "stm32f4xx.h"


#define REFERENCE_BIAS  1640
#define PEAK_INTERVAL  	50
#define PEAK_NEARBY  		50
#define NOISE_FACTOR  	3
#define SNORE_PEAK_MIN  160
#define SNORE_PEAK_MAX  300				//135
#define SNORE_LASTING   4500				//185
#define NOISE_LASTING		12000
#define NOISE_THRETHOLD_MIN 40
#define NOISE_THRETHOLD_MAX 550
#define SNR_THRETHOLD 	0
#define NOISE_RATIO_THRESHOLD 0.85f
#define SNORENUMMAX 	10 


struct OutputBatch2 {
    unsigned char snoreNum;                     //?????????????
    unsigned short ampArray[SNORENUMMAX];       //?????????
    unsigned long pointArray[SNORENUMMAX];      //?????????
};

void snore_status_clear(void);

void SnoreDetect(unsigned short *dataArray,struct OutputBatch2 *pOutput);
int SnoreData_Return(unsigned short *dataArray,struct OutputBatch2 *pOutput, int snore_process_flag);
void noise_suppression(signed short *data);

#endif
