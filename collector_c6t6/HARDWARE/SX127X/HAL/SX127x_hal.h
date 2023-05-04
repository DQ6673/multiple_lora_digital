#ifndef __SX127X_HAL_H__
#define __SX127X_HAL_H__
/*
 * LoRa sx1278/76����
 *
 * \version    1.0.0 
 * \date       Dec 17 2019
 * \author     Specter
 */
 
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stdio.h"

#define DEBUG( format , ... )	printf( format , ##__VA_ARGS__ )	//��ӡlog��Ϣ�ĺ��������Բ�����
#define GET_TICK_COUNT() GetTick()	//��ȡsystick�ĺ���

void Soft_delay_ms(u16 time);
void SX1276HALInit( void );
uint8_t SpiInOut( uint8_t outData );
void SpiNSSEnable( uint8_t status );
void SX127X_ResetPinControl( uint8_t status );
uint8_t SX1276ReadDio0(void);
uint8_t SX1276ReadDio1(void);
uint8_t SX1276ReadDio2(void);
uint8_t SX1276ReadDio3(void);
uint8_t SX1276ReadDio4(void);
uint8_t SX1276ReadDio5(void);

#endif //end of __SX127X_HAL_H__
