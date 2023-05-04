#include "lora.h"
#include "syscfg.h"

uint8_t rxBuf[20]={0};
uint8_t txBuf[20];
uint8_t txLen = 12;																	//���ĳ���
uint16_t rxCount=20;
uint8_t LoraRxBuf_Ana_EN = 0;												//ʹ�ܱ�־λ������ص�������������
u8 test1=0;

//��ʼ��Loraģ��
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
					g_Radio.StartRx(0);
					break;
		
    case 	RF_RX_DONE:	//�������
					LED=~LED;
					memset(rxBuf,0,20);
					g_Radio.GetRxPacket(rxBuf,&rxCount);
					g_Radio.StartRx(0);
					LoraRxBuf_Ana_EN = 1;														//��������ջ������е�����
					break;
		
    case 	RF_RX_TIMEOUT:	//���ճ�ʱ
					g_Radio.StartRx(0);
					break;

		default:	break;

	}
}



//Lora�������ݺ���������������ģʽ
//���͸�ʽ # ��Ա�־ # 32λUID # �豸���� # �豸�ɼ�����
void lorasend(void)
{	
	sprintf((char *)txBuf,"#%c#%c%c%c%c#%c#%c#",
					pair_flag,																														//��Ա�־λ
					(char)(stm32mcuUID >> 24), (char)(stm32mcuUID >> 16), 
					(char)(stm32mcuUID >> 8), (char)stm32mcuUID,													//32λоƬID��������ʶ��
					DEVICE_S_CAP,																													//������̽ͷ����
					detect_temp_now);																											//������������ݣ�8λ
	
	TIM_Cmd(TIM4, DISABLE);
	LoraSendMessage();
	TIM_Cmd(TIM4, ENABLE);
}



//����һ��
void LoraSendMessage(void)
{
	tRFProcessReturnCodes processRet=RF_IDLE;	
	
	processRet=g_Radio.Process();
	
	if(processRet == RF_IDLE)
	{
		LED = 0;
		g_Radio.SetTxPacket(txBuf,txLen,500);
		
		while(processRet != RF_TX_DONE)
			processRet=g_Radio.Process();
		LED = 1;
		test1++;
	}
	
	g_Radio.Reset();	//��λ
	g_Radio.Init(NULL);	//�����ϴε�����
}




