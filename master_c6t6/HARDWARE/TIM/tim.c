#include "tim.h"
#include "sys.h"
#include "DWIN_APP.h"
#include "syscfg.h"

int error_check_count = 0; 
u8 error_check_enable = 0;

void TIM4_Init(void)      		                             								  									
{
 	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;                 
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);           		
 
	TIM_TimeBaseStructure.TIM_Period = 2001;                        //20ms中断一次    1001             
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






//定时累加errorcount，判断采集设备是否断连，用于定时器中断调用
//每20ms调用一次
void error_tickcount(void)
{
	for(u8 i=0; i < DEVICE_NUM_MAX; i++)
	{
		if((sensor_device[i].devicenum != 0) && (sensor_device[i].errcount < (ERRCOUNT + 100)))  //计时器计数，寻找非零设备号的结构体，并判断是否溢出
		{	
			sensor_device[i].errcount++;
		}
	}
}





//独立看门狗定时器设置，
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{    
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
	IWDG_SetPrescaler( prv );
	IWDG_SetReload( rlv );
	IWDG_ReloadCounter();
	IWDG_Enable();
}

//喂狗
void IWDG_Feed(void)
{
   IWDG_ReloadCounter();
}



void TIM3_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{   
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
		
		DWIN_Buffer_Analysis();														//迪文缓存器回调函数
		error_tickcount();																//增加信号计数
		
		error_check_count++;
		if(error_check_count >= 50) 
		{
			error_check_count =0;
			error_check_enable = 1;
		}
	}
}
