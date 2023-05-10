#ifndef __SYSCFG_H
#define __SYSCFG_H


//������
/************************************************************************/
#define			DEVICE_S_CAP			5						//�豸����	��ָһ̨�ɼ��豸��̽ͷ�ӿ���			

/************************************************************************/



//ͷ�ļ�
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
	u8 devicenum;				//�豸��
	u8 comnum;					//�˿���
	u32 ID;							//�ɼ���ΨһID
	u8 dat;							//�ɼ����ݣ�8λ��
	u8 sta;							//�ɼ���ͨ��״̬
}sensor;


extern u8 pair_flag;
extern u32 stm32mcuUID;

//����
/************************************************************************/

void systemconfig(void);
void Lorapairrequest(void);
void GetUID(void);
void IWDG_Config(uint8_t prv ,uint16_t rlv);
void IWDG_Feed(void);


#endif
