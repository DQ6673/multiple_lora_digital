#ifndef __SYSCFG_H
#define __SYSCFG_H


//参数宏
/************************************************************************/
#define			DEVICE_S_CAP			5						//设备容量	，指一台采集设备的探头接口数			

/************************************************************************/



//头文件
/************************************************************************/
//lib include
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"

//system include
#include "usart.h"
#include "delay.h"
#include "sys.h"

//hardware include
#include "led.h"
#include "lora.h"
#include "tim.h"
#include "detection.h"

/************************************************************************/


typedef struct
{
	u8 devicenum;				//设备号
	u8 comnum;					//端口数
	u32 ID;							//采集端唯一ID
	u8 dat;							//采集数据（8位）
	u8 sta;							//采集端通信状态
}sensor;


extern u8 pair_flag;
extern u32 stm32mcuUID;

//函数
/************************************************************************/

void systemconfig(void);
void Lorapairrequest(void);
void GetUID(void);
void IWDG_Config(uint8_t prv ,uint16_t rlv);
void IWDG_Feed(void);


#endif
