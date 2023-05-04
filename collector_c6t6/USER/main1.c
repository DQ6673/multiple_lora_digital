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
////标准库需要支持的函数
//struct __FILE 
//{
//	int handle;
//};

//FILE __stdout;       
////定义_sys_exit()以避免工作在半主机状态
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 

//int fputc(int ch, FILE *f)
//{
//	//注意：USART_FLAG_TXE是检查发送缓冲区是否为空，这个要在发送前检查，检查这个提议提高发送效率，但是在休眠的时候可能导致最后一个字符丢失
//	//USART_FLAG_TC是检查发送完成标志，这个在发送后检查，这个不会出现睡眠丢失字符问题，但是效率低（发送过程中发送缓冲区已经为空了，可以接收下一个数据了，但是因为要等待发送完成，所以效率低）
//	//不要两个一起用，一起用效率最低
//	
//	//循环等待直到发送缓冲区为空(TX Empty)此时可以发送数据到缓冲区
//  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
//  {}
//	USART_SendData(USART3, (uint8_t) ch);

//  /* 循环等待直到发送结束*/
//  //while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
//  //{}

//	return ch;
//}

//void uartInit(){
//	USART_InitTypeDef USART_InitStructure;
//	GPIO_InitTypeDef  GPIO_InitStructure;

//	USART_DeInit(USART3);  //复位串口
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//注意APB1和APB2时钟使能函数不一样
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //使能指定端口时钟
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz，这里不用传参，直接写死用最大速度50MHZ
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	//初始化GPIO
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	//初始化GPIO
//	
//	//USART配置
//	USART_InitStructure.USART_BaudRate = 115200;	//设置串口波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶校验
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无流控
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//	USART_Init(USART3, &USART_InitStructure); //配置USART参数
//	USART_Cmd(USART3, ENABLE);                    //使能USART
//}


int main(void){
	tLoRaSettings stting={435000000,20,7,12,1,0x0005};	//频率435M，功率20dbm，带宽125kHz，SF=12，误码编码率4/5,前导码长度0x0005
	SysTick_Config(SystemCoreClock/1000);
	
	g_Radio.Init(&stting);	//初始化配置
	
//	uartInit();
	LED_Init();
	
	while(1){
			t1++;
			exampleRx();	//循环接收
			//exampleCAD();	//CAD循环检测测试

	}
}

