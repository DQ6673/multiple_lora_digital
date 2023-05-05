//////////////////////////////////////////////////////////////////
// 该文件用于定义 app 函数
//////////////////////////////////////////////////////////////////
#include "syscfg.h"

sensor sensor_device[8];
u16 test_pin = 0;
extern u8 error_check_enable;

void systemconfig(void)
{
	delay_init();

	USART2_Init(115200); // 串口初始化
	DWINPagePicSet(0);	 // 转到开始页（欢迎页）

	LED_Init();	 // LED 指示灯
	lora_init(); // Lora 初始化

	RELAY_Init(); // 继电器初始化

	IIC_Init();
	AT24CXX_Init(); // EEPROM 初始化（自检）

	TIM3_Init(); // TIM4 初始化，用于定时处理屏幕的回传数据

	// DeviceInfDeleteAllBlock();

	delay_ms(1500);
	DWIN_init(); // DWIN屏幕初始化
	NodeInfInit();	// 节点信息初始化
	IWDG_Config(IWDG_Prescaler_64, 625); // 看门狗配置
}

u8 OPSTA_Get(u8 item)
{
	switch (item)
	{
	case sensornum:
		return (OP_STA >> 4);

	case pairEN:
		return ((OP_STA >> 1) & 0X01);

	case pagenum:
		return (OP_STA & 0X01);

	default:
		return 0;
	}
}

void OPSTA_Set(u8 item, u8 num)
{
	switch (item)
	{
	case sensornum:
		OP_STA &= 0X0F;
		OP_STA |= num << 4;
		break;

	case pairEN:
		OP_STA &= 0XFD;
		OP_STA |= num << 1;
		break;

	case pagenum:
		OP_STA &= 0XFE;
		OP_STA |= num;
		break;

	default:
		break;
	}
}

void NodeInfInit(void)
{
	DeviceInfReadRom();
	nodeBuf_refresh();
	PageNodelist_Clear();
	PageNodeList_Update();
}

// 判断各节点 error 计数是否超限制
void signalerror_check(void)
{
	u8 sigerr_flag = 0;
	u8 node_num = 0;

	if (error_check_enable)
	{
		// test_pin ++;
		for (u8 i = 0; i < DEVICE_NUM_MAX; i++) // 比较每一个节点
		{
			if ((sensor_device[i].devicenum != 0)) // 当devicenum不为零
			{
				if ((sensor_device[i].errcount > ERRCOUNT) && (sensor_device[i].errflag == 0)) // 计时超限却没给 error flag 复位
				{
					sensor_device[i].errflag = 1; // 置位 errflag
					sensor_device[i].dat = 0;

					while ((NodeInfBuf[node_num][0] != sensor_device[i].devicenum) && (node_num < DEVICE_NUM_MAX))
						node_num++;				   // 在缓存区找到对应的设备起始点
					if (node_num < DEVICE_NUM_MAX) // 判断是否超出限制
					{
						for (u8 j = 0; j < sensor_device[i].comnum; j++) // 循环更新次数为端口数
						{
							NodeInfBuf[node_num + j][15] = ERR;
							Relay_Set(NodeInfBuf[node_num + j][10] - 1, 0); // 寻找该端口对应的继电器号并断开
							NodeInfBuf[node_num + j][12] = (CLOSE >> 16) & 0X0000FFFF;
							NodeInfBuf[node_num + j][13] = (CLOSE & 0X0000FFFF);
						}
						OneNode_Update(); // 修改后的数据从 nodebuf 更新到屏幕
					}
				}

				else if ((sensor_device[i].errcount < ERRCOUNT) && (sensor_device[i].errflag != 0)) // 计时没超却有flag置位
				{
					sensor_device[i].errflag = 0; // 复位 errflag

					while ((NodeInfBuf[node_num][0] != sensor_device[i].devicenum) && (node_num < DEVICE_NUM_MAX))
						node_num++;				   // 从缓存区找到对应的设备起始点
					if (node_num < DEVICE_NUM_MAX) // 判断是否超出限制
					{
						for (u8 j = 0; j < sensor_device[i].comnum; j++) // 循环更新次数为端口数
						{
							NodeInfBuf[node_num + j][15] = FINE;
						}
						OneNode_Update(); // 修改后的数据从 nodebuf 更新到屏幕
					}
				}
			}
		}

		for (u8 j = 0; j < DEVICE_NUM_MAX; j++)
		{
			if ((sensor_device[j].devicenum != 0) && (sensor_device[j].errflag == 1))
			{
				sigerr_flag = 1;
			}
		}

		if (sigerr_flag == 1)
		{
			Relay_Set(DEVICE_NUM_MAX, 1);
		}
		else
			Relay_Set(DEVICE_NUM_MAX, 0);

		error_check_enable = 0;
	}
}

/////////////////////////////////////
// 存储节点信息（一个节点）
/////////////////////////////////////
void DeviceInfSaveRom(sensor *dev)
{
	u16 addr = addr_base + (dev->devicenum - 1) * Point_size; // 计算起始地址

	AT24CXX_WriteOneByte(addr, '#');						   // ‘#’代表存在节点
	AT24CXX_WriteOneByte(addr + 0X0001, (char)dev->devicenum); // 设备号

	AT24CXX_WriteOneByte(addr + 0X0002, (char)(dev->UID >> 24)); // UID
	AT24CXX_WriteOneByte(addr + 0X0003, (char)(dev->UID >> 16));
	AT24CXX_WriteOneByte(addr + 0X0004, (char)(dev->UID >> 8));
	AT24CXX_WriteOneByte(addr + 0X0005, (char)(dev->UID));

	AT24CXX_WriteOneByte(addr + 0X0006, dev->comnum); // 该设备端口数

	for (u8 k = 0; k < dev->comnum; k++)
	{
		for (u8 i = 0; i < (dev->addr[k][0] / 2) + 1; i++)
		{
			AT24CXX_WriteOneByte(addr + 0X0007 + 16 * k + i * 2, (dev->addr[k][i] >> 8) & 0X00FF); // 存放地址，每两个字节一个字
			AT24CXX_WriteOneByte(addr + 0X0007 + 16 * k + i * 2 + 1, (dev->addr[k][i] & 0X00FF));
		}
	}
}

/////////////////////////////////////
// 读取节点数据
/////////////////////////////////////
void DeviceInfReadRom(void)
{
	u8 sensor_num = 0;
	u16 addr = addr_base;

	for (u8 i = 0; i < DEVICE_NUM_MAX; i++)
	{
		// addr += i * Point_size;
		if (AT24CXX_ReadOneByte(addr) == '#') // 这里是否有有效节点
		{
			sensor_device[sensor_num].devicenum = AT24CXX_ReadOneByte(addr + 0X0001); // 设备号

			sensor_device[sensor_num].UID |= ((u32)AT24CXX_ReadOneByte(addr + 0X0002) << 24); // UID
			sensor_device[sensor_num].UID |= ((u32)AT24CXX_ReadOneByte(addr + 0X0003) << 16);
			sensor_device[sensor_num].UID |= ((u32)AT24CXX_ReadOneByte(addr + 0X0004) << 8);
			sensor_device[sensor_num].UID |= (u32)AT24CXX_ReadOneByte(addr + 0X0005);

			sensor_device[sensor_num].comnum = AT24CXX_ReadOneByte(addr + 0X0006); // 端口数

			for (u8 k = 0; k < sensor_device[sensor_num].comnum; k++)
			{
				for (u8 j = 0; j < 8; j++) // 读出节点地址
				{
					sensor_device[sensor_num].addr[k][j] |= ((u16)AT24CXX_ReadOneByte(addr + 0X0007 + 16 * k + j * 2) << 8); // ��ȡ�豸��ַ
					sensor_device[sensor_num].addr[k][j] |= (u16)AT24CXX_ReadOneByte(addr + 0X0007 + 16 * k + j * 2 + 1);
				}
			}
			sensor_num++;
			addr += Point_size;
		}
	}
}

/////////////////////////////////////
// 删除一个节点信息
/////////////////////////////////////
void DeviceInfDelete(sensor *dev)
{
	u16 addr = addr_base + (dev->devicenum - 1) * Point_size; // 根据设备号计算节点位置

	AT24CXX_WriteOneByte(addr, 0XFF); // 该处写入 0XFF 即擦除 ‘#’ 标识

	memset(dev, 0, sizeof(sensor)); // 清空该结构体
}

/////////////////////////////////////
// 清除所有节点信息
/////////////////////////////////////
void DeviceInfDeleteAllBlock(void)
{
	u16 addr = addr_base;
	for (u8 i = 0; i < DEVICE_NUM_MAX; i++)
	{
		AT24CXX_WriteOneByte(addr + i * Point_size, 0XFF);
	}
}
