#include "syscfg.h"

u8 pair_flag = 0;
u32 stm32mcuUID;

void systemconfig(void)
{
	GetUID();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	delay_init();
	
	detection_init();
	
	LED_Init();							//LED 初始化，一般用于调试
	lora_init();						//Lora 初始化
	
	TIM3_Init();						//打开 TIM4 定时器，用于定时检测端口输入端口
	IWDG_Config(IWDG_Prescaler_64, 625);		//看门狗
}


//Lora发出配对请求信息函数
void Lorapairrequest(void)
{
	pair_flag = 1;
	lorasend();
	pair_flag = 0;
}



//获取stm32f1xxxx芯片的唯一ID函数
void GetUID(void)
{    
  stm32mcuUID += *(__IO u32*)(0X1FFFF7F2);//ID地址的高32位寄存器地址
  stm32mcuUID += *(__IO u32*)(0X1FFFF7EE);//ID地址的中32位寄存器地址
  stm32mcuUID += *(__IO u32*)(0X1FFFF7E8);//ID地址的低32位寄存器地址
	//stm32mcuUID += 2;
}



//独立看门狗定时器设置，
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{    
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
	IWDG_SetPrescaler( prv );
	IWDG_SetReload( rlv );
	IWDG_ReloadCounter();
	IWDG_Enable();
}


//喂狗
void IWDG_Feed(void)
{
   IWDG_ReloadCounter();
}



