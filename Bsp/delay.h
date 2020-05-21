#ifndef _DELAY_H
#define _DELAY_H

#include "STC8A8K64S4A12.h"

//#include "MyConfig.h"

void Timer1_Init(void);
void Delay_ms(uint16_t Times);
//void Delay1ms(uint16_t Num);		//@11.0592MHz
#endif