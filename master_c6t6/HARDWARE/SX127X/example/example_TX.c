#include "example/example_TX.h"
#include "radio/sx127x_driver.h"
#include "hal/sx127x_hal.h"
#include "string.h"
#include "led.h"

/*
 * Copyright (c) 2019-2020 AIThinker.yangbin All rights reserved.
 *
 * ������ֻ��SX127X������demo�������ο�������֤�����ȶ��ԡ�
 *
 * author     Specter
 *
 *  ����Ƿ��Ͳ��ԣ���Ͻ���ģ����Բ����շ�����
 */
 
//���main����ʹ�õķ��Ͳ��ԣ���Ҫ��ѯ���ã���Ҫͬʱ�ܶ��demo��������ɻ��ң�
void exampleTx(void){
	tRFProcessReturnCodes processRet=RF_IDLE;
	static uint32_t systickBak=200000;
	uint8_t *txBuf=(uint8_t *)"hello world";
	
	processRet=g_Radio.Process();
	switch(processRet){
		case RF_IDLE:	//����״̬
			if(GET_TICK_COUNT()-systickBak >1000){
				//��ʼһ�η���
				//DEBUG("[DEBUG %s()-%d]start send\r\n",__func__,__LINE__);
				g_Radio.SetTxPacket(txBuf,strlen((char *)txBuf),15000);
			}
			break;
    case RF_TX_DONE:	//�������
			//DEBUG("[DEBUG %s()-%d]send OK\r\n",__func__,__LINE__);
			LED=~LED;
			g_Radio.Reset();	//��λ
			g_Radio.Init(NULL);	//�����ϴε�����
			systickBak=GET_TICK_COUNT();
			break;
    case RF_TX_TIMEOUT:	//���ͳ�ʱ
			//DEBUG("[DEBUG %s()-%d]send Timeout\r\n",__func__,__LINE__);
			g_Radio.Reset();	//��λ
			g_Radio.Init(NULL);	//�����ϴε�����
			systickBak=GET_TICK_COUNT();
			break;
		case RF_BUSY:	//ģ��ִ��������
			break;
		case RF_UNKNOW_STATUS:	//�쳣״̬��
		default:
			//DEBUG("[ERROR %s()-%d]unknow status:%d\r\n",__func__,__LINE__,processRet);
			break;
	}
}

