#ifndef _TIM_H
#define _TIM_H


#include  "stm32f10x.h"


#define Relay_Fresh_Peroid  		11							//�̵���״̬����ʱ��100ms
#define DAC_Fresh_Peroid    		102							//DAC״̬����ʱ��1000ms
#define Warning_Fresh_Peroid  	100							//������ѯ����100ms
#define ReWarning_Fresh_Peroid  6001				//��λ��̵�������ʹ��10min				
#define SENSOR_ERROR_OVERTIME		18002				//����������ʱ3min
#define DataRX_TimeOut          6501				//�������ݳ�ʱ1min
#define Refresh_Div_Min         5


typedef struct
{
	uint8_t  rom_timer_count;
	uint16_t relay_timer_count;                   //�̵���ˢ�¼�ʱ
	uint16_t DAC_timer_count;											//ADCˢ�¼�ʱ
	uint16_t Warning_timer_count;									//����ˢ�¼�ʱ
	uint32_t Rewarning_timer_count;								//���±�����ʱ
}tim;


void TIM1_Init(void);
void TIM2_Init(void); 
void TIM3_Init(void);
void TIM4_Init(void);   


#endif

