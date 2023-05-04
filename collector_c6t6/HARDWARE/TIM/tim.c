#include "tim.h"
#include "sys.h"
#include "detection.h"

int tim3_test;
unsigned int tim3_count;

void TIM3_Init(void)      		                             								  									
{
 	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;                 
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);           		
 
	TIM_TimeBaseStructure.TIM_Period = 1001;                        //10msÖÐ¶ÏÒ»´Î                 
	TIM_TimeBaseStructure.TIM_Prescaler =719; 											
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;           
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );                        

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;								
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 					
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 
	NVIC_Init(&NVIC_InitStructure);                               
	TIM_Cmd(TIM3, ENABLE);
}




void TIM3_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{   
		detect_scan();
		tim3_test ++;
		tim3_count ++;
		if(tim3_count >= 300)
		{
			tim3_count = 0;
			detect_flag = 1;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
	}
}
