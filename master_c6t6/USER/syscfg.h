#ifndef __SYSCFG_H
#define __SYSCFG_H


//参数宏
/************************************************************************/

																					
#define 		DEVICE_NUM_MAX		8						//允许使用的本机输出端口数量
																					//如为8则可用9个继电器，第9个继电器为信号检测继电器
																					//如为5则可用6个继电器，第6个继电器为信号检测继电器

#define			sensornum					0
#define			pairEN						1
#define			pagenum						2

#define 		ERRCOUNT					500		

#define 	Point_size		160
#define   addr_base     0x00F0
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
#include "myiic.h"
#include "24cxx.h"
#include "relay.h"
#include "lora.h"
#include "DWIN_APP.h"
#include "tim.h"

/************************************************************************/


typedef struct
{
	u8 devicenum;				//设备号
	u8 comnum;					//端口数
	u32 UID;						//采集端唯一ID
	u8 dat;							//采集数据（8位）
	u16 errcount;		//采集端通信状态
	u8  errflag;
	u16 addr[8][10];// = {14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020};
}sensor;


extern u8 OP_STA;
extern sensor sensor_device[8];
extern u16 NodeInfBuf[10][16];
extern u16 test_pin;

//函数
/************************************************************************/

void systemconfig(void);
void DeviceInfSaveRom(sensor *dev);
void DeviceInfReadRom(void);
void DeviceInfDelete(sensor *dev);
void DeviceInfDeleteAllBlock(void);
void OPSTA_Set(u8 item, u8 num);
u8 OPSTA_Get(u8 item);
void NodeInfInit(void);
void signalerror_check(void);


#endif
