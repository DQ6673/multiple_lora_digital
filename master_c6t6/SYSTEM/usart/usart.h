#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 


	extern u8  Res_sta;
//����봮���жϽ��գ��벻Ҫע�����º궨��

void USART2_Init(uint32_t baud);
void USART_SendString(USART_TypeDef* USARTx,u8 *pbuff,u8 len);

#endif


