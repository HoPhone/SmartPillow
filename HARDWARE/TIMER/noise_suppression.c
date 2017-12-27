#include "timer.h"
#include <stdbool.h>
#include "usart1.h"	
#include "adc1.h"
#include "pga112.h"
#include "sleep_manage.h"
#include "noise_suppression.h"
#include "stm32f4xx.h"
#include "sys.h"


unsigned int pre_voice_dur_flag;									//?????????????
unsigned int pre_voice_start_mark;								//?????????????????????
unsigned int pre_voice_minus;													
unsigned int pre_voice_last_mark;

int cnt_processing;
int mean_cnt;
int snore_num;	//????
int mistake_cnt;
int array_end_flag;
int array_start_flag;
int pre_mistake_flag;
int pre_peakValue;

float subband_mean_amp_pre[8];
float noise_ratio;
float mistake_ratio;

int sum;
int mean;

extern struct OutputBatch2 Snore_Output;

void snore_status_clear () {
	int cnt = 0;
	snore_num = 0;
	mean_cnt = 0;
	pre_voice_dur_flag = 0;
	pre_voice_start_mark = 0;
	pre_voice_minus = 0;
	pre_voice_last_mark = 0;
	pre_mistake_flag = 0;
	pre_peakValue = 0;
	
	mistake_cnt = 0;
	mistake_ratio = 0;
	//pre_peak_max = 0;
	array_end_flag = 0;
	array_start_flag = 0;
			
	for(cnt=0;cnt<4;cnt++)
		{
			Snore_Output.snoreNum = 0;
			Snore_Output.ampArray[cnt] = 0;
			Snore_Output.pointArray[cnt] = 0;
		}
		
	for (cnt = 0; cnt < 8; cnt++)
		{
			subband_mean_amp_pre[cnt] = 0;
		}
//	pre_voice_peak_cnt = 0;
//	sum_pre_voice_peak = 0;
//	sum_voice_peak = 0;
//	peak_mean = 0.0;
//	voice_peak_cnt = 0;
}



void SnoreDetect(unsigned short *dataArray,struct OutputBatch2 *pOutput) 
{
	int i , j;
	signed short abs_dataArray[8000];
	signed int tempL;	
	signed int sum_array = 0;
	int arrayLength = 8000;
	int voice_start_flag = 0;
	int voice_dur_flag = 0;
	int voice_start_mark;
	int voice_stop_mark;
	int voice_dur;
	int peak_start_flag = 1;
	int peak_cnt = 0;
	int peak_mark = 0;
	int peakValue = 0;
	int temp_position = 0;
//	int p;
	//int sum_peak = 0;
	int pre_voice_dur;
	int voice_start_peak_mark = 1;
	int mistake_flag = 0;
	



//	int temp_position;
//	int peak_max;
	//int peak_single_position;
	//float peak_mean;
	short peak_position[1000];
	float subband_mean_amp;
	float subband_mean_amp_temp;
	float subband_mean_amp_sum;
	float ratio;
//	float compare_threshold;
//	float voice_noise_ratio;
//	float peak_noise_ratio;

	sum = 0;
	for (i = 0 ; i < arrayLength ; i++)
	{
		sum = sum + dataArray[i];
	}
	mean = sum / arrayLength;
	sum = 0;
	
	for (i = 0 ; i < arrayLength ; i++)								//?????????
	{
		tempL = dataArray[i] - REFERENCE_BIAS;
		if (tempL >= 0) {
		abs_dataArray[i] = tempL;
		}
		else {
		abs_dataArray[i] = 0 - tempL;
		}
		sum_array = abs_dataArray[i] + sum_array;
	}
	
	if (pre_mistake_flag == 1)
	{
		mistake_flag = 1;
		pre_mistake_flag = 0;
	}
	else
		;
	
	subband_mean_amp_temp = (float)sum_array / arrayLength * NOISE_FACTOR;
	//compare_threshold = subband_mean_amp;
	switch (mean_cnt)
	{
    case 0:
		{
				subband_mean_amp_pre[0] = subband_mean_amp_temp;
        subband_mean_amp = subband_mean_amp_temp;
        mean_cnt = mean_cnt + 1;
				break;
		}
    case 1:
		{
        subband_mean_amp_pre[1] = subband_mean_amp_pre[0];
        subband_mean_amp_pre[0] = subband_mean_amp_temp;
        subband_mean_amp = (float)(subband_mean_amp_pre[0] + subband_mean_amp_pre[1]) / 2.0f;
        mean_cnt = mean_cnt + 1;
				break;
		}
    case 2:
		{			
        subband_mean_amp_pre[2] = subband_mean_amp_pre[1];
        subband_mean_amp_pre[1] = subband_mean_amp_pre[0];
        subband_mean_amp_pre[0] = subband_mean_amp_temp;
        subband_mean_amp = (float)(subband_mean_amp_pre[0] + subband_mean_amp_pre[1] + subband_mean_amp_pre[2]) / 3.0f;
        mean_cnt = mean_cnt + 1;
				break;
		}
    case 3:
		{
        subband_mean_amp_pre[3]	= subband_mean_amp_pre[2];
        subband_mean_amp_pre[2]	= subband_mean_amp_pre[1];
        subband_mean_amp_pre[1] = subband_mean_amp_pre[0];
        subband_mean_amp_pre[0] = subband_mean_amp_temp;
        subband_mean_amp = (float)(subband_mean_amp_pre[0] + subband_mean_amp_pre[1] + subband_mean_amp_pre[2] + subband_mean_amp_pre[3]) / 4.0f;
				mean_cnt = mean_cnt + 1;
				break;
		}
		case 4:
		{
				for (i = 4 ; i > 0 ; i-- )
				{
					subband_mean_amp_pre[i] = subband_mean_amp_pre[i - 1];
					subband_mean_amp_sum = subband_mean_amp_sum + subband_mean_amp_pre[i];
				}	
				subband_mean_amp_pre[0] = subband_mean_amp_temp;
				subband_mean_amp = (float)(subband_mean_amp_sum + subband_mean_amp_pre[0]) / 5.0f;
				subband_mean_amp_sum = 0;
				mean_cnt = mean_cnt + 1;
				break;
		}
		case 5:
		{
				for (i = 5 ; i > 0 ; i-- )
				{
					subband_mean_amp_pre[i] = subband_mean_amp_pre[i - 1];
					subband_mean_amp_sum = subband_mean_amp_sum + subband_mean_amp_pre[i];
				}	
				subband_mean_amp_pre[0] = subband_mean_amp_temp;
				subband_mean_amp = (float)(subband_mean_amp_sum + subband_mean_amp_pre[0]) / 6.0f;
				subband_mean_amp_sum = 0;
				mean_cnt = mean_cnt + 1;
				break;
		}
		case 6:
		{
				for (i = 6 ; i > 0 ; i-- )
				{
					subband_mean_amp_pre[i] = subband_mean_amp_pre[i - 1];
					subband_mean_amp_sum = subband_mean_amp_sum + subband_mean_amp_pre[i];
				}	
				subband_mean_amp_pre[0] = subband_mean_amp_temp;
				subband_mean_amp = (float)(subband_mean_amp_sum + subband_mean_amp_pre[0]) / 7.0f;
				subband_mean_amp_sum = 0;
				mean_cnt = mean_cnt + 1;
				break;
		}
				case 7:
		{
				subband_mean_amp_sum = 0;
				for (i = 7 ; i > 0 ; i-- )
				{
					subband_mean_amp_pre[i] = subband_mean_amp_pre[i - 1];
					subband_mean_amp_sum = subband_mean_amp_sum + subband_mean_amp_pre[i];
				}	
				subband_mean_amp_pre[0] = subband_mean_amp_temp;
				subband_mean_amp = (float)(subband_mean_amp_sum + subband_mean_amp_pre[0]) / 8.0f;
				subband_mean_amp_sum = 0;
				noise_ratio = subband_mean_amp_temp / subband_mean_amp;
				for (i = 0 ; i < 7 ; i++)
				{
					if (subband_mean_amp < subband_mean_amp_pre[i])
					{
						subband_mean_amp = subband_mean_amp_pre[i];
					}
				}			
				break;
		}
		default:
		{
				subband_mean_amp = subband_mean_amp_temp;
				break;
		}
	}
	
	
	for (i = 1 ; i < arrayLength ; i++)
	{
    if (abs_dataArray[i] < subband_mean_amp)
		{
        //abs_dataArray[i] = 0;
		}
    else
		{
        //abs_dataArray[i] = abs_dataArray[i];
        peak_mark = i;
        peak_cnt = peak_cnt + 1;
        peak_position[peak_cnt] = i;
    }
	}
	
	
if ((subband_mean_amp > NOISE_THRETHOLD_MIN) && (subband_mean_amp < NOISE_THRETHOLD_MAX)){
	
		

		
	
	if ((arrayLength - peak_position[peak_cnt]) > SNORE_PEAK_MAX){
    array_end_flag = 1;
	}
	else
	{
    array_end_flag = 0;
	}
	
	pre_voice_dur = pre_voice_minus + peak_position[1];
	if (pre_voice_dur > SNORE_PEAK_MAX)
	{
    array_start_flag = 1;
    pre_voice_minus = 0;
    pre_voice_dur = 0;
	}
	else
	{
    array_start_flag = 0;
	}
	
	if (pre_voice_dur_flag != 0) {
    voice_start_flag = 1;
    voice_dur_flag = 1;
    voice_start_mark = pre_voice_start_mark;
	}
	else
		;
	
	for (i = 1 ; i <= (peak_cnt - 1) ; i++ )
	{
    if (voice_dur_flag == 0) {
        if ((peak_position[i + 1] - peak_position[i]) < SNORE_PEAK_MIN) {
						voice_start_peak_mark = i;
            voice_start_flag = 1;
            voice_start_mark = peak_position[i];
            voice_dur_flag = 1;
        }
				else
				;
		}
    else
		{
        if (((peak_position[i + 1] - peak_position[i]) > SNORE_PEAK_MAX) || ((i == peak_cnt - 1) && (array_end_flag)) || ((i == 1) && (array_start_flag)))
				{
            voice_stop_mark = peak_position[i];
            if (pre_voice_dur_flag > 0) {
								if (i == 1) {
									pre_voice_dur_flag = 0;
									voice_dur = pre_voice_last_mark - pre_voice_start_mark;
									pre_voice_start_mark = 0;
                  pre_voice_last_mark = 0;
                  array_start_flag = 0;
									peakValue = pre_peakValue;
								}
								else
								{
									voice_dur = voice_stop_mark - voice_start_mark + arrayLength * pre_voice_dur_flag;		//?????????8000???2,????????8000???????
									pre_voice_dur_flag = 0;                                                          			//??????
                  pre_voice_start_mark = 0;
									peakValue = pre_peakValue;
									for (j = voice_start_peak_mark; j <= i; j++)
									{
										temp_position = peak_position[j];
										if (peakValue < abs_dataArray[temp_position])
											peakValue = abs_dataArray[temp_position];
									}
								}								
								}
            else
						{
              voice_dur = voice_stop_mark - voice_start_mark;
							for (j = voice_start_peak_mark ; j < (i + array_end_flag) ; j++)
							{	
								temp_position = peak_position[j];
								if (peakValue < abs_dataArray[temp_position])
										peakValue = abs_dataArray[temp_position];
							}
            }
            voice_dur_flag = 0;
            if ((voice_dur > SNORE_LASTING) && (voice_dur < NOISE_LASTING)){
							if ((noise_ratio > NOISE_RATIO_THRESHOLD) && (mistake_flag == 0))
							{
								if (pOutput->snoreNum < SNORENUMMAX)
								{
									pOutput->ampArray[pOutput->snoreNum] = peakValue + REFERENCE_BIAS;
									if (i == 1)
										pOutput->pointArray[pOutput->snoreNum] = cnt_processing;
									else
										pOutput->pointArray[pOutput->snoreNum] = cnt_processing + 1;
									pOutput->snoreNum = pOutput->snoreNum + 1;							
									snore_num = snore_num + 1;
									pre_mistake_flag = 1;
								}
							}
							else
							{
							mistake_cnt = mistake_cnt + 1;
							mistake_ratio = noise_ratio;
							}
            }
						else
						;
						voice_start_mark = 0;
						voice_start_peak_mark = 1;
						peakValue = 0;
        }
				else
				;
				
				if (voice_start_mark != 0){
						if ((i == peak_cnt - 1) && ((arrayLength - peak_position[i + 1]) < SNORE_PEAK_MAX))   {              //????????????
								pre_voice_dur_flag = pre_voice_dur_flag + 1;
								pre_voice_start_mark = voice_start_mark;
								pre_voice_minus = arrayLength - peak_position[i + 1];
                pre_voice_last_mark = peak_position[i + 1];
								pre_peakValue = 0;
								for (j = voice_start_peak_mark; j <= i + 1; j++)
								{
									temp_position = peak_position[j];
									if (pre_peakValue < abs_dataArray[temp_position])
										pre_peakValue = abs_dataArray[temp_position];
								}
						}
						else
						;		
				}
    }
	}


//}								//????????

}

}	



int SnoreData_Return(unsigned short *dataArray, struct OutputBatch2 *pOutput, int snore_process_flag)
{

	int flag_15s_processing;
	int i;
	
	//if (!snore_process_flag)
	{
	if (cnt_processing == 0)
	{			
		for(i = 0; i < SNORENUMMAX; i++)
				{
					pOutput->snoreNum = 0;
					pOutput->ampArray[i] = 0;
					pOutput->pointArray[i] = 0;
				}
	}
	else
		;

	SnoreDetect(dataArray, pOutput);
	
	if (cnt_processing < 14)
	{
		cnt_processing++;
		flag_15s_processing = 0;
	}
	else
	{
		cnt_processing = 0;										//cnt?0???14???,????1
		flag_15s_processing = 1;
	}		
	
	return flag_15s_processing;
 }
	//else
	//return snore_process_flag;
}



void noise_suppression(signed short *data)
{
	
	
//	u16 filter_length = 100;
//	float filter_matrix[101];
//	float theta = 0.0001;
//	u16 Ntimes = 3;
//	float data_process[101];
//	float y_out[2501];
//	u16 i, j, k, g;
//	float filter_out;
//	float error;
//	
//	float data_nomi[2501];
//	int data_mean;
//	float temp;
//	int data_max;
//	int data_sum;
//	
//	for (i = 0; i < 2500; i++)
//	{
//		data_sum = data[i] + data_sum;
//	}
//	data_mean = data_sum / 2500;
////	for (i = 0; i < 2500; i++)
////	{printf(" %d \r\n",data[i]);}
//	for (i = 0; i < 2500; i++)
//	{
//		data[i] = data[i] - data_mean;
//		if (data[i] > 0)
//			temp = data[i];
//		else
//			temp = 0 - data[i];
//		if (temp > data_max)
//		{
//			data_max = temp;
//		}
//	}
//	for (i = 1; i < 2501; i++)
//	{
//		data_nomi[i] = (float)data[i - 1] / data_max;
//	}

//	
//	for (i = 0; i < 2501; i++)
//	{
//		y_out[i] = 0;
//	}
//	
//	for (i = 0; i < 101; i++)
//	{
//		filter_matrix[i] = 0;
//	}
//	
//	for (i = 0; i < Ntimes; i++)
//	{
//		for (k = filter_length; k < 2501; k++)
//			{
//				g = 1;
//				filter_out = 0;
//				for (j = k; j >= (k - filter_length + 1); j--)
//				{
//					data_process[g] = data_nomi[j];
//					filter_out = filter_out + filter_matrix[g] * data_process[g];
//					g = g + 1;
//				}
//				y_out[k] = filter_out;
//				error = (float)data_nomi[k] - filter_out;
//				for (g = 1; g < 101; g++)
//				{
//					filter_matrix[g] = filter_matrix[g] + theta * error * data_process[g];
//				}
//			}		
//	}
//			for (i = 1; i < 2501; i++)
//	{printf(" %f \r\n",y_out[i]);}
//		for (i = 1; i < 2501; i++)
//	{printf(" %f \r\n",data_nomi[i]);}
	
}