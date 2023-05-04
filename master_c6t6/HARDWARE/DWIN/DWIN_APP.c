#include "DWIN_APP.H"
#include "delay.h"
#include "sys.h"
#include "syscfg.h"

u8 debug_flag = 0;
u16 NodeInfBuf[10][16] =
	{
		{0, 0, 14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0, 4, 0X2020, 0X2020, 2, 0X2020},
		{0, 0, 14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0, 4, 0X2020, 0X2020, 2, 0X2020},
		{0, 0, 14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0, 4, 0X2020, 0X2020, 2, 0X2020},
		{0, 0, 14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0, 4, 0X2020, 0X2020, 2, 0X2020},
		{0, 0, 14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0, 4, 0X2020, 0X2020, 2, 0X2020},
		{0, 0, 14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0, 4, 0X2020, 0X2020, 2, 0X2020},
		{0, 0, 14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0, 4, 0X2020, 0X2020, 2, 0X2020},
		{0, 0, 14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0, 4, 0X2020, 0X2020, 2, 0X2020},
		{0, 0, 14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0, 4, 0X2020, 0X2020, 2, 0X2020}, // 第五条数据用于添加缓存区信息

		{0, 0, 14, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0, 4, 0X2020, 0X2020, 2, 0X2020} // 第6条数据用于初始化前5条缓存区，不可修改
};

// 位置：		0			1		  2         10			  	 11						  14
//			设备号 端口 位置（T）  继电器号		 开关状态（T）		通信状态（T）

u8 OP_STA = 0XF0;
u8 test = 0;

/////////////////////////////////////////////////////////////////
// 分析串口缓冲区数据
// RX_BUF中无5AA5前缀，所以触控变量地址为 DWIN_RX_BUF[2]和[3]
/////////////////////////////////////////////////////////////////

void DWIN_Buffer_Analysis() // test
{
	if (DWIN_Analysis_EN == 1)
	{
		switch (DWIN_RX_BUF[2])
		{
		case 0X18:
			DWINchangepage();
			break;
		case 0X19:
			Addnewdevice();
			break;
		case 0X16:
			NodeInfHandler();
			break;
		case 0X20:
			DeleteOneNode();
			break;
		case 0X17:
			Addr_change();
			break;

		default:
			break;
		}
		DWIN_Analysis_EN = 0;
		debug_flag++;
	}
}

// 更改地址
void Addr_change(void)
{
	u8 click_num = DWIN_RX_BUF[3] >> 4;
	u8 device_num = NodeInfBuf[OPSTA_Get(pagenum) * 4 + click_num][0]; // 获得缓存区中的设备号
	u8 com_num = NodeInfBuf[OPSTA_Get(pagenum) * 4 + click_num][1];
	u8 i = 0;
	u8 addr_len = DWIN_RX_BUF[4]; // 获取该地址长度

	while ((sensor_device[i].devicenum != device_num) && i < DEVICE_NUM_MAX)
		i++; // 根据缓存区中设备号找到对应结构体

	if ((i < DEVICE_NUM_MAX) && (addr_len < 9))
	{
		for (u8 k = 0; k < addr_len; k++) //
		{
			sensor_device[i].addr[com_num - 1][k + 1] = (u16)(DWIN_RX_BUF[5 + k * 2] << 8) | (u16)(DWIN_RX_BUF[5 + k * 2 + 1]);
		}

		addr_len = 0;
		while ((DWIN_RX_BUF[5 + addr_len] != 0XFF) && (addr_len < 20))
			addr_len++;
		if (addr_len < 20)
			sensor_device[i].addr[com_num - 1][0] = addr_len;

		DeviceInfSaveRom(&sensor_device[i]);
		nodeBuf_refresh();
	}

	PageNodelist_Clear();
	PageNodeList_Update();
}

// 节点列表翻页操作
void DWINchangepage(void)
{
	switch (DWIN_RX_BUF[3])
	{
	case 0X00:
		if (OPSTA_Get(pagenum) == 1)
		{
			OPSTA_Set(pagenum, 0);
			nodeBuf_refresh();
			PageNodelist_Clear();
			PageNodeList_Update();
		}
		break;

	case 0X10:
		if (OPSTA_Get(pagenum) == 0)
		{
			OPSTA_Set(pagenum, 1);
			nodeBuf_refresh();
			PageNodelist_Clear();
			PageNodeList_Update();
		}
		break;

	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////
// 添加设备系列按键
// DWIN_RX_BUF[3] = 00 添加按钮，10 确认添加， 20取消添加
/////////////////////////////////////////////////////////////////
void Addnewdevice(void)
{
	u8 i = 0;
	u8 pair_flag = 0;

	switch (DWIN_RX_BUF[3])
	{
	case 0X00:
		OPSTA_Set(pairEN, 1);	 // 允许配对
		DWINPagePicSet(2);		 // 转到添加页
		DWINsenddata(0X1940, 0); // 清除设置设备号
		DWINsenddata(0X1950, 0); // 清除侦测设备端口数
		DWINsenddata(0X1960, 0); // 清除侦测设备号
		NodeInfBuf[8][0] = 0;	 // 清理nodebuf设备号和端口号，等待写入
		NodeInfBuf[8][1] = 0;
		break;

	case 0X10:
		OPSTA_Set(pairEN, 0);	  // 禁止配对
		i = OPSTA_Get(sensornum); // 获得已找到的空结构体号
		if (i < DEVICE_NUM_MAX)	  // 判断是否超出范围
		{
			for (u8 j = 0; j < DEVICE_NUM_MAX; j++)
			{
				if (sensor_device[j].devicenum == NodeInfBuf[8][0]) // 判断设备号是否已经存在
				{
					pair_flag = 1;
				}
			}

			if ((pair_flag == 0) && (NodeInfBuf[8][0] != 0)) // 如果没发现重复并且已经设置了设备号
			{
				sensor_device[i].devicenum = NodeInfBuf[8][0]; // 将设置的设备号存入该结构体
				DeviceInfSaveRom(&sensor_device[i]);		   // 存储结构体信息到eeprom
				test++;
			}
			else
				sensor_device[i].UID = 0;
		}
		OPSTA_Set(sensornum, 0X0F); // 复位结构体暂存号
		nodeBuf_refresh();			// 刷新缓存区，并刷新显示
		PageNodelist_Clear();
		PageNodeList_Update();
		DWINPagePicSet(1); // 回到设备列表页
		break;

	case 0X20:
		OPSTA_Set(pairEN, 0); // 禁止配对
		PageNodelist_Clear();
		PageNodeList_Update();
		DWINPagePicSet(1); // 回到设备列表页
		break;

	case 0X30:
		NodeInfBuf[8][0] = DWIN_RX_BUF[6]; // 获取设置的设备号
		DWINsenddata(0X1940, NodeInfBuf[8][0]);
		break;

	default:
		break;
	}
}

void NodeInfHandler(void)
{
	u8 click_num = (DWIN_RX_BUF[3] >> 4) & 0X0F;
	u8 node_num = OPSTA_Get(pagenum) + click_num;

	if (click_num == 4) // 执行返回操作，返回节点列表
	{
		OPSTA_Set(sensornum, 0X0F); // 复位结构体暂存号
		DWINPagePicSet(1);
	}

	else // 进入设备信息页，刷新显示信息
	{
		if (NodeInfBuf[node_num][0] != 0) // 判断点击处是否存在节点，否则不执行操作
		{
			DWINPagePicSet(3);

			for (u8 i = 0; i < DEVICE_NUM_MAX; i++)
			{
				if (sensor_device[i].devicenum == NodeInfBuf[node_num][0]) // 寻找结构体
				{
					OPSTA_Set(sensornum, i);						  // 写入操作中的结构体号
					DWINsenddata(0X1970, sensor_device[i].devicenum); // 刷新三个数据
					DWINsenddata(0X1980, sensor_device[i].comnum);
					DWINsenddata(0X1990, (u16)sensor_device[i].UID);
					break;
				}
			}
		}
	}
}

// 在节点详情信息页中删除一个节点
void DeleteOneNode(void)
{
	u8 sensor_num = OPSTA_Get(sensornum);
	DeviceInfDelete(&sensor_device[sensor_num]);
	DWINsenddata(0X1970, 0); // 清除三个数据
	DWINsenddata(0X1980, 0);
	DWINsenddata(0X1990, 0);

	nodeBuf_refresh();
	PageNodelist_Clear();
	PageNodeList_Update();
}

// 将屏幕缓存区清除后更新到屏幕显示
void PageNodelist_Clear()
{
	for (u8 i = 0; i < 4; i++) // 清除缓存区
	{
		DWINdatadisPlay(0X1000 + (i << 4), 0); // 隐藏设备号
		DWINdatadisPlay(0X1100 + (i << 4), 0); // 隐藏端口号
		DWINdatadisPlay(0X1200 + (i << 4), 0); // 隐藏继电器号

		DWINtextclear(0X1300 + (i << 4), 14); // 清除位置文字
		DWINtextclear(0X1400 + (i << 4), 4);  // 清除开关状态文字
		DWINtextclear(0X1500 + (i << 4), 2);  // 清除通信状态文字
	}
}

// 节点首页更新数据
void PageNodeList_Update()
{
	u8 read_p = OPSTA_Get(pagenum) * 4; // 获取页数

	for (u8 i = 0; i < 4; i++, read_p++)
	{
		if (NodeInfBuf[read_p][0] != 0) // 缓存区中的设备号不为零才执行显示操作
		{
			DWINsenddata(0X1000 + (i << 4), NodeInfBuf[read_p][0]); // 显示设备号
			DWINdatadisPlay(0X1000 + (i << 4), 1);

			DWINsenddata(0X1100 + (i << 4), NodeInfBuf[read_p][1]); // 显示端口号
			DWINdatadisPlay(0X1100 + (i << 4), 1);

			DWINsenddata(0X1200 + (i << 4), NodeInfBuf[read_p][10]); // 显示继电器号
			DWINdatadisPlay(0X1200 + (i << 4), 1);

			DWINsendtext(0X1300 + (i << 4), &NodeInfBuf[read_p][2]); // 显示位置

			DWINsendtext(0X1400 + (i << 4), &NodeInfBuf[read_p][11]); // 显示继电器通断
			if (NodeInfBuf[read_p][13] == (OPEN & 0X0000FFFF))
				DWINsettextcolor(0X1400 + (i << 4), D_YELLOW);
			else
				DWINsettextcolor(0X1400 + (i << 4), D_ORANGE);

			DWINsendtext(0X1500 + (i << 4), &NodeInfBuf[read_p][14]); // 显示通信状况
			if (NodeInfBuf[read_p][15] == ERR)
				DWINsettextcolor(0X1500 + (i << 4), D_RED);
			else
				DWINsettextcolor(0X1500 + (i << 4), D_GREEN);
		}
	}
}

// 此函数用于实时状态更新，仅更新频繁信息
void OneNode_Update(void)
{
	u8 read_p = OPSTA_Get(pagenum) * 4; // 获取页数

	for (u8 i = 0; i < 4; i++, read_p++)
	{
		if (NodeInfBuf[read_p][0] != 0) // 缓存区中的设备号不为零才执行显示操作
		{
			DWINsendtext(0X1400 + (i << 4), &NodeInfBuf[read_p][11]); // 显示继电器通断
			if (NodeInfBuf[read_p][13] == (OPEN & 0X0000FFFF))
				DWINsettextcolor(0X1400 + (i << 4), D_YELLOW);
			else
				DWINsettextcolor(0X1400 + (i << 4), D_ORANGE);

			DWINsendtext(0X1500 + (i << 4), &NodeInfBuf[read_p][14]); // 显示通信状况
			if (NodeInfBuf[read_p][15] == ERR)
				DWINsettextcolor(0X1500 + (i << 4), D_RED);
			else
				DWINsettextcolor(0X1500 + (i << 4), D_GREEN);
		}
	}
}

// 将结构体中信息按顺序排列好装入缓存数组，供屏幕读取显示
void nodeBuf_refresh(void)
{
	u8 devnodenum[8];
	u8 t = 0;
	u8 sensor_num = 0;
	u8 writeBuf_p = 0;

	for (u8 i = 0; i < DEVICE_NUM_MAX; i++)
	{
		devnodenum[i] = sensor_device[i].devicenum; // 拷贝设备号到缓存数组
		NodeInfBuf[i][0] = 0;						// 清理掉8个设备号等待重新写入
	}

	for (u8 i = DEVICE_NUM_MAX - 1; i > 0; i--)
	{
		for (u8 j = 0; j < i; j++)
		{
			if (devnodenum[j] > devnodenum[j + 1])
			{
				t = devnodenum[j];
				devnodenum[j] = devnodenum[j + 1];
				devnodenum[j + 1] = t;
			}
		}
	} // 冒泡排序，从小到大排好设备号

	t = 0;
	while ((devnodenum[t] == 0) && (t < DEVICE_NUM_MAX))
		t++; // 找到第一个不为零的数

	if (t < DEVICE_NUM_MAX)
	{
		for (; t < DEVICE_NUM_MAX; t++)
		{
			sensor_num = 0;
			while (devnodenum[t] != sensor_device[sensor_num].devicenum)
				sensor_num++; // 使用sensor_num找到对应的结构体（从小到大）

			for (u8 i = 0; (i < sensor_device[sensor_num].comnum) && (writeBuf_p < DEVICE_NUM_MAX); i++) // 每个节点按端口数量向下写入nodeinfbuf
			{																							 // writeBuf_p 不允许大于8
				test_pin = 3;
				NodeInfBuf[writeBuf_p][0] = sensor_device[sensor_num].devicenum; // 设备号
				NodeInfBuf[writeBuf_p][1] = i + 1;								 // 端口号，从1开始

				for (u8 j = 0; j < 8; j++)
				{
					NodeInfBuf[writeBuf_p][2 + j] = sensor_device[sensor_num].addr[i][j]; // 写入地址
				}
				NodeInfBuf[writeBuf_p][10] = writeBuf_p + 1;																						 // 动态分配继电器输出
				NodeInfBuf[writeBuf_p][12] = ((sensor_device[sensor_num].dat >> i) & 0X01) ? (OPEN >> 16) & 0X0000FFFF : (CLOSE >> 16) & 0X0000FFFF; // 写入开关状态
				NodeInfBuf[writeBuf_p][13] = ((sensor_device[sensor_num].dat >> i) & 0X01) ? OPEN & 0X0000FFFF : CLOSE & 0X0000FFFF;
				NodeInfBuf[writeBuf_p][15] = (sensor_device[sensor_num].errflag) ? ERR : FINE; // 写入通信状态

				writeBuf_p++;
			}
		}
	}
}

// DWIN初始化函数
void DWIN_init(void)
{
	//	DWINSendMessage();  		//虚晃一枪，防止首条指令会吞掉一个5A

	PageNodelist_Clear();
	DWINPagePicSet(1); // 转到开始页
}
