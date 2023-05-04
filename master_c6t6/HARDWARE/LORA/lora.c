#include "lora.h"
#include "RELAY.h"
#include "syscfg.h"

uint8_t rxBuf[rxLen]={0};
uint16_t rxCount=rxLen;
uint8_t LoraRxBuf_Ana_EN = 0;												//ʹ�ܱ�־λ������ص�������������

//Loraģ��������ݻص����������ڴ����յ�������
void Lora_callback(void)
{
	u8 detect_temp = rxBuf[10];
	u8 sensor_num = 0;
	u8 node_num = 0;
	u32 UID_temp = 0;
	u8 comnum_temp = 0;
	u8 comnum_sum = 0;
	u8 pair_flag = 0;
	
	
	
	if( LoraRxBuf_Ana_EN )
	{
		if(rxBuf[0] == '#' && rxBuf[7] == '#' && rxBuf[11] == '#')			//������ݸ�ʽ
		{
			UID_temp = ((u32)rxBuf[3] << 24) + ((u32)rxBuf[3] << 16) + ((u32)rxBuf[3] << 8) + ((u32)rxBuf[3]); 	//ƴ��UID
			comnum_temp = rxBuf[8];																																							//��ȡ�˿�����
			comnum_sum = comnum_temp;
			
			if(rxBuf[1] == 0X01 && OPSTA_Get(pairEN))											//������������Ҵ�ʱ������ӽڵ�״̬
			{
				test_pin = 0;
				for(u8 j=0; j<DEVICE_NUM_MAX; j++)
				{
					if(sensor_device[j].UID == UID_temp) 											//�ж��Ƿ�Ϊͬһ�豸�����󣬼��ж�UID�Ƿ��Ѿ�����
					{
						pair_flag = 1;																					
						test_pin += 1;
					}
					if(sensor_device[j].devicenum != 0)
						comnum_sum += sensor_device[j].comnum;
				}
				
				if(comnum_sum > DEVICE_NUM_MAX) 
				{
					pair_flag = 1;												//�����������
					test_pin += 2;
				}
				
				if(pair_flag == 0)
				{
					test_pin +=4;
					while((sensor_device[sensor_num].devicenum != 0) && (sensor_num < DEVICE_NUM_MAX)) sensor_num++; //Ѱ�Ҷ�Ӧ�ṹ���
			
					if(sensor_num < DEVICE_NUM_MAX)																				//����Ƿ񳬳��ṹ����������
					{
						test_pin +=8;
						OPSTA_Set(sensornum, sensor_num);																		//д�����ڲ����Ľṹ���
						sensor_device[sensor_num].UID = UID_temp;														//д��UID
						sensor_device[sensor_num].comnum = comnum_temp;											//д��˿ں�
						DWINsenddata(0X1960, (u16) UID_temp);																//��ʾUID
						DWINsenddata(0X1950, (u16) comnum_temp);														//��ʾ�˿ں�
					}
				}
			}

			else																													//����������󣬼�Ϊ���������ݴ���
			{
				for(u8 j=0; j< DEVICE_NUM_MAX; j++)
				{
					if(sensor_device[j].UID == UID_temp)											//Ѱ�Ҷ�Ӧ�ṹ��
					{
						if(sensor_device[j].dat != detect_temp)									//��ɼ��˳��ֱ仯��ˢ����Ļ��ʾ�ͼ̵������
						{
							sensor_device[j].dat = detect_temp;
							
							while((NodeInfBuf[node_num][0] != sensor_device[j].devicenum) && (node_num < DEVICE_NUM_MAX)) node_num++;	//�ڻ������ҵ���Ӧ���豸��ʼ��
							if(node_num < DEVICE_NUM_MAX)														//�ж��Ƿ񳬳�����
							{
								for(u8 i=0; i<sensor_device[j].comnum; i++)						//ѭ�����´���Ϊ�˿���
								{
									if(((detect_temp >> i) & 0X01) == 1)								//�жϵ�i�Ŷ˿��Ƿ�Ϊ1
									{
										Relay_Set(NodeInfBuf[node_num + i][10] - 1, 1);				//Ѱ�Ҹö˿ڶ�Ӧ�ļ̵����Ų��պ�
										NodeInfBuf[node_num + i][12] = (OPEN>>16) & 0X0000FFFF;
										NodeInfBuf[node_num + i][13] = (OPEN & 0X0000FFFF);
									}
									
									else										
									{
										Relay_Set(NodeInfBuf[node_num + i][10] - 1, 0);				//Ѱ�Ҹö˿ڶ�Ӧ�ļ̵����Ų��Ͽ�
										NodeInfBuf[node_num + i][12] = (CLOSE>>16) & 0X0000FFFF;
										NodeInfBuf[node_num + i][13] = (CLOSE & 0X0000FFFF);
									}

								}
								OneNode_Update();																			//�޸ĺ�����ݴ�nodebuf���µ�����Ļ
							}
						}
						sensor_device[j].errcount = 0;
						break;																										//�ҵ��˴������˾Ͳ��ü�������
					}
				}
			}
		}
	}

	LoraRxBuf_Ana_EN = 0;
}


//
void lora_init(void)
{
	tLoRaSettings stting={435000000,20,9,8,1,0x0005};	//Ƶ��435M������20dbm������125kHz��SF=12�����������4/5,ǰ���볤��0x0005
	//SysTick_Config(SystemCoreClock/1000);
	g_Radio.Init(&stting);	//��ʼ������
}



//Lora���������պ���������ѯ����
void lora_receive(void)
{
	
	tRFProcessReturnCodes processRet=RF_IDLE;

	processRet=g_Radio.Process();
	
	switch(processRet){
		
		case 	RF_IDLE:
					g_Radio.StartRx(3000);
					break;
		
    case 	RF_RX_DONE:	//�������
					LED=~LED;
					memset(rxBuf,0,10);
					g_Radio.GetRxPacket(rxBuf,&rxCount);
					g_Radio.StartRx(3000);
					LoraRxBuf_Ana_EN = 1;														//��������ջ������е�����
					break;
		
    case 	RF_RX_TIMEOUT:	//���ճ�ʱ
					g_Radio.StartRx(3000);
					break;

		default:	break;

	}
}



//Lora�������ݺ���������������ģʽ������ѯ����
void lora_senddata(void)
{
	tRFProcessReturnCodes processRet=RF_IDLE;
	uint8_t *txBuf=(uint8_t *)"hello world";
	
	processRet=g_Radio.Process();
	
	if(processRet == RF_IDLE)
	{
		
		g_Radio.SetTxPacket(txBuf,strlen((char *)txBuf),500);
		
		while(processRet != RF_TX_DONE)
			processRet=g_Radio.Process();
		LED = ~LED;
	}
	
	g_Radio.Reset();	//��λ
	g_Radio.Init(NULL);	//�����ϴε�����
}







