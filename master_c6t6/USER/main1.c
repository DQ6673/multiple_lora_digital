#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "stm32f10x_it.h"
#include "radio/sx127x_driver.h"
#include "example/example_TX.h"
#include "example/example_RX.h"
#include "example/example_CAD.h"
#include "led.h"

unsigned char t1 = 0;

//#pragma import(__use_no_semihosting)             
////��׼����Ҫ֧�ֵĺ���
//struct __FILE 
//{
//	int handle;
//};

//FILE __stdout;       
////����_sys_exit()�Ա��⹤���ڰ�����״̬
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 

//int fputc(int ch, FILE *f)
//{
//	//ע�⣺USART_FLAG_TXE�Ǽ�鷢�ͻ������Ƿ�Ϊ�գ����Ҫ�ڷ���ǰ��飬������������߷���Ч�ʣ����������ߵ�ʱ����ܵ������һ���ַ���ʧ
//	//USART_FLAG_TC�Ǽ�鷢����ɱ�־������ڷ��ͺ��飬����������˯�߶�ʧ�ַ����⣬����Ч�ʵͣ����͹����з��ͻ������Ѿ�Ϊ���ˣ����Խ�����һ�������ˣ�������ΪҪ�ȴ�������ɣ�����Ч�ʵͣ�
//	//��Ҫ����һ���ã�һ����Ч�����
//	
//	//ѭ���ȴ�ֱ�����ͻ�����Ϊ��(TX Empty)��ʱ���Է������ݵ�������
//  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
//  {}
//	USART_SendData(USART3, (uint8_t) ch);

//  /* ѭ���ȴ�ֱ�����ͽ���*/
//  //while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
//  //{}

//	return ch;
//}

//void uartInit(){
//	USART_InitTypeDef USART_InitStructure;
//	GPIO_InitTypeDef  GPIO_InitStructure;

//	USART_DeInit(USART3);  //��λ����
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//ע��APB1��APB2ʱ��ʹ�ܺ�����һ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //ʹ��ָ���˿�ʱ��
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz�����ﲻ�ô��Σ�ֱ��д��������ٶ�50MHZ
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ��GPIO
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ��GPIO
//	
//	//USART����
//	USART_InitStructure.USART_BaudRate = 115200;	//���ô��ڲ�����
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;	//����żУ��
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
//	USART_Init(USART3, &USART_InitStructure); //����USART����
//	USART_Cmd(USART3, ENABLE);                    //ʹ��USART
//}


int main(void){
	tLoRaSettings stting={435000000,20,7,12,1,0x0005};	//Ƶ��435M������20dbm������125kHz��SF=12�����������4/5,ǰ���볤��0x0005
	SysTick_Config(SystemCoreClock/1000);
	
	g_Radio.Init(&stting);	//��ʼ������
	
//	uartInit();
	LED_Init();
	
	while(1){
			t1++;
			exampleRx();	//ѭ������
			//exampleCAD();	//CADѭ��������

	}
}

