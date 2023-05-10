#ifndef __SYSCFG_H
#define __SYSCFG_H


#define DEVICE_NUM_MAX 5 // 支持的最多节点数

#define sensornum 0
#define pairEN 1
#define pagenum 2

#define ERRCOUNT 500

#define Point_size 160
#define addr_base 0x00F0
/************************************************************************/

/************************************************************************/
// lib include
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"

// system include
#include "usart.h"
#include "delay.h"
#include "sys.h"

// hardware include
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
	u8 devicenum; // �豸��
	u8 comnum;	  // �˿���
	u32 UID;	  // �ɼ���ΨһID
	u8 dat;		  // �ɼ����ݣ�8λ��
	u16 errcount; // �ɼ���ͨ��״̬
	u8 errflag;
	u16 addr[8][10]; // = {14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020};
} sensor;

extern u8 OP_STA;
extern sensor sensor_device[8];
extern u16 NodeInfBuf[10][16];
extern u16 test_pin;

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
