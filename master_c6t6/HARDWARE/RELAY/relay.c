#include "relay.h"
#include "delay.h"

void RELAY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��B�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | 
																GPIO_Pin_3 | 
																GPIO_Pin_4 | 
																GPIO_Pin_5 | 
																GPIO_Pin_6 | 
																GPIO_Pin_7 | 
																GPIO_Pin_10 | 
																GPIO_Pin_11 ;	 
	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOB
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��A�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��GPIOA
	
	RELAY_1 = 0;
	RELAY_2 = 0;
	RELAY_3 = 0;
	RELAY_4 = 0;
	RELAY_5 = 0;
	RELAY_6 = 0;
	RELAY_7 = 0;
	RELAY_8 = 0;
	RELAY_9 = 0;
}


//���ü̵���״̬
//num �̵����ţ�comΪ����״̬��1Ϊ�պϣ�0Ϊ�Ͽ�
void Relay_Set(u8 num, u8 com)
{
	switch(num)
	{
		case	0 : RELAY_1 = com; break;
		case	1 : RELAY_2 = com; break;
		case	2 : RELAY_3 = com; break;
		case	3 : RELAY_4 = com; break;
		case	4 : RELAY_5 = com; break;
		case	5 : RELAY_6 = com; break;
		case	6 : RELAY_7 = com; break;
		case	7 : RELAY_8 = com; break;
		case  8 : RELAY_9 = com; break;
		
		default : break; 
	}
}

