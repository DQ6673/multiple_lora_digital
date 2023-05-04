//////////////////////////////////////////////////////////////////
//���ļ����ڶ���app����
//////////////////////////////////////////////////////////////////
#include "syscfg.h"

sensor sensor_device[8];
u16 test_pin = 0;
extern u8 error_check_enable;

void systemconfig(void)
{
	delay_init();
	
	USART2_Init(115200);		//���ڳ�ʼ����������Ļ�����շ�
	DWINPagePicSet(0);				//ת����ʼҳ
	
	LED_Init();							//LED ��ʼ����һ�����ڵ���
	lora_init();						//Lora ��ʼ��
	
	RELAY_Init();						//�̵������
	
	IIC_Init();
	AT24CXX_Init();					//EEPROM ��ʼ��
	
	TIM4_Init();						//�� TIM4 ��ʱ�������������Ļ�ϴ�������,�� TIM4 �жϴ�����
	
	
	//DeviceInfDeleteAllBlock();
	
	delay_ms(1500);
	DWIN_init();						//��������ʼ��
	NodeInfInit();
	IWDG_Config(IWDG_Prescaler_64, 625);		//���Ź�
}



u8 OPSTA_Get(u8 item)
{
	switch(item)
	{
		case sensornum :  return (OP_STA >> 4);
		
		case pairEN :  		return ((OP_STA >> 1) & 0X01);
		
		case pagenum : 		return (OP_STA & 0X01);
		
		default : 				return 0;
	}
}



void OPSTA_Set(u8 item, u8 num)
{
	switch(item)
	{
		case sensornum :  OP_STA &= 0X0F;
											OP_STA |= num << 4; break;
		
		case pairEN :  		OP_STA &= 0XFD;
											OP_STA |= num << 1; break;
		
		case pagenum :		OP_STA &= 0XFE;
											OP_STA |= num; 			break;
		
		default : 				break;
	}
}




void NodeInfInit(void)
{
	DeviceInfReadRom();
	nodeBuf_refresh();
	PageNodelist_Clear();
	PageNodeList_Update();
}




//�жϸ��ڵ��errorcountֵ�Ƿ񳬳����ƣ�ERRCOUNT��
void signalerror_check(void)
{
	u8 sigerr_flag = 0;
	u8 node_num = 0;
	
	if(error_check_enable)
	{
		//test_pin ++;      
		for(u8 i=0; i < DEVICE_NUM_MAX; i++)																																					//�Ƚ�ÿһ���ڵ�
		{
			if((sensor_device[i].devicenum != 0))																												//��devicenum��Ϊ��
			{
				if((sensor_device[i].errcount > ERRCOUNT) && (sensor_device[i].errflag == 0))							//��ʱ����ȴû��errflag��λ
				{
					sensor_device[i].errflag = 1;																														//��λerrflag
					sensor_device[i].dat = 0;
					
					while((NodeInfBuf[node_num][0] != sensor_device[i].devicenum) && (node_num < DEVICE_NUM_MAX)) node_num++;	//�ڻ������ҵ���Ӧ���豸��ʼ��
					if(node_num < DEVICE_NUM_MAX)																				//�ж��Ƿ񳬳�����
					{
						for(u8 j=0; j<sensor_device[i].comnum; j++)						//ѭ�����´���Ϊ�˿���
						{
							NodeInfBuf[node_num + j][15] = ERR;
							Relay_Set(NodeInfBuf[node_num + j][10] - 1, 0);				//Ѱ�Ҹö˿ڶ�Ӧ�ļ̵����Ų��Ͽ�
							NodeInfBuf[node_num + j][12] = (CLOSE>>16) & 0X0000FFFF;
							NodeInfBuf[node_num + j][13] = (CLOSE & 0X0000FFFF);
						}
						OneNode_Update();																			//�޸ĺ�����ݴ�nodebuf���µ�����Ļ
					}		
				}
				
				else if((sensor_device[i].errcount < ERRCOUNT) && (sensor_device[i].errflag != 0))				//��ʱû��ȴ��flag��λ
				{
					sensor_device[i].errflag = 0;																														//��λerrflag
					
					while((NodeInfBuf[node_num][0] != sensor_device[i].devicenum) && (node_num < DEVICE_NUM_MAX)) node_num++;	//�ڻ������ҵ���Ӧ���豸��ʼ��
					if(node_num < DEVICE_NUM_MAX)																				//�ж��Ƿ񳬳�����
					{
						for(u8 j=0; j<sensor_device[i].comnum; j++)						//ѭ�����´���Ϊ�˿���
						{
							NodeInfBuf[node_num + j][15] = FINE;
						}
						OneNode_Update();																			//�޸ĺ�����ݴ�nodebuf���µ�����Ļ
					}		
				}
			}
			
		}
		
		for(u8 j=0; j < DEVICE_NUM_MAX; j++)
		{
			if((sensor_device[j].devicenum != 0) && (sensor_device[j].errflag == 1))  
			{
				sigerr_flag = 1;
			}
		}
		
		if(sigerr_flag == 1)  
		{
			Relay_Set(DEVICE_NUM_MAX, 1);
		}
		else						 			Relay_Set(DEVICE_NUM_MAX, 0);
		
		error_check_enable = 0;
	}
}




/////////////////////////////////////
//�洢�ڵ���Ϣ
//dev : sensor�ṹ��, blocknum : ��Ҫ�洢��λ�ã�0-8��
/////////////////////////////////////
void DeviceInfSaveRom(sensor *dev)
{
	u16 addr = addr_base + (dev->devicenum - 1) * Point_size;														//����blocknum����24C256��ַ
	
	AT24CXX_WriteOneByte(addr, '#');																									//ROM��ռ�ñ�ʶ��
	AT24CXX_WriteOneByte(addr + 0X0001, (char) dev->devicenum);												//�豸�ţ��û�����
	
	AT24CXX_WriteOneByte(addr + 0X0002, (char) (dev->UID >> 24));											//��������32λʶ��ID	
	AT24CXX_WriteOneByte(addr + 0X0003, (char) (dev->UID >> 16));	
	AT24CXX_WriteOneByte(addr + 0X0004, (char) (dev->UID >> 8));	
	AT24CXX_WriteOneByte(addr + 0X0005, (char) (dev->UID));	
	
	AT24CXX_WriteOneByte(addr + 0X0006, dev->comnum);																	//�˿�����
	
	for(u8 k=0; k<dev->comnum; k++)
	{
		for(u8 i=0; i<(dev->addr[k][0]/2)+1; i++)
		{
			AT24CXX_WriteOneByte(addr + 0X0007 + 16 * k + i*2  , (dev->addr[k][i] >> 8) & 0X00FF);			//�豸��ַ
			AT24CXX_WriteOneByte(addr + 0X0007 + 16 * k + i*2+1, (dev->addr[k][i] & 0X00FF));
		}
	}
}


/////////////////////////////////////
//��ȡrom�еĽڵ���Ϣ��
//dev : sensor�ṹ��, blocknum : ��Ҫ�洢��λ�ã�0-7��
/////////////////////////////////////
void DeviceInfReadRom(void)
{
	u8 sensor_num = 0;
	u16 addr = addr_base;
	
	for(u8 i=0 ; i<DEVICE_NUM_MAX ; i++)
	{
		//addr += i * Point_size;
		if(AT24CXX_ReadOneByte(addr) == '#')																											//�жϴ˴��Ƿ���нڵ���Ϣ
		{
			sensor_device[sensor_num].devicenum = AT24CXX_ReadOneByte(addr + 0X0001);							//��ȡ�豸��
			
			sensor_device[sensor_num].UID |= ((u32)AT24CXX_ReadOneByte(addr + 0X0002) << 24);			//��ȡUID
			sensor_device[sensor_num].UID |= ((u32)AT24CXX_ReadOneByte(addr + 0X0003) << 16);			
			sensor_device[sensor_num].UID |= ((u32)AT24CXX_ReadOneByte(addr + 0X0004) << 8);			
			sensor_device[sensor_num].UID |= (u32)AT24CXX_ReadOneByte(addr + 0X0005);			
			
			sensor_device[sensor_num].comnum = AT24CXX_ReadOneByte(addr + 0X0006);							//��ȡ�˿�����
			
			for(u8 k=0; k<sensor_device[sensor_num].comnum; k++)
			{
				for(u8 j=0;  j < 8; j++)																																							//��ȡ�豸λ��
				{
					sensor_device[sensor_num].addr[k][j] |= ((u16)AT24CXX_ReadOneByte(addr + 0X0007 + 16 * k + j * 2) << 8); 	//��ȡ�豸��ַ
					sensor_device[sensor_num].addr[k][j] |= (u16)AT24CXX_ReadOneByte(addr + 0X0007 + 16 * k + j * 2 + 1);
				}
			}
			sensor_num ++;
			addr += Point_size;
		}
	}
}




/////////////////////////////////////
//ɾ���ڵ���Ϣ
//dev : sensor�ṹ��, blocknum : ��Ҫ�洢��λ�ã�0-7��
/////////////////////////////////////
void DeviceInfDelete(sensor *dev)
{
	u16 addr = addr_base + (dev->devicenum - 1) * Point_size;														//����ýڵ��Ӧ��rom��ַ
	
	AT24CXX_WriteOneByte(addr,0XFF);																		//���ô���ռ�ñ�ʶ���������#����
	
	memset(dev,0,sizeof(sensor));																				//����ڵ������ڴ���
}




/////////////////////////////////////
//ɾ��ȫ����Ϣ   �����ڿ�������
//dev : sensor�ṹ��, blocknum : ��Ҫ�洢��λ�ã�0-7��
/////////////////////////////////////
void DeviceInfDeleteAllBlock(void)
{
	u16 addr = addr_base;																									//���ô���ռ�ñ�ʶ���������#����
	for(u8 i=0; i < DEVICE_NUM_MAX ; i++)
	{	
		AT24CXX_WriteOneByte(addr + i * Point_size, 0XFF);		
	}
}
