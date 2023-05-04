#ifndef _TIM_H
#define _TIM_H


#include  "stm32f10x.h"


#define Relay_Fresh_Peroid  		11							//继电器状态更新时间100ms
#define DAC_Fresh_Peroid    		102							//DAC状态更新时间1000ms
#define Warning_Fresh_Peroid  	100							//报警查询更新100ms
#define ReWarning_Fresh_Peroid  6001				//复位后继电器重新使能10min				
#define SENSOR_ERROR_OVERTIME		18002				//传感器错误超时3min
#define DataRX_TimeOut          6501				//接收数据超时1min
#define Refresh_Div_Min         5


typedef struct
{
	uint8_t  rom_timer_count;
	uint16_t relay_timer_count;                   //继电器刷新计时
	uint16_t DAC_timer_count;											//ADC刷新计时
	uint16_t Warning_timer_count;									//报警刷新计时
	uint32_t Rewarning_timer_count;								//重新报警计时
}tim;


void TIM1_Init(void);
void TIM2_Init(void); 
void TIM3_Init(void);
void TIM4_Init(void);   


#endif

