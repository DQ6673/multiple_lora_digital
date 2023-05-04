#include "sys.h"
#include "usart.h"	  

#include "DWIN_SET.H"


u8  Res_sta = 0;
u8  IN_Buf_flag = 0;
u8  Buf_len_num = 0;

u16 USART_RX_STA=0;       //接收状态标记	  




void USART3_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	// 外设使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART_DeInit(USART3); 
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_11;
  GPIO_Init(GPIOB,&GPIO_InitStrue);
	
	USART_InitStrue.USART_BaudRate=baud;
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; 
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; 
	USART_InitStrue.USART_Parity=USART_Parity_No; 
	USART_InitStrue.USART_StopBits=USART_StopBits_1;
	USART_InitStrue.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3,&USART_InitStrue);
	
	
	NVIC_InitStrue.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStrue);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_Cmd(USART3,ENABLE);
}

/************************************************************************************/

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	static u8 Res = 0;
	
	if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)
	{
		USART_ReceiveData(USART3);
		USART_ClearFlag(USART3, USART_FLAG_PE);
	} 
	
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART3);
		USART_ClearFlag(USART3, USART_FLAG_ORE);
	} 
	
	if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET) 
	{
		USART_ReceiveData(USART3);
		USART_ClearFlag(USART3,USART_FLAG_FE);
	} 
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART3,USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); 
		Res = USART_ReceiveData(USART3);
		switch(Res)
		{
			case 0x5A : if(Res_sta == 0) Res_sta = 1; break;

			case 0xA5 : if(Res_sta == 1) Res_sta = 2; break;
						
			default   : if(Res_sta == 2)
						{
							Res_sta = 3;
							Buf_len_num = Res;
							IN_Buf_flag = 1;
						}						
						break;
		}

		if(IN_Buf_flag == 1)
		{
			if(DWIN_RX_CNT <= Buf_len_num)
			{
				DWIN_RX_BUF[ DWIN_RX_CNT ] = Res;
				DWIN_RX_CNT ++ ;

				if (DWIN_RX_CNT >= DWIN_RX_BUF[0] + 1)
				{
					DWIN_RX_CNT = 0;
					Res_sta = 0;
					IN_Buf_flag = 0;
					DWIN_Analysis_EN = 1;
//					DWIN_Buffer_Analysis();
				}
			}
		}
	}
	
	else if(USART_GetITStatus(USART3, USART_FLAG_LBD) != RESET)
	{
		Res = USART_ReceiveData(USART3);
		USART_ClearFlag(USART3,USART_FLAG_LBD);
	}
}




/****************************************************************************/




void USART_SendString(USART_TypeDef* USARTx,u8 *pbuff,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		USART_SendData(USARTx,*pbuff++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}
