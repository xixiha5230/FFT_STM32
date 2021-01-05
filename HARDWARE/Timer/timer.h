#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"                  // Device header

void ALL_NVIC_Configuration(void);
void AllClock_Init(void);
void TIM2_Configuration(unsigned short per,unsigned short sca);
void TIM3_Configuration(unsigned short per,unsigned short sca);
void TIM4_Configuration(unsigned short per,unsigned short sca);
void ConfigTimerForRunTimeStates(void);


extern volatile unsigned long long FreeRTOSRunTimeTicks;

#endif
