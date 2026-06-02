#ifndef __NTC__H
#define __NTC__H
#include "Function_Init.H"

#ifndef GET_ARRAY_LEN
#define GET_ARRAY_LEN(arr)   ((u16)(sizeof(arr) / sizeof((arr)[0])))
#endif

#define CH_Rref    510u // 标准电阻 5.1K 
#define AD_BIT     4096u


/* CH_Rref 已经是 10ohm 单位，不能再除以 10，否则阻值会小 10 倍，温度会虚高。 */
#define CH_D2R(adc)   ((u16)(((u32)(adc) * CH_Rref) / (AD_BIT - (u32)(adc))))

s16 CH_R2T(u16 res10);


#endif


