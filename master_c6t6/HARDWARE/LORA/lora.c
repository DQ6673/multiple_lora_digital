#include "lora.h"
#include "RELAY.h"
#include "syscfg.h"

uint8_t rxBuf[rxLen]={0};
uint16_t rxCount=rxLen;
uint8_t LoraRxBuf_Ana_EN = 0;												//使能标志位，允许回调函数处理数据

//Lora模组接收数据回调函数，用于处理收到的数据
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
		if(rxBuf[0] == '#' && rxBuf[7] == '#' && rxBuf[11] == '#')			//检查数据格式
		{
			UID_temp = ((u32)rxBuf[3] << 24) + ((u32)rxBuf[3] << 16) + ((u32)rxBuf[3] << 8) + ((u32)rxBuf[3]); 	//拼接UID
			comnum_temp = rxBuf[8];																																							//获取端口数量
			comnum_sum = comnum_temp;
			
			if(rxBuf[1] == 0X01 && OPSTA_Get(pairEN))											//若是配对请求并且此时处于添加节点状态
			{
				test_pin = 0;
				for(u8 j=0; j<DEVICE_NUM_MAX; j++)
				{
					if(sensor_device[j].UID == UID_temp) 											//判断是否为同一设备的请求，即判断UID是否已经存在
					{
						pair_flag = 1;																					
						test_pin += 1;
					}
					if(sensor_device[j].devicenum != 0)
						comnum_sum += sensor_device[j].comnum;
				}
				
				if(comnum_sum > DEVICE_NUM_MAX) 
				{
					pair_flag = 1;												//如果挤不下了
					test_pin += 2;
				}
				
				if(pair_flag == 0)
				{
					test_pin +=4;
					while((sensor_device[sensor_num].devicenum != 0) && (sensor_num < DEVICE_NUM_MAX)) sensor_num++; //寻找对应结构体号
			
					if(sensor_num < DEVICE_NUM_MAX)																				//检查是否超出结构体数量限制
					{
						test_pin +=8;
						OPSTA_Set(sensornum, sensor_num);																		//写入正在操作的结构体号
						sensor_device[sensor_num].UID = UID_temp;														//写入UID
						sensor_device[sensor_num].comnum = comnum_temp;											//写入端口号
						DWINsenddata(0X1960, (u16) UID_temp);																//显示UID
						DWINsenddata(0X1950, (u16) comnum_temp);														//显示端口号
					}
				}
			}

			else																													//若非配对请求，即为传感器数据传输
			{
				for(u8 j=0; j< DEVICE_NUM_MAX; j++)
				{
					if(sensor_device[j].UID == UID_temp)											//寻找对应结构体
					{
						if(sensor_device[j].dat != detect_temp)									//如采集端出现变化则刷新屏幕显示和继电器输出
						{
							sensor_device[j].dat = detect_temp;
							
							while((NodeInfBuf[node_num][0] != sensor_device[j].devicenum) && (node_num < DEVICE_NUM_MAX)) node_num++;	//在缓存区找到对应的设备起始点
							if(node_num < DEVICE_NUM_MAX)														//判断是否超出限制
							{
								for(u8 i=0; i<sensor_device[j].comnum; i++)						//循环更新次数为端口数
								{
									if(((detect_temp >> i) & 0X01) == 1)								//判断第i号端口是否为1
									{
										Relay_Set(NodeInfBuf[node_num + i][10] - 1, 1);				//寻找该端口对应的继电器号并闭合
										NodeInfBuf[node_num + i][12] = (OPEN>>16) & 0X0000FFFF;
										NodeInfBuf[node_num + i][13] = (OPEN & 0X0000FFFF);
									}
									
									else										
									{
										Relay_Set(NodeInfBuf[node_num + i][10] - 1, 0);				//寻找该端口对应的继电器号并断开
										NodeInfBuf[node_num + i][12] = (CLOSE>>16) & 0X0000FFFF;
										NodeInfBuf[node_num + i][13] = (CLOSE & 0X0000FFFF);
									}

								}
								OneNode_Update();																			//修改后的数据从nodebuf更新到到屏幕
							}
						}
						sensor_device[j].errcount = 0;
						break;																										//找到了处理完了就不用继续找了
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
					g_Radio.StartRx(3000);
					break;
		
    case 	RF_RX_DONE:	//接收完成
					LED=~LED;
					memset(rxBuf,0,10);
					g_Radio.GetRxPacket(rxBuf,&rxCount);
					g_Radio.StartRx(3000);
					LoraRxBuf_Ana_EN = 1;														//允许处理接收缓存区中的数据
					break;
		
    case 	RF_RX_TIMEOUT:	//接收超时
					g_Radio.StartRx(3000);
					break;

		default:	break;

	}
}



//Lora发送数据函数，非阻塞发送模式，需轮询调用
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
	
	g_Radio.Reset();	//复位
	g_Radio.Init(NULL);	//加载上次的配置
}







