#ifndef _TIM_H
#define _TIM_H


#include  "stm32f10x.h"



void TIM4_Init(void);   
void error_tickcount(void);
void IWDG_Config(uint8_t prv ,uint16_t rlv);
void IWDG_Feed(void);




#endif

