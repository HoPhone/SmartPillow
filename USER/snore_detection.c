#include <stdlib.h>
#include <string.h>
#include "snore_detection.h"
#include "dcblock.h"
#include "stm32f4xx.h"
#include "arm_math.h"

float aud_data[FRAME_LENGTH];
float energy[FRAME_NUM_IN_HALF_SECOND];
int zero_cross_rate[FRAME_NUM_IN_HALF_SECOND];
float mfcc_coef[FRAME_NUM_IN_3P5_SECOND][MFCC_COEF_LENGTH];
float mfcc_dtw[FRAME_NUM_IN_3_SECOND][MFCC_COEF_LENGTH];
int frame_count;
int mfcc_count;
int mfcc_2_dtw_length;
int event_flag;
int error_count;
float dcblock_mem[2];

float threshold_energy_zcr = 0.0026;

static float get_energy(float samples[], int size);
static int get_zcr(float samples[], int size);

/*
 *
 */
void SnoreDetect_init()
{
    frame_count = 0;
    mfcc_count = 0;
    event_flag = EVENT_INIT;
    error_count = 0;
    memset(aud_data, 0, sizeof(aud_data));
    memset(energy, 0, sizeof(energy));
    memset(zero_cross_rate, 0, sizeof(zero_cross_rate));
    memset(mfcc_coef, 0, sizeof(mfcc_coef));
    memset(dcblock_mem, 0, sizeof(dcblock_mem));
}

/*
 *
 */
int SnoreDetect_int_proc(float samples1[])
{
    float energy_sum = 0.0F;
    int zcr_sum = 0;
    int trigger_dtw_task = 0;
    int i;
	  float32_t samples_max;
	  uint32_t pIndex;
	  float samples[128];

	  
		arm_max_f32(samples1, 128, &samples_max, &pIndex);			//by Phone: 求出最大值和所在索引
	
	  for(int mm=0;mm<128;mm++)
	  {
			samples[mm]=samples1[mm]/samples_max;									//by Phone: 归一化
		}
		
    dcblock(samples, FRAME_MOVE, dcblock_mem);

    if (frame_count < FRAME_NUM_IN_HALF_SECOND)							//by Phone: 64
    {
				energy[frame_count] = get_energy(samples, FRAME_MOVE);	//by Phone: 求能量，128点内的平方和
				zero_cross_rate[frame_count] = get_zcr(samples, FRAME_MOVE); //by Phone: 过零点次数

        frame_count++;
    }
    else
    {
        memmove(energy, energy+1, (FRAME_NUM_IN_HALF_SECOND-1) * sizeof(float));
        memmove(zero_cross_rate, zero_cross_rate+1, (FRAME_NUM_IN_HALF_SECOND-1) * sizeof(int));

        energy[FRAME_NUM_IN_HALF_SECOND-1] = get_energy(samples, FRAME_MOVE);
        zero_cross_rate[FRAME_NUM_IN_HALF_SECOND-1] = get_zcr(samples, FRAME_MOVE);
    }

    
    memmove(aud_data, aud_data + FRAME_MOVE, FRAME_MOVE * sizeof(float));
    memcpy(aud_data + FRAME_MOVE, samples, FRAME_MOVE * sizeof(float));

    if (mfcc_count < FRAME_NUM_IN_3P5_SECOND)
    {
       

        mfcc_count++;
    }
    else
    {
        
        memmove(mfcc_coef[0], mfcc_coef[1], (FRAME_NUM_IN_3P5_SECOND-1) * MFCC_COEF_LENGTH * sizeof(float));
        
    }

    if (EVENT_INIT == event_flag || EVENT_END == event_flag)
    {

        if (frame_count >= FRAME_NUM_IN_HALF_SECOND)
        {
            frame_count = 0;        

        
            for (i = 0; i < FRAME_NUM_IN_HALF_SECOND; i++)
            {
                energy_sum += energy[i];
                zcr_sum += zero_cross_rate[i];
            }

            if (energy_sum > threshold_energy_zcr * zcr_sum)
            {
                event_flag = EVENT_START;

             
                if (mfcc_count < FRAME_NUM_IN_HALF_SECOND)
                {
                    error_count++;
                   
                }
                else
                {
                    memmove(mfcc_coef[0], mfcc_coef[mfcc_count - FRAME_NUM_IN_HALF_SECOND], FRAME_NUM_IN_HALF_SECOND * MFCC_COEF_LENGTH * sizeof(float));
                    mfcc_count = FRAME_NUM_IN_HALF_SECOND;
                }
            }
        }
    }
    else
    {
       
        if (frame_count >= FRAME_NUM_IN_HALF_SECOND)
        {
            frame_count = 0;        

            for (i = 0; i < FRAME_NUM_IN_HALF_SECOND; i++)
            {
                energy_sum += energy[i];
                zcr_sum += zero_cross_rate[i];
            }

            if (energy_sum > threshold_energy_zcr * zcr_sum)
            {
                if (FRAME_NUM_IN_3_SECOND == mfcc_count)
                {
                    
                    memcpy(mfcc_dtw[0], mfcc_coef[0], FRAME_NUM_IN_3_SECOND * MFCC_COEF_LENGTH * sizeof(float));
                    mfcc_2_dtw_length = mfcc_count;
                    mfcc_count = 0;

                    trigger_dtw_task = 1;
                }
            }
            else
            {
                event_flag = EVENT_END;

                
                memcpy(mfcc_dtw[0], mfcc_coef[0], mfcc_count * MFCC_COEF_LENGTH * sizeof(float));
                mfcc_2_dtw_length = mfcc_count;
                mfcc_count = 0;

                trigger_dtw_task = 1;
            }
        }
    }

    return event_flag;
}

void SnoreCore_dtw_proc()
{
}


static float get_energy(float *data, int size)
{
    float data_energy = 0.0F;
    int i;

    for (i = 0; i < size; i++)
    {
        data_energy += data[i] * data[i];
    }
    data_energy = data_energy;

    return data_energy;
}


static int get_zcr(float *data, int size)
{
	int num=0;
	int i;
	for (i = 0; i < (size-1); i++)
	{
		if (data[i] * data[i+1] < 0)
		{
			num++;
		}
	}

	return num;
}


float Get_mean(float* data,int size)
{
    float mean=0;
    int length=0;
    float sum=0;
    for(length=0;length<size;length++)
    {
        sum+=data[length];
    }
    mean=sum/size;
    return mean;
}

