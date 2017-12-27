#ifndef SNORE_DETECTION_H_
#define SNORE_DETECTION_H_

#define FRAME_MOVE                  128
#define FRAME_LENGTH                256
#define FRAME_NUM_IN_HALF_SECOND    64
#define FRAME_NUM_IN_3_SECOND       384
#define FRAME_NUM_IN_3P5_SECOND     (384 + 64)
#define MFCC_COEF_LENGTH            12

enum
{
    EVENT_INIT = 0,
    EVENT_START = 1,
    EVENT_END = 2,
};

typedef struct
{
    float aud_data[FRAME_LENGTH];
    float energy[FRAME_NUM_IN_HALF_SECOND];
    int zero_cross_rate[FRAME_NUM_IN_HALF_SECOND];
    float mfcc_coef[FRAME_NUM_IN_3P5_SECOND][MFCC_COEF_LENGTH];
    float mfcc_dtw[FRAME_NUM_IN_3_SECOND][MFCC_COEF_LENGTH];
    int frame_count;
    int mfcc_count;
    int event_flag;
    int error_count;
} SnoreCore;

void SnoreDetect_init();
int SnoreDetect_int_proc(float samples1[]);

#endif

