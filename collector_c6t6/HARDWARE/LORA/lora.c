#include "lora.h"
#include "syscfg.h"

uint8_t rxBuf[20]={0};
uint8_t txBuf[20];
uint8_t txLen = 12;																	//报文长度
uint16_t rxCount=20;
uint8_t LoraRxBuf_Ana_EN = 0;												//使能标志位，允许回调函数处理数据
u8 test1=0;

//初始化Lora模组
void lora_init(void)
{
	tLoRaSettings stting={435000000,20,9,8,1,0x0005};	//频率435M，功率20dbm，带宽125kHz，SF=12，误码编码率4/5,前导码长度0x0005
	//SysTick_Config(SystemCoreClock/1000);
	g_Radio.Init(&stting);	//初始化配置
}




//Lora非阻塞接收函数，需轮询调用
void lora_receive(void)
{
	
	tRFProcessReturnCodes processRet=RF_IDLE;

	processRet=g_Radio.Process();
	
	switch(processRet){
		
		case 	RF_IDLE:
					g_Radio.StartRx(0);
					break;
		
    case 	RF_RX_DONE:	//接收完成
					LED=~LED;
					memset(rxBuf,0,20);
					g_Radio.GetRxPacket(rxBuf,&rxCount);
					g_Radio.StartRx(0);
					LoraRxBuf_Ana_EN = 1;														//允许处理接收缓存区中的数据
					break;
		
    case 	RF_RX_TIMEOUT:	//接收超时
					g_Radio.StartRx(0);
					break;

		default:	break;

	}
}



//Lora发送数据函数，非阻塞发送模式
//发送格式 # 配对标志 # 32位UID # 设备容量 # 设备采集数据
void lorasend(void)
{	
	sprintf((char *)txBuf,"#%c#%c%c%c%c#%c#%c#",
					pair_flag,																														//配对标志位
					(char)(stm32mcuUID >> 24), (char)(stm32mcuUID >> 16), 
					(char)(stm32mcuUID >> 8), (char)stm32mcuUID,													//32位芯片ID，供主机识别
					DEVICE_S_CAP,																													//传感器探头数量
					detect_temp_now);																											//传感器检测数据，8位
	
	TIM_Cmd(TIM4, DISABLE);
	LoraSendMessage();
	TIM_Cmd(TIM4, ENABLE);
}



//发送一次
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
	
	g_Radio.Reset();	//复位
	g_Radio.Init(NULL);	//加载上次的配置
}




