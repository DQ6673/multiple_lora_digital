#include "example/example_CAD.h"
#include "radio/sx127x_driver.h"
#include "hal/sx127x_hal.h"

/*
 * Copyright (c) 2019-2020 AIThinker.yangbin All rights reserved.
 *
 * ������ֻ��SX127X������demo�������ο�������֤�����ȶ��ԡ�
 *
 * author     Specter
 */
 
//���main����ʹ�õ�CAD���demo����Ҫ��ѯ���ã���Ҫͬʱ�ܶ��demo��������ɻ��ң�
//ע��CAD���ֻ�Ǽ��ǰ���룬�����ǰû��ǰ���룬���������ݷ���Ҳ���޷���⵽��
void exampleCAD(void){
	tRFProcessReturnCodes processRet=RF_IDLE;
	
	processRet=g_Radio.Process();
	switch(processRet){
		case RF_IDLE:	//����״̬
			g_Radio.StartCAD();
			break;
    case RF_CAD_DETECTED:	//CAD��⵽ǰ����
			DEBUG("[DEBUG %s()-%d]Cad detected--------------------\r\n",__func__,__LINE__);
			g_Radio.Init(NULL);	//�����ϴε�����
			g_Radio.StartCAD();
			break;
		case RF_CAD_EMPTY:	//CAD������û�м�⵽ǰ����
			DEBUG("[DEBUG %s()-%d]Cad empty\r\n",__func__,__LINE__);
			g_Radio.Init(NULL);	//�����ϴε�����
			g_Radio.StartCAD();
			break;
		case RF_CAD_TIMEOUT:	//CAD��ʱ
			DEBUG("[ERROR %s()-%d]Cad timeout\r\n",__func__,__LINE__);
			g_Radio.Init(NULL);	//�����ϴε�����
			g_Radio.StartCAD();
			break;
		case RF_BUSY:	//ģ��ִ��������
			break;
		case RF_UNKNOW_STATUS:	//�쳣״̬��
		default:
			DEBUG("[ERROR %s()-%d]unknow status:%d\r\n",__func__,__LINE__,processRet);
			break;
	}
}
