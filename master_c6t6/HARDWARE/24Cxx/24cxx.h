#ifndef __24CXX_H
#define __24CXX_H
#include "myiic.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//24CXX驱动 代码(适合24C01~24C16,24C32~256未经过测试!有待验证!)		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	  8191
#define AT24C128	16383
#define AT24C256	32767  

#define EE_TYPE AT24C256

//第一区储存设备相关信息
#define PAIR_STATE_ADDR   	0
#define RUNTIME_ADDR				128
//第二区储存传感器相关配置信息
#define SENSOR_INF_ADDR 		255
#define SENSOR_INF_OFFSET		12
//第三区储存传感器历史数据
#define SENSOR_DATA_ADDR    1024
#define SENSOR_DATA_OFFSET  576
#define Three_Day_DataSize  6								
//第四区储存报警信息
#define SENSOR_WDATA_ADDR		30000
#define SENSOR_WDATA_OFFSET 10
//第五区储存字符
#define GBK_Code_ADDR       31000
#define GBK_Code_OFFSET     20

#define Day1   0
#define Day2   2
#define Day3   4

#define ROM_OP_NONE		0x00
#define ROM_Read  		0x01
#define ROM_Write 		0x02
						
u8 AT24CXX_ReadOneByte(u16 ReadAddr);														//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);				//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);										//指定地址开始读取指定长度数据
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC

void Sensor_Inf_Load(uint8_t sn_temp);	//开机数据载入
void Sensor_Inf_Save(uint8_t sn_temp);
void Sensor_Data_Read(u8 sensor_sn,u8 day);	//读取历史数据到缓冲区
#endif
















