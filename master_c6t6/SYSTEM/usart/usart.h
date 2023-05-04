#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 


	extern u8  Res_sta;
//如果想串口中断接收，请不要注释以下宏定义

void USART2_Init(uint32_t baud);
void USART_SendString(USART_TypeDef* USARTx,u8 *pbuff,u8 len);

#endif


