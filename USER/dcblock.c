
#include "dcblock.h"
void dcblock(float signal[], int length, float mem[])
{
    float x0, x1, y1;
    int i;

    x1 = mem[0];
    y1 = mem[1];

    for (i = 0; i < length; i++)
    {
        x0 = signal[i];

				signal[i] = x0 - x1 + 0.960003F * y1;			//by Phone: Y[i] = X[i] - X[i - 1] + 0.960003 * Y[i - 1]
																									//IIR
        x1 = x0;
        y1 = signal[i];
    }

    mem[0] = x1;
    mem[1] = y1;
}



