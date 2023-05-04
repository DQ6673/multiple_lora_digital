#include "detection.h"

u8 detect_temp = 0;
u8 detect_temp_now = 0;
u8 detect_flag = 0;

//检测端口初始化
void detection_init(void)
{
	detectIOinit();
	detectNVICinit();
	detectEXTIinit();
}

int temp_test[8];


void detect_scan(void)
{
	if(Din_4 == 0)		detect_temp |=  (1 << 3);
	else							detect_temp &= ~(1 << 3);
	
	if(Din_7 == 0)		detect_temp |=  (1 << 6);
	else							detect_temp &= ~(1 << 6);
	
	if(Din_3 == 0)		detect_temp |=  (1 << 2);
	else							detect_temp &= ~(1 << 2);
	
	if(Din_2 == 0)		detect_temp |=  (1 << 1);
	else							detect_temp &= ~(1 << 1);
	
	if(Din_1 == 0)		detect_temp |=  (1 << 0);
	else							detect_temp &= ~(1 << 0);
	
	if(Din_5 == 0)		detect_temp |=  (1 << 4);
	else							detect_temp &= ~(1 << 4);
	
	if(Din_8 == 0)		detect_temp |=  (1 << 7);
	else							detect_temp &= ~(1 << 7);
	
	if(Din_6 == 0)		detect_temp |=  (1 << 5);
	else							detect_temp &= ~(1 << 5);
}




void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)					//检测标志位
	{
		if(Din_4 == 0)		detect_temp |=  (1 << 3);
		else							detect_temp &= ~(1 << 3);
		
		detect_flag = 1;		temp_test[3]++;
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}




void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)					//检测标志位
	{
		if(Din_7 == 0)		detect_temp |=  (1 << 6);
		else							detect_temp &= ~(1 << 6);
		
		detect_flag = 1;			temp_test[6]++;
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}




void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)					//检测标志位
	{
		if(Din_3 == 0)		detect_temp |=  (1 << 2);
		else							detect_temp &= ~(1 << 2);
		
		detect_flag = 1;		temp_test[2]++;
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}




void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)					//检测标志位
	{
		if(Din_2 == 0)		detect_temp |=  (1 << 1);
		else							detect_temp &= ~(1 << 1);
		
		detect_flag = 1;		temp_test[1]++;
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)					//检测标志位
	{
		if(Din_1 == 0)		detect_temp |=  (1 << 0);
		else							detect_temp &= ~(1 << 0);
		
		detect_flag = 1;		temp_test[0]++;
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}


void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)					//检测标志位
	{
		if(Din_5 == 0)		detect_temp |=  (1 << 4);
		else							detect_temp &= ~(1 << 4);
		
		detect_flag = 1;		temp_test[4]++;
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)					//检测标志位
	{
		if(Din_8 == 0)		detect_temp |=  (1 << 7);
		else							detect_temp &= ~(1 << 7);
		
		detect_flag = 1;		temp_test[7]++;
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)					//检测标志位
	{
		if(Din_6 == 0)		detect_temp |=  (1 << 5);
		else							detect_temp &= ~(1 << 5);
		
		detect_flag = 1;		temp_test[5]++;
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}




//配置检测输入IO口
void detectIOinit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);									//使能GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);									//使能GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);									//产生外部中断须使能复用时钟
	
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_11; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 												//选择输入模式为浮空输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         						//输出频率最大50MHz
  GPIO_Init(GPIOB,&GPIO_InitStructure);                                 //
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 														//A15 引脚
  GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2); 								//GPIO默认不产生中断，此函数将GPIO连接到中断线上
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3); 								
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4); 								
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5); 								
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6); 								
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10); 								
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11); 								
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15); 								
}





//配置与使能中断
void detectNVICinit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;     				//选择中断通道2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	//抢占式中断优先级设置为0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        	//响应式中断优先级设置为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           	//使能中断
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line2);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;     				//选择中断通道3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	//抢占式中断优先级设置为0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        	//响应式中断优先级设置为0
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line3);
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;     				//选择中断通道4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	//抢占式中断优先级设置为0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        	//响应式中断优先级设置为0
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line4);
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;     			//选择中断通道5-9
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	//抢占式中断优先级设置为0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        	//响应式中断优先级设置为0
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line5);
	EXTI_ClearITPendingBit(EXTI_Line6);
			
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;     		//选择中断通道10-15
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	//抢占式中断优先级设置为0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;        	//响应式中断优先级设置为0
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line10);
	EXTI_ClearITPendingBit(EXTI_Line11);
	EXTI_ClearITPendingBit(EXTI_Line15);
}





//配置外部中断参数
void detectEXTIinit(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

  EXTI_InitStructure.EXTI_Line = 	EXTI_Line2 | 
																	EXTI_Line3 |
																	EXTI_Line4 |
																	EXTI_Line5 | 
																	EXTI_Line6 | 
																	EXTI_Line10 | 
																	EXTI_Line11 | 
																	EXTI_Line15 ; 															//选择中断线路1
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 												//设置为中断请求，非事件请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 							//设置中断触发方式为上下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //外部中断使能

	EXTI_Init(&EXTI_InitStructure);
}

